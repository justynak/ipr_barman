-- ipr_barman schema v2 — the refactor target.
--
-- The current application binary still runs against schema.sql (v1).
-- The repository refactor will migrate the code onto this schema.
--
-- Design decisions (agreed 2026-07, see git history of the discussion):
--  * Surrogate ids everywhere; names, card numbers etc. are mutable attributes.
--    No PESEL stored anywhere — the login card carries an opaque number (UUID).
--  * order_item copies product_name and unit_price at sale time (receipts are
--    immutable history); product_id stays as the analytical reference.
--  * Order totals are NOT stored — derived from lines and discount_rate.
--  * Stock is NOT stored as a counter — derived from ingredient_movement
--    (append-only ledger: deliveries +, pours -, corrections, waste).
--  * business_day is stamped explicitly at order creation (cutoff rule lives
--    in one place in the application, not in every report query).
--  * Draft orders live in application memory; the DB is written once, in one
--    transaction, at finalize. Hence no 'closed' flag: every stored order is
--    final. If bar tabs must survive an application restart, revisit this.

CREATE DATABASE IF NOT EXISTS barman_v2 CHARACTER SET utf8mb4;
USE barman_v2;

CREATE TABLE employee (
    id          INT AUTO_INCREMENT PRIMARY KEY,
    role        ENUM('bartender', 'manager') NOT NULL,
    card_number VARCHAR(36) NOT NULL UNIQUE,   -- scanned at login; opaque (UUID)
    first_name  VARCHAR(60) NOT NULL,
    last_name   VARCHAR(60) NOT NULL
) ENGINE=InnoDB;

CREATE TABLE shift (
    id          INT AUTO_INCREMENT PRIMARY KEY,
    employee_id INT NOT NULL,
    opened_at   DATETIME NOT NULL,
    closed_at   DATETIME NULL,                 -- NULL = shift in progress
    FOREIGN KEY (employee_id) REFERENCES employee(id)
) ENGINE=InnoDB;

CREATE TABLE customer (
    id          INT AUTO_INCREMENT PRIMARY KEY,
    card_number VARCHAR(36) NOT NULL UNIQUE    -- replaceable without losing history
) ENGINE=InnoDB;

CREATE TABLE bar (
    id       INT AUTO_INCREMENT PRIMARY KEY,
    name     VARCHAR(100) NOT NULL UNIQUE,
    currency CHAR(3) NOT NULL DEFAULT 'PLN'    -- ISO 4217; all product prices are in this currency
) ENGINE=InnoDB;

CREATE TABLE product (
    id            INT AUTO_INCREMENT PRIMARY KEY,
    name          VARCHAR(100) NOT NULL UNIQUE,
    current_price DECIMAL(8,2) NOT NULL,
    category      VARCHAR(60)  NOT NULL
) ENGINE=InnoDB;

CREATE TABLE bar_product (
    bar_id     INT NOT NULL,
    product_id INT NOT NULL,
    PRIMARY KEY (bar_id, product_id),
    FOREIGN KEY (bar_id)     REFERENCES bar(id),
    FOREIGN KEY (product_id) REFERENCES product(id)
) ENGINE=InnoDB;

CREATE TABLE ingredient (
    id   INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL UNIQUE,
    unit ENUM('cl', 'g', 'pcs') NOT NULL       -- stock and recipe amounts share this unit
) ENGINE=InnoDB;

CREATE TABLE product_ingredient (
    product_id    INT NOT NULL,
    ingredient_id INT NOT NULL,
    amount        DECIMAL(8,2) NOT NULL,       -- recipe quantity, in ingredient.unit
    PRIMARY KEY (product_id, ingredient_id),
    FOREIGN KEY (product_id)    REFERENCES product(id),
    FOREIGN KEY (ingredient_id) REFERENCES ingredient(id)
) ENGINE=InnoDB;

CREATE TABLE customer_order (
    id            INT AUTO_INCREMENT PRIMARY KEY,
    shift_id      INT NOT NULL,
    business_day  DATE NOT NULL,
    customer_id   INT NULL,
    discount_rate DECIMAL(4,3) NOT NULL DEFAULT 0,  -- rate at sale time (copy)
    currency      CHAR(3) NOT NULL,                 -- bar.currency at sale time (copy)
    created_at    DATETIME NOT NULL,
    FOREIGN KEY (shift_id)    REFERENCES shift(id),
    FOREIGN KEY (customer_id) REFERENCES customer(id),
    INDEX idx_order_business_day (business_day)
) ENGINE=InnoDB;

CREATE TABLE order_item (
    id           INT AUTO_INCREMENT PRIMARY KEY,
    order_id     INT NOT NULL,
    product_id   INT NOT NULL,                 -- reference (analytics)
    product_name VARCHAR(100) NOT NULL,        -- copy at sale time (receipt)
    unit_price   DECIMAL(8,2)  NOT NULL,       -- copy at sale time (receipt)
    quantity     INT NOT NULL,
    FOREIGN KEY (order_id)   REFERENCES customer_order(id),
    FOREIGN KEY (product_id) REFERENCES product(id)
) ENGINE=InnoDB;

-- Append-only. Current stock is the SUM; never UPDATE or DELETE rows here.
CREATE TABLE ingredient_movement (
    id            INT AUTO_INCREMENT PRIMARY KEY,
    ingredient_id INT NOT NULL,
    amount        DECIMAL(10,2) NOT NULL,      -- negative = consumed, positive = delivered
    reason        ENUM('order', 'delivery', 'correction', 'waste') NOT NULL,
    order_item_id INT NULL,                    -- set when reason = 'order'
    occurred_at   DATETIME NOT NULL,
    FOREIGN KEY (ingredient_id) REFERENCES ingredient(id),
    FOREIGN KEY (order_item_id) REFERENCES order_item(id),
    INDEX idx_movement_ingredient (ingredient_id)
) ENGINE=InnoDB;

CREATE VIEW ingredient_stock AS
    SELECT i.id, i.name, i.unit, COALESCE(SUM(m.amount), 0) AS stock
    FROM ingredient i
    LEFT JOIN ingredient_movement m ON m.ingredient_id = i.id
    GROUP BY i.id, i.name, i.unit;

-- ---------------------------------------------------------------------------
-- Seed data
-- ---------------------------------------------------------------------------

INSERT INTO bar (name) VALUES ('Boboli 8, Warszawa');

INSERT INTO employee (role, card_number, first_name, last_name) VALUES
    ('bartender', '8b1e9d3a-0000-4000-8000-000000000001', 'Jan',  'Kowalski'),
    ('manager',   '8b1e9d3a-0000-4000-8000-000000000002', 'Anna', 'Nowak');

INSERT INTO customer (card_number) VALUES ('1001'), ('1002');

INSERT INTO ingredient (name, unit) VALUES
    ('rum', 'cl'), ('vodka', 'cl'), ('cola', 'cl'),
    ('orange juice', 'cl'), ('lime', 'pcs'), ('mint', 'g');

INSERT INTO product (name, current_price, category) VALUES
    ('Mojito', 22.00, 'Cocktails'),
    ('Cuba Libre', 20.00, 'Cocktails'),
    ('Screwdriver', 18.00, 'Cocktails'),
    ('Rum shot', 12.00, 'Shots'),
    ('Vodka shot', 10.00, 'Shots');

INSERT INTO product_ingredient (product_id, ingredient_id, amount) VALUES
    ((SELECT id FROM product WHERE name='Mojito'),      (SELECT id FROM ingredient WHERE name='rum'),          4),
    ((SELECT id FROM product WHERE name='Mojito'),      (SELECT id FROM ingredient WHERE name='lime'),         1),
    ((SELECT id FROM product WHERE name='Mojito'),      (SELECT id FROM ingredient WHERE name='mint'),         8),
    ((SELECT id FROM product WHERE name='Cuba Libre'),  (SELECT id FROM ingredient WHERE name='rum'),          4),
    ((SELECT id FROM product WHERE name='Cuba Libre'),  (SELECT id FROM ingredient WHERE name='cola'),        20),
    ((SELECT id FROM product WHERE name='Cuba Libre'),  (SELECT id FROM ingredient WHERE name='lime'),         1),
    ((SELECT id FROM product WHERE name='Screwdriver'), (SELECT id FROM ingredient WHERE name='vodka'),        4),
    ((SELECT id FROM product WHERE name='Screwdriver'), (SELECT id FROM ingredient WHERE name='orange juice'),15),
    ((SELECT id FROM product WHERE name='Rum shot'),    (SELECT id FROM ingredient WHERE name='rum'),          4),
    ((SELECT id FROM product WHERE name='Vodka shot'),  (SELECT id FROM ingredient WHERE name='vodka'),        4);

INSERT INTO bar_product (bar_id, product_id)
    SELECT (SELECT id FROM bar WHERE name='Boboli 8, Warszawa'), id FROM product;

-- Opening stock arrives as delivery movements, not as counter values.
INSERT INTO ingredient_movement (ingredient_id, amount, reason, occurred_at)
    SELECT id, 700, 'delivery', NOW() FROM ingredient WHERE unit = 'cl';
INSERT INTO ingredient_movement (ingredient_id, amount, reason, occurred_at)
    SELECT id, 50, 'delivery', NOW() FROM ingredient WHERE unit IN ('pcs', 'g');
