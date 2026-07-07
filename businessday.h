#ifndef BUSINESSDAY_H
#define BUSINESSDAY_H

#include <QDate>
#include <QDateTime>

// A bar's "day" does not end at midnight: everything sold before the cutoff
// hour still belongs to the previous calendar day's business day.
const int BUSINESS_DAY_CUTOFF_HOUR = 6;

QDate businessDay(const QDateTime& moment, int cutoffHour = BUSINESS_DAY_CUTOFF_HOUR);

#endif // BUSINESSDAY_H
