# Data Structures

## About
This project contains two implementations of a priority queue in C++, which is a fundamental structure used for representing a queue of items sorted by priority. The program enables users to choose between two modes of running:
* **Interactive menu** - Used to interactively show the implementation of structure and to provide a user-friendly learning interface
* **Performance tester** - Strictly concentrating in testing performance of operations done on priority queue, enabling users to define test parameters and entry data.

Requires entry of **.csv** file with correct data format, that being:
```data format
{value<int>, priority<int>}
```

## Prerequisites
* A C++ compiler that supports the **C++17** standard (e.g., GCC, Clang, MSVC).
* **CMake** (version 3.10 or higher).

## How to Build

This project uses CMake for an out-of-source build. Follow these steps to compile the code:

1. **Clone the repository** and navigate to the project directory:
   ```bash
   cd datastructures/project2
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ./project2
