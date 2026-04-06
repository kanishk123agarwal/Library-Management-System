#include "BookFactory.h"
#include<iostream>

bool BookFactory::validateISBN(const std::string& isbn)
{
    return isbn.length() == 13;
}
Book BookFactory::createBook(int id,const std::string& title,const std::string& author,const std::string& isbn)
{
    if (!validateISBN(isbn))
    {
        throw std::invalid_argument("ISBN must contain 13 characters");
    }

    std::cout<< "[BookFactory] Creating book: "<< title<< std::endl;
    return Book(id,title,author,isbn);
}