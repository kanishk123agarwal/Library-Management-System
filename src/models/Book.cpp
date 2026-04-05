#include "Book.h"

Book::Book(int id,const std::string& title,const std::string& author,const std::string& isbn): bookId(id),title(title),author(author),isbn(isbn),available(true){}

int Book::getBookId() const
{
    return bookId;
}

std::string Book::getTitle() const
{
    return title;
}

std::string Book::getAuthor() const
{
    return author;
}

std::string Book::getISBN() const
{
    return isbn;
}

bool Book::isAvailable() const
{
    return available;
}

void Book::issue()
{
    available = false;
}

void Book::returnBook()
{
    available = true;
}