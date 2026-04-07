#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <vector>
#include "../observers/IObserver.h"

class Book
{
private:
    int bookId;
    std::string title;
    std::string author;
    std::string isbn;
    bool available;
    std::vector<IObserver*> observers;
public:
    Book(int id,const std::string& title,const std::string& author,const std::string& isbn);

    int getBookId() const;

    std::string getTitle() const;
    std::string getAuthor() const;
    std::string getISBN() const;

    bool isAvailable() const;

    void issue();
    void returnBook();

    void addObserver(IObserver* observer);

    void removeObserver(IObserver* observer);

    void notifyObservers();

    void setAvailable(bool status);

};

#endif