#ifndef IOBSERVER_H
#define IOBSERVER_H

#include <string>

class IObserver
{
public:
    virtual void update(const std::string& message) = 0;

    virtual ~IObserver() = default;
};

#endif