# Refactoring plan — ipr_barman → schema v2

Target: migrate the 2015 Qt bar POS from `schema.sql` (v1, name-keyed, mutable
stock counters) to `schema_v2.sql` (surrogate ids, in-memory draft orders,
single-transaction finalize, append-only ingredient ledger), with a test
harness in place *before* the behaviour-changing work starts.

## Status (2026-07-07)

Phases 0–4 are implemented, each as one PR in a stacked chain (merge in
order, or retarget each PR's base to `master` as its predecessor merges):

| Phase | PR | Branch | State |
|-------|----|--------|-------|
| 0 — test harness | #2 | `test/qttest-harness` | done, CI green |
| 1 — pin domain, fix UB | #3 | `refactor/phase1-domain-tests` | done, CI green |
| 2 — repository seam | #4 | `refactor/phase2-repository-seam` | done |
| 3 — schema v2 migration | #5 | `refactor/phase3-schema-v2` | done |
| 4 — UI cleanup | #6 | `refactor/phase4-ui-cleanup` | done |
| 5 — modernization sweep | #7 | `refactor/phase5-modernization` | done |

Deviations from / additions to the original plan, discovered while
implementing:

- Phase 1 also fixed a B7-class bug the plan missed: the `Bartender`
  *default* ctor left `_oManager` uninitialized while the dtor deleted it.
- Phase 2 already moved `ProductManager`'s selected product to value
  semantics (the plan had no owner for that dangling-pointer risk), removed
  the dead `CategoryList::GetProductsFromDB` / `OrderManager::RefreshData` /
  `ProductManager::ChangeProductNumber`, and made `ScanCustomer` grant the
  discount only after `CustomerExists` — pulling part of the phase-3
  "validated card" rule forward.
- Phase 3: `DraftOrder::AddProduct` *merges* lines with the same product id
  (v1 appended duplicate rows); quantity ≤ 0 in `ChangeQuantity` removes the
  line. `Product` keeps a derived `available` field filled by the repository
  query. The optional MySQL-service-container CI job was added later as
  PR #8 (`test/mysql-integration-ci`): `tst_mysql_repository` runs against a
  `mariadb:11` service with `schema_v2.sql` loaded, opt-in via
  `BARMAN_MYSQL_TEST=1` so the unit-test job stays DB-free.
- Phase 4 renamed all slots away from the `on_<object>_<signal>` pattern:
  the old code both auto-connected them (`connectSlotsByName`) *and*
  connected some explicitly, so several handlers fired twice per click.
  `ManageProductWindow` lost its approve-time stock mutation (stock is
  ledger-derived since phase 3) and its unused `productSelected` member;
  `DetailsWindow` takes the currency as a ctor argument.
- Money display everywhere is `moneyToDecimalString()` + `bar.currency`
  (phase 3 groundwork made this trivial in phase 4).
- Phase 5 additions beyond the listed items: `QT_NO_FOREACH` is defined
  `PUBLIC` on `barman_core` so `foreach` cannot creep back in; three window
  constructors had their init lists reordered to match member declaration
  order (latent `-Wreorder`); `EditWindow`'s `#define COL_*` became
  `constexpr int`; empty destructors deleted (Rule of Zero) and interface
  destructors `= default`. Range-for over Qt containers uses
  `std::as_const()` on members and const locals for returned temporaries, to
  avoid detach. Builds clean with `-Wall -Wextra`.

## 1. Current state assessment

Layering is actually recognizable — UI (`*window.cpp`) → facade (`Bartender`)
→ managers (`OrderManager`, `ProductManager`) → domain (`Order`, `Product`,
`OrderDetails`) → `DatabaseConnector` singleton. The problems are in the
seams: raw-pointer ownership, DB writes scattered through every user action,
and a data-access class that is untestable and unsafe.

### 1.1 Outright bugs (fix, don't preserve)

| # | Where | Bug |
|---|-------|-----|
| B1 | `databaseconnector.cpp` (all queries) | SQL built with `QString::arg`; no prepared statements. A product name containing `'` or `"` breaks or injects. |
| B2 | `DatabaseConnector::ChangeProductNumber` (databaseconnector.cpp:247) | Reads `q.value(0)` from an UPDATE (no result set); the second query interpolates `p.GetPrice()` where the product *name* belongs; and it sets ingredient stock to an absolute value instead of adjusting it. The whole function is broken. |
| B3 | `AddProductToOrder` / `RemoveProductFromOrder` | Stock adjusted by the *order quantity* for **every** ingredient of the product, ignoring recipe amounts (v1 has none — v2's `product_ingredient.amount` fixes the model). Delete also matches rows by `(order, name, quantity)`, so a row whose quantity drifted from the UI value is silently never deleted. |
| B4 | `Order::ChangeProductNumber` (order.cpp) | `uint occurence = _productList->indexOf(p)` — `indexOf` returns `-1` as `int`; stored in `uint` it wraps, `occurence >= 0` is always true → out-of-bounds write when the product isn't found. |
| B5 | `OrderDetails::OrderDetails()` default ctor | Leaves `_order`, `_cost`, `_discount` uninitialized. `OrderManager`'s ctor creates exactly this object, and `EditWindow` calls `GetSelectedOrderCost()` → UB before the first order is selected. |
| B6 | `OrderManager::OrderManager` | `_selectedOrder = &(_orderList->GetOrder(0))` — dereferences element 0 of a possibly empty list (bartender with no orders → crash at login). |
| B7 | `MainWindow::~MainWindow` | `delete _bartender` on an uninitialized pointer if the app closes before login. |
| B8 | `Bartender::SetSurname` | Assigns to `_name` instead of `_surname`. |
| B9 | `Order` ownership | `_productList` is an owning raw pointer, but `Order` is copied by value into `QList<Order>` with the default copy ctor (shared pointer, no deep copy) and the destructor is commented out. `SetProductList` clears the old list then overwrites the pointer (leak). `OrderList::AddOrder` and `DatabaseConnector::CreateOrder` leak too. |
| B10 | `GetRandomBartender` / `GetRandomCustomerID` | `qrand() % list.size()` — division by zero on an empty table; `qsrand`/`qrand` are removed in Qt 6. |
| B11 | `EditWindow::on_button_add_product_clicked` | Runs `_bartender->AddProduct()` even when the dialog was *discarded* — inserts a row with empty name / quantity 0 into `order_item`. |
| B12 | `ManageProductWindow::on_box_products_activated` | `GetProductByName` may return `NULL`; dereferenced immediately. |
| B13 | `OrderManager::CloseOrder` + `GetOrders` | Orders are closed but `GetOrders` never filters on `closed`, so paid bills reappear in the combo box after re-login. (Moot under v2 — drafts never hit the DB.) |
| B14 | `OrderDetails` ctor | `if(GetCustomerID() != "" || GetCustomerID() != "")` — duplicated condition; discount also granted by `ScanCustomer` unconditionally, even if `CustomerExists` returned false. |

### 1.2 Structural debt (the refactor proper)

- **`DatabaseConnector` singleton**: hardcoded credentials, `#define LOCAL_NAME`,
  `QMessageBox` inside `connect()` (GUI in the data layer), reachable from every
  class — the reason nothing is unit-testable today.
- **DB writes on every click**: add/remove/± product each fire 2 UPDATEs, no
  transactions anywhere. v2's model (draft in memory, one transaction at
  finalize) removes this entire class of partial-write bugs.
- **Name-keyed domain**: `Product`/`Order` identified by name/number strings;
  v2 uses surrogate ids and copies `product_name`/`unit_price` into
  `order_item` at sale time.
- **Money as `double`**, `#define DISCOUNT 0.1` duplicated in two files.
- **UI duplication**: the 15-line product-table setup block is pasted 4× in
  `editwindow.cpp`; string-based `SIGNAL(activated(QString))` connects (that
  overload is gone in Qt 6).
- **Dead weight**: `CategoryList::GetProductsFromDB` (empty), `OrderManager::RefreshData`
  (empty), `ProductManager::ChangeProductNumber` (operates on a by-value copy —
  no effect), `Order::_date` (never read), `ipr_barman.pro` (CMake is
  authoritative), `dialog.ui` (unreferenced), `order.h`'s include in
  `productmanager.h`.
- **Scanners are stubs** (`LoginScanner`, `CustomerScanner` pick a random DB
  row). Fine as simulators, but they should sit behind an interface so tests
  and real hardware can substitute them.

## 2. What to cover with tests **now** (before touching behaviour)

The branch (`test/qttest-harness`) is the right first step: QtTest + CTest,
no DB required. Pure-logic classes are testable as soon as the code is split
into a library. These tests pin current behaviour and immediately expose
B4/B5/B9 (write the tests to the *intended* behaviour and fix those three —
UB cannot be meaningfully pinned):

1. **`Product`** — equality semantics (name+number, price ignored — pin it,
   it's what `removeOne` relies on), getters/setters.
2. **`Order`** — `AddProduct`, `RemoveProduct` (incl. remove-non-existent),
   `ChangeProductNumber` (incl. product-not-found → B4), copy behaviour (B9).
3. **`OrderDetails`** — cost = Σ quantity×price over the list; empty list;
   discount set only for non-empty customer id; `RecalculateCost` after
   mutation; discounted price math currently in `DetailsWindow` (move it into
   `OrderDetails` so it's testable — the view just formats).
4. **Discount constant** — single definition, referenced from both call sites.

Not worth testing yet (will be rewritten or need seams first):
`DatabaseConnector` (rewritten in phase 3), managers (need the repository
seam), window classes (thin them instead).

## 3. Phased plan

Each phase is one PR, green CI, no behaviour change unless listed.

### Phase 0 — Test harness (this branch)
- Split CMake into `barman_core` static library (domain + managers +
  databaseconnector) and the `ipr_barman` executable (main + windows + .ui).
- Add `tests/` with QtTest (`find_package(Qt5 COMPONENTS Test)`), `enable_testing()`,
  one smoke test target per tested class, wired into CTest.
- CI: add `ctest --test-dir build --output-on-failure` step (the workflow
  already has the placeholder comment).
- Delete `ipr_barman.pro` and `dialog.ui`.

### Phase 1 — Pin the domain, fix the UB
- Write the §2 test suite.
- Fix B4 (int, guard `-1`), B5 (initialize members / require an order), B7,
  B8, B14 (single condition), B6 (no order selected = empty state, not
  element 0).
- Fix B9 by giving `Order` value semantics: `QList<Product> _productList`
  (member, not pointer), Rule of Zero. `GetProductsFromBill` returns by value.
  This deletes every `new`/commented-out-destructor in the domain layer.

### Phase 2 — Repository seam (kills the singleton)
- Define `BarRepository` (abstract): domain-shaped methods —
  `findEmployeeByCard`, `productsInCategory`, `categories`, `saveOrder(...)`, …
- `DatabaseConnector` becomes `MySqlBarRepository implements BarRepository`;
  all queries converted to prepared statements with `bindValue` (kills B1),
  the multi-statement writes wrapped in `QSqlDatabase::transaction()`.
  Connection settings move to a config struct (host/db/user/password/bar
  name) — no more `#define LOCAL_NAME`, no `QMessageBox` (return
  `QSqlError`/expected-style result; the UI shows the message).
- Constructor-inject the repository: `MainWindow → Bartender → OrderManager →
  ProductManager`. Singleton `GetInstance()` deleted.
- Add `FakeBarRepository` (in-memory maps) in `tests/` and cover the managers:
  - `OrderManager`: create/select/delete order, add/remove/change product,
    cost recalculation, close order — including B2/B3 replacements.
  - `ProductManager`: category → products, `GetProductByName` null path (B12).
  - `LoginScanner`/`CustomerScanner` behind a `CardScanner` interface with a
    scripted fake (also fixes B10 by using `QRandomGenerator` in the simulator).

### Phase 3 — Migrate the domain to schema v2 (the behaviour change)
Model changes, driven by the v2 design notes:
- **Ids**: `Employee{id, role, card}`, `Customer{id, card}`, `Product{id, name,
  price, category}`; domain stops keying on names. Money handled as `qint64`
  cents (or at minimum a single `Money` typedef + rounding policy) — DECIMAL
  in DB, integers in code.
- **Shift**: login opens a shift (`openShift(employee)`), logout/exit closes
  it; orders reference `shift_id`.
- **Draft orders live in memory**: `OrderManager` keeps `QList<DraftOrder>`;
  add/remove/± product and customer scan touch *only* memory. `CloseOrder`
  becomes `finalizeOrder(draft)` → one repository call, one DB transaction:
  `INSERT customer_order` (business_day, discount_rate copy, currency copy)
  + `INSERT order_item` per line (product_name + unit_price copies)
  + `INSERT ingredient_movement` per line-ingredient (−amount × quantity,
  reason `'order'`). Deleting a draft is a memory operation — `RemoveOrder`/
  `CloseOrder` DB methods disappear (fixes B13 by construction).
- **Stock**: availability = `ingredient_stock` view ⊗ recipe amounts
  (`floor(min(stock / amount))` per product) — computed in one query;
  replaces v1's `min(quantity)` and the broken counter updates (B2/B3 gone).
- **business_day**: one function `businessDay(QDateTime)` with the cutoff rule
  (e.g. 6:00), unit-tested around the boundary; stamped at finalize.
- **Discount**: rate copied onto the draft when a *validated* customer card is
  scanned; total derived, never stored.
- Repository: `MySqlBarRepository` rewritten against `barman_v2`;
  `FakeBarRepository` updated the same way. Tests to write in this phase:
  finalize composes the right rows (assert against the fake), ingredient
  movements match recipe×quantity, business-day cutoff, discount copy,
  stock-derivation arithmetic, money rounding.
- Optional but recommended: a small integration test job in CI using the
  `mysql` service container + `schema_v2.sql`, running the repository tests
  against real MySQL (driver: `sudo apt-get install libqt5sql5-mysql`). Keeps
  unit tests DB-free, catches SQL typos the fake can't.

### Phase 4 — UI cleanup
- Extract `setupProductTable()`/`refreshBill()` helpers in `EditWindow`
  (deletes the 4× duplication); refresh from a single `updateSummaryLabels()`.
- `ManageProductWindow` returns an accept/reject result and the chosen
  product; `EditWindow` adds the product **only on accept** (fixes B11).
- New-style connects everywhere (removes the Qt6-blocking
  `SIGNAL(activated(QString))`); `qsrand/qrand` already gone in phase 2.
- Show shift/employee (v2 vocabulary) instead of bartender-PESEL wording;
  currency label from `bar.currency` instead of the hardcoded `zł`.

### Phase 5 — Modernization sweep (optional, cheap after the above)
- C++17 in CMake, `nullptr`, range-for instead of `foreach`, `override`,
  member-init in headers.
- Then a Qt 6 build is one `find_package` away if ever wanted.

## 4. Suggested test inventory (summary)

| Phase | Suite | Backing |
|-------|-------|---------|
| 1 | `tst_product`, `tst_order`, `tst_orderdetails` | none (pure) |
| 2 | `tst_ordermanager`, `tst_productmanager`, `tst_scanners` | `FakeBarRepository` |
| 3 | `tst_draftorder`, `tst_finalize`, `tst_stock`, `tst_businessday`, `tst_money` | fake |
| 3 (CI opt-in) | `tst_mysql_repository` | MySQL service container + `schema_v2.sql` |

## 5. Risks / decisions taken

- **Bar tabs don't survive a restart** under v2's draft-in-memory model — the
  schema notes flag this deliberately. If that's unacceptable, add a
  `draft_order` table later; the repository seam makes it a contained change.
- v1 behaviours listed in §1.1 are treated as bugs and fixed, not preserved;
  everything else is pinned by tests before being moved.
- No data migration script v1→v2 is planned (university project, seed data
  only). If real data existed, add a `migrate_v1_to_v2.sql` in phase 3.
