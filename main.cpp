#include <iostream> 
#include <fstream> 
#include <string> 
#include <vector> 
using namespace std; 
 
class Student { 
private: 
    string indexNumber; 
    string name; 
public: 
    Student() {} 
    Student(string idx, string n) : indexNumber(idx), name(n) {} 
    string getIndex() { return indexNumber; } 
    string getName() { return name; } 
    void display() { cout << indexNumber << " - " << name << endl; } 
}; 
 
int main() { 
    cout << "Digital Attendance System Working!" << endl; 
    return 0; 
} 
