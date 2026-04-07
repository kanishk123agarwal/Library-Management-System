#ifndef LOAN_H
#define LOAN_H

#include <memory>

#include "../strategies/IFineStrategy.h"

class Loan
{
private:
    int overdueDays;
    std::shared_ptr<IFineStrategy> fineStrategy;

    int loanId;
    int bookId;
    int memberId;

public:
    Loan(int overdueDays,std::shared_ptr<IFineStrategy> fineStrategy);

    Loan(int loanId,int bookId,int memberId);

    int getLoanId() const;

    int getBookId() const;

    int getMemberId() const;

    double calculateFine() const;
};

#endif