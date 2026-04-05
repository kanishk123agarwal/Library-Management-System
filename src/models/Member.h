#ifndef MEMBER_H
#define MEMBER_H

#include "User.h"

class Member : public User
{
public:
    Member(int id,const std::string& name,const std::string& email);

    void searchBook();
    void borrowBook();
    void returnBook();
    void reserveBook();

    void displayRole() const override;
};

#endif