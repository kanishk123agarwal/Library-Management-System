#ifndef NOTIFICATION_OBSERVER_H
#define NOTIFICATION_OBSERVER_H

// ============================================================
// Library Management System — Console Notification Observer
// ============================================================

#include "IObserver.h"
#include <string>

/**
 * NotificationObserver — A generic console observer.
 *
 * Unlike Member (which is both a domain object and observer),
 * this class is a pure notification sink used for system-level
 * events (e.g. logging, admin alerts).
 */
class NotificationObserver : public IObserver
{
private:
    std::string label; // Identifies who is being notified

public:
    explicit NotificationObserver(const std::string& label);

    /**
     * Called by the subject (Book) when availability changes.
     */
    void update(const std::string& message) override;
};

#endif // NOTIFICATION_OBSERVER_H
