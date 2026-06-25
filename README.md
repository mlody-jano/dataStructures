# 📚 Data Structures - Projects

Welcome to the project repository for the **Data Structures** course! This repository contains a collection of tasks and implementations aimed at providing a practical understanding of how basic data structures and memory management work.

---

## 📌 Projects Overview

The repository consists of three main projects:

### 1. Project: Lists
This directory contains the implementation of list-based data structures (e.g., singly linked list, doubly linked list).
* **Main features:** `insert` and `remove` operations at specific locations in structure, `find` operation and general iteration through list contents.
* **Educational goal:** Understanding dynamic memory allocation and operations on pointers/references.

### 2. Project: Queues
This directory contains two implementations of a priority queue. First one relying on a list structure for storing records, second using a heap to reduce access times for specific records. Two diffrent structures underlying behind the general priority queue enabled to showcase the diffrences coming from selecting an efficient data structure for holding records.
* **Main features:** `enqueue` (adding to the back), `dequeue` (removing from the front), `peek` / `front` (viewing the first element), and checking if the queue is empty.
* **Educational goal:** Applying buffering concepts and handling data in the order of its arrival.

### 3. Project: Dictionary
The final project focuses on implementing a dictionary (map) structure, which allows storing data as key-value pairs (e.g., using a hash table or a BST tree). In this case, the following repository contains three implementations: open addressing, separate chaining with linked lists as buckets and lastly, seperate chaining with an AVL tree as bucket. This enabled to show important diffrences between the two collision resolution approaches and how the structure of the bucket affected operation times.
* **Main features:** Fast insertion of key-value pairs, removing elements by key, and efficient searching.
* **Educational goal:** Learning about hash functions, collision resolution, and optimizing data access time.

---

## 🛠️ Technologies

* **Programming language:** C++26 (general logic), Python (data generation and visualization for reports)
* **Build tools:** (version 3.10 or higher).

---

## 🚀 How to Run the Projects?

To clone the repository and run the projects locally, follow these steps:

1. **Clone the repository:**
   ```bash
   git clone https://github.com/mlody-jano/dataStructures.git
   ```
