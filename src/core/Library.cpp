#include "Library.h"

void Library::addBook(Book* book)
{
    books.push_back(book);
}

void Library::addMember(Member* member)
{
    members.push_back(member);
}

void Library::addLoan(Loan* loan)
{
    loans.push_back(loan);
}

void Library::reserveBook(Book* book,Member* member,Reservation* reservation)
{
    reservations.push_back(reservation);

    book->addObserver(member);
}

void Library::returnBook(Book* book)
{
    book->setAvailable(true);
}