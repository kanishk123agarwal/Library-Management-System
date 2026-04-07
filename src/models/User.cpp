#include "User.h"
#include <iostream>
using namespace std;
User::User(int id,const string& name,const string& email): userId(id),name(name),email(email){}


void User::login()
{
    cout << name << " logged in.\n";
}

void User::logout()
{
    cout << name << " logged out.\n";
}

const std::string& User::getName() const
{
    return name;
}

int User::getUserId() const
{
    return userId;
}

const std::string& User::getEmail() const
{
    return email;
}