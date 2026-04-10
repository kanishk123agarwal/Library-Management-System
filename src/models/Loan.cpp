#include "Loan.h"
#include "../utils/DateUtils.h"

// ============================================================
// Library Management System — Loan Implementation
// ============================================================

Loan::Loan(int loanId,
           int bookId,
           int memberId,
           const std::string& issueDate,
           const std::string& dueDate,
           std::shared_ptr<IFineStrategy> fineStrategy)
    : loanId(loanId)
    , bookId(bookId)
    , memberId(memberId)
    , issueDate(issueDate)
    , dueDate(dueDate)
    , fineStrategy(fineStrategy)
{
}

// ── Getters ───────────────────────────────────────────────────────────────────

int Loan::getLoanId()   const { return loanId;   }
int Loan::getBookId()   const { return bookId;    }
int Loan::getMemberId() const { return memberId;  }

const std::string& Loan::getIssueDate()  const { return issueDate;  }
const std::string& Loan::getDueDate()    const { return dueDate;    }
const std::string& Loan::getReturnDate() const { return returnDate; }

// ── Setters ───────────────────────────────────────────────────────────────────

void Loan::setReturnDate(const std::string& date)
{
    returnDate = date;
}

void Loan::setFineStrategy(std::shared_ptr<IFineStrategy> strategy)
{
    fineStrategy = strategy;
}

// ── Business Logic ────────────────────────────────────────────────────────────

bool Loan::isReturned() const
{
    return !returnDate.empty();
}

double Loan::calculateFine() const
{
    if (!fineStrategy) return 0.0;

    // Use returnDate if available; otherwise use today
    std::string compareDate = isReturned() ? returnDate : DateUtils::getCurrentDate();

    int overdueDays = DateUtils::daysBetween(dueDate, compareDate);
    if (overdueDays <= 0) return 0.0;  // no fine if not overdue

    return fineStrategy->calculateFine(overdueDays);
}