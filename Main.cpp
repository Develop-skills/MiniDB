#include <iostream>
#include <fstream>
#include <cstring>
#include <map>
using namespace std;

struct Student {
    int id;
    char name[20];
    float marks;
    bool isDeleted;
};

// Global index: maps student ID -> byte position in file
map<int, streampos> index_;

// Build the index by scanning the whole file once
void buildIndex() {
    index_.clear();
    ifstream inFile("students.db", ios::binary);
    if (!inFile) return;  // file might not exist yet

    Student temp;
    streampos pos = 0;

    while (inFile.read((char*)&temp, sizeof(Student))) {
        if (!temp.isDeleted) {
            index_[temp.id] = pos;
        } else {
            index_.erase(temp.id);  // in case it was deleted after being indexed
        }
        pos = inFile.tellg();
    }
    inFile.close();
    cout << "Index built with " << index_.size() << " active records." << endl;
}

bool idExists(int id) {
    return index_.find(id) != index_.end();
}

void insertStudent(Student s) {
    if (idExists(s.id)) {
        cout << "Error: Student with ID " << s.id << " already exists. Insert rejected." << endl;
        return;
    }

    s.isDeleted = false;

    ofstream outFile("students.db", ios::binary | ios::app);
    streampos pos = outFile.tellp();  // position where this record is about to be written
    outFile.write((char*)&s, sizeof(Student));
    outFile.close();

    index_[s.id] = pos;  // update index immediately, no need to rebuild
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

// UPDATED: findStudentById now uses the index directly, no scanning
void findStudentById(int id) {
    auto it = index_.find(id);
    if (it == index_.end()) {
        cout << "No student found with ID " << id << endl;
        return;
    }

    fstream file("students.db", ios::binary | ios::in);
    Student temp;

    file.seekg(it->second);              // jump straight to the byte position
    file.read((char*)&temp, sizeof(Student));
    file.close();

    cout << "Found -> ID: " << temp.id << ", Name: " << temp.name << ", Marks: " << temp.marks << endl;
}

// UPDATED: deleteStudent now uses the index to jump straight to the record
void deleteStudent(int id) {
    auto it = index_.find(id);
    if (it == index_.end()) {
        cout << "No student found with ID " << id << " to delete" << endl;
        return;
    }

    fstream file("students.db", ios::binary | ios::in | ios::out);
    Student temp;

    file.seekg(it->second);
    file.read((char*)&temp, sizeof(Student));

    temp.isDeleted = true;

    file.seekp(it->second);
    file.write((char*)&temp, sizeof(Student));
    file.close();

    index_.erase(id);  // remove from index since it's no longer active
    cout << "Deleted student with ID " << id << endl;
}

// UPDATED: updateMarks now uses the index too
void updateMarks(int id, float newMarks) {
    auto it = index_.find(id);
    if (it == index_.end()) {
        cout << "No student found with ID " << id << " to update" << endl;
        return;
    }

    fstream file("students.db", ios::binary | ios::in | ios::out);
    Student temp;

    file.seekg(it->second);
    file.read((char*)&temp, sizeof(Student));

    temp.marks = newMarks;

    file.seekp(it->second);
    file.write((char*)&temp, sizeof(Student));
    file.close();

    cout << "Updated ID " << id << " marks to " << newMarks << endl;
}

int main() {
    buildIndex();   // always build the index first, before doing anything else

    printAllStudents();

    findStudentById(1);
    findStudentById(999);

    updateMarks(3, 88.0);
    printAllStudents();

    deleteStudent(1);
    printAllStudents();

    return 0;
}