cmake_minimum_required(VERSION 3.16)
project(LibraryManagementSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ─── Sources ────────────────────────────────────────────────────────────────
set(SOURCES
    src/main.cpp

    # Models
    src/models/User.cpp
    src/models/Member.cpp
    src/models/Librarian.cpp
    src/models/Book.cpp
    src/models/Loan.cpp
    src/models/Reservation.cpp

    # Factories
    src/factories/BookFactory.cpp
    src/factories/UserFactory.cpp

    # Strategies
    src/strategies/DailyFineStrategy.cpp
    src/strategies/FlatFineStrategy.cpp

    # Observers
    src/observers/NotificationObserver.cpp

    # Core / Services
    src/core/Library.cpp
    src/services/LoanService.cpp
    src/services/ReservationService.cpp

    # Utils
    src/utils/DateUtils.cpp

    # Database
    src/database/DatabaseManager.cpp
)

add_executable(LMS ${SOURCES})

# ─── Include paths ──────────────────────────────────────────────────────────
target_include_directories(LMS PRIVATE src)

# ─── SQLite3 ────────────────────────────────────────────────────────────────
find_package(PkgConfig)
if(PkgConfig_FOUND)
    pkg_check_modules(SQLITE3 sqlite3)
endif()

if(SQLITE3_FOUND)
    target_include_directories(LMS PRIVATE ${SQLITE3_INCLUDE_DIRS})
    target_link_libraries(LMS ${SQLITE3_LIBRARIES})
else()
    # Fallback: link by name
    target_link_libraries(LMS sqlite3)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
