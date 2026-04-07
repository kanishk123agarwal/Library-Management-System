#include "Book.h"
#include <algorithm>
#include <iostream>

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

void Book::addObserver(IObserver* observer)
{
    observers.push_back(observer);
}

void Book::removeObserver(IObserver* observer)
{
    observers.erase(std::remove(observers.begin(),observers.end(),observer),observers.end());
}

void Book::notifyObservers()
{
    std::string message ="Book Available:\n" + title;

    for (auto observer : observers)
    {
        observer->update(message);
    }
}

void Book::setAvailable(bool status)
{
    available = status;

    if (available)
    {
        notifyObservers();
    }
}
