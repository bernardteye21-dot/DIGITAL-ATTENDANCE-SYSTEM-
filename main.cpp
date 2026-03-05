#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

// Student Class
class Student {
private:
    string indexNumber;
    string name;
public:
    Student() {}
    Student(string idx, string n) : indexNumber(idx), name(n) {}
    
    string getIndex() { return indexNumber; }
    string getName() { return name; }
    
    void display() {
        cout << indexNumber << " - " << name << endl;
    }
};

// AttendanceSession Class
class AttendanceSession {
private:
    string courseCode;
    string date;
    string startTime;
    int duration;
public:
    AttendanceSession(string c, string d, string t, int dur) {
        courseCode = c;
        date = d;
        startTime = t;
        duration = dur;
    }
    
    void display() {
        cout << courseCode << " | " << date << " | " << startTime << " | " << duration << "min" << endl;
    }
    
    void saveToFile() {
        ofstream file("sessions.txt", ios::app);
        file << courseCode << "," << date << "," << startTime << "," << duration << endl;
        file.close();
    }
};

// Global Variables
vector<Student> students;
vector<AttendanceSession> sessions;

// File Handling Functions
void saveStudents() {
    ofstream file("students.txt");
    for (Student s : students) {
        file << s.getIndex() << "," << s.getName() << endl;
    }
    file.close();
    cout << "Students saved to file successfully!\n";
}

void loadStudents() {
    ifstream file("students.txt");
    string line, idx, name;
    students.clear();
    
    while (getline(file, line)) {
        size_t pos = line.find(",");
        if (pos != string::npos) {
            idx = line.substr(0, pos);
            name = line.substr(pos + 1);
            students.push_back(Student(idx, name));
        }
    }
    file.close();
    cout << "Students loaded from file successfully!\n";
}

void loadSessions() {
    ifstream file("sessions.txt");
    string line, course, date, time;
    int duration;
    sessions.clear();
    
    while (getline(file, line)) {
        size_t pos1 = line.find(",");
        size_t pos2 = line.find(",", pos1 + 1);
        size_t pos3 = line.find(",", pos2 + 1);
        
        if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos) {
            course = line.substr(0, pos1);
            date = line.substr(pos1 + 1, pos2 - pos1 - 1);
            time = line.substr(pos2 + 1, pos3 - pos2 - 1);
            duration = stoi(line.substr(pos3 + 1));
            sessions.push_back(AttendanceSession(course, date, time, duration));
        }
    }
    file.close();
    cout << "Sessions loaded from file successfully!\n";
}

// Menu Functions
void registerStudent() {
    string idx, name;
    cout << "\n--- REGISTER STUDENT ---\n";
    cout << "Enter Index Number: ";
    cin >> idx;
    cin.ignore();
    cout << "Enter Student Name: ";
    getline(cin, name);
    
    students.push_back(Student(idx, name));
    saveStudents();
    cout << "Student Registered Successfully!\n";
}

void viewStudents() {
    cout << "\n--- STUDENT LIST ---\n";
    if (students.empty()) {
        cout << "No students registered.\n";
        return;
    }
    for (int i = 0; i < students.size(); i++) {
        cout << i+1 << ". ";
        students[i].display();
    }
}

void createSession() {
    string course, date, time;
    int duration;
    
    cout << "\n--- CREATE LECTURE SESSION ---\n";
    cout << "Course Code (e.g., EEE227): ";
    cin >> course;
    cout << "Date (YYYY-MM-DD): ";
    cin >> date;
    cout << "Start Time (HH:MM): ";
    cin >> time;
    cout << "Duration (minutes): ";
    cin >> duration;
    
    AttendanceSession newSession(course, date, time, duration);
    sessions.push_back(newSession);
    newSession.saveToFile();
    cout << "Session Created Successfully!\n";
}

void viewSessions() {
    cout << "\n--- LECTURE SESSIONS ---\n";
    if (sessions.empty()) {
        cout << "No sessions created.\n";
        return;
    }
    for (int i = 0; i < sessions.size(); i++) {
        cout << i+1 << ". ";
        sessions[i].display();
    }
}

void markAttendance() {
    cout << "\n--- MARK ATTENDANCE ---\n";
    
    if (sessions.empty()) {
        cout << "No sessions available. Please create a session first.\n";
        return;
    }
    
    // Display sessions
    viewSessions();
    
    int sessionChoice;
    cout << "Select session number: ";
    cin >> sessionChoice;
    
    if (sessionChoice < 1 || sessionChoice > sessions.size()) {
        cout << "Invalid session choice!\n";
        return;
    }
    
    if (students.empty()) {
        cout << "No students registered. Please register students first.\n";
        return;
    }
    
    // Create attendance file for this session
    string filename = "attendance_" + to_string(sessionChoice) + ".txt";
    ofstream attFile(filename, ios::app);
    attFile << "Session: ";
    sessions[sessionChoice-1].display();
    attFile << "\nAttendance:\n";
    
    cout << "\n--- MARK ATTENDANCE FOR SESSION " << sessionChoice << " ---\n";
    
    for (int i = 0; i < students.size(); i++) {
        cout << i+1 << ". ";
        students[i].display();
        
        char status;
        cout << "   Present (P), Absent (A), or Late (L)? ";
        cin >> status;
        
        attFile << students[i].getIndex() << "," << students[i].getName() << "," << status << endl;
    }
    
    attFile.close();
    cout << "Attendance saved to " << filename << "!\n";
}

void viewReports() {
    cout << "\n--- ATTENDANCE REPORTS ---\n";
    
    cout << "1. View all students\n";
    cout << "2. View all sessions\n";
    cout << "3. View attendance for a session\n";
    cout << "Choose option: ";
    
    int reportChoice;
    cin >> reportChoice;
    
    switch(reportChoice) {
        case 1:
            viewStudents();
            break;
        case 2:
            viewSessions();
            break;
        case 3: {
            viewSessions();
            if (sessions.empty()) break;
            
            cout << "Enter session number: ";
            int sessNum;
            cin >> sessNum;
            
            string filename = "attendance_" + to_string(sessNum) + ".txt";
            ifstream attFile(filename);
            
            if (!attFile.is_open()) {
                cout << "No attendance records for this session.\n";
                break;
            }
            
            string line;
            cout << "\n--- ATTENDANCE FOR SESSION " << sessNum << " ---\n";
            while (getline(attFile, line)) {
                cout << line << endl;
            }
            attFile.close();
            break;
        }
        default:
            cout << "Invalid option!\n";
    }
}

// Main Function
int main() {
    // Load data at start
    loadStudents();
    loadSessions();
    
    int choice;
    
    do {
        cout << "\n=== DIGITAL ATTENDANCE SYSTEM ===\n";
        cout << "1. Register Student\n";
        cout << "2. View All Students\n";
        cout << "3. Create Lecture Session\n";
        cout << "4. View All Sessions\n";
        cout << "5. Mark Attendance\n";
        cout << "6. View Reports\n";
        cout << "7. Save All Data\n";
        cout << "8. Exit\n";
        cout << "Enter choice (1-8): ";
        cin >> choice;
        
        switch(choice) {
            case 1: 
                registerStudent(); 
                break;
            case 2: 
                viewStudents(); 
                break;
            case 3: 
                createSession(); 
                break;
            case 4: 
                viewSessions(); 
                break;
            case 5: 
                markAttendance(); 
                break;
            case 6: 
                viewReports(); 
                break;
            case 7:
                saveStudents();
                cout << "Data saved successfully!\n";
                break;
            case 8: 
                cout << "Goodbye! Thanks for using the system.\n"; 
                break;
            default: 
                cout << "Invalid choice! Please try again.\n";
        }
    } while(choice != 8);
    
    return 0;
}
