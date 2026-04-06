#include "DailyFineStrategy.h"

DailyFineStrategy::DailyFineStrategy(double finePerDay): finePerDay(finePerDay){}


double DailyFineStrategy::calculateFine(int overdueDays) const
{
    return overdueDays * finePerDay;
}