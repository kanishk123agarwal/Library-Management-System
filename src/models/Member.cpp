#include "Member.h"
#include <iostream>

Member::Member(int id,const std::string& name,const std::string& email): User(id, name, email){}

void Member::searchBook()
{
    std::cout << "Searching books...\n";
}

void Member::borrowBook()
{
    std::cout << "Borrowing book...\n";
}

void Member::returnBook()
{
    std::cout << "Returning book...\n";
}

void Member::reserveBook()
{
    std::cout << "Reserving book...\n";
}

void Member::displayRole() const
{
    std::cout << "Role: Member\n";
}