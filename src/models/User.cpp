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