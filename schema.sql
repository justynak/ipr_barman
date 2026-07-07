-- ipr_barman database schema (English naming).
-- Recreated from the queries in databaseconnector.cpp; the original 2015
-- Polish-named schema was never checked into the repository.
--
-- Polish -> English mapping:
--   m_artykul            -> product          (nazwa->name, cena_aktualna->current_price, kategoria->category)
--   m_skladnik           -> ingredient       (nazwa->name, ilosc->quantity)
--   m_artykul_skladnik   -> product_ingredient
--   d_bar_artykul        -> bar_product      (d_numer_baru->bar_id, d_numer_artykulu->product_name)
--   d_barman             -> bartender        (pesel->card_number, imie->first_name, nazwisko->last_name)
--   d_staly_klient       -> loyal_customer   (numer_karty->card_number)
--   d_zamowienie         -> customer_order   (nr_zamowienia->order_number, data_zamowienia->order_date,
--                                             d_barman_pesel->bartender_card_number,
--                                             d_numer_karty->customer_card_number, zamkniety->closed)
--   d_zamowienie_artykul -> order_item       (d_numer_zamowienia->order_number,
--                                             d_nazwa_artykulu->product_name, ilosc->quantity)

CREATE DATABASE IF NOT EXISTS mydb CHARACTER SET utf8mb4;
USE mydb;

-- Matches the credentials hardcoded in databaseconnector.cpp; change both together.
-- CREATE USER IF NOT EXISTS 'bar'@'localhost' IDENTIFIED BY 'kelner2015';
-- GRANT ALL PRIVILEGES ON mydb.* TO 'bar'@'localhost';

CREATE TABLE bartender (
    card_number VARCHAR(36)  PRIMARY KEY,  -- currently holds PESEL; will hold UUID later
    first_name  VARCHAR(60)  NOT NULL,
    last_name   VARCHAR(60)  NOT NULL
) ENGINE=InnoDB;

CREATE TABLE loyal_customer (
    card_number VARCHAR(36) PRIMARY KEY
) ENGINE=InnoDB;

CREATE TABLE bar (
    id VARCHAR(100) PRIMARY KEY            -- e.g. 'Boboli_8_Warszawa' (LOCAL_NAME in the app)
) ENGINE=InnoDB;

CREATE TABLE product (
    name          VARCHAR(100) PRIMARY KEY,
    current_price DECIMAL(8,2) NOT NULL,
    category      VARCHAR(60)  NOT NULL
) ENGINE=InnoDB;

CREATE TABLE ingredient (
    name     VARCHAR(100) PRIMARY KEY,
    quantity INT NOT NULL DEFAULT 0
) ENGINE=InnoDB;

CREATE TABLE product_ingredient (
    product_name    VARCHAR(100) NOT NULL,
    ingredient_name VARCHAR(100) NOT NULL,
    PRIMARY KEY (product_name, ingredient_name),
    FOREIGN KEY (product_name)    REFERENCES product(name),
    FOREIGN KEY (ingredient_name) REFERENCES ingredient(name)
) ENGINE=InnoDB;

CREATE TABLE bar_product (
    bar_id       VARCHAR(100) NOT NULL,
    product_name VARCHAR(100) NOT NULL,
    PRIMARY KEY (bar_id, product_name),
    FOREIGN KEY (bar_id)       REFERENCES bar(id),
    FOREIGN KEY (product_name) REFERENCES product(name)
) ENGINE=InnoDB;

CREATE TABLE customer_order (
    order_number         INT AUTO_INCREMENT PRIMARY KEY,
    bartender_card_number VARCHAR(36) NOT NULL,
    order_date           DATE NOT NULL,
    customer_card_number VARCHAR(36) NULL,
    closed               TINYINT(1) NOT NULL DEFAULT 0,
    FOREIGN KEY (bartender_card_number) REFERENCES bartender(card_number),
    FOREIGN KEY (customer_card_number)  REFERENCES loyal_customer(card_number)
) ENGINE=InnoDB;

-- No primary key on purpose: the application currently allows the same product
-- to appear on an order as several rows. Tighten this during the refactor.
CREATE TABLE order_item (
    order_number INT          NOT NULL,
    product_name VARCHAR(100) NOT NULL,
    quantity     INT          NOT NULL,
    FOREIGN KEY (order_number) REFERENCES customer_order(order_number),
    FOREIGN KEY (product_name) REFERENCES product(name),
    INDEX idx_order_item_order (order_number)
) ENGINE=InnoDB;

-- Minimal seed data so the application starts.
INSERT INTO bar (id) VALUES ('Boboli_8_Warszawa');

INSERT INTO bartender (card_number, first_name, last_name) VALUES
    ('85010112345', 'Jan', 'Kowalski');

INSERT INTO loyal_customer (card_number) VALUES ('1001'), ('1002');

INSERT INTO ingredient (name, quantity) VALUES
    ('rum',        100),
    ('lime',       100),
    ('mint',       100),
    ('cola',       100),
    ('vodka',      100),
    ('orange juice', 100);

INSERT INTO product (name, current_price, category) VALUES
    ('Mojito',      22.00, 'Cocktails'),
    ('Cuba Libre',  20.00, 'Cocktails'),
    ('Screwdriver', 18.00, 'Cocktails'),
    ('Rum shot',    12.00, 'Shots'),
    ('Vodka shot',  10.00, 'Shots');

INSERT INTO product_ingredient (product_name, ingredient_name) VALUES
    ('Mojito',      'rum'),
    ('Mojito',      'lime'),
    ('Mojito',      'mint'),
    ('Cuba Libre',  'rum'),
    ('Cuba Libre',  'cola'),
    ('Cuba Libre',  'lime'),
    ('Screwdriver', 'vodka'),
    ('Screwdriver', 'orange juice'),
    ('Rum shot',    'rum'),
    ('Vodka shot',  'vodka');

INSERT INTO bar_product (bar_id, product_name)
    SELECT 'Boboli_8_Warszawa', name FROM product;
