#include "UserFactory.h"

std::unique_ptr<User> UserFactory::createUser(UserType type,int id,const std::string& name,const std::string& email)
{
    switch (type)
    {
        case UserType::MEMBER:return std::make_unique<Member>(id,name,email);

        case UserType::LIBRARIAN:return std::make_unique<Librarian>(id,name,email);

        default:return nullptr;
    }
}