#ifndef LIBRARY_H
#define LIBRARY_H

#include <vector>

#include "../models/Book.h"
#include "../models/Member.h"
#include "../models/Loan.h"
#include "../models/Reservation.h"

class Library
{
private:
    std::vector<Book*> books;

    std::vector<Member*> members;

    std::vector<Loan*> loans;

    std::vector<Reservation*> reservations;

public:
    void addBook(Book* book);

    void addMember(Member* member);

    void addLoan(Loan* loan);

    void reserveBook(Book* book,Member* member,Reservation* reservation);

    void returnBook(Book* book);
};

#endif