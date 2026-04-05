#ifndef BOOK_H
#define BOOK_H

#include <string>

class Book
{
private:
    int bookId;
    std::string title;
    std::string author;
    std::string isbn;
    bool available;

public:
    Book(int id,const std::string& title,const std::string& author,const std::string& isbn);

    int getBookId() const;

    std::string getTitle() const;
    std::string getAuthor() const;
    std::string getISBN() const;

    bool isAvailable() const;

    void issue();
    void returnBook();
};

#endif