#ifndef MONEY_H
#define MONEY_H

#include <QString>
#include <QtGlobal>

// Money is an integer number of cents (grosze). The DB stores DECIMAL(8,2);
// the code never does arithmetic on floating-point amounts.
typedef qint64 Money;

// Rounding policy: half away from zero, once, at the final amount.
inline Money moneyFromDouble(double amount)
{
    return qRound64(amount * 100.0);
}

// Parses a DECIMAL string coming from the DB ("22.00").
inline Money moneyFromDecimalString(const QString& s)
{
    return qRound64(s.toDouble() * 100.0);
}

// Formats for the DB / display: "22.00".
inline QString moneyToDecimalString(Money m)
{
    QString sign = m < 0 ? "-" : "";
    qint64 absolute = m < 0 ? -m : m;
    return QString("%1%2.%3")
            .arg(sign)
            .arg(absolute / 100)
            .arg(absolute % 100, 2, 10, QChar('0'));
}

inline Money applyDiscount(Money amount, double rate)
{
    return qRound64(amount * (1.0 - rate));
}

#endif // MONEY_H
