#ifndef DAILY_FINE_STRATEGY_H
#define DAILY_FINE_STRATEGY_H

#include "IFineStrategy.h"

class DailyFineStrategy : public IFineStrategy
{
private:
    double finePerDay;

public:
    DailyFineStrategy(double finePerDay);

    double calculateFine(int overdueDays) const override;
};

#endif