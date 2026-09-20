#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

struct Student {
    int id;
    char name[20];
    float marks;
};

void insertStudent(Student s) {
    ofstream outFile("students.db", ios::binary | ios::app);  // app = append mode
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
    newStudent.id = 4;
    strcpy(newStudent.name, "Ananya");
    newStudent.marks = 81.0;

    insertStudent(newStudent);
    printAllStudents();

    return 0;
}