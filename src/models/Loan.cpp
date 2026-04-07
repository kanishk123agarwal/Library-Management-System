#include "Loan.h"

Loan::Loan(int loanId,int bookId,int memberId): loanId(loanId),bookId(bookId),memberId(memberId){}


int Loan::getLoanId() const
{
    return loanId;
}

int Loan::getBookId() const
{
    return bookId;
}

int Loan::getMemberId() const
{
    return memberId;
}

Loan::Loan(int overdueDays,std::shared_ptr<IFineStrategy> fineStrategy): overdueDays(overdueDays),fineStrategy(fineStrategy){}


double Loan::calculateFine() const
{
    return fineStrategy->calculateFine(overdueDays);
}