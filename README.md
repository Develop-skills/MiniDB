# MiniDB

A lightweight file-based database engine built from scratch in C++. It stores records directly in a custom binary file format and supports full CRUD operations, primary key constraints, soft-delete, and an in-memory hash index for fast lookups — all without using any external database library.

## Features

- Binary file-based storage (custom format, not SQLite or any external DB)
- Full CRUD operations (Create, Read, Update, Delete)
- Primary key uniqueness enforcement (rejects duplicate IDs)
- Soft-delete using a tombstone flag (records are marked deleted, not physically removed)
- In-memory hash index (`map<int, streampos>`) for fast record lookups
- Interactive command-line interface

## Tech Stack

C++ (STL only — `fstream`, `map`, `sstream`). No external libraries or database engines used.

## How to Build and Run

```bash
g++ -std=c++11 Main.cpp -o Main
```

Then run it:

```bash
.\Main       # Windows
./Main       # Linux/Mac
```

## Available Commands

| Command | Description | Example |
|---|---|---|
| `insert <id> <name> <marks>` | Add a new student record | `insert 1 Utkarsh 89` |
| `find <id>` | Look up a student by ID | `find 1` |
| `update <id> <marks>` | Update a student's marks | `update 1 95` |
| `delete <id>` | Soft-delete a student record | `delete 1` |
| `show` | List all active records | `show` |
| `help` | Show all available commands | `help` |
| `exit` | Exit the program | `exit` |

### Example session

## Design Decisions

- **Fixed-size binary structs**: Each student record is stored as a fixed-size C++ struct written directly to disk in binary form. This allows records to be read and written at exact byte offsets without needing a parser.
- **Soft-delete (tombstone pattern)**: Deleting a record doesn't physically remove its bytes from the file — it flips an `isDeleted` flag instead. This avoids the cost of rewriting the entire file on every delete, at the cost of the file not shrinking automatically.
- **In-memory hash index**: An index (`map<int, streampos>`) mapping each student ID to its exact byte position in the file is built once at startup and kept updated on every insert/update/delete. This means lookups, updates, and deletes jump directly to the correct record instead of scanning the whole file.

## Possible Future Improvements

- Persist the index to its own file instead of rebuilding it on every startup
- Replace the hash index with a B-Tree to support range queries (e.g., "find all students with marks > 80")
- Add a simple SQL-like query parser (`SELECT * FROM students WHERE id = 1`)
- Support multiple tables instead of a single hardcoded `students.db` file

## License

This project is licensed under the MIT License.
