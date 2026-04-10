#ifndef RESERVATION_SERVICE_H
#define RESERVATION_SERVICE_H

// ============================================================
// Library Management System — ReservationService
// ============================================================

#include <vector>
#include "../database/DatabaseManager.h"

class ReservationService
{
public:
    /**
     * Reserves a book for a member.
     * Returns the new reservation_id, or -1 on failure.
     */
    int reserveBook(int bookId, int memberId);

    /**
     * Cancels a reservation by its ID.
     */
    bool cancelReservation(int reservationId);

    /**
     * Gets all reservations for a specific book.
     */
    std::vector<ReservationRecord> getReservationsByBook(int bookId);
};

#endif // RESERVATION_SERVICE_H
