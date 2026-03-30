#include "DynamicTable.h"
#include "SinglyLinkedList.h"
#include "DoublyLinkedList.h"
#include "Menu.h"
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <random>

using namespace std;
using namespace chrono;

// Main catches 3 parameters, first one being the data structure, second one being the size of data structure, and third one being the path to data file.
// In result it fills the data structure with data from file, and does operations being tested.
// The object returned by this program is a std string with 4 comma-separated values, which are the times of each operation being tested.

int main(int argc, char* argv[])
{
    if (argc < 4) {
        std::cerr << "Please provide the data structure, size, and data file path as command line arguments." << endl;
        return 1;
    }

    string data_structure = argv[1];
    int size = stoi(argv[2]);
    string data_file = argv[3];

    ifstream infile(data_file);
    if (!infile) {
        cerr << "Error opening file: " << data_file << endl;
        return 1;
    }

    // DynamicTable myTable;
    // int i{0};

    // while (i < size && infile)
    // {
    //     string line;
    //     getline(infile, line);
    //     myList.addElementAtEnd(atoi(line.c_str()));
    //     i++;
    // }

    // long long timePushFront = 0, timePushBack = 0, timeInsertRandom = 0, timeSearch = 0;
    
    // // --- TEST 1: Dodawanie na początku ---

    // auto start = high_resolution_clock::now();
    // myList.addElementAtBeginning(rand() % 10000 + 1);
    // auto stop = high_resolution_clock::now();
    // timePushFront = duration_cast<nanoseconds>(stop - start).count();

    // // --- TEST 2: Dodawanie na końcu ---

    // start = high_resolution_clock::now();
    // myList.addElementAtEnd(rand() % 10000 + 1);
    // stop = high_resolution_clock::now();
    // timePushBack = duration_cast<nanoseconds>(stop - start).count();

    // // --- TEST 3: Dodawanie w losowym miejscu ---

    // start = high_resolution_clock::now();
    // myList.addElementAtPosition(rand() % 10000 + 1, rand() % size);
    // stop = high_resolution_clock::now();
    // timeInsertRandom = duration_cast<nanoseconds>(stop - start).count();

    // // --- TEST 4: Wyszukiwanie losowego elementu ---

    // // Pobieramy wartość, o której wiemy, że może tam być, lub losujemy
    // start = high_resolution_clock::now();
    // volatile bool found = myList.searchElement(myList.returnElementAtPosition(rand() % size)); // volatile zapobiega optymalizacji!
    // stop = high_resolution_clock::now();
    // timeSearch = duration_cast<nanoseconds>(stop - start).count();

    // // 3. Wypisanie wyników (Format: Rozmiar, Czas_Front, Czas_Back, Czas_Random, Czas_Search)
    // cout << size << "," 
    //      << timePushFront << "," 
    //      << timePushBack << "," 
    //      << timeInsertRandom << "," 
    //      << timeSearch << endl;

    // SinglyLinkedList myList;
    // int i{0};

    // while (i < size && infile)
    // {
    //     string line;
    //     getline(infile, line);
    //     myList.addElementAtEnd(atoi(line.c_str()));
    //     i++;
    // }

    // long long timePushFront = 0, timePushBack = 0, timeInsertRandom = 0, timeSearch = 0;
    
    // // --- TEST 1: Dodawanie na początku ---

    // auto start = high_resolution_clock::now();
    // myList.addElementAtBeginning(rand() % 10000 + 1);
    // auto stop = high_resolution_clock::now();
    // timePushFront = duration_cast<nanoseconds>(stop - start).count();

    // // --- TEST 2: Dodawanie na końcu ---

    // start = high_resolution_clock::now();
    // myList.addElementAtEnd(rand() % 10000 + 1);
    // stop = high_resolution_clock::now();
    // timePushBack = duration_cast<nanoseconds>(stop - start).count();

    // // --- TEST 3: Dodawanie w losowym miejscu ---

    // start = high_resolution_clock::now();
    // myList.addElementAtPosition(rand() % 10000 + 1, rand() % size);
    // stop = high_resolution_clock::now();
    // timeInsertRandom = duration_cast<nanoseconds>(stop - start).count();

    // // --- TEST 4: Wyszukiwanie losowego elementu ---

    // // Pobieramy wartość, o której wiemy, że może tam być, lub losujemy
    // start = high_resolution_clock::now();
    // volatile bool found = myList.searchElement(myList.returnElementAtPosition(rand() % size)); // volatile zapobiega optymalizacji!
    // stop = high_resolution_clock::now();
    // timeSearch = duration_cast<nanoseconds>(stop - start).count();

    DoublyLinkedList myDoubleList = DoublyLinkedList();
    int i{0};

    while (i < size && infile)
    {
        string line = "";
        getline(infile, line);
        myDoubleList.addElementAtEnd(atoi(line.c_str()));
        i++;
    }

    long long timePushFront = 0, timePushBack = 0, timeInsertRandom = 0, timeSearch = 0;
    
    // --- TEST 1: Dodawanie na początku ---

    auto start = high_resolution_clock::now();
    myDoubleList.addElementAtBeginning(rand() % 10000 + 1);
    auto stop = high_resolution_clock::now();
    timePushFront = duration_cast<nanoseconds>(stop - start).count();

    // --- TEST 2: Dodawanie na końcu ---

    start = high_resolution_clock::now();
    myDoubleList.addElementAtEnd(rand() % 10000 + 1);
    stop = high_resolution_clock::now();
    timePushBack = duration_cast<nanoseconds>(stop - start).count();

    // --- TEST 3: Dodawanie w losowym miejscu ---

    start = high_resolution_clock::now();
    myDoubleList.addElementAtPosition(rand() % 10000 + 1, rand() % size);
    stop = high_resolution_clock::now();
    timeInsertRandom = duration_cast<nanoseconds>(stop - start).count();

    // --- TEST 4: Wyszukiwanie losowego elementu ---

    // Pobieramy wartość, o której wiemy, że może tam być, lub losujemy
    start = high_resolution_clock::now();
    volatile bool found = myDoubleList.searchElement(myDoubleList.returnElementAtPosition(rand() % size)); // volatile zapobiega optymalizacji!
    stop = high_resolution_clock::now();
    timeSearch = duration_cast<nanoseconds>(stop - start).count();

    // 3. Wypisanie wyników (Format: Rozmiar, Czas_Front, Czas_Back, Czas_Random, Czas_Search)
    cout << size << "," 
         << timePushFront << "," 
         << timePushBack << "," 
         << timeInsertRandom << "," 
         << timeSearch << endl;

    return 0;
}