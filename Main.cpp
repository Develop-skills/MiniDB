#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

struct Student {
    int id;
    char name[20];
    float marks;
};

bool idExists(int id) {
    ifstream inFile("students.db", ios::binary);
    Student temp;

    while (inFile.read((char*)&temp, sizeof(Student))) {
        if (temp.id == id) {
            inFile.close();
            return true;
        }
    }
    inFile.close();
    return false;
}

void insertStudent(Student s) {
    if (idExists(s.id)) {
        cout << "Error: Student with ID " << s.id << " already exists. Insert rejected." << endl;
        return;
    }

    ofstream outFile("students.db", ios::binary | ios::app);
    outFile.write((char*)&s, sizeof(Student));
    outFile.close();
    cout << "Inserted: " << s.id << ", " << s.name << ", " << s.marks << endl;
}

void printAllStudents() {
    ifstream inFile("students.db", ios::binary);
    Student temp;

    cout << "\nAll records in students.db:" << endl;
    while (inFile.read((char*)&temp, sizeof(Student))) {
        cout << "ID: " << temp.id << ", Name: " << temp.name << ", Marks: " << temp.marks << endl;
    }
    inFile.close();
}

// NEW FUNCTION
void findStudentById(int id) {
    ifstream inFile("students.db", ios::binary);
    Student temp;
    bool found = false;

    while (inFile.read((char*)&temp, sizeof(Student))) {
        if (temp.id == id) {
            cout << "Found -> ID: " << temp.id << ", Name: " << temp.name << ", Marks: " << temp.marks << endl;
            found = true;
            break;  // stop searching once found
        }
    }
    inFile.close();

    if (!found) {
        cout << "No student found with ID " << id << endl;
    }
}

int main() {
    findStudentById(2);   // should find Priya
    findStudentById(100); // should not exist

    return 0;
}