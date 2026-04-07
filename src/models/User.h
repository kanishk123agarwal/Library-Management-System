#ifndef USER_H
#define USER_H

#include <string>

class User
{
protected:
    int userId;
    std::string name;
    std::string email;

public:
    User(int id,const std::string& name,const std::string& email);

    virtual ~User()=default;

    void login();
    void logout();
    const std::string& getName() const;

    int getUserId() const;

    const std::string& getEmail() const;

    virtual void displayRole() const = 0;
};

#endif