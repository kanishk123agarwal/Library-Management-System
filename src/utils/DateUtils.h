#ifndef DATE_UTILS_H
#define DATE_UTILS_H

// ============================================================
// Library Management System — Date Utility Functions
// ============================================================

#include <string>

class DateUtils
{
public:
    /**
     * Returns the current date as an ISO-8601 string: "YYYY-MM-DD"
     */
    static std::string getCurrentDate();

    /**
     * Adds `days` to the given ISO date string and returns the new date string.
     * @param date  ISO-8601 date "YYYY-MM-DD"
     * @param days  Number of days to add
     */
    static std::string addDays(const std::string& date, int days);

    /**
     * Calculates the number of days between two ISO-8601 dates.
     * Returns a positive value if `end` is after `start`.
     */
    static int daysBetween(const std::string& start, const std::string& end);

    /**
     * Returns true if `date` is before today (i.e. the loan is overdue).
     */
    static bool isOverdue(const std::string& dueDate);
};

#endif // DATE_UTILS_H
