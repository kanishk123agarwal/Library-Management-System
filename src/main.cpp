// ============================================================
//  Library Management System — Main CLI Entry Point
//  Day 6: Full interactive menu with SQLite persistence
// ============================================================

#include <iostream>
#include <string>
#include <limits>
#ifdef _WIN32
  #include <io.h>
#else
  #include <unistd.h>
#endif
#include "core/Library.h"
#include "utils/Constants.h"

// ─── Helpers ─────────────────────────────────────────────────────────────────

static void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

static void waitForEnter()
{
    // Only consume stdin if running interactively (not piped)
#ifdef _WIN32
    bool interactive = _isatty(_fileno(stdin)) != 0;
#else
    bool interactive = isatty(fileno(stdin)) != 0;
#endif
    if (interactive)
    {
        std::cout << "\n  Press Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

static int readInt(const std::string& prompt)
{
    int value;
    std::cout << "  " << prompt;
    while (!(std::cin >> value))
    {
        if (std::cin.eof()) return -1;  // piped input ended
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  Invalid input. " << prompt;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return value;
}

static std::string readLine(const std::string& prompt)
{
    std::string value;
    std::cout << "  " << prompt;
    std::getline(std::cin, value);
    return value;
}

static void printHeader(const std::string& title)
{
    std::cout << "\n" << Constants::SEPARATOR << "\n";
    std::cout << "   " << title << "\n";
    std::cout << Constants::SEPARATOR << "\n";
}

// ─── Sub-menus ───────────────────────────────────────────────────────────────

void bookMenu(Library& lib)
{
    bool back = false;
    while (!back)
    {
        printHeader("BOOK MANAGEMENT");
        std::cout << "  1. Add Book\n";
        std::cout << "  2. View All Books\n";
        std::cout << "  3. Search Book by Title\n";
        std::cout << "  4. Delete Book\n";
        std::cout << "  0. Back\n";

        int choice = readInt("Choice: ");
        if (std::cin.eof() || choice == -1) break;

        switch (choice)
        {
            case 1:
            {
                printHeader("ADD BOOK");
                std::string title  = readLine("Title  : ");
                std::string author = readLine("Author : ");
                std::string isbn   = readLine("ISBN   : ");
                lib.addBook(title, author, isbn);
                waitForEnter();
                break;
            }
            case 2:
            {
                printHeader("ALL BOOKS");
                lib.viewAllBooks();
                waitForEnter();
                break;
            }
            case 3:
            {
                printHeader("SEARCH BOOK");
                std::string kw = readLine("Keyword: ");
                lib.searchBook(kw);
                waitForEnter();
                break;
            }
            case 4:
            {
                printHeader("DELETE BOOK");
                int id = readInt("Book ID: ");
                lib.deleteBook(id);
                waitForEnter();
                break;
            }
            case 0:
                back = true;
                break;
            default:
                std::cout << "  Invalid choice.\n";
                waitForEnter();
        }
    }
}

void memberMenu(Library& lib)
{
    bool back = false;
    while (!back)
    {
        printHeader("MEMBER MANAGEMENT");
        std::cout << "  1. Register Member\n";
        std::cout << "  2. View All Members\n";
        std::cout << "  0. Back\n";

        int choice = readInt("Choice: ");
        if (std::cin.eof() || choice == -1) break;

        switch (choice)
        {
            case 1:
            {
                printHeader("REGISTER MEMBER");
                std::string name  = readLine("Name  : ");
                std::string email = readLine("Email : ");
                lib.registerMember(name, email);
                waitForEnter();
                break;
            }
            case 2:
            {
                printHeader("ALL MEMBERS");
                lib.viewAllMembers();
                waitForEnter();
                break;
            }
            case 0:
                back = true;
                break;
            default:
                std::cout << "  Invalid choice.\n";
                waitForEnter();
        }
    }
}

void loanMenu(Library& lib)
{
    bool back = false;
    while (!back)
    {
        printHeader("LOAN MANAGEMENT");
        std::cout << "  1. Issue Book\n";
        std::cout << "  2. Return Book\n";
        std::cout << "  3. View Active Loans\n";
        std::cout << "  4. Update Loan Due Date\n";
        std::cout << "  0. Back\n";

        int choice = readInt("Choice: ");
        if (std::cin.eof() || choice == -1) break;

        switch (choice)
        {
            case 1:
            {
                printHeader("ISSUE BOOK");
                lib.viewAllBooks();
                int bookId   = readInt("Book ID  : ");
                lib.viewAllMembers();
                int memberId = readInt("Member ID: ");
                lib.issueBook(bookId, memberId);
                waitForEnter();
                break;
            }
            case 2:
            {
                printHeader("RETURN BOOK");
                lib.viewActiveLoans();
                int loanId = readInt("Loan ID: ");
                lib.returnBook(loanId);
                waitForEnter();
                break;
            }
            case 3:
            {
                printHeader("ACTIVE LOANS");
                lib.viewActiveLoans();
                waitForEnter();
                break;
            }
            case 4:
            {
                printHeader("UPDATE DUE DATE");
                lib.viewActiveLoans();
                int loanId = readInt("Loan ID: ");
                std::string newDate = readLine("New Due Date (YYYY-MM-DD): ");
                lib.updateLoanDueDate(loanId, newDate);
                waitForEnter();
                break;
            }
            case 0:
                back = true;
                break;
            default:
                std::cout << "  Invalid choice.\n";
                waitForEnter();
        }
    }
}

void reservationMenu(Library& lib)
{
    bool back = false;
    while (!back)
    {
        printHeader("RESERVATION MANAGEMENT");
        std::cout << "  1. Reserve a Book\n";
        std::cout << "  2. View Reservations for a Book\n";
        std::cout << "  0. Back\n";

        int choice = readInt("Choice: ");
        if (std::cin.eof() || choice == -1) break;

        switch (choice)
        {
            case 1:
            {
                printHeader("RESERVE BOOK");
                lib.viewAllBooks();
                int bookId   = readInt("Book ID  : ");
                lib.viewAllMembers();
                int memberId = readInt("Member ID: ");
                lib.reserveBook(bookId, memberId);
                waitForEnter();
                break;
            }
            case 2:
            {
                printHeader("VIEW RESERVATIONS");
                int bookId = readInt("Book ID: ");
                lib.viewReservations(bookId);
                waitForEnter();
                break;
            }
            case 0:
                back = true;
                break;
            default:
                std::cout << "  Invalid choice.\n";
                waitForEnter();
        }
    }
}

// ─── Main ────────────────────────────────────────────────────────────────────

int main()
{
    Library library;

    bool running = true;
    while (running)
    {
        clearScreen();
        printHeader(Constants::APP_TITLE);

        std::cout << "\n";
        std::cout << "  1. Book Management\n";
        std::cout << "  2. Member Management\n";
        std::cout << "  3. Loan Management\n";
        std::cout << "  4. Reservation Management\n";
        std::cout << "  0. Exit\n";
        std::cout << "\n";

        int choice = readInt("Choose an option: ");

        // Handle EOF (e.g., piped input ended)
        if (std::cin.eof()) break;

        switch (choice)
        {
            case 1:
                bookMenu(library);
                break;
            case 2:
                memberMenu(library);
                break;
            case 3:
                loanMenu(library);
                break;
            case 4:
                reservationMenu(library);
                break;
            case 0:
                running = false;
                std::cout << "\n  Goodbye!\n\n";
                break;
            default:
                std::cout << "  Invalid option. Please try again.\n";
                waitForEnter();
        }
    }

    return 0;
}