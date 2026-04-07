#include "Reservation.h"

Reservation::Reservation(int reservationId,int bookId,int memberId): reservationId(reservationId),bookId(bookId),memberId(memberId){}


int Reservation::getBookId() const
{
    return bookId;
}

int Reservation::getMemberId() const
{
    return memberId;
}