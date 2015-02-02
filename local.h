#ifndef LOCAL_H
#define LOCAL_H
#include <QString>

class Local
{
private:
    QString _name;

public:
    Local();
    ~Local();

    void SetName(QString name){_name = name;}
    QString GetName(){return _name;}
};

#endif // LOCAL_H
