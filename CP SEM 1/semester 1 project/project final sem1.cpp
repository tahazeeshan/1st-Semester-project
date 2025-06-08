#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <sstream>
#include <windows.h>
#include <iomanip>

using namespace std;

struct User {
    string role, id, password;
};

struct Assignment {
    string title, deadline, uploader;
};

struct Trip {
    string userId, destination, date;
};

struct Expense {
    string userId, description;
    float amount;
};

// -------------------- Utility --------------------

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void typeEffect(const string& text, int delay = 30) {
    setColor(11);
    for (size_t i = 0; i < text.length(); ++i) {
        cout << text[i] << flush;
        Sleep(delay);
    }
    setColor(7);
    cout << endl;
}

void loadingBar() {
    setColor(11);
    cout << "\n\n\t\t\t\t....MANAGE YOUR STUDY LIFE";
    for (int i = 0; i < 4; ++i) {
        Sleep(300);
       cout << ".";
    }
    setColor(7);
    cout << "\n";
}

void clearScreen() {
    system("cls");
}

void animatedTitle(const string& title) {
    clearScreen();
    setColor(11);
    cout << "\n\n\t\t\t\t==== STUDENT MANAGER 1.0 ====\n";
     
    setColor(7);
}

bool isDueSoon(const string& deadlineStr) {
    time_t now = time(0);
    tm deadline;
    memset(&deadline, 0, sizeof(tm));
    int year, month, day, hour, minute;
    if (sscanf(deadlineStr.c_str(), "%d-%d-%d %d:%d", &year, &month, &day, &hour, &minute) != 5) {
        return false;
    }
    deadline.tm_year = year - 1900;
    deadline.tm_mon = month - 1;
    deadline.tm_mday = day;
    deadline.tm_hour = hour;
    deadline.tm_min = minute;
    deadline.tm_sec = 0;
    time_t deadlineTime = mktime(&deadline);
    return difftime(deadlineTime, now) > 0 && difftime(deadlineTime, now) <= 86400;
}

// -------------------- File Handling Utilities --------------------

vector<User> loadUsers() {
    vector<User> users;
    ifstream in("users.txt");
    string line;
    while (getline(in, line)) {
        istringstream iss(line);
        string role, id, pass;
        getline(iss, role, '|');
        getline(iss, id, '|');
        getline(iss, pass);
        if (!id.empty()) {
            User u;
            u.role = role;
            u.id = id;
            u.password = pass;
            users.push_back(u);
        }
    }
    return users;
}

void saveUser(const User& user) {
    ofstream out("users.txt", ios::app);
    out << user.role << "|" << user.id << "|" << user.password << "\n";
}

vector<Assignment> loadAssignments() {
    vector<Assignment> assignments;
    ifstream in("assignments.txt");
    string line;
    while (getline(in, line)) {
        istringstream iss(line);
        string title, deadline, uploader;
        getline(iss, title, '|');
        getline(iss, deadline, '|');
        getline(iss, uploader);
        Assignment a;
        a.title = title;
        a.deadline = deadline;
        a.uploader = uploader;
        assignments.push_back(a);
    }
    return assignments;
}

void saveAssignment(const Assignment& a) {
    ofstream out("assignments.txt", ios::app);
    out << a.title << "|" << a.deadline << "|" << a.uploader << "\n";
}

vector<Trip> loadTrips() {
    vector<Trip> trips;
    ifstream in("trips.txt");
    string line;
    while (getline(in, line)) {
        istringstream iss(line);
        string uid, dest, date;
        getline(iss, uid, '|');
        getline(iss, dest, '|');
        getline(iss, date);
        Trip t;
        t.userId = uid;
        t.destination = dest;
        t.date = date;
        trips.push_back(t);
    }
    return trips;
}

void saveTrip(const Trip& t) {
    ofstream out("trips.txt", ios::app);
    out << t.userId << "|" << t.destination << "|" << t.date << "\n";
}

vector<Expense> loadExpenses() {
    vector<Expense> expenses;
    ifstream in("expenses.txt");
    string line;
    while (getline(in, line)) {
        istringstream iss(line);
        string uid, desc, amtStr;
        float amt = 0.0f;
        getline(iss, uid, '|');
        getline(iss, desc, '|');
        getline(iss, amtStr);
        istringstream amtStream(amtStr);
        amtStream >> amt;
        Expense e;
        e.userId = uid;
        e.description = desc;
        e.amount = amt;
        expenses.push_back(e);
    }
    return expenses;
}

void saveExpense(const Expense& e) {
    ofstream out("expenses.txt", ios::app);
    out << e.userId << "|" << e.description << "|" << fixed << setprecision(2) << e.amount << "\n";
}

void addExpenseMenu(const string& userId) {
    Expense e;
    e.userId = userId;
    setColor(11);
    cout << "\nAdd New Expense" << endl;
    setColor(7);
    cout << "Description: ";
    cin.ignore();
    getline(cin, e.description);
    cout << "Amount: ";
    cin >> e.amount;
    saveExpense(e);
    setColor(10);
    cout << "Expense saved successfully!\n";
    setColor(7);
}

void tripManagerMenu(const string& userId) {
    Trip t;
    t.userId = userId;
    setColor(11);
    cout << "\nTrip Manager\n";
    setColor(7);
    cin.ignore();
    cout << "Destination: ";
    getline(cin, t.destination);
    cout << "Date (YYYY-MM-DD): ";
    getline(cin, t.date);
    saveTrip(t);
    setColor(10);
    cout << "Trip Added successfully!\n";
    setColor(7);
}

void showAllAssignments() {
    vector<Assignment> assignments = loadAssignments();
    setColor(11);
    cout << "\nAll Assignments:\n";
    setColor(7);
    for (size_t i = 0; i < assignments.size(); ++i) {
        cout << "Title: " << assignments[i].title << "\n";
        cout << "Deadline: " << assignments[i].deadline << "\n";
        cout << "Uploader: " << assignments[i].uploader << "\n\n";
    }
}

void registerUser(string role) {
    User u;
    u.role = role;
    cin.ignore();
    setColor(11);
    cout << "\nRegister new " << role << "\n";
    setColor(7);
    cout << "ID: ";
    getline(cin, u.id);
    cout << "Password: ";
    getline(cin, u.password);
    saveUser(u);
    setColor(10);
    cout << role << " registered successfully!\n";
    setColor(7);
}

void teacherMenu(const string& userId) {
    int choice;
    do {
        setColor(11);
        cout << "\nTeacher Menu:\n";
        setColor(7);
        cout << "1. Upload Assignment\n";
        cout << "2. Show All Assignments\n";
        cout << "0. Logout\n";
        cout << "Choose an option: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                Assignment a;
                cin.ignore();
                setColor(11);
                cout << "\nUpload New Assignment\n";
                setColor(7);
                cout << "Title: ";
                getline(cin, a.title);
                cout << "Deadline (YYYY-MM-DD HH:MM): ";
                getline(cin, a.deadline);
                a.uploader = userId;
                saveAssignment(a);
                setColor(10);
                cout << "Assignment uploaded successfully!\n";
                setColor(7);
                break;
            }
            case 2:
                showAllAssignments();
                break;
            case 0:
                setColor(10);
                cout << "Logging out...\n";
                setColor(7);
                break;
            default:
                setColor(12);
                cout << "Invalid option. Try again.\n";
                setColor(7);
        }
    } while (choice != 0);
}

void adminMenu(const string& userId) {
    int choice;
    do {
        setColor(11);
        cout << "\nAdmin Menu:\n";
        setColor(7);
        cout << "1. Show All Assignments\n";
        cout << "2. Show All trips\n";
        cout << "3. Add Teacher\n";
        cout << "4. Add Student\n";
        cout << "0. Logout\n";
        cout << "Choose an option: ";
        cin >> choice;

        switch (choice) {
            case 1:
                showAllAssignments();
                break;
            case 2: {
                vector<Trip> trips = loadTrips();
                setColor(11);
                cout << "\nAll Trips:\n";
                setColor(7);
                for (size_t i = 0; i < trips.size(); ++i) {
                    cout << "User ID: " << trips[i].userId << "\n";
                    cout << "Destination: " << trips[i].destination << "\n";
                    cout << "Date: " << trips[i].date << "\n\n";
                }
                break;
            }
            case 3:
                registerUser("teacher");
                break;
            case 4:
                registerUser("student");
                break;
            case 0:
                setColor(10);
                cout << "Logging out...\n";
                setColor(7);
                break;
            default:
                setColor(12);
                cout << "Invalid option. Try again.\n";
                setColor(7);
        }
    } while (choice != 0);
}

int main() {
    while (true) {
        animatedTitle("Trip Manager & LMS");
        loadingBar();
        vector<User> users = loadUsers();
        string role, id, password;

        setColor(11);
        cout << "\nLogin as (admin/teacher/student or exit): ";
        setColor(7);
        cin >> role;

        if (role == "exit") {
            setColor(10);
            cout << "\nExiting the program. Goodbye!\n";
            setColor(7);
            break;
        }

        setColor(11);
        cout << "ID: ";
        setColor(7);
        cin >> id;
        setColor(11);
        cout << "Password: ";
        setColor(7);
        cin >> password;

        bool authenticated = false;
       
        setColor(10);
        cout << "\nWelcome, " << role << " " << id << "!\n";
        setColor(7);

        if (role == "student") {
            int choice;
            do {
                setColor(11);
                cout << "\nStudent Menu:\n";
                setColor(7);
                cout << "1. View Assignment Reminders\n";
                cout << "2. Add Expense\n";
                cout << "3. Trip Manager\n";
                cout << "4. Show All Assignments\n";
                cout << "0. Logout\n";
                cout << "Choose an option: ";
                cin >> choice;

                switch (choice) {
                    case 1: {
                        vector<Assignment> assignments = loadAssignments();
                        bool found = false;

                        setColor(11);
                        cout << "\nAssignment Reminder: Due in Next 24 Hours\n";
                        setColor(7);

                        for (size_t i = 0; i < assignments.size(); ++i) {
                            if (isDueSoon(assignments[i].deadline)) {
                                found = true;
                                setColor(11);
                                cout << "\nTitle: " << assignments[i].title << "\n";
                                cout << "Deadline: " << assignments[i].deadline << "\n";
                                setColor(7);
                            }
                        }

                        if (!found) {
                            setColor(10);
                            cout << "\nNo urgent assignments. You're all caught up!\n";
                            setColor(7);
                        }
                        break;
                    }
                    case 2:
                        addExpenseMenu(id);
                        break;
                    case 3:
                        tripManagerMenu(id);
                        break;
                    case 4:
                        showAllAssignments();
                        break;
                    case 0:
                        setColor(10);
                        cout << "Logging out...\n";
                        setColor(7);
                        break;
                    default:
                        setColor(12);
                        cout << "Invalid option. Try again.\n";
                        setColor(7);
                }
            } while (choice != 0);
        } else if (role == "teacher") {
            teacherMenu(id);
        } else if (role == "admin") {
            adminMenu(id);
        }
    }
    return 0;
}
