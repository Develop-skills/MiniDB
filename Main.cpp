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
            return true;  // found a match, ID already exists
        }
    }
    inFile.close();
    return false;  // went through whole file, no match
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

int main() {
    Student newStudent;
    newStudent.id = 5;
    strcpy(newStudent.name, "Karan");
    newStudent.marks = 70.0;

    insertStudent(newStudent);   // should succeed, ID 5 is new
    insertStudent(newStudent);   // should fail, ID 5 already exists now

    printAllStudents();

    return 0;
}