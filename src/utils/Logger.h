#ifndef LOGGER_H
#define LOGGER_H

// ============================================================
// Library Management System — Simple Console Logger
// ============================================================

#include <iostream>
#include <string>

enum class LogLevel
{
    INFO,
    WARN,
    ERROR
};

class Logger
{
public:
    static void log(LogLevel level, const std::string& message)
    {
        switch (level)
        {
            case LogLevel::INFO:
                std::cout << "[INFO]  " << message << "\n";
                break;
            case LogLevel::WARN:
                std::cout << "[WARN]  " << message << "\n";
                break;
            case LogLevel::ERROR:
                std::cerr << "[ERROR] " << message << "\n";
                break;
        }
    }

    static void info(const std::string& msg)  { log(LogLevel::INFO,  msg); }
    static void warn(const std::string& msg)  { log(LogLevel::WARN,  msg); }
    static void error(const std::string& msg) { log(LogLevel::ERROR, msg); }
};

#endif // LOGGER_H
