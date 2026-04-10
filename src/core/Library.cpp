#include "Library.h"

// ============================================================
// Library Management System — Library Implementation
// Day 6: All operations persist to SQLite via DatabaseManager
// ============================================================

#include <iostream>
#include <iomanip>
#include "../utils/Logger.h"
#include "../utils/DateUtils.h"
#include "../factories/BookFactory.h"
#include "../factories/UserFactory.h"

// ─── Books ────────────────────────────────────────────────────────────────────

int Library::addBook(const std::string& title,
                     const std::string& author,
                     const std::string& isbn)
{
    try
    {
        // Enforce the Factory pattern (ISBN validation occurs here)
        BookFactory::createBook(0, title, author, isbn);
    }
    catch (const std::invalid_argument& e)
    {
        Logger::error(std::string("Failed to add book: ") + e.what());
        std::cerr << "  Error: " << e.what() << "\n";
        return -1;
    }

    auto& db = DatabaseManager::getInstance();
    int id = db.insertBook(title, author, isbn);

    if (id != -1)
    {
        std::cout << "  Book added! ID: " << id << "\n";
    }
    return id;
}

void Library::viewAllBooks()
{
    auto books = DatabaseManager::getInstance().getAllBooks();

    if (books.empty())
    {
        std::cout << "  No books in the library.\n";
        return;
    }

    std::cout << "\n  " << std::left
              << std::setw(6)  << "ID"
              << std::setw(32) << "Title"
              << std::setw(24) << "Author"
              << std::setw(16) << "ISBN"
              << "Status\n";

    std::cout << "  " << std::string(85, '-') << "\n";

    for (const auto& b : books)
    {
        std::cout << "  "
                  << std::setw(6)  << b.bookId
                  << std::setw(32) << b.title
                  << std::setw(24) << b.author
                  << std::setw(16) << b.isbn
                  << (b.available ? "Available" : "Issued")
                  << "\n";
    }
}

void Library::searchBook(const std::string& keyword)
{
    auto books = DatabaseManager::getInstance().searchBookByTitle(keyword);

    if (books.empty())
    {
        std::cout << "  No books found matching \"" << keyword << "\".\n";
        return;
    }

    std::cout << "\n  Search results for \"" << keyword << "\":\n";

    for (const auto& b : books)
    {
        std::cout << "  [" << b.bookId << "] "
                  << b.title << " — " << b.author
                  << " (ISBN: " << b.isbn << ") "
                  << (b.available ? "[Available]" : "[Issued]")
                  << "\n";
    }
}

bool Library::deleteBook(int bookId)
{
    bool ok = DatabaseManager::getInstance().deleteBook(bookId);
    if (ok)
        std::cout << "  Book #" << bookId << " deleted.\n";
    return ok;
}

// ─── Members ─────────────────────────────────────────────────────────────────

int Library::registerMember(const std::string& name, const std::string& email)
{
    try
    {
        // Enforce the Factory pattern
        auto user = UserFactory::createUser(UserType::MEMBER, 0, name, email);
        if (!user)
        {
            Logger::error("Failed to create user object via UserFactory.");
            return -1;
        }
    }
    catch (const std::invalid_argument& e)
    {
        Logger::error(std::string("Failed to register member: ") + e.what());
        std::cerr << "  Error: " << e.what() << "\n";
        return -1;
    }

    auto& db = DatabaseManager::getInstance();
    int id = db.insertMember(name, email);

    if (id != -1)
    {
        std::cout << "  Member registered! ID: " << id << "\n";
    }
    return id;
}

void Library::viewAllMembers()
{
    auto members = DatabaseManager::getInstance().getAllMembers();

    if (members.empty())
    {
        std::cout << "  No members registered.\n";
        return;
    }

    std::cout << "\n  " << std::left
              << std::setw(6)  << "ID"
              << std::setw(28) << "Name"
              << "Email\n";
    std::cout << "  " << std::string(55, '-') << "\n";

    for (const auto& m : members)
    {
        std::cout << "  "
                  << std::setw(6)  << m.memberId
                  << std::setw(28) << m.name
                  << m.email << "\n";
    }
}

// ─── Loans ────────────────────────────────────────────────────────────────────

int Library::issueBook(int bookId, int memberId)
{
    return loanService.issueBook(bookId, memberId);
}

bool Library::returnBook(int loanId)
{
    return loanService.returnBook(loanId);
}

void Library::viewActiveLoans()
{
    auto loans = loanService.getActiveLoans();

    if (loans.empty())
    {
        std::cout << "  No active loans.\n";
        return;
    }

    std::cout << "\n  " << std::left
              << std::setw(8)  << "LoanID"
              << std::setw(8)  << "BookID"
              << std::setw(10) << "MemberID"
              << std::setw(14) << "Issue Date"
              << std::setw(14) << "Due Date"
              << "Overdue?\n";
    std::cout << "  " << std::string(65, '-') << "\n";

    for (const auto& l : loans)
    {
        bool overdue = DateUtils::isOverdue(l.dueDate);
        double fine  = loanService.calculateFine(l);

        std::cout << "  "
                  << std::setw(8)  << l.loanId
                  << std::setw(8)  << l.bookId
                  << std::setw(10) << l.memberId
                  << std::setw(14) << l.issueDate
                  << std::setw(14) << l.dueDate;

        if (overdue)
            std::cout << "YES (Fine: Rs." << fine << ")";
        else
            std::cout << "No";

        std::cout << "\n";
    }
}

bool Library::updateLoanDueDate(int loanId, const std::string& newDueDate)
{
    bool ok = loanService.updateLoanDueDate(loanId, newDueDate);
    if (ok)
    {
        std::cout << "  Loan ID #" << loanId << " due date updated to " << newDueDate << " successfully!\n";
    }
    return ok;
}

// ─── Reservations ─────────────────────────────────────────────────────────────

int Library::reserveBook(int bookId, int memberId)
{
    return reservationService.reserveBook(bookId, memberId);
}

void Library::viewReservations(int bookId)
{
    auto reservations = reservationService.getReservationsByBook(bookId);

    if (reservations.empty())
    {
        std::cout << "  No reservations for Book #" << bookId << ".\n";
        return;
    }

    std::cout << "\n  Reservations for Book #" << bookId << ":\n";
    std::cout << "  " << std::string(45, '-') << "\n";

    for (const auto& r : reservations)
    {
        std::cout << "  Reservation #" << r.reservationId
                  << "  Member #" << r.memberId
                  << "  Date: " << r.reserveDate << "\n";
    }
}