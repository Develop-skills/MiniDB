#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

struct Student {
    int id;
    char name[20];
    float marks;
    bool isDeleted;   // false = active, true = deleted
};

bool idExists(int id) {
    ifstream inFile("students.db", ios::binary);
    Student temp;

    while (inFile.read((char*)&temp, sizeof(Student))) {
        if (temp.id == id && !temp.isDeleted) {
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

    s.isDeleted = false;  // always false for a brand new record

    ofstream outFile("students.db", ios::binary | ios::app);
    outFile.write((char*)&s, sizeof(Student));
    outFile.close();
    cout << "Inserted: " << s.id << ", " << s.name << ", " << s.marks << endl;
}

void printAllStudents() {
    ifstream inFile("students.db", ios::binary);
    Student temp;

    cout << "\nAll active records in students.db:" << endl;
    while (inFile.read((char*)&temp, sizeof(Student))) {
        if (!temp.isDeleted) {   // skip deleted records
            cout << "ID: " << temp.id << ", Name: " << temp.name << ", Marks: " << temp.marks << endl;
        }
    }
    inFile.close();
}

void findStudentById(int id) {
    ifstream inFile("students.db", ios::binary);
    Student temp;
    bool found = false;

    while (inFile.read((char*)&temp, sizeof(Student))) {
        if (temp.id == id && !temp.isDeleted) {
            cout << "Found -> ID: " << temp.id << ", Name: " << temp.name << ", Marks: " << temp.marks << endl;
            found = true;
            break;
        }
    }
    inFile.close();

    if (!found) {
        cout << "No student found with ID " << id << endl;
    }
}

// NEW: DELETE function
void deleteStudent(int id) {
    fstream file("students.db", ios::binary | ios::in | ios::out);
    Student temp;
    bool found = false;
    streampos pos;  // to remember byte position of the record

    while (file.read((char*)&temp, sizeof(Student))) {
        if (temp.id == id && !temp.isDeleted) {
            pos = file.tellg();               // current position (after reading this record)
            pos -= sizeof(Student);           // go back to the start of this record
            temp.isDeleted = true;            // flip the flag in memory

            file.seekp(pos);                  // move write pointer to that exact spot
            file.write((char*)&temp, sizeof(Student));  // overwrite just this record
            found = true;
            break;
        }
    }
    file.close();

    if (found) {
        cout << "Deleted student with ID " << id << endl;
    } else {
        cout << "No student found with ID " << id << " to delete" << endl;
    }
}

int main() {
    // Insert fresh test data
    Student s1 = {1, "Utkarsh", 89.5, false};
    Student s2 = {2, "Priya", 76.2, false};
    Student s3 = {3, "Rahul", 92.1, false};

    insertStudent(s1);
    insertStudent(s2);
    insertStudent(s3);

    printAllStudents();

    // Now delete Priya (ID 2)
    deleteStudent(2);

    printAllStudents();

    // Try to find Priya again — should say not found
    findStudentById(2);

    return 0;
}