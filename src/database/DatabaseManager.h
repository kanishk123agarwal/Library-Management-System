#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

// ============================================================
// Library Management System — DatabaseManager (Singleton)
// Day 6: Full SQLite CRUD — Books, Members, Loans, Reservations
// ============================================================

#include <sqlite3.h>
#include <string>
#include <vector>

// Forward declarations of domain types used in query results
struct BookRecord
{
    int         bookId;
    std::string title;
    std::string author;
    std::string isbn;
    bool        available;
};

struct MemberRecord
{
    int         memberId;
    std::string name;
    std::string email;
};

struct LoanRecord
{
    int         loanId;
    int         bookId;
    int         memberId;
    std::string issueDate;
    std::string dueDate;
    std::string returnDate; // empty if not yet returned
};

struct ReservationRecord
{
    int         reservationId;
    int         bookId;
    int         memberId;
    std::string reserveDate;
};

// ─────────────────────────────────────────────────────────────────────────────

class DatabaseManager
{
private:
    sqlite3* db;

    // Private constructor — Singleton
    DatabaseManager();

    static DatabaseManager* instance;

public:
    ~DatabaseManager();

    // Non-copyable / non-assignable
    DatabaseManager(const DatabaseManager&)            = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    // ── Singleton access ────────────────────────────────────────────────────
    static DatabaseManager& getInstance();

    // ── Schema ──────────────────────────────────────────────────────────────
    /**
     * Creates all required tables if they do not already exist.
     * Call once at startup.
     */
    bool createTables();

    // ── Raw query (DDL / DML without results) ───────────────────────────────
    bool executeQuery(const std::string& query);

    // ── Connection handle ────────────────────────────────────────────────────
    sqlite3* getConnection();

    // ── Books ────────────────────────────────────────────────────────────────
    /**
     * Inserts a new book. Returns the new book_id, or -1 on failure.
     */
    int insertBook(const std::string& title,
                   const std::string& author,
                   const std::string& isbn);

    std::vector<BookRecord> getAllBooks();

    std::vector<BookRecord> searchBookByTitle(const std::string& keyword);

    BookRecord getBookById(int bookId);

    /**
     * Sets book availability (1 = available, 0 = issued).
     */
    bool updateBookAvailability(int bookId, bool available);

    bool deleteBook(int bookId);

    // ── Members ──────────────────────────────────────────────────────────────
    /**
     * Inserts a new member. Returns the new member_id, or -1 on failure.
     */
    int insertMember(const std::string& name, const std::string& email);

    std::vector<MemberRecord> getAllMembers();

    MemberRecord getMemberById(int memberId);

    // ── Loans ────────────────────────────────────────────────────────────────
    /**
     * Creates a loan record (marks book as issued in the same call).
     * Returns the new loan_id, or -1 on failure.
     * @param issueDate  ISO-8601 "YYYY-MM-DD"
     * @param dueDate    ISO-8601 "YYYY-MM-DD"
     */
    int createLoan(int bookId, int memberId,
                   const std::string& issueDate,
                   const std::string& dueDate);

    /**
     * Records a book return — sets return_date and marks book available.
     */
    bool returnLoan(int loanId, const std::string& returnDate);

    std::vector<LoanRecord> getActiveLoans();   // return_date IS NULL

    std::vector<LoanRecord> getAllLoans();

    /**
     * Updates the due date of a loan record.
     */
    bool updateLoanDueDate(int loanId, const std::string& newDueDate);

    // ── Reservations ─────────────────────────────────────────────────────────
    /**
     * Inserts a reservation record.
     * Returns new reservation_id, or -1 on failure.
     */
    int insertReservation(int bookId, int memberId,
                          const std::string& reserveDate);

    std::vector<ReservationRecord> getReservationsByBook(int bookId);

    bool deleteReservation(int reservationId);
};

#endif // DATABASE_MANAGER_H