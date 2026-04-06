#include "FlatFineStrategy.h"

FlatFineStrategy::FlatFineStrategy(double flatAmount): flatAmount(flatAmount){}

double FlatFineStrategy::calculateFine(int overdueDays) const
{
    return overdueDays > 0 ? flatAmount : 0;
}