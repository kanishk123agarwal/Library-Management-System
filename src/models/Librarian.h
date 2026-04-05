#ifndef LIBRARIAN_H
#define LIBRARIAN_H

#include "User.h"

class Librarian : public User
{
public:
    Librarian(int id,const std::string& name,const std::string& email);

    void addBook();
    void removeBook();
    void updateBook();

    void displayRole() const override;
};

#endif