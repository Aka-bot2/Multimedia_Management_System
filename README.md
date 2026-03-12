# Multimedia Management System
> A menu-driven C++ OOP project simulating role-based user permissions, file management, and club operations. Built to deepen my understanding of OOP and C++.

---

## About

This project simulates the internal management system of the multimedia departments resources. Different users log in with their credentials and get access to different features based on their role. All data is stored and retrieved using CSV files, which act as a lightweight database.

---

## Features

- **Role-based login** — each user type gets a different menu and set of permissions
- **File management** — upload, edit, delete, and copy files across events
- **User storage** — each user has a personal file storage with a 20-file limit
- **Project management** — admins can create projects and manage members
- **Equipment tracking** — create and manage equipment objects with availability and cost info
- **Password management** — users can change their own passwords

---

## User Roles & Permissions

| Role | Permissions |
|---|---|
| **Admin** | All Member permissions + manage projects & members |
| **Member** | View, upload, edit, delete files + add events |
| **Editor** | Member permissions + create pre-movies |
| **Designer** | Member permissions + create posters |
| **Graphic Editor** | Editor + Designer permissions |
| **Photographer** | Member permissions + click pictures |
| **Artist** | View files + submit designs |
| **Studio** | View files + submit songs |
| **Other (Non-Member)** | View and copy files only |

---

## OOP Concepts Used

- **Inheritance** — single, multilevel, multiple, and hybrid
- **Polymorphism** — runtime polymorphism via virtual functions and function overriding
- **Abstract Classes** — `User` as an abstract base class with pure virtual functions
- **Friend Classes** — `Admin` as a friend of `Project`; `EquipmentIssue` as a friend of `Equipment`
- **Composition** — `Admin` contains a `Project` object; `User` contains `DepartmentRepository` and `UserStorage`
- **Constructors** — default, parameterized, and static factory methods
- **File Handling** — CSV-based persistent storage using `fstream`
- **Vectors** — dynamic data management using `std::vector`
- **Static Methods** — `User::login()` and `Equipment::create()`

---

## Class Structure

```
User  (Abstract)
├── NonMember
│   ├── Other
│   ├── Studio
│   └── Artist
└── Member
    ├── Admin
    ├── Photographer
    ├── Editor (also inherits Studio)
    ├── Designer (also inherits Artist)
    └── GraphicEditor (inherits Editor + Designer)

Equipment
EquipmentIssue  (friend of Equipment + Member)
DepartmentRepository
UserStorage
Project  (accessed via Admin)
```

---

## CSV Files Used

| File | Purpose |
|---|---|
| `users.csv` | Stores user credentials and roles |
| `release.csv` | Stores events and their associated files |
| `userfiles.csv` | Tracks files copied by each user |
| `projects.csv` | Stores projects and their members |
| `equipment.csv` | Stores equipment details and availability |
| `equipment_issues.csv` | Tracks issued and returned equipment |

---

## How to Run

```bash
g++ proj_3.cpp -o 3
./3
```

> Make sure all CSV files and other cpp files/headers are in the same directory as the executable.

---

## What I Learned

- How to design and implement a class hierarchy for a real-world use case
- How runtime polymorphism works with virtual functions and base class pointers
- How to use CSV files as a simple persistent data store in C++
- How friend classes and composition differ from inheritance
- How to manage multiple inheritance and resolve ambiguity using virtual base classes

**Contributors**
    - Akashh (https://github.com/Aka-bot2)
    - Chetan (https://github.com/Chetan-37) 
