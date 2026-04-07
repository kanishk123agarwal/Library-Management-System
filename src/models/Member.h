#ifndef MEMBER_H
#define MEMBER_H

#include "User.h"
#include "../observers/IObserver.h"

class Member : public User,public IObserver
{
public:
    Member(int id,const std::string& name,const std::string& email);

    void searchBook();
    void borrowBook();
    void returnBook();
    void reserveBook();

    void displayRole() const override;

    void update(const std::string& message) override;
};

#endif