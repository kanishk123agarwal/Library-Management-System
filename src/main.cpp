#include <iostream>

#include "models/Member.h"
#include "models/Librarian.h"
#include "models/Book.h"

using namespace std;

int main()
{
    cout << "========== MEMBER TEST ==========\n";

    Member member(
        1,
        "Kanishk",
        "member@gmail.com"
    );

    member.login();
    member.displayRole();

    member.searchBook();
    member.borrowBook();
    member.reserveBook();
    member.returnBook();

    cout << "\n";


    cout << "========== LIBRARIAN TEST ==========\n";

    Librarian librarian(
        2,
        "Admin",
        "admin@gmail.com"
    );

    librarian.login();
    librarian.displayRole();

    librarian.addBook();
    librarian.updateBook();
    librarian.removeBook();

    cout << "\n";


    cout << "========== BOOK TEST ==========\n";

    Book book(
        101,
        "Clean Code",
        "Robert Martin",
        "9780132350884"
    );

    cout << "Book ID      : " << book.getBookId() << endl;
    cout << "Title        : " << book.getTitle() << endl;
    cout << "Author       : " << book.getAuthor() << endl;
    cout << "ISBN         : " << book.getISBN() << endl;

    cout << "\nAvailability : ";

    if(book.isAvailable())
    {
        cout << "Available\n";
    }
    else
    {
        cout << "Not Available\n";
    }

    cout << "\nIssuing Book...\n";

    book.issue();

    cout << "Availability : ";

    if(book.isAvailable())
    {
        cout << "Available\n";
    }
    else
    {
        cout << "Not Available\n";
    }

    cout << "\nReturning Book...\n";

    book.returnBook();

    cout << "Availability : ";

    if(book.isAvailable())
    {
        cout << "Available\n";
    }
    else
    {
        cout << "Not Available\n";
    }

    cout << "\n";


    cout << "========== POLYMORPHISM TEST ==========\n";

    User* user1 = &member;
    User* user2 = &librarian;

    user1->displayRole();
    user2->displayRole();

    cout << "\n";


    cout << "========== LOGOUT TEST ==========\n";

    member.logout();
    librarian.logout();

    return 0;
}