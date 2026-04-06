#ifndef IFINESTRATEGY_H
#define IFINESTRATEGY_H

class IFineStrategy
{
public:
    virtual double calculateFine(int overdueDays) const = 0;

    virtual ~IFineStrategy() = default;
};

#endif