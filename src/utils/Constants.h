#ifndef CONSTANTS_H
#define CONSTANTS_H

// ============================================================
// Library Management System — Global Constants
// ============================================================

namespace Constants
{
    // Database file path (relative to executable)
    constexpr const char* DB_PATH = "library.db";

    // Loan policy
    constexpr int DEFAULT_LOAN_DAYS   = 14;   // 2 weeks
    constexpr double FINE_PER_DAY     = 5.0;  // ₹5 per day
    constexpr double FLAT_FINE_AMOUNT = 50.0; // ₹50 flat fine

    // UI
    constexpr const char* APP_TITLE = "Library Management System";
    constexpr const char* SEPARATOR = "============================================";
}

#endif // CONSTANTS_H
