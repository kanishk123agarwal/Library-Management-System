#ifndef ISUBJECT_H
#define ISUBJECT_H

// ============================================================
// Library Management System — Subject Interface (Observer Pattern)
// ============================================================

#include "IObserver.h"

/**
 * ISubject — Any class that wants to emit events to observers.
 *
 * Book implements ISubject so that when a reserved book becomes
 * available, all registered Member observers are notified.
 */
class ISubject
{
public:
    virtual void addObserver(IObserver* observer)    = 0;
    virtual void removeObserver(IObserver* observer) = 0;
    virtual void notifyObservers()                   = 0;

    virtual ~ISubject() = default;
};

#endif // ISUBJECT_H
