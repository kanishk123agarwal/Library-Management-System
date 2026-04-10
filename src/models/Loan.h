#ifndef LOAN_H
#define LOAN_H

// ============================================================
// Library Management System — Loan Model
// Day 6: Added dueDate / issueDate for proper fine calculation
// ============================================================

#include <memory>
#include <string>
#include "../strategies/IFineStrategy.h"

class Loan
{
private:
    int         loanId;
    int         bookId;
    int         memberId;
    std::string issueDate;   // ISO-8601: "YYYY-MM-DD"
    std::string dueDate;     // ISO-8601: "YYYY-MM-DD"
    std::string returnDate;  // empty until returned

    std::shared_ptr<IFineStrategy> fineStrategy;

public:
    // Full constructor (used when loading from DB or creating a new loan)
    Loan(int loanId,
         int bookId,
         int memberId,
         const std::string& issueDate,
         const std::string& dueDate,
         std::shared_ptr<IFineStrategy> fineStrategy = nullptr);

    // ── Getters ──────────────────────────────────────────────
    int getLoanId()   const;
    int getBookId()   const;
    int getMemberId() const;

    const std::string& getIssueDate()  const;
    const std::string& getDueDate()    const;
    const std::string& getReturnDate() const;

    // ── Setters ──────────────────────────────────────────────
    void setReturnDate(const std::string& date);
    void setFineStrategy(std::shared_ptr<IFineStrategy> strategy);

    // ── Business Logic ───────────────────────────────────────
    bool isReturned() const;

    /**
     * Calculates fine based on overdue days and the injected strategy.
     * Returns 0.0 if the fine strategy is not set or the book is not overdue.
     */
    double calculateFine() const;
};

#endif // LOAN_H