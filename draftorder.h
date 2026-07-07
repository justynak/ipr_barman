#ifndef DRAFTORDER_H
#define DRAFTORDER_H

#include <QList>
#include <QString>

#include "money.h"
#include "product.h"

// Discount rate copied onto a draft when a validated loyal-customer card is
// scanned. Single definition for the whole application.
const double LOYAL_CUSTOMER_DISCOUNT = 0.1;

// One line of a draft order. product_name and unit_price are copies taken at
// the moment the line is added — exactly what order_item stores at finalize.
struct OrderLine
{
    int productId = 0;
    QString productName;
    Money unitPrice = 0;
    int quantity = 0;

    OrderLine() = default;
    OrderLine(int id, QString name, Money price, int qty)
        : productId(id), productName(name), unitPrice(price), quantity(qty) {}
};

// A bill being composed at the bar. Lives purely in memory; the DB sees it
// only once, as a single transaction, when it is finalized.
class DraftOrder
{
private:
    QString _label;
    int _customerId = 0;
    QString _customerCard;
    double _discountRate = 0.0;
    QList<OrderLine> _lines;

public:
    DraftOrder() = default;
    explicit DraftOrder(QString label) : _label(label) {}

    QString GetLabel() const { return _label; }

    // Customer / discount: the rate is copied onto the draft when a validated
    // customer card is scanned, and travels with the order into the DB.
    void SetCustomer(int customerId, QString cardNumber, double discountRate)
    {
        _customerId = customerId;
        _customerCard = cardNumber;
        _discountRate = discountRate;
    }
    bool HasCustomer() const { return _customerId > 0; }
    int GetCustomerId() const { return _customerId; }
    QString GetCustomerCard() const { return _customerCard; }
    double GetDiscountRate() const { return _discountRate; }

    void AddProduct(const Product& p, int quantity);
    bool ChangeQuantity(int productId, int quantity);
    bool RemoveProduct(int productId);

    QList<OrderLine> GetLines() const { return _lines; }
    bool IsEmpty() const { return _lines.isEmpty(); }

    Money Subtotal() const;
    Money Total() const;    // discount applied, rounded once
};

#endif // DRAFTORDER_H
