#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <ctime>

using namespace std;

struct Book {
    string title;
    string author;
    string ISBN;
    bool isAvailable;
};

struct Borrower {
    string name;
    string borrowedISBN;
    time_t dueDate;
};

unordered_map<string, Book> books;
unordered_map<string, Borrower> borrowers;
const int finePerDay = 1; // fine amount per day

// Function declarations
void addBook();
void searchBook();
void checkoutBook();
void returnBook();
void calculateFine();

void displayMenu() {
    cout << "\nLibrary Management System Menu:\n";
    cout << "1. Add Book\n";
    cout << "2. Search Book\n";
    cout << "3. Checkout Book\n";
    cout << "4. Return Book\n";
    cout << "5. Calculate Fine\n";
    cout << "6. Exit\n";
}

void addBook() {
    Book newBook;
    cout << "Enter book title: ";
    getline(cin, newBook.title);
    cout << "Enter book author: ";
    getline(cin, newBook.author);
    cout << "Enter book ISBN: ";
    getline(cin, newBook.ISBN);
    newBook.isAvailable = true;
    books[newBook.ISBN] = newBook;
    cout << "Book added successfully.\n";
}

void searchBook() {
    string query;
    cout << "Enter title, author, or ISBN to search: ";
    getline(cin, query);
    bool found = false;
    for (const auto& pair : books) {
        const Book& book = pair.second;
        if (book.title.find(query) != string::npos || book.author.find(query) != string::npos || book.ISBN == query) {
            cout << "Title: " << book.title << ", Author: " << book.author << ", ISBN: " << book.ISBN << ", Available: " << (book.isAvailable ? "Yes" : "No") << '\n';
            found = true;
        }
    }
    if (!found) {
        cout << "No books found.\n";
    }
}

void checkoutBook() {
    string ISBN, borrowerName;
    cout << "Enter ISBN of the book to checkout: ";
    getline(cin, ISBN);
    if (books.find(ISBN) == books.end()) {
        cout << "Book not found.\n";
        return;
    }
    if (!books[ISBN].isAvailable) {
        cout << "Book is already checked out.\n";
        return;
    }
    cout << "Enter borrower's name: ";
    getline(cin, borrowerName);

    books[ISBN].isAvailable = false;
    Borrower newBorrower = { borrowerName, ISBN, time(nullptr) + 7 * 24 * 60 * 60 }; // 1 week from now
    borrowers[borrowerName] = newBorrower;

    cout << "Book checked out successfully. Due in 7 days.\n";
}

void returnBook() {
    string borrowerName;
    cout << "Enter borrower's name: ";
    getline(cin, borrowerName);
    if (borrowers.find(borrowerName) == borrowers.end()) {
        cout << "Borrower not found.\n";
        return;
    }
    string ISBN = borrowers[borrowerName].borrowedISBN;
    books[ISBN].isAvailable = true;
    borrowers.erase(borrowerName);

    cout << "Book returned successfully.\n";
}

void calculateFine() {
    string borrowerName;
    cout << "Enter borrower's name: ";
    getline(cin, borrowerName);
    if (borrowers.find(borrowerName) == borrowers.end()) {
        cout << "Borrower not found.\n";
        return;
    }
    time_t now = time(nullptr);
    time_t dueDate = borrowers[borrowerName].dueDate;
    if (now > dueDate) {
        int overdueDays = (now - dueDate) / (24 * 60 * 60);
        cout << "Fine amount: $" << overdueDays * finePerDay << '\n';
    } else {
        cout << "No fine. Book is not overdue.\n";
    }
}

int main() {
    while (true) {
        displayMenu();
        int choice;
        cout << "Choose an option (1-6): ";
        cin >> choice;
        cin.ignore(); // to consume the newline character left by cin
        switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                searchBook();
                break;
            case 3:
                checkoutBook();
                break;
            case 4:
                returnBook();
                break;
            case 5:
                calculateFine();
                break;
            case 6:
                cout << "Exiting Library Management System. Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice. Please choose a valid option.\n";
        }
    }
    return 0;
}

