#include "Librarian.h"
#include <iostream>

// ============================================================
// Library Management System — Librarian Implementation
// ============================================================

Librarian::Librarian(int id, const std::string& name, const std::string& email)
    : User(id, name, email)
{
}

void Librarian::addBook()
{
    std::cout << "[Librarian " << getName()
              << "] Action: Add a new book to the library.\n";
}

void Librarian::removeBook()
{
    std::cout << "[Librarian " << getName()
              << "] Action: Remove a book from the library.\n";
}

void Librarian::updateBook()
{
    std::cout << "[Librarian " << getName()
              << "] Action: Update book details.\n";
}

void Librarian::displayRole() const
{
    std::cout << "Role: Librarian\n";
}