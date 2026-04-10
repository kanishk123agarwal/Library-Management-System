#include "ReservationService.h"

// ============================================================
// Library Management System — ReservationService Implementation
// ============================================================

#include <iostream>
#include "../utils/DateUtils.h"
#include "../utils/Logger.h"

int ReservationService::reserveBook(int bookId, int memberId)
{
    auto& db = DatabaseManager::getInstance();

    // Verify the book exists
    auto books = db.getAllBooks();
    bool bookFound = false;
    for (const auto& b : books)
    {
        if (b.bookId == bookId)
        {
            bookFound = true;
            break;
        }
    }

    if (!bookFound)
    {
        Logger::warn("Cannot reserve — Book ID " + std::to_string(bookId) + " not found.");
        return -1;
    }

    std::string today = DateUtils::getCurrentDate();
    int resId = db.insertReservation(bookId, memberId, today);

    if (resId != -1)
    {
        std::cout << "  Book reserved successfully!\n"
                  << "  Reservation ID : " << resId << "\n"
                  << "  Reserve Date   : " << today  << "\n";
    }

    return resId;
}

bool ReservationService::cancelReservation(int reservationId)
{
    bool ok = DatabaseManager::getInstance().deleteReservation(reservationId);
    if (ok)
        std::cout << "  Reservation #" << reservationId << " cancelled.\n";
    return ok;
}

std::vector<ReservationRecord> ReservationService::getReservationsByBook(int bookId)
{
    return DatabaseManager::getInstance().getReservationsByBook(bookId);
}
