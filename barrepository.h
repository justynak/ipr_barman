#ifndef BARREPOSITORY_H
#define BARREPOSITORY_H

#include <QDate>
#include <QDateTime>
#include <QList>
#include <QString>

#include "customer.h"
#include "draftorder.h"
#include "employee.h"
#include "product.h"

// Data-access seam for the application (schema v2). The production
// implementation is MySqlBarRepository; tests use FakeBarRepository.
class BarRepository
{
public:
    virtual ~BarRepository() {}

    // employees & shifts
    virtual Employee FindEmployeeByCard(QString cardNumber) = 0;
    virtual QList<QString> GetEmployeeCardNumbers() = 0;
    virtual int OpenShift(int employeeId, QDateTime openedAt) = 0;      // returns shift id, 0 on failure
    virtual bool CloseShift(int shiftId, QDateTime closedAt) = 0;

    // products (availability derived from the ingredient ledger + recipes)
    virtual QList<QString> GetCategories() = 0;
    virtual QList<Product> GetProductsFromCategory(QString category) = 0;

    // customers
    virtual Customer FindCustomerByCard(QString cardNumber) = 0;
    virtual QList<QString> GetCustomerCardNumbers() = 0;

    // The only write path for orders: one call, one transaction. Persists the
    // order row (business_day, discount_rate and currency copies), its
    // order_item lines (product_name / unit_price copies) and one negative
    // ingredient_movement per line-ingredient (recipe amount × quantity).
    virtual bool FinalizeOrder(const DraftOrder& draft, int shiftId,
                               QDate businessDay, QDateTime createdAt) = 0;
};

#endif // BARREPOSITORY_H
