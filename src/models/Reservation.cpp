#include "Reservation.h"

// ============================================================
// Library Management System — Reservation Implementation
// ============================================================

Reservation::Reservation(int reservationId,
                         int bookId,
                         int memberId,
                         const std::string& reserveDate)
    : reservationId(reservationId)
    , bookId(bookId)
    , memberId(memberId)
    , reserveDate(reserveDate)
{
}

int Reservation::getReservationId() const { return reservationId; }
int Reservation::getBookId()        const { return bookId;        }
int Reservation::getMemberId()      const { return memberId;      }

const std::string& Reservation::getReserveDate() const { return reserveDate; }