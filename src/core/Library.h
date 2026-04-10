#ifndef LIBRARY_H
#define LIBRARY_H

// ============================================================
// Library Management System — Library (Core Orchestrator)
// Day 6: Wired to DatabaseManager + LoanService + ReservationService
// ============================================================

#include <vector>
#include <string>
#include "../models/Book.h"
#include "../models/Member.h"
#include "../models/Loan.h"
#include "../models/Reservation.h"
#include "../database/DatabaseManager.h"
#include "../services/LoanService.h"
#include "../services/ReservationService.h"

class Library
{
private:
    LoanService        loanService;
    ReservationService reservationService;

public:
    // ── Books ─────────────────────────────────────────────────────────────
    /**
     * Adds a new book to the library (persists to DB).
     * Returns the new book_id, or -1 on failure.
     */
    int addBook(const std::string& title,
                const std::string& author,
                const std::string& isbn);

    void viewAllBooks();

    void searchBook(const std::string& keyword);

    bool deleteBook(int bookId);

    // ── Members ───────────────────────────────────────────────────────────
    /**
     * Registers a new member (persists to DB).
     * Returns the new member_id, or -1 on failure.
     */
    int registerMember(const std::string& name, const std::string& email);

    void viewAllMembers();

    // ── Loans ─────────────────────────────────────────────────────────────
    /**
     * Issues a book to a member.
     */
    int issueBook(int bookId, int memberId);

    /**
     * Returns a book and processes fines.
     */
    bool returnBook(int loanId);

    void viewActiveLoans();

    /**
     * Updates the due date of a loan.
     */
    bool updateLoanDueDate(int loanId, const std::string& newDueDate);

    // ── Reservations ──────────────────────────────────────────────────────
    /**
     * Reserves a book for a member.
     */
    int reserveBook(int bookId, int memberId);

    void viewReservations(int bookId);
};

#endif // LIBRARY_H