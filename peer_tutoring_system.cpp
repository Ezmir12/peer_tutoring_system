#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// Fields are '|'-delimited rather than space-delimited. Names and contact
// info are free text and can legitimately contain spaces (e.g. "John
// Smith"), so a space delimiter would silently corrupt every field read
// after the first one that contains a space.
const char DELIM = '|';

struct User {
    string id;
    string name;
    string contact;
    string password;
};

bool loginUser(ifstream& in, string& userID) {
    if (!in) {
        cout << "Error: Unable to open file" << "\n";
        return false;
    }

    string enteredID, enteredPassword;
    cout << "Enter ID: ";
    cin >> enteredID;
    cout << "Enter Password: ";
    cin >> enteredPassword;

    string line;
    while (getline(in, line)) {
        stringstream ss(line);
        string id, name, contact, password;
        getline(ss, id, DELIM);
        getline(ss, name, DELIM);
        getline(ss, contact, DELIM);
        getline(ss, password, DELIM);

        if (id == enteredID && password == enteredPassword) {
            userID = id;
            return true;
        }
    }
    return false;
}

void registerUser(ofstream& out, const string& userType) {
    if (!out) {
        cout << "Error: Unable to open file" << "\n";
        return;
    }

    User user;
    cout << "Enter " << userType << " ID: ";
    cin >> user.id;
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, user.name);
    cout << "Enter Contact Info: ";
    getline(cin, user.contact);
    cout << "Enter Password: ";
    cin >> user.password;

    out << user.id << DELIM << user.name << DELIM << user.contact << DELIM << user.password << "\n";
    out.close();
    cout << userType << " registered successfully!\n";
}

void scheduleSession() {
    string studentID;
    ifstream in("students.txt");
    if (!loginUser(in, studentID)) {
        cout << "Invalid login!\n";
        return;
    }
    in.close();

    in.open("tutoring.txt");
    if (!in) {
        cout << "Error: No tutors available.\n";
        return;
    }

    ofstream out("sessions.txt", ios::app);

    cout << "Available Tutors:\n";
    string line;
    while (getline(in, line)) {
        stringstream ss(line);
        string id, name, contact, password;
        getline(ss, id, DELIM);
        getline(ss, name, DELIM);
        getline(ss, contact, DELIM);
        getline(ss, password, DELIM);
        cout << "ID: " << id << ", Name: " << name << ", Contact: " << contact << "\n";
    }
    in.close();

    string tutorID, date, time;
    cout << "Enter Tutor ID to schedule session: ";
    cin >> tutorID;
    cout << "Enter Session Date (YYYY-MM-DD): ";
    cin >> date;
    cout << "Enter Session Time (HH:MM) using 24 hr format: ";
    cin >> time;

    out << studentID << DELIM << tutorID << DELIM << date << DELIM << time << DELIM << "pending" << "\n";
    out.close();
    cout << "Session requested successfully!\n";
}

void approveSessions() {
    string tutorID;
    ifstream in("tutoring.txt");
    if (!loginUser(in, tutorID)) {
        cout << "Invalid login!\n";
        return;
    }
    in.close();

    in.open("sessions.txt");
    if (!in) {
        cout << "Error: No sessions available.\n";
        return;
    }

    ofstream out("temp.txt");
    bool found = false;

    string line;
    while (getline(in, line)) {
        stringstream ss(line);
        string studentID, sessionTutorID, date, time, status;
        getline(ss, studentID, DELIM);
        getline(ss, sessionTutorID, DELIM);
        getline(ss, date, DELIM);
        getline(ss, time, DELIM);
        getline(ss, status, DELIM);

        if (sessionTutorID == tutorID && status == "pending") {
            found = true;
            cout << "Approve session for Student ID: " << studentID << " on " << date << " at " << time << "? (yes/no): ";
            string response;
            cin >> response;
            if (response == "yes") {
                out << studentID << DELIM << tutorID << DELIM << date << DELIM << time << DELIM << "approved" << "\n";

                ifstream inStudents("students.txt");
                string sline;
                while (getline(inStudents, sline)) {
                    stringstream sss(sline);
                    string id, name, contact, password;
                    getline(sss, id, DELIM);
                    getline(sss, name, DELIM);
                    getline(sss, contact, DELIM);
                    getline(sss, password, DELIM);
                    if (id == studentID) {
                        cout << "Student Contact: " << contact << "\n";
                        break;
                    }
                }
                inStudents.close();
            } else {
                out << studentID << DELIM << tutorID << DELIM << date << DELIM << time << DELIM << "pending" << "\n";
            }
        } else {
            out << studentID << DELIM << sessionTutorID << DELIM << date << DELIM << time << DELIM << status << "\n";
        }
    }
    in.close();
    out.close();
    remove("sessions.txt");
    rename("temp.txt", "sessions.txt");

    if (!found) {
        cout << "No pending sessions found for you.\n";
    } else {
        cout << "Session approvals updated!\n";
    }
}

int main() {
    int choice;
    while (true) {
        cout << "1. Register Student\n2. Register Tutor\n3. Schedule Session\n4. Approve Sessions\n5. Exit\nChoice: ";
        cin >> choice;
        switch (choice) {
            case 1: {
                ofstream out("students.txt", ios::app);
                registerUser(out, "Student");
                break;
            }
            case 2: {
                ofstream out("tutoring.txt", ios::app);
                registerUser(out, "Tutor");
                break;
            }
            case 3: scheduleSession(); break;
            case 4: approveSessions(); break;
            case 5: return 0;
            default: cout << "Invalid choice!\n";
        }
    }
}
