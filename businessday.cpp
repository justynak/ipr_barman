#include "businessday.h"

QDate businessDay(const QDateTime& moment, int cutoffHour)
{
    if(moment.time().hour() < cutoffHour)
        return moment.date().addDays(-1);
    return moment.date();
}
