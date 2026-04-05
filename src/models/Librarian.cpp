#include "Librarian.h"
#include <iostream>

Librarian::Librarian(int id,const std::string& name,const std::string& email): User(id, name, email){}

void Librarian::addBook()
{
    std::cout << "Adding book...\n";
}

void Librarian::removeBook()
{
    std::cout << "Removing book...\n";
}

void Librarian::updateBook()
{
    std::cout << "Updating book...\n";
}

void Librarian::displayRole() const
{
    std::cout << "Role: Librarian\n";
}