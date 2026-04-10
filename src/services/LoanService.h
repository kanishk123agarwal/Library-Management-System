#ifndef LOAN_SERVICE_H
#define LOAN_SERVICE_H

// ============================================================
// Library Management System — LoanService
// Handles issue / return / fine calculation
// ============================================================

#include <vector>
#include "../models/Loan.h"
#include "../database/DatabaseManager.h"

class LoanService
{
public:
    /**
     * Issues a book to a member.
     * - Inserts loan record into DB
     * - Marks book as unavailable
     * Returns the new loan_id, or -1 on failure.
     */
    int issueBook(int bookId, int memberId);

    /**
     * Returns a book.
     * - Sets return_date in DB
     * - Marks book as available
     * - Calculates and prints fine if overdue
     * Returns true on success.
     */
    bool returnBook(int loanId);

    /**
     * Lists all currently active (un-returned) loans.
     */
    std::vector<LoanRecord> getActiveLoans();

    /**
     * Lists all loans (including returned ones).
     */
    std::vector<LoanRecord> getAllLoans();

    /**
     * Updates the due date of a loan.
     */
    bool updateLoanDueDate(int loanId, const std::string& newDueDate);

    /**
     * Calculates the fine for a given loan record.
     */
    double calculateFine(const LoanRecord& loan);
};

#endif // LOAN_SERVICE_H
