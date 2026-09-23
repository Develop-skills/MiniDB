#include <iostream>
#include <fstream>
#include <cstring>
#include <map>
#include <sstream>
using namespace std;

struct Student {
    int id;
    char name[20];
    float marks;
    bool isDeleted;
};

map<int, streampos> index_;

void buildIndex() {
    index_.clear();
    ifstream inFile("students.db", ios::binary);
    if (!inFile) return;

    Student temp;
    streampos pos = 0;

    while (inFile.read((char*)&temp, sizeof(Student))) {
        if (!temp.isDeleted) {
            index_[temp.id] = pos;
        } else {
            index_.erase(temp.id);
        }
        pos = inFile.tellg();
    }
    inFile.close();
}

bool idExists(int id) {
    return index_.find(id) != index_.end();
}

void insertStudent(int id, string name, float marks) {
    if (idExists(id)) {
        cout << "Error: Student with ID " << id << " already exists. Insert rejected." << endl;
        return;
    }

    Student s;
    s.id = id;
    strncpy(s.name, name.c_str(), 20);
    s.marks = marks;
    s.isDeleted = false;

    // Reliably get the current file size = position where new record will land
    ifstream sizeCheck("students.db", ios::binary | ios::ate);
    streampos pos = sizeCheck.tellg();
    if (pos == -1) pos = 0;  // file doesn't exist yet, so this will be the first record
    sizeCheck.close();

    ofstream outFile("students.db", ios::binary | ios::app);
    outFile.write((char*)&s, sizeof(Student));
    outFile.close();

    index_[s.id] = pos;
    cout << "Inserted: " << s.id << ", " << s.name << ", " << s.marks << endl;
}

void printAllStudents() {
    ifstream inFile("students.db", ios::binary);
    Student temp;

    cout << "\nAll active records:" << endl;
    while (inFile.read((char*)&temp, sizeof(Student))) {
        if (!temp.isDeleted) {
            cout << "ID: " << temp.id << ", Name: " << temp.name << ", Marks: " << temp.marks << endl;
        }
    }
    inFile.close();
}

void findStudentById(int id) {
    auto it = index_.find(id);
    if (it == index_.end()) {
        cout << "No student found with ID " << id << endl;
        return;
    }

    fstream file("students.db", ios::binary | ios::in);
    Student temp;
    file.seekg(it->second);
    file.read((char*)&temp, sizeof(Student));
    file.close();

    cout << "Found -> ID: " << temp.id << ", Name: " << temp.name << ", Marks: " << temp.marks << endl;
}

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

    index_.erase(id);
    cout << "Deleted student with ID " << id << endl;
}

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

void printHelp() {
    cout << "\nAvailable commands:" << endl;
    cout << "  insert <id> <name> <marks>" << endl;
    cout << "  find <id>" << endl;
    cout << "  update <id> <marks>" << endl;
    cout << "  delete <id>" << endl;
    cout << "  show" << endl;
    cout << "  help" << endl;
    cout << "  exit" << endl;
}

int main() {
    buildIndex();
    cout << "MiniDB started. Type 'help' to see commands." << endl;

    string line;
    while (true) {
        cout << "\nminidb> ";
        getline(cin, line);

        stringstream ss(line);
        string command;
        ss >> command;

        if (command == "exit") {
            cout << "Goodbye!" << endl;
            break;
        }
        else if (command == "help") {
            printHelp();
        }
        else if (command == "show") {
            printAllStudents();
        }
        else if (command == "insert") {
            int id;
            string name;
            float marks;
            ss >> id >> name >> marks;
            insertStudent(id, name, marks);
        }
        else if (command == "find") {
            int id;
            ss >> id;
            findStudentById(id);
        }
        else if (command == "update") {
            int id;
            float marks;
            ss >> id >> marks;
            updateMarks(id, marks);
        }
        else if (command == "delete") {
            int id;
            ss >> id;
            deleteStudent(id);
        }
        else {
            cout << "Unknown command. Type 'help' to see available commands." << endl;
        }
    }

    return 0;
}