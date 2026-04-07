#include <iostream>
#include <memory>

#include "models/Loan.h"
#include "core/Library.h"
#include "models/Book.h"
#include "models/Member.h"
#include "models/Reservation.h"

#include "strategies/DailyFineStrategy.h"
#include "strategies/FlatFineStrategy.h"

#include "factories/UserFactory.h"
#include "factories/BookFactory.h"

using namespace std;

int main()
{
    cout << "========== MEMBER TEST ==========\n";

    auto member = UserFactory::createUser(UserType::MEMBER, 1, "Kanishk", "member@gmail.com");

    member->login();
    member->displayRole();

    // member->searchBook();
    // member->borrowBook();
    // member->reserveBook();
    // member->returnBook();

    cout << "\n";

    cout << "========== LIBRARIAN TEST ==========\n";

    auto librarian = UserFactory::createUser(UserType::LIBRARIAN, 2, "Admin", "admin@gmail.com");

    librarian->login();
    librarian->displayRole();
    // librarian->addBook();
    // librarian->updateBook();
    // librarian->removeBook();

    cout << "\n";

    cout << "========== BOOK TEST ==========\n";

    Book book = BookFactory::createBook(
        101,
        "Clean Code",
        "Robert Martin",
        "9780132350884");

    cout << "Book ID      : " << book.getBookId() << endl;
    cout << "Title        : " << book.getTitle() << endl;
    cout << "Author       : " << book.getAuthor() << endl;
    cout << "ISBN         : " << book.getISBN() << endl;

    cout << "\nAvailability : ";

    if (book.isAvailable())
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

    if (book.isAvailable())
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

    if (book.isAvailable())
    {
        cout << "Available\n";
    }
    else
    {
        cout << "Not Available\n";
    }

    cout << "\n";

    cout << "========== POLYMORPHISM TEST ==========\n";

    User *user1 = member.get();
    User *user2 = librarian.get();

    user1->displayRole();
    user2->displayRole();

    cout << "\n";

    cout << "========== LOGOUT TEST ==========\n";

    member->logout();
    librarian->logout();

    cout << "========== LOAN TEST ==========\n";

    std::shared_ptr<IFineStrategy> dailyStrategy = std::make_shared<DailyFineStrategy>(10);

    Loan loan1(5, dailyStrategy);

    std::cout << "Daily Fine: " << loan1.calculateFine() << std::endl;

    std::shared_ptr<IFineStrategy> flatStrategy = std::make_shared<FlatFineStrategy>(100);

    Loan loan2(5, flatStrategy);

    std::cout << "Flat Fine: " << loan2.calculateFine() << std::endl;

    cout << "\n";
    cout << "========== OBSERVER TEST ==========\n";

    Library library;

    auto memberObserver =dynamic_cast<Member *>(member.get());

    if (memberObserver)
    {
        library.addBook(&book);

        library.addMember(memberObserver);

        Reservation reservation(1,book.getBookId(),memberObserver->getUserId());

        library.reserveBook(&book,memberObserver,&reservation);

        cout << "\nBook Reserved\n";

        cout << "\nReturning Book...\n";

        library.returnBook(&book);
    }

    return 0;
}