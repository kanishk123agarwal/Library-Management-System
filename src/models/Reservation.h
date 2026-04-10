#ifndef RESERVATION_H
#define RESERVATION_H

// ============================================================
// Library Management System — Reservation Model
// ============================================================

#include <string>

class Reservation
{
private:
    int         reservationId;
    int         bookId;
    int         memberId;
    std::string reserveDate;   // ISO-8601: "YYYY-MM-DD"

public:
    Reservation(int reservationId,
                int bookId,
                int memberId,
                const std::string& reserveDate = "");

    int getReservationId() const;
    int getBookId()        const;
    int getMemberId()      const;
    const std::string& getReserveDate() const;
};

#endif // RESERVATION_H