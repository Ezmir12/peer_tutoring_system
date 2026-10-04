# Peer Tutoring System

A C++ console application for students and tutors to register, schedule,
and manage tutoring sessions. Data is persisted to plain text files
(`students.txt`, `tutoring.txt`, `sessions.txt`) in the working directory.

## Features

- User registration for students and tutors
- Students can view available tutors and schedule sessions
- Tutors can log in and approve or reject pending session requests
- Simple file-based persistence, no external dependencies

## Build & Run

```bash
g++ -o peer_tutoring_system peer_tutoring_system.cpp
./peer_tutoring_system
```

## Data format

Each record is one line, with fields separated by `|`:

```
students.txt / tutoring.txt:  id|name|contact|password
sessions.txt:                 student_id|tutor_id|date|time|status
```

A `|` delimiter is used instead of whitespace because names and contact
info are free text and can legitimately contain spaces.

## Known limitations

- Passwords are stored in plain text, not hashed
- No validation that a tutor ID entered when scheduling a session
  actually exists

## Future Improvements
- Hash passwords instead of storing them in plain text
- Improve UI with a menu-driven interface
- Store data in a database instead of flat files

## Author
Yonatan Azmir
