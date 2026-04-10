#include "DateUtils.h"

// ============================================================
// Library Management System — DateUtils Implementation
// Uses <ctime> for portability (C++17 compatible)
// ============================================================

#include <ctime>
#include <sstream>
#include <iomanip>
#include <stdexcept>

// ─── Helper: parse "YYYY-MM-DD" → std::tm ───────────────────────────────────
static std::tm parseDate(const std::string& dateStr)
{
    std::tm tm = {};
    std::istringstream ss(dateStr);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    if (ss.fail())
        throw std::invalid_argument("Invalid date format: " + dateStr);
    return tm;
}

// ─── Helper: std::tm → "YYYY-MM-DD" ─────────────────────────────────────────
static std::string formatDate(std::tm& tm)
{
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d");
    return oss.str();
}

// ─── Public API ──────────────────────────────────────────────────────────────

std::string DateUtils::getCurrentDate()
{
    std::time_t now = std::time(nullptr);
    std::tm* tm_now = std::localtime(&now);
    return formatDate(*tm_now);
}

std::string DateUtils::addDays(const std::string& date, int days)
{
    std::tm tm = parseDate(date);
    tm.tm_mday += days;
    std::mktime(&tm);   // normalise the struct (handles month/year overflow)
    return formatDate(tm);
}

int DateUtils::daysBetween(const std::string& start, const std::string& end)
{
    std::tm tmStart = parseDate(start);
    std::tm tmEnd   = parseDate(end);

    std::time_t t1 = std::mktime(&tmStart);
    std::time_t t2 = std::mktime(&tmEnd);

    // Difference in seconds → days
    double diff = std::difftime(t2, t1);
    return static_cast<int>(diff / (60.0 * 60.0 * 24.0));
}

bool DateUtils::isOverdue(const std::string& dueDate)
{
    std::string today = getCurrentDate();
    return daysBetween(dueDate, today) > 0;
}
