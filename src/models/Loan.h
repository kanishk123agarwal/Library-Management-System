#ifndef LOAN_H
#define LOAN_H

#include <memory>

#include "../strategies/IFineStrategy.h"

class Loan
{
private:
    int overdueDays;
    std::shared_ptr<IFineStrategy> fineStrategy;

public:
    Loan(int overdueDays,std::shared_ptr<IFineStrategy> fineStrategy);

    double calculateFine() const;
};

#endif