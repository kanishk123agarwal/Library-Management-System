#ifndef RESERVATION_H
#define RESERVATION_H

class Reservation
{
private:
    int reservationId;
    int bookId;
    int memberId;

public:
    Reservation(int reservationId,int bookId,int memberId);

    int getBookId() const;

    int getMemberId() const;
};

#endif