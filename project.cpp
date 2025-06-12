#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>
#include <algorithm>

using namespace std;

class Book {
public:
    int id;
    string title;
    string author;
    int quantity;

    Book() : id(0), title(""), author(""), quantity(0) {}
    
    void inputBookData() {
        cout << "Enter Book ID: ";
        cin >> id;
        cin.ignore();
        cout << "Enter Book Title: ";
        getline(cin, title);
        cout << "Enter Author: ";
        getline(cin, author);
        cout << "Enter Quantity: ";
        cin >> quantity;
    }

    void displayBookData() const {
        cout << left << setw(10) << id
             << left << setw(30) << title
             << left << setw(30) << author
             << left << setw(10) << quantity << endl;
    }
};

class User {
public:
    int id;
    string name;
    string role;  // Can be "Student" or "Staff"

    User() : id(0), name(""), role("") {}
    
    void inputUserData() {
        cout << "Enter User ID: ";
        cin >> id;
        cin.ignore();
        cout << "Enter User Name: ";
        getline(cin, name);
        cout << "Enter Role (Student/Staff): ";
        getline(cin, role);
    }

    void displayUserData() const {
        cout << left << setw(10) << id
             << left << setw(30) << name
             << left << setw(20) << role << endl;
    }
};

class IssueRecord {
public:
    int userId;
    int bookId;
    time_t issueDate;
    bool returned;

    IssueRecord() : userId(0), bookId(0), issueDate(time(0)), returned(false) {}

    void displayIssueRecord() const {
        tm* lt = localtime(&issueDate);
        cout << left << setw(10) << userId
             << left << setw(10) << bookId
             << left << setw(20) << put_time(lt, "%Y-%m-%d %H:%M:%S")
             << left << setw(10) << (returned ? "Yes" : "No") << endl;
    }
};

class LibraryManagementSystem {
private:
    vector<Book> books;
    vector<User> users;
    vector<IssueRecord> issuedBooks;

    void loadBooks() {
        ifstream file("books.dat", ios::binary);
        Book book;
        while (file.read(reinterpret_cast<char*>(&book), sizeof(Book))) {
            books.push_back(book);
        }
        file.close();
    }

    void saveBooks() {
        ofstream file("books.dat", ios::binary);
        for (const auto& book : books) {
            file.write(reinterpret_cast<const char*>(&book), sizeof(Book));
        }
        file.close();
    }

    void loadUsers() {
        ifstream file("users.dat", ios::binary);
        User user;
        while (file.read(reinterpret_cast<char*>(&user), sizeof(User))) {
            users.push_back(user);
        }
        file.close();
    }

    void saveUsers() {
        ofstream file("users.dat", ios::binary);
        for (const auto& user : users) {
            file.write(reinterpret_cast<const char*>(&user), sizeof(User));
        }
        file.close();
    }

    void loadIssuedBooks() {
        ifstream file("issuedBooks.dat", ios::binary);
        IssueRecord record;
        while (file.read(reinterpret_cast<char*>(&record), sizeof(IssueRecord))) {
            issuedBooks.push_back(record);
        }
        file.close();
    }

    void saveIssuedBooks() {
        ofstream file("issuedBooks.dat", ios::binary);
        for (const auto& record : issuedBooks) {
            file.write(reinterpret_cast<const char*>(&record), sizeof(IssueRecord));
        }
        file.close();
    }

public:
    LibraryManagementSystem() {
        loadBooks();
        loadUsers();
        loadIssuedBooks();
    }

    ~LibraryManagementSystem() {
        saveBooks();
        saveUsers();
        saveIssuedBooks();
    }

    void addBook() {
        Book book;
        book.inputBookData();
        books.push_back(book);
        saveBooks();
        cout << "Book added successfully.\n";
    }

    void displayBooks() const {
        cout << left << setw(10) << "ID"
             << left << setw(30) << "Title"
             << left << setw(30) << "Author"
             << left << setw(10) << "Quantity" << endl;
        for (const auto& book : books) {
            book.displayBookData();
        }
    }

    void searchBook(int id) const {
        for (const auto& book : books) {
            if (book.id == id) {
                book.displayBookData();
                return;
            }
        }
        cout << "Book not found.\n";
    }

    void updateBook(int id) {
        for (auto& book : books) {
            if (book.id == id) {
                cout << "Current details:\n";
                book.displayBookData();
                cout << "Enter new details:\n";
                book.inputBookData();
                saveBooks();
                cout << "Book updated successfully.\n";
                return;
            }
        }
        cout << "Book not found.\n";
    }

    void deleteBook(int id) {
        auto it = remove_if(books.begin(), books.end(), [id](const Book& book) {
            return book.id == id;
        });
        if (it != books.end()) {
            books.erase(it, books.end());
            saveBooks();
            cout << "Book deleted successfully.\n";
        } else {
            cout << "Book not found.\n";
        }
    }

    void addUser() {
        User user;
        user.inputUserData();
        users.push_back(user);
        saveUsers();
        cout << "User added successfully....\n";
    }

    void displayUsers() const {
        cout << left << setw(10) << "ID"
             << left << setw(30) << "Name"
             << left << setw(20) << "Role" << endl;
        for (const auto& user : users) {
            user.displayUserData();
        }
    }

    void searchUser(int id) const {
        for (const auto& user : users) {
            if (user.id == id) {
                user.displayUserData();
                return;
            }
        }
        cout << "User not found.\n";
    }

    void updateUser(int id) {
        for (auto& user : users) {
            if (user.id == id) {
                cout << "Current details:\n";
                user.displayUserData();
                cout << "Enter new details:\n";
                user.inputUserData();
                saveUsers();
                cout << "User updated successfully.\n";
                return;
            }
        }
        cout << "User not found.\n";
    }

    void deleteUser(int id) {
        auto it = remove_if(users.begin(), users.end(), [id](const User& user) {
            return user.id == id;
        });
        if (it != users.end()) {
            users.erase(it, users.end());
            saveUsers();
            cout << "User deleted successfully.\n";
        } else {
            cout << "User not found.\n";
        }
    }

    void issueBook(int userId, int bookId) {
        auto userIt = find_if(users.begin(), users.end(), [userId](const User& user) {
            return user.id == userId;
        });
        auto bookIt = find_if(books.begin(), books.end(), [bookId](const Book& book) {
            return book.id == bookId;
        });

        if (userIt != users.end() && bookIt != books.end()) {
            if (bookIt->quantity > 0) {
                IssueRecord record;
                record.userId = userId;
                record.bookId = bookId;
                record.issueDate = time(0);
                record.returned = false;

                issuedBooks.push_back(record);
                bookIt->quantity--;

                saveBooks();
                saveIssuedBooks();

                cout << "Book issued successfully.\n";
            } else {
                cout << "Sorry, the book is currently out of stock.\n";
            }
        } else {
            cout << "User or Book not found.\n";
        }
    }

    void returnBook(int userId, int bookId) {
        for (auto& record : issuedBooks) {
            if (record.userId == userId && record.bookId == bookId && !record.returned) {
                record.returned = true;
                for (auto& book : books) {
                    if (book.id == bookId) {
                        book.quantity++;
                        break;
                    }
                }

                saveBooks();
                saveIssuedBooks();

                cout << "Book returned successfully.\n";
                return;
            }
        }
        cout << "No record of this book being issued to this user.\n";
    }

    void displayIssuedBooks() const {
        cout << left << setw(10) << "User ID"
             << left << setw(10) << "Book ID"
             << left << setw(20) << "Issue Date"
             << left << setw(10) << "Returned" << endl;
        for (const auto& record : issuedBooks) {
            record.displayIssueRecord();
        }
    }

    void displayOverdueBooks(int daysOverdue) const {
        time_t now = time(0);
        cout << left << setw(10) << "User ID"
             << left << setw(10) << "Book ID"
             << left << setw(20) << "Issue Date"
             << left << setw(10) << "Returned" << endl;
        for (const auto& record : issuedBooks) {
            if (!record.returned) {
                double diffDays = difftime(now, record.issueDate) / (60 * 60 * 24);
                if (diffDays > daysOverdue) {
                    record.displayIssueRecord();
                }
            }
        }
    }
};

int main() {
    LibraryManagementSystem library;

    int choice;
    while (true) {
        cout << "                        =========== LIBRARY MANAGEMENT SYSTEM ==========            \n";
        cout << "1. Add Book\n";
        cout << "2. Display Books\n";
        cout << "3. Search Book\n";
        cout << "4. Update Book\n";
        cout << "5. Delete Book\n";
        cout << "6. Add User\n";
        cout << "7. Display Users\n";
        cout << "8. Search User\n";
        cout << "9. Update User\n";
        cout << "10. Delete User\n";
        cout << "11. Issue Book\n";
        cout << "12. Return Book\n";
        cout << "13. Display Issued Books\n";
        cout << "14. Display Overdue Books\n";
        cout << "15. Exit\n"<<endl;
        cout << "Enter your choice: "<<endl;
        cin >> choice;
        cout<<endl;

        if (choice == 1) {
            library.addBook();
            cout<<endl;
        } else if (choice == 2) {
            library.displayBooks();
            cout<<endl;

        } else if (choice == 3) {
            int id;
            cout << "Enter Book ID: ";
            cin >> id;
            library.searchBook(id);
            cout<<endl;
        } else if (choice == 4) {
            int id;
            cout << "Enter Book ID: ";
            cin >> id;
            library.updateBook(id);
            cout<<endl;
        } else if (choice == 5) {
            int id;
            cout << "Enter Book ID: ";
            cin >> id;
            library.deleteBook(id);
            cout<<endl; 
        } else if (choice == 6) {
            library.addUser();
            cout<<endl;

        } else if (choice == 7) {
            library.displayUsers();
            cout<<endl;
        } else if (choice == 8) {
            int id;
            cout << "Enter User ID: ";
            cin >> id;
            library.searchUser(id);
            cout<<endl;
        } else if (choice == 9) {
            int id;
            cout << "Enter User ID: ";
            cin >> id;
            library.updateUser(id);
            cout<<endl;
        } else if (choice == 10) {
            int id;
            cout << "Enter User ID: ";
            cin >> id;
            library.deleteUser(id);
            cout<<endl;
        } else if (choice == 11) {
            int userId, bookId;
            cout << "Enter User ID: ";
            cin >> userId;
            cout << "Enter Book ID: ";
            cin >> bookId;
            library.issueBook(userId, bookId);
            cout<<endl;
        } else if (choice == 12) {
            int userId, bookId;
            cout << "Enter User ID: ";
            cin >> userId;
            cout << "Enter Book ID: ";
            cin >> bookId;
            library.returnBook(userId, bookId);
            cout<<endl;
        } else if (choice == 13) {
            library.displayIssuedBooks();
        } else if (choice == 14) {
            int daysOverdue;
            cout << "Enter number of days overdue: ";
            cin >> daysOverdue;
            library.displayOverdueBooks(daysOverdue);
            cout<<endl;
        } else if (choice == 15) {
            cout << "Exiting...\n";
            cout<<endl;
            break;
        } else {
            cout << "Invalid choice. Please try again.\n";
            cout<<endl;
        }
    }

    return 0;
}



