#include "DatabaseManager.h"

// ============================================================
// Library Management System — DatabaseManager Implementation
//Full SQLite CRUD
// ============================================================

#include <iostream>
#include <sstream>
#include "../utils/Logger.h"
#include "../utils/Constants.h"

// ─── Static member ────────────────────────────────────────────────────────────
DatabaseManager* DatabaseManager::instance = nullptr;

// ─── Constructor / Destructor ────────────────────────────────────────────────

DatabaseManager::DatabaseManager()
{
    int rc = sqlite3_open(Constants::DB_PATH, &db);
    if (rc != SQLITE_OK)
    {
        Logger::error(std::string("Failed to open database: ") + sqlite3_errmsg(db));
        db = nullptr;
    }
    else
    {
        Logger::info("Database connected successfully.");
        // Enable foreign-key enforcement
        sqlite3_exec(db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);
        createTables();
    }
}

DatabaseManager::~DatabaseManager()
{
    if (db)
    {
        sqlite3_close(db);
        Logger::info("Database connection closed.");
    }
}

// ─── Singleton ────────────────────────────────────────────────────────────────

DatabaseManager& DatabaseManager::getInstance()
{
    if (instance == nullptr)
        instance = new DatabaseManager();
    return *instance;
}

// ─── Raw helpers ─────────────────────────────────────────────────────────────

bool DatabaseManager::executeQuery(const std::string& query)
{
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        Logger::error(std::string("SQL Error: ") + errMsg);
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

sqlite3* DatabaseManager::getConnection()
{
    return db;
}

// ─── Schema creation ──────────────────────────────────────────────────────────

bool DatabaseManager::createTables()
{
    const std::string sql = R"(
        CREATE TABLE IF NOT EXISTS books (
            book_id   INTEGER PRIMARY KEY AUTOINCREMENT,
            title     TEXT    NOT NULL,
            author    TEXT    NOT NULL,
            isbn      TEXT    NOT NULL UNIQUE,
            available INTEGER NOT NULL DEFAULT 1
        );

        CREATE TABLE IF NOT EXISTS members (
            member_id INTEGER PRIMARY KEY AUTOINCREMENT,
            name      TEXT NOT NULL,
            email     TEXT NOT NULL UNIQUE
        );

        CREATE TABLE IF NOT EXISTS loans (
            loan_id     INTEGER PRIMARY KEY AUTOINCREMENT,
            book_id     INTEGER NOT NULL,
            member_id   INTEGER NOT NULL,
            issue_date  TEXT    NOT NULL,
            due_date    TEXT    NOT NULL,
            return_date TEXT    DEFAULT NULL,
            FOREIGN KEY (book_id)   REFERENCES books(book_id),
            FOREIGN KEY (member_id) REFERENCES members(member_id)
        );

        CREATE TABLE IF NOT EXISTS reservations (
            reservation_id INTEGER PRIMARY KEY AUTOINCREMENT,
            book_id        INTEGER NOT NULL,
            member_id      INTEGER NOT NULL,
            reserve_date   TEXT    NOT NULL,
            FOREIGN KEY (book_id)   REFERENCES books(book_id),
            FOREIGN KEY (member_id) REFERENCES members(member_id)
        );
    )";

    bool ok = executeQuery(sql);
    if (ok)
        Logger::info("Database tables verified / created.");
    return ok;
}

// ─── Books ────────────────────────────────────────────────────────────────────

int DatabaseManager::insertBook(const std::string& title,
                                const std::string& author,
                                const std::string& isbn)
{
    sqlite3_stmt* stmt = nullptr;
    const char* sql =
        "INSERT INTO books (title, author, isbn, available) VALUES (?, ?, ?, 1);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        Logger::error("insertBook: prepare failed.");
        return -1;
    }

    sqlite3_bind_text(stmt, 1, title.c_str(),  -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, author.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, isbn.c_str(),   -1, SQLITE_TRANSIENT);

    int result = -1;
    if (sqlite3_step(stmt) == SQLITE_DONE)
        result = static_cast<int>(sqlite3_last_insert_rowid(db));
    else
        Logger::error(std::string("insertBook: step failed: ") + sqlite3_errmsg(db));

    sqlite3_finalize(stmt);
    return result;
}

BookRecord DatabaseManager::getBookById(int bookId)
{
    BookRecord r{-1, "", "", "", false};
    const char* sql = "SELECT book_id, title, author, isbn, available FROM books WHERE book_id = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        Logger::error("getBookById: prepare failed.");
        return r;
    }

    sqlite3_bind_int(stmt, 1, bookId);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        r.bookId    = sqlite3_column_int(stmt, 0);
        r.title     = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        r.author    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        r.isbn      = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        r.available = sqlite3_column_int(stmt, 4) == 1;
    }

    sqlite3_finalize(stmt);
    return r;
}

std::vector<BookRecord> DatabaseManager::getAllBooks()
{
    std::vector<BookRecord> books;
    const char* sql = "SELECT book_id, title, author, isbn, available FROM books ORDER BY book_id;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        Logger::error("getAllBooks: prepare failed.");
        return books;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        BookRecord r;
        r.bookId    = sqlite3_column_int(stmt, 0);
        r.title     = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        r.author    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        r.isbn      = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        r.available = sqlite3_column_int(stmt, 4) == 1;
        books.push_back(r);
    }

    sqlite3_finalize(stmt);
    return books;
}

std::vector<BookRecord> DatabaseManager::searchBookByTitle(const std::string& keyword)
{
    std::vector<BookRecord> books;
    const char* sql =
        "SELECT book_id, title, author, isbn, available FROM books "
        "WHERE title LIKE ? ORDER BY book_id;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        Logger::error("searchBookByTitle: prepare failed.");
        return books;
    }

    std::string pattern = "%" + keyword + "%";
    sqlite3_bind_text(stmt, 1, pattern.c_str(), -1, SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        BookRecord r;
        r.bookId    = sqlite3_column_int(stmt, 0);
        r.title     = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        r.author    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        r.isbn      = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        r.available = sqlite3_column_int(stmt, 4) == 1;
        books.push_back(r);
    }

    sqlite3_finalize(stmt);
    return books;
}

bool DatabaseManager::updateBookAvailability(int bookId, bool available)
{
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "UPDATE books SET available = ? WHERE book_id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        Logger::error("updateBookAvailability: prepare failed.");
        return false;
    }

    sqlite3_bind_int(stmt, 1, available ? 1 : 0);
    sqlite3_bind_int(stmt, 2, bookId);

    bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
    if (!ok)
        Logger::error(std::string("updateBookAvailability: ") + sqlite3_errmsg(db));

    sqlite3_finalize(stmt);
    return ok;
}

bool DatabaseManager::deleteBook(int bookId)
{
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "DELETE FROM books WHERE book_id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        Logger::error("deleteBook: prepare failed.");
        return false;
    }

    sqlite3_bind_int(stmt, 1, bookId);

    bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
    if (!ok)
        Logger::error(std::string("deleteBook: ") + sqlite3_errmsg(db));

    sqlite3_finalize(stmt);
    return ok;
}

// ─── Members ──────────────────────────────────────────────────────────────────

int DatabaseManager::insertMember(const std::string& name, const std::string& email)
{
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "INSERT INTO members (name, email) VALUES (?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        Logger::error("insertMember: prepare failed.");
        return -1;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(),  -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_TRANSIENT);

    int result = -1;
    if (sqlite3_step(stmt) == SQLITE_DONE)
        result = static_cast<int>(sqlite3_last_insert_rowid(db));
    else
        Logger::error(std::string("insertMember: step failed: ") + sqlite3_errmsg(db));

    sqlite3_finalize(stmt);
    return result;
}

std::vector<MemberRecord> DatabaseManager::getAllMembers()
{
    std::vector<MemberRecord> members;
    const char* sql = "SELECT member_id, name, email FROM members ORDER BY member_id;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        Logger::error("getAllMembers: prepare failed.");
        return members;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        MemberRecord r;
        r.memberId = sqlite3_column_int(stmt, 0);
        r.name     = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        r.email    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        members.push_back(r);
    }

    sqlite3_finalize(stmt);
    return members;
}

MemberRecord DatabaseManager::getMemberById(int memberId)
{
    MemberRecord r{-1, "", ""};
    const char* sql =
        "SELECT member_id, name, email FROM members WHERE member_id = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        Logger::error("getMemberById: prepare failed.");
        return r;
    }

    sqlite3_bind_int(stmt, 1, memberId);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        r.memberId = sqlite3_column_int(stmt, 0);
        r.name     = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        r.email    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    }

    sqlite3_finalize(stmt);
    return r;
}

// ─── Loans ────────────────────────────────────────────────────────────────────

int DatabaseManager::createLoan(int bookId, int memberId,
                                const std::string& issueDate,
                                const std::string& dueDate)
{
    // Insert loan record
    sqlite3_stmt* stmt = nullptr;
    const char* sql =
        "INSERT INTO loans (book_id, member_id, issue_date, due_date) "
        "VALUES (?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        Logger::error("createLoan: prepare failed.");
        return -1;
    }

    sqlite3_bind_int (stmt, 1, bookId);
    sqlite3_bind_int (stmt, 2, memberId);
    sqlite3_bind_text(stmt, 3, issueDate.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, dueDate.c_str(),   -1, SQLITE_TRANSIENT);

    int result = -1;
    if (sqlite3_step(stmt) == SQLITE_DONE)
    {
        result = static_cast<int>(sqlite3_last_insert_rowid(db));
        // Mark book as not available
        updateBookAvailability(bookId, false);
    }
    else
    {
        Logger::error(std::string("createLoan: step failed: ") + sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    return result;
}

bool DatabaseManager::returnLoan(int loanId, const std::string& returnDate)
{
    // Step 1: find book_id for this loan
    int bookId = -1;
    {
        sqlite3_stmt* s = nullptr;
        const char* sql = "SELECT book_id FROM loans WHERE loan_id = ?;";
        if (sqlite3_prepare_v2(db, sql, -1, &s, nullptr) == SQLITE_OK)
        {
            sqlite3_bind_int(s, 1, loanId);
            if (sqlite3_step(s) == SQLITE_ROW)
                bookId = sqlite3_column_int(s, 0);
            sqlite3_finalize(s);
        }
    }

    if (bookId == -1)
    {
        Logger::error("returnLoan: loan not found.");
        return false;
    }

    // Step 2: update return_date
    {
        sqlite3_stmt* stmt = nullptr;
        const char* sql =
            "UPDATE loans SET return_date = ? WHERE loan_id = ?;";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        {
            Logger::error("returnLoan: prepare failed.");
            return false;
        }
        sqlite3_bind_text(stmt, 1, returnDate.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int (stmt, 2, loanId);
        bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
        sqlite3_finalize(stmt);
        if (!ok) return false;
    }

    // Step 3: mark book as available
    return updateBookAvailability(bookId, true);
}

std::vector<LoanRecord> DatabaseManager::getActiveLoans()
{
    std::vector<LoanRecord> loans;
    const char* sql =
        "SELECT loan_id, book_id, member_id, issue_date, due_date "
        "FROM loans WHERE return_date IS NULL ORDER BY loan_id;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        Logger::error("getActiveLoans: prepare failed.");
        return loans;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        LoanRecord r;
        r.loanId    = sqlite3_column_int(stmt, 0);
        r.bookId    = sqlite3_column_int(stmt, 1);
        r.memberId  = sqlite3_column_int(stmt, 2);
        r.issueDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        r.dueDate   = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        loans.push_back(r);
    }

    sqlite3_finalize(stmt);
    return loans;
}

std::vector<LoanRecord> DatabaseManager::getAllLoans()
{
    std::vector<LoanRecord> loans;
    const char* sql =
        "SELECT loan_id, book_id, member_id, issue_date, due_date, "
        "COALESCE(return_date, '') FROM loans ORDER BY loan_id;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        Logger::error("getAllLoans: prepare failed.");
        return loans;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        LoanRecord r;
        r.loanId     = sqlite3_column_int(stmt, 0);
        r.bookId     = sqlite3_column_int(stmt, 1);
        r.memberId   = sqlite3_column_int(stmt, 2);
        r.issueDate  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        r.dueDate    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        r.returnDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        loans.push_back(r);
    }

    sqlite3_finalize(stmt);
    return loans;
}

bool DatabaseManager::updateLoanDueDate(int loanId, const std::string& newDueDate)
{
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "UPDATE loans SET due_date = ? WHERE loan_id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        Logger::error("updateLoanDueDate: prepare failed.");
        return false;
    }

    sqlite3_bind_text(stmt, 1, newDueDate.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, loanId);

    bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
    if (!ok)
        Logger::error(std::string("updateLoanDueDate: step failed: ") + sqlite3_errmsg(db));

    sqlite3_finalize(stmt);
    return ok;
}

// ─── Reservations ─────────────────────────────────────────────────────────────

int DatabaseManager::insertReservation(int bookId, int memberId,
                                       const std::string& reserveDate)
{
    sqlite3_stmt* stmt = nullptr;
    const char* sql =
        "INSERT INTO reservations (book_id, member_id, reserve_date) "
        "VALUES (?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        Logger::error("insertReservation: prepare failed.");
        return -1;
    }

    sqlite3_bind_int (stmt, 1, bookId);
    sqlite3_bind_int (stmt, 2, memberId);
    sqlite3_bind_text(stmt, 3, reserveDate.c_str(), -1, SQLITE_TRANSIENT);

    int result = -1;
    if (sqlite3_step(stmt) == SQLITE_DONE)
        result = static_cast<int>(sqlite3_last_insert_rowid(db));
    else
        Logger::error(std::string("insertReservation: ") + sqlite3_errmsg(db));

    sqlite3_finalize(stmt);
    return result;
}

std::vector<ReservationRecord> DatabaseManager::getReservationsByBook(int bookId)
{
    std::vector<ReservationRecord> reservations;
    const char* sql =
        "SELECT reservation_id, book_id, member_id, reserve_date "
        "FROM reservations WHERE book_id = ? ORDER BY reservation_id;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        Logger::error("getReservationsByBook: prepare failed.");
        return reservations;
    }

    sqlite3_bind_int(stmt, 1, bookId);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        ReservationRecord r;
        r.reservationId = sqlite3_column_int(stmt, 0);
        r.bookId        = sqlite3_column_int(stmt, 1);
        r.memberId      = sqlite3_column_int(stmt, 2);
        r.reserveDate   = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        reservations.push_back(r);
    }

    sqlite3_finalize(stmt);
    return reservations;
}

bool DatabaseManager::deleteReservation(int reservationId)
{
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "DELETE FROM reservations WHERE reservation_id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        Logger::error("deleteReservation: prepare failed.");
        return false;
    }

    sqlite3_bind_int(stmt, 1, reservationId);

    bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
    if (!ok)
        Logger::error(std::string("deleteReservation: ") + sqlite3_errmsg(db));

    sqlite3_finalize(stmt);
    return ok;
}