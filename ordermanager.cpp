#include "ordermanager.h"
#include "businessday.h"


OrderManager::OrderManager(BarRepository* repository, CardScanner* customerScanner)
{
    db = repository;
    _customerScanner = customerScanner;

    _selected = -1;
    _nextDraftNumber = 1;

    _pManager = new ProductManager(db);
}

OrderManager::~OrderManager()
{
    delete _pManager;
}

DraftOrder* OrderManager::Selected()
{
    if(_selected < 0 || _selected >= _drafts.size())
        return NULL;
    return &_drafts[_selected];
}

QList<QString> OrderManager::GetOrders()
{
    QList<QString> labels;
    foreach(const DraftOrder& d, _drafts)
        labels.append(d.GetLabel());
    return labels;
}

bool OrderManager::CreateOrder()
{
    _drafts.append(DraftOrder(QString::number(_nextDraftNumber++)));
    _selected = _drafts.size() - 1;
    return true;
}

bool OrderManager::SetSelectedOrder(QString label)
{
    for(int i = 0; i < _drafts.size(); ++i)
    {
        if(_drafts[i].GetLabel() == label)
        {
            _selected = i;
            return true;
        }
    }
    return false;
}

bool OrderManager::DeleteOrder()
{
    if(Selected() == NULL) return false;

    _drafts.removeAt(_selected);
    _selected = -1;
    return true;
}

bool OrderManager::AddProduct()
{
    DraftOrder* d = Selected();
    if(d == NULL) return false;

    Product p = _pManager->GetSelectedProduct();
    int quantity = _pManager->GetSelectedQuantity();

    if(!p.IsValid() || quantity <= 0)
        return false;

    d->AddProduct(p, quantity);
    return true;
}

bool OrderManager::ChangeProductQuantity(int productId, int quantity)
{
    DraftOrder* d = Selected();
    if(d == NULL) return false;

    return d->ChangeQuantity(productId, quantity);
}

bool OrderManager::DeleteProduct(int productId)
{
    DraftOrder* d = Selected();
    if(d == NULL) return false;

    return d->RemoveProduct(productId);
}

bool OrderManager::ScanCustomer()
{
    DraftOrder* d = Selected();
    if(d == NULL) return false;

    QString cardNumber = _customerScanner->ScanCard();
    if(cardNumber == "")
        return false;

    // The discount rate is copied onto the draft only for a validated card.
    Customer customer = db->FindCustomerByCard(cardNumber);
    if(!customer.IsValid())
        return false;

    d->SetCustomer(customer.id, cardNumber, LOYAL_CUSTOMER_DISCOUNT);
    return true;
}

bool OrderManager::FinalizeOrder(int shiftId)
{
    DraftOrder* d = Selected();
    if(d == NULL || d->IsEmpty()) return false;

    QDateTime now = QDateTime::currentDateTime();

    if(!db->FinalizeOrder(*d, shiftId, businessDay(now), now))
        return false;

    _drafts.removeAt(_selected);
    _selected = -1;
    return true;
}

DraftOrder OrderManager::GetSelectedOrder()
{
    DraftOrder* d = Selected();
    if(d == NULL)
        return DraftOrder();
    return *d;
}

QString OrderManager::GetSelectedOrderNumber()
{
    DraftOrder* d = Selected();
    return d != NULL ? d->GetLabel() : QString("");
}

QString OrderManager::GetCustomerID()
{
    DraftOrder* d = Selected();
    return d != NULL ? d->GetCustomerCard() : QString("");
}

Money OrderManager::GetCost()
{
    DraftOrder* d = Selected();
    return d != NULL ? d->Subtotal() : 0;
}
