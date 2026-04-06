#ifndef USER_FACTORY_H
#define USER_FACTORY_H

#include <memory>
#include <string>

#include "../models/User.h"
#include "../models/Member.h"
#include "../models/Librarian.h"
#include "../models/UserType.h"

class UserFactory
{
public:
    static std::unique_ptr<User> createUser(UserType type,int id,const std::string& name,const std::string& email);
};

#endif