#ifndef FAKEBARREPOSITORY_H
#define FAKEBARREPOSITORY_H

#include <QMap>
#include <QStringList>

#include <utility>

#include "barrepository.h"

// In-memory BarRepository mirroring schema v2: employees/shifts, customers,
// a product catalog with recipes, and an append-only movement ledger from
// which stock (and product availability) is derived — same arithmetic as the
// ingredient_stock view.
class FakeBarRepository : public BarRepository
{
public:
    struct RecipeLine
    {
        int ingredientId = 0;
        double amount = 0;

        RecipeLine() = default;
        RecipeLine(int id, double a) : ingredientId(id), amount(a) {}
    };

    struct Movement
    {
        int ingredientId = 0;
        double amount = 0;      // negative = consumed
        QString reason;
        int orderItemId = 0;    // 0 = none
        QDateTime occurredAt;
    };

    struct FinalizedItem
    {
        int id = 0;
        int productId = 0;
        QString productName;    // copy at sale time
        Money unitPrice = 0;    // copy at sale time
        int quantity = 0;
    };

    struct FinalizedOrder
    {
        int shiftId = 0;
        QDate businessDay;
        int customerId = 0;     // 0 = none
        double discountRate = 0;
        QString currency;       // copy at sale time
        QDateTime createdAt;
        QList<FinalizedItem> items;
    };

    struct Shift
    {
        int id = 0;
        int employeeId = 0;
        QDateTime openedAt;
        QDateTime closedAt;
        bool closed = false;
    };

    QMap<QString, Employee> employees;          // card -> employee
    QMap<QString, int> customers;               // card -> id
    QString currency = "PLN";

    QList<Product> catalog;                     // availability field ignored; derived
    QMap<int, QList<RecipeLine> > recipes;      // product id -> recipe
    QMap<int, double> openingStock;             // ingredient id -> delivered amount
    QList<Movement> movements;                  // the ledger

    QList<Shift> shifts;
    QList<FinalizedOrder> finalizedOrders;

    // -- test setup helpers --------------------------------------------------

    void AddEmployee(int id, QString role, QString card, QString name, QString surname)
    {
        Employee e;
        e.id = id;
        e.role = role;
        e.cardNumber = card;
        e.firstName = name;
        e.lastName = surname;
        employees[card] = e;
    }

    void AddCustomer(int id, QString card)
    {
        customers[card] = id;
    }

    void AddIngredient(int id, double delivered)
    {
        openingStock[id] = delivered;
    }

    void AddProduct(const Product& p, const QList<RecipeLine>& recipe)
    {
        catalog.append(p);
        recipes[p.GetId()] = recipe;
    }

    double CurrentStock(int ingredientId) const
    {
        double stock = openingStock.value(ingredientId, 0);
        for(const Movement& m : movements)
        {
            if(m.ingredientId == ingredientId)
                stock += m.amount;
        }
        return stock;
    }

    // -- BarRepository -------------------------------------------------------

    Employee FindEmployeeByCard(QString cardNumber) override
    {
        return employees.value(cardNumber, Employee());
    }

    QList<QString> GetEmployeeCardNumbers() override
    {
        return employees.keys();
    }

    int OpenShift(int employeeId, QDateTime openedAt) override
    {
        Shift s;
        s.id = shifts.size() + 1;
        s.employeeId = employeeId;
        s.openedAt = openedAt;
        s.closed = false;
        shifts.append(s);
        return s.id;
    }

    bool CloseShift(int shiftId, QDateTime closedAt) override
    {
        for(int i = 0; i < shifts.size(); ++i)
        {
            if(shifts[i].id == shiftId)
            {
                shifts[i].closedAt = closedAt;
                shifts[i].closed = true;
                return true;
            }
        }
        return false;
    }

    QList<QString> GetCategories() override
    {
        QList<QString> categories;
        for(const Product& p : std::as_const(catalog))
        {
            if(!categories.contains(p.GetCategory()))
                categories.append(p.GetCategory());
        }
        return categories;
    }

    QList<Product> GetProductsFromCategory(QString category) override
    {
        QList<Product> list;
        for(const Product& p : std::as_const(catalog))
        {
            if(p.GetCategory() != category)
                continue;

            // floor(min(stock / amount)) over the recipe — the same
            // derivation the ingredient_stock view query performs.
            int available = 0;
            bool first = true;
            const QList<RecipeLine> recipe = recipes.value(p.GetId());
            for(const RecipeLine& line : recipe)
            {
                int possible = (int)(CurrentStock(line.ingredientId) / line.amount);
                if(possible < 0)
                    possible = 0;
                if(first || possible < available)
                    available = possible;
                first = false;
            }

            list.append(Product(p.GetId(), p.GetName(), p.GetPrice(),
                                p.GetCategory(), first ? 0 : available));
        }
        return list;
    }

    Customer FindCustomerByCard(QString cardNumber) override
    {
        Customer c;
        if(!customers.contains(cardNumber))
            return c;
        c.id = customers[cardNumber];
        c.cardNumber = cardNumber;
        return c;
    }

    QList<QString> GetCustomerCardNumbers() override
    {
        return customers.keys();
    }

    QString GetBarCurrency() override
    {
        return currency;
    }

    bool FinalizeOrder(const DraftOrder& draft, int shiftId,
                       QDate businessDay, QDateTime createdAt) override
    {
        if(draft.IsEmpty())
            return false;

        FinalizedOrder order;
        order.shiftId = shiftId;
        order.businessDay = businessDay;
        order.customerId = draft.HasCustomer() ? draft.GetCustomerId() : 0;
        order.discountRate = draft.GetDiscountRate();
        order.currency = currency;
        order.createdAt = createdAt;

        const QList<OrderLine> lines = draft.GetLines();
        for(const OrderLine& line : lines)
        {
            FinalizedItem item;
            item.id = _nextOrderItemId++;
            item.productId = line.productId;
            item.productName = line.productName;
            item.unitPrice = line.unitPrice;
            item.quantity = line.quantity;
            order.items.append(item);

            const QList<RecipeLine> recipe = recipes.value(line.productId);
            for(const RecipeLine& recipeLine : recipe)
            {
                Movement m;
                m.ingredientId = recipeLine.ingredientId;
                m.amount = -(recipeLine.amount * line.quantity);
                m.reason = "order";
                m.orderItemId = item.id;
                m.occurredAt = createdAt;
                movements.append(m);
            }
        }

        finalizedOrders.append(order);
        return true;
    }

private:
    int _nextOrderItemId = 1;
};

#endif // FAKEBARREPOSITORY_H
