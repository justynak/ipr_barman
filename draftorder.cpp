#include "draftorder.h"

void DraftOrder::AddProduct(const Product& p, int quantity)
{
    if(quantity <= 0)
        return;

    for(int i = 0; i < _lines.size(); ++i)
    {
        if(_lines[i].productId == p.GetId())
        {
            _lines[i].quantity += quantity;
            return;
        }
    }

    _lines.append(OrderLine(p.GetId(), p.GetName(), p.GetPrice(), quantity));
}

bool DraftOrder::ChangeQuantity(int productId, int quantity)
{
    if(quantity <= 0)
        return RemoveProduct(productId);

    for(int i = 0; i < _lines.size(); ++i)
    {
        if(_lines[i].productId == productId)
        {
            _lines[i].quantity = quantity;
            return true;
        }
    }
    return false;
}

bool DraftOrder::RemoveProduct(int productId)
{
    for(int i = 0; i < _lines.size(); ++i)
    {
        if(_lines[i].productId == productId)
        {
            _lines.removeAt(i);
            return true;
        }
    }
    return false;
}

Money DraftOrder::Subtotal() const
{
    Money sum = 0;
    for(const OrderLine& line : _lines)
        sum += line.unitPrice * line.quantity;
    return sum;
}

Money DraftOrder::Total() const
{
    return applyDiscount(Subtotal(), _discountRate);
}
