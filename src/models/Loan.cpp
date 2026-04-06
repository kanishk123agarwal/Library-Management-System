#include "Loan.h"

Loan::Loan(int overdueDays,std::shared_ptr<IFineStrategy> fineStrategy): overdueDays(overdueDays),fineStrategy(fineStrategy){}


double Loan::calculateFine() const
{
    return fineStrategy->calculateFine(overdueDays);
}