#ifndef FAKEBARREPOSITORY_H
#define FAKEBARREPOSITORY_H

#include <QMap>
#include <QStringList>

#include "barrepository.h"

// In-memory BarRepository for unit tests. Mirrors the v1 model: orders hold
// item lines; ingredient stock is not modelled (v2 replaces it with a ledger),
// so stock-affecting calls only record the item changes.
class FakeBarRepository : public BarRepository
{
public:
    struct FakeOrder
    {
        QString bartender;
        QString customerID;
        bool closed;
        QList<Product> items;

        FakeOrder() : closed(false) {}
    };

    QMap<QString, QString> bartenderNames;      // card -> first name
    QMap<QString, QString> bartenderSurnames;   // card -> last name
    QMap<QString, QList<Product> > productsByCategory;
    QStringList customerCards;
    QMap<QString, FakeOrder> orders;            // order number -> order

    FakeBarRepository() : _nextOrderNumber(1) {}

    void AddBartender(QString card, QString name, QString surname)
    {
        bartenderNames[card] = name;
        bartenderSurnames[card] = surname;
    }

    bool BartenderExists(QString cardNumber)
    {
        return bartenderNames.contains(cardNumber);
    }

    QString GetBartenderName(QString cardNumber)
    {
        return bartenderNames.value(cardNumber, QString(""));
    }

    QString GetBartenderSurname(QString cardNumber)
    {
        return bartenderSurnames.value(cardNumber, QString(""));
    }

    QList<QString> GetBartenderCardNumbers()
    {
        return bartenderNames.keys();
    }

    QList<QString> GetCategories()
    {
        return productsByCategory.keys();
    }

    QList<Product> GetProductsFromCategory(QString category)
    {
        return productsByCategory.value(category);
    }

    bool CustomerExists(QString cardNumber)
    {
        return customerCards.contains(cardNumber);
    }

    QList<QString> GetCustomerCardNumbers()
    {
        return customerCards;
    }

    bool SetCustomerIDinOrder(QString cardNumber, QString orderNumber)
    {
        if(!orders.contains(orderNumber))
            return false;
        orders[orderNumber].customerID = cardNumber;
        return true;
    }

    QList<Order> GetOrders(QString bartenderNumber)
    {
        QList<Order> list;
        foreach(QString number, orders.keys())
        {
            const FakeOrder& fake = orders[number];
            if(fake.bartender != bartenderNumber)
                continue;
            Order o(number);
            o.SetCustomerID(fake.customerID);
            o.SetProductList(fake.items);
            list.append(o);
        }
        return list;
    }

    QList<Product> GetProductsFromBill(QString billNumber)
    {
        if(!orders.contains(billNumber))
            return QList<Product>();
        return orders[billNumber].items;
    }

    bool CreateOrder(QString bartenderNumber, Order* newOrder)
    {
        QString number = QString::number(_nextOrderNumber++);
        FakeOrder fake;
        fake.bartender = bartenderNumber;
        orders[number] = fake;
        newOrder->SetOrderNumber(number);
        return true;
    }

    bool RemoveOrder(QString billNumber)
    {
        return orders.remove(billNumber) > 0;
    }

    bool CloseOrder(QString billNumber)
    {
        if(!orders.contains(billNumber))
            return false;
        orders[billNumber].closed = true;
        return true;
    }

    bool AddProductToOrder(QString billNumber, Product p)
    {
        if(!orders.contains(billNumber))
            return false;
        orders[billNumber].items.append(p);
        return true;
    }

    bool RemoveProductFromOrder(QString billNumber, Product p)
    {
        if(!orders.contains(billNumber))
            return false;

        QList<Product>& items = orders[billNumber].items;
        for(int i = 0; i < items.size(); ++i)
        {
            if(items[i].GetName() == p.GetName())
            {
                items.removeAt(i);
                return true;
            }
        }
        return false;
    }

    bool ChangeProductNumber(QString billNumber, Product p)
    {
        if(!orders.contains(billNumber))
            return false;

        QList<Product>& items = orders[billNumber].items;
        for(int i = 0; i < items.size(); ++i)
        {
            if(items[i].GetName() == p.GetName())
            {
                items[i].SetNumber(p.GetNumber());
                return true;
            }
        }
        return false;
    }

private:
    int _nextOrderNumber;
};

#endif // FAKEBARREPOSITORY_H
