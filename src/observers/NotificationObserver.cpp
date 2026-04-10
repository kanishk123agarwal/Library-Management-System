#include "NotificationObserver.h"
#include <iostream>

NotificationObserver::NotificationObserver(const std::string& label)
    : label(label)
{
}

void NotificationObserver::update(const std::string& message)
{
    std::cout << "\n[Notification → " << label << "]\n"
              << message << "\n";
}
