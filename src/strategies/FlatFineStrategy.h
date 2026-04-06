#ifndef FLAT_FINE_STRATEGY_H
#define FLAT_FINE_STRATEGY_H

#include "IFineStrategy.h"

class FlatFineStrategy : public IFineStrategy
{
private:
    double flatAmount;

public:
    FlatFineStrategy(double flatAmount);

    double calculateFine(int overdueDays) const override;
};

#endif