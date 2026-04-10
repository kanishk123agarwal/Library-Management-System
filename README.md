# Library Management System (LLD & System Design Showcase)

A professional console-based **Library Management System (LMS)** designed to showcase clean software architecture, Object-Oriented Design (OOD) principles, and **Low-Level Design (LLD)** best practices. Built in C++17, this project demonstrates how to decouple core business logic, persistence layers, and event-driven patterns using a persistent **SQLite3** database.

---

## 📸 Design Diagrams & Visuals

### 1. UML Class Diagram
![UML Class Diagram](images/uml_class_diagram.png)
*Figure 1: Low-Level Class Structure and structural design relationships.*

### 2. Database ER Diagram
![Database ER Diagram](images/database_er_diagram.png)
*Figure 2: Relational schema representation in SQLite.*

### 3. Main Interface
![LMS CLI Main Menu](images/lms_main_menu.png)
*Figure 3: Polished console-based CLI menu.*

### 4. Member Management
![Member Management](images/lms_member.png)
*Figure 4: Listing registered members.*

### 5. Loan & Fine Management
![Issue Book](images/lms_issue_book.png)
*Figure 5: Issuing a book to a member.*

![Active Loans](images/lms_loan.png)
*Figure 6: Checking active loans table.*

![Return Book](images/lms_return_book.png)
*Figure 7: Book return process with live Observer alerts.*

### 6. Reservation System
![Reservations List](images/lms_reservation.png)
*Figure 8: Active reservations listing.*

### 7. Test Suite Execution
![Automated Test Suite](images/lms_test_suite.png)
*Figure 9: Automated validation runner showing all tests passing.*

---

## 🏛️ LLD Architecture & SOLID Compliance

The system is split into distinct layers: **Models** (Domain Entities), **Factories** (Object Creation), **Strategies** (Swappable Logic), **Observers** (Event Handling), **Services** (Business Rules), and **Database** (Persistence).

This division ensures strict compliance with **SOLID Principles**:

*   **Single Responsibility Principle (SRP):** Classes have focused reasons to change. For example, `DatabaseManager` handles SQL queries, `Book` models represent book details, and `LoanService` orchestrates issuing/returning loans.
*   **Open/Closed Principle (OCP):** The system is open for extension but closed for modification. For instance, new fine calculation strategies can be added by implementing the `IFineStrategy` interface without altering the existing loan return business rules.
*   **Liskov Substitution Principle (LSP):** Users of `User` (such as `Member` or `Librarian`) can be used interchangeably through base class interfaces without breaking the program's correctness.
*   **Interface Segregation Principle (ISP):** Interfaces like `IObserver` and `ISubject` are kept minimal and focused on their specific roles, preventing classes from depending on methods they do not use.
*   **Dependency Inversion Principle (DIP):** High-level services (like `LoanService`) do not depend directly on concrete fine strategies; they depend on the `IFineStrategy` abstraction.

---

## 🎨 GoF Design Patterns (Deep Dive)

### 1. Singleton Pattern (`src/database/DatabaseManager`)
*   **Problem:** Multiple components accessing the database could open duplicate connections, causing thread synchronization issues and file locks.
*   **Solution:** The database manager implements a thread-safe Singleton pattern. It exposes a static `getInstance()` method, ensuring a single connection instance handles all SQL queries.

### 2. Factory Pattern (`src/factories/BookFactory`, `UserFactory`)
*   **Problem:** Directly calling constructors (e.g., `new Book(...)`) spreads creation logic and validation rules throughout the codebase.
*   **Solution:** Factories handle object creation. `BookFactory::createBook` checks domain constraints (e.g., validating that the ISBN is exactly 13 characters) before instantiating the object, ensuring invalid domain entities never enter the system.

### 3. Strategy Pattern (`src/strategies/IFineStrategy`)
*   **Problem:** Different libraries calculate late fees differently. Hardcoding `if-else` blocks for "daily rate" or "flat rate" calculations violates OCP.
*   **Solution:** The fine calculation is decoupled. The system injects a concrete implementation of `IFineStrategy` (`DailyFineStrategy` charging ₹5/day or `FlatFineStrategy` charging a flat ₹50 fee) at runtime to calculate overdue charges dynamically.

### 4. Observer Pattern (`src/observers/IObserver`, `ISubject`)
*   **Problem:** When a book becomes available, notifying waiting members should not tightly couple the `Book` and `Member` classes, which would create circular dependencies.
*   **Solution:** `Book` acts as a `ISubject` and `Member` implements `IObserver`. When a book is returned, the system automatically fetches the pending reservations, registers the members as observers to the book, and calls `notifyObservers()`, writing real-time console notification events.

---

## 🛢️ Database Schema & LLD Mapping

The database schema reflects the LLD relationships and maintains data integrity:
*   **Books (`books` table):** Tracks titles, authors, and availability status.
*   **Members (`members` table):** Tracks registered users.
*   **Loans (`loans` table):** Captures the many-to-many relationship between books and members over time. It stores issue dates, due dates, return dates, and references foreign keys with cascade constraints.
*   **Reservations (`reservations` table):** Represents the queue of members waiting for a borrowed book.

---

## ⚙️ Quick Start

### 1. Build
Ensure you have CMake and SQLite3 dev packages installed, then run:
```bash
cd /path/to/LMS
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)
```

### 2. Run
Use the setup utility script to clear previous state, seed sample data, and start the app:
```bash
cd /path/to/LMS
bash tests/run_interactive.sh
```

### 3. Automated Tests
```bash
cd /path/to/LMS
bash tests/run_all_tests.sh
```

---

## 📅 Overdue Fine Simulation
To easily test the system's dynamic overdue calculations:
1. Issue a book: **`3. Loan Management`** ➔ **`1. Issue Book`**.
2. Update the due date to a past date: **`4. Update Loan Due Date`** (e.g., enter `2026-05-01`).
3. Return the book: **`2. Return Book`**. The active Strategy will calculate the fine and log the output.

For a detailed verification walkthrough, see the [Manual Test Guide](tests/manual_test_guide.md).

---

## 🔮 Future Changes

To expand the capabilities of this LLD architecture, the following updates are planned:
*   **GUI Subsystem:** Introduce a Qt-based desktop frontend that interfaces with the existing service layers without changing domain logic.
*   **Service API Layer:** Package the core orchestrator behind a REST API (using Crow or Pistache) to support web clients.
*   **Concurrency Handling:** Add SQLite write-ahead logging (WAL) and mutex guards inside service layers to handle concurrent borrow/return operations safely.
*   **Notification Dispatchers:** Refactor the Observer pattern to support asynchronous notification methods (e.g., sending email alerts via curl or SMS APIs) instead of console logs.
