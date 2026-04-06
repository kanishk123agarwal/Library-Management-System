#ifndef BOOK_FACTORY_H
#define BOOK_FACTORY_H

#include "../models/Book.h"

class BookFactory
{
public:
    static Book createBook(int id,const std::string& title,const std::string& author,const std::string& isbn);

private:
    static bool validateISBN(const std::string& isbn);
};

#endif