# Car Rental Management System

[![Build](https://img.shields.io/badge/build-passing-brightgreen)]()
[![C++](https://img.shields.io/badge/C%2B%2B-17-blue)]()

A console-based car rental management system built in modern C++17, demonstrating 
clean architecture, separation of concerns, and robust data handling.

## Features
- **Role-Based Access Control**: Separate interfaces for Managers, Employees, and Customers.
- **Robust Persistence**: Custom file-backed storage using standard library streams.
- **Modern C++ Practices**: 
  - Hash-based password verification (`std::hash`)
  - O(1) data access via `std::unordered_map`
  - Strict compiler warnings and type safety
- **Date Handling**: Accurate rental duration and penalty calculation.

## Quick Start

### Prerequisites
- CMake (3.14+)
- A C++17 compliant compiler (GCC, Clang, or MSVC)

### Building
```bash
cmake -B build
cmake --build build
```

### Running
```bash
./build/rental
```
*(On Windows, run `.\build\rental.exe`)*

## Architecture

- **Models**: Plain C++ objects representing Domain entities (`Car`, `Employee`, `Customer`).
- **Services**: Business logic isolated in specific service classes (`RentalService`).
- **Data Management**: In-memory `std::unordered_map` caching synced to flat-file storage (`Manager`).

## Design Decisions
- **Separation of Concerns**: Split a 1,000+ line monolith into a maintainable header/source structure.
- **Memory Safety**: Eliminated circular dependencies and object-slicing risks by returning pointers from collections.
- **Input Resilience**: `std::cin` wrappers to gracefully handle invalid console inputs without crashing.
