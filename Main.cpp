#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

struct Student {
    int id;
    char name[20];
    float marks;
    bool isDeleted;
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

    s.isDeleted = false;

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
        if (!temp.isDeleted) {
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

void deleteStudent(int id) {
    fstream file("students.db", ios::binary | ios::in | ios::out);
    Student temp;
    bool found = false;
    streampos pos;

    while (file.read((char*)&temp, sizeof(Student))) {
        if (temp.id == id && !temp.isDeleted) {
            pos = file.tellg();
            pos -= sizeof(Student);
            temp.isDeleted = true;

            file.seekp(pos);
            file.write((char*)&temp, sizeof(Student));
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

// NEW: UPDATE function
void updateMarks(int id, float newMarks) {
    fstream file("students.db", ios::binary | ios::in | ios::out);
    Student temp;
    bool found = false;
    streampos pos;

    while (file.read((char*)&temp, sizeof(Student))) {
        if (temp.id == id && !temp.isDeleted) {
            pos = file.tellg();
            pos -= sizeof(Student);

            temp.marks = newMarks;   // change the data we want to update

            file.seekp(pos);
            file.write((char*)&temp, sizeof(Student));  // overwrite with updated record
            found = true;
            break;
        }
    }
    file.close();

    if (found) {
        cout << "Updated ID " << id << " marks to " << newMarks << endl;
    } else {
        cout << "No student found with ID " << id << " to update" << endl;
    }
}

int main() {
    printAllStudents();

    updateMarks(1, 95.0);   // change Utkarsh's marks

    printAllStudents();

    return 0;
}