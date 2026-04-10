#include "LoanService.h"

// ============================================================
// Library Management System — LoanService Implementation
// ============================================================

#include <iostream>
#include <memory>
#include "../utils/DateUtils.h"
#include "../utils/Constants.h"
#include "../utils/Logger.h"
#include "../strategies/DailyFineStrategy.h"
#include "../models/Book.h"
#include "../models/Member.h"
#include "../observers/NotificationObserver.h"

int LoanService::issueBook(int bookId, int memberId)
{
    auto& db = DatabaseManager::getInstance();

    // Check if the book is available
    auto books = db.getAllBooks();
    bool bookFound = false;
    for (const auto& b : books)
    {
        if (b.bookId == bookId)
        {
            bookFound = true;
            if (!b.available)
            {
                Logger::warn("Book ID " + std::to_string(bookId) + " is not available.");
                return -1;
            }
            break;
        }
    }

    if (!bookFound)
    {
        Logger::warn("Book ID " + std::to_string(bookId) + " not found.");
        return -1;
    }

    std::string issueDate = DateUtils::getCurrentDate();
    std::string dueDate   = DateUtils::addDays(issueDate, Constants::DEFAULT_LOAN_DAYS);

    int loanId = db.createLoan(bookId, memberId, issueDate, dueDate);

    if (loanId != -1)
    {
        std::cout << "  Book issued successfully!\n"
                  << "  Loan ID   : " << loanId   << "\n"
                  << "  Issue Date: " << issueDate << "\n"
                  << "  Due Date  : " << dueDate   << "\n";
    }

    return loanId;
}

bool LoanService::returnBook(int loanId)
{
    auto& db = DatabaseManager::getInstance();

    // Find the loan
    auto activeLoans = db.getActiveLoans();
    LoanRecord* target = nullptr;
    for (auto& l : activeLoans)
    {
        if (l.loanId == loanId)
        {
            target = &l;
            break;
        }
    }

    if (!target)
    {
        Logger::warn("Active loan with ID " + std::to_string(loanId) + " not found.");
        return false;
    }

    std::string returnDate = DateUtils::getCurrentDate();
    double fine = calculateFine(*target);

    bool ok = db.returnLoan(loanId, returnDate);

    if (ok)
    {
        std::cout << "  Book returned successfully!\n"
                  << "  Return Date: " << returnDate << "\n";

        if (fine > 0.0)
        {
            std::cout << "  *** OVERDUE FINE: Rs." << fine << " ***\n";
        }
        else
        {
            std::cout << "  No fine — returned on time.\n";
        }

        // ─── Observer Pattern Notification ───────────────────────────────────
        int bookId = target->bookId;
        BookRecord bookRec = db.getBookById(bookId);
        if (bookRec.bookId != -1)
        {
            // Create the Subject (Book) in-memory
            Book book(bookRec.bookId, bookRec.title, bookRec.author, bookRec.isbn);

            // Fetch all members who reserved this book
            auto reservations = db.getReservationsByBook(bookId);
            std::vector<std::shared_ptr<IObserver>> activeObservers;

            for (const auto& res : reservations)
            {
                MemberRecord memRec = db.getMemberById(res.memberId);
                if (memRec.memberId != -1)
                {
                    // Create member observers dynamically
                    auto memberObserver = std::make_shared<Member>(memRec.memberId, memRec.name, memRec.email);
                    book.addObserver(memberObserver.get());
                    activeObservers.push_back(memberObserver);
                }
            }

            // Also add a system alert admin observer
            auto adminAlert = std::make_shared<NotificationObserver>("Admin Desk");
            book.addObserver(adminAlert.get());
            activeObservers.push_back(adminAlert);

            // Notify all registered observers that the book is now available
            book.setAvailable(true);
        }
        // ─────────────────────────────────────────────────────────────────────
    }

    return ok;
}

std::vector<LoanRecord> LoanService::getActiveLoans()
{
    return DatabaseManager::getInstance().getActiveLoans();
}

std::vector<LoanRecord> LoanService::getAllLoans()
{
    return DatabaseManager::getInstance().getAllLoans();
}

bool LoanService::updateLoanDueDate(int loanId, const std::string& newDueDate)
{
    return DatabaseManager::getInstance().updateLoanDueDate(loanId, newDueDate);
}

double LoanService::calculateFine(const LoanRecord& loan)
{
    // Use DailyFineStrategy with rate from Constants
    auto strategy = std::make_shared<DailyFineStrategy>(Constants::FINE_PER_DAY);

    std::string compareDate = loan.returnDate.empty()
                                  ? DateUtils::getCurrentDate()
                                  : loan.returnDate;

    int overdueDays = DateUtils::daysBetween(loan.dueDate, compareDate);
    if (overdueDays <= 0) return 0.0;

    return strategy->calculateFine(overdueDays);
}
