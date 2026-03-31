#include "DynamicTable.h"
#include "SinglyLinkedList.h"
#include "DoublyLinkedList.h"
#include "Test.h"
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

int main()
{

    Test test("../testing/data/data_1.txt", 10, 2000, 2000, 5); // domyślnie: 100 powt., rozmiary 2000–10000 co 2000
    test.runAll();
    test.saveToCSV("../testing/data/wyniki.csv");
    /*if (argc < 4) {
        std::cerr << "Please provide data structure, size, and data file path as command line arguments." << endl;
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

    if (data_structure == "DynamicTable") {
        DynamicTable myTable[100];
        int i{0};
        while (i < size && infile)
        {
            string line;
            getline(infile, line);
            for (int j{0}; j < 100; j++)
            {
                myTable[j].addElementAtEnd(atoi(line.c_str()));
            }
            i++;
        }

        // Testy dla DynamicTable

        long long timePushFront = 0, timePushBack = 0, timeInsertRandom = 0, timeSearch = 0;
    }
    else if (data_structure == "SinglyLinkedList") {
        SinglyLinkedList myList[100];
        int i{0};
        while (i < size && infile)
        {
            string line;
            getline(infile, line);
            for (int j{0}; j < 100; j++)
            {
                myList[j].addElementAtEnd(atoi(line.c_str()));
            }
            i++;
        }
    }
    else if (data_structure == "DoublyLinkedList") {
        DoublyLinkedList myDoubleList[100];
        int i{0};
        while (i < size && infile)
        {
            string line;
            getline(infile, line);
            for (int j{0}; j < 100; j++)
            {
                myDoubleList[j].addElementAtEnd(atoi(line.c_str()));
            }
            i++;
        }
    }
    else {
        cerr << "Unknown data structure: " << data_structure << endl;
    }*/

    // 3. Wypisanie wyników (Format: Czas_PushFront_DTable, Czas_PushFront_SLL, Czas_PushFront_DLL,Czas_PushBack_DTable, Czas_PushBack_SLL, Czas_PushBack_DLL, Czas_InsertRandom_DTable, Czas_InsertRandom_SLL, Czas_InsertRandom_DLL, Czas_DeleteLast_DTable, Czas_DeleteLast_SLL, Czas_DeleteLast_DLL, Czas_Search_DTable, Czas_Search_SLL, Czas_Search_DLL)

    // // --- TEST 2: Dodawanie na końcu ---

    // start = high_resolution_clock::now();
    // for (int j{0}; j < 100; j++)
    // {
    //     myTable[j].addElementAtEnd(rand() % 10000 + 1);
    // }
    // stop = high_resolution_clock::now();
    // timePushBack = duration_cast<nanoseconds>(stop - start).count();

    // // --- TEST 3: Dodawanie w losowym miejscu ---

    // start = high_resolution_clock::now();
    // for (int j{0}; j < 100; j++)
    // {
    //     myTable[j].addElementAtPosition(rand() % 10000 + 1, rand() % size);
    // }

    // stop = high_resolution_clock::now();
    // timeInsertRandom = duration_cast<nanoseconds>(stop - start).count();

    // // --- TEST 4: Wyszukiwanie losowego elementu ---

    // // Pobieramy wartość, o której wiemy, że może tam być, lub losujemy
    // start = high_resolution_clock::now();
    // for (int j{0}; j < 100; j++)
    // {
    //     volatile bool found = myTable[j].searchElement(myTable[j].returnElementAtPosition(rand() % size)); // volatile zapobiega optymalizacji!
    // }

    // stop = high_resolution_clock::now();
    // timeSearch = duration_cast<nanoseconds>(stop - start).count();

    // // 3. Wypisanie wyników (Format: Rozmiar, Czas_Front, Czas_Back, Czas_Random, Czas_Search)
    // cout << timePushFront << ","
    //      << timePushBack << ","
    //      << timeInsertRandom << ","
    //      << timeSearch << endl;

    // SinglyLinkedList myList[100];
    // int i{0};

    // while (i < size && infile)
    // {
    //     string line;
    //     getline(infile, line);
    //     for(int j{0}; j < 100; j++)
    //     {
    //         myList[j].addElementAtEnd(atoi(line.c_str()));
    //     }
    // }

    // long long timePushFront = 0, timePushBack = 0, timeInsertRandom = 0, timeSearch = 0;

    // // --- TEST 1: Dodawanie na początku ---

    // auto start = high_resolution_clock::now();
    // for (int j{0}; j < 100; j++)
    // {
    //     myList[j].addElementAtBeginning(rand() % 10000 + 1);
    // }
    // auto stop = high_resolution_clock::now();
    // timePushFront = duration_cast<nanoseconds>(stop - start).count();

    // // --- TEST 2: Dodawanie na końcu ---

    // start = high_resolution_clock::now();
    // for (int j{0}; j < 100; j++)
    // {
    //     myList[j].addElementAtEnd(rand() % 10000 + 1);
    // }
    // stop = high_resolution_clock::now();
    // timePushBack = duration_cast<nanoseconds>(stop - start).count();

    // // --- TEST 3: Dodawanie w losowym miejscu ---

    // start = high_resolution_clock::now();
    // for (int j{0}; j < 100; j++)
    // {
    //     myList[j].addElementAtPosition(rand() % 10000 + 1, rand() % size);
    // }
    // stop = high_resolution_clock::now();
    // timeInsertRandom = duration_cast<nanoseconds>(stop - start).count();

    // // --- TEST 4: Wyszukiwanie losowego elementu ---

    // // Pobieramy wartość, o której wiemy, że może tam być, lub losujemy
    // start = high_resolution_clock::now();
    // for (int j{0}; j < 100; j++)
    // {
    //     volatile bool found = myList[j].searchElement(myList[j].returnElementAtPosition(rand() % size)); // volatile zapobiega optymalizacji!
    // }
    // stop = high_resolution_clock::now();
    // timeSearch = duration_cast<nanoseconds>(stop - start).count();

    // // 3. Wypisanie wyników (Format: Rozmiar, Czas_Front, Czas_Back, Czas_Random, Czas_Search)
    // cout << timePushFront << ","
    //      << timePushBack << ","
    //      << timeInsertRandom << ","
    //      << timeSearch << endl;

    // DoublyLinkedList myDoubleList[100];
    // int i{0};

    // while (i < size && infile)
    // {
    //     string line = "";
    //     getline(infile, line);
    //     for (int j{0}; j < 100; j++)
    //     {
    //         myDoubleList[j].addElementAtEnd(atoi(line.c_str()));
    //     }

    //     i++;
    // }

    // long long timePushFront = 0, timePushBack = 0, timeInsertRandom = 0, timeSearch = 0;

    // // --- TEST 1: Dodawanie na początku ---

    // auto start = high_resolution_clock::now();
    // for (int j{0}; j < 100; j++)
    // {
    //     myDoubleList[j].addElementAtBeginning(rand() % 10000 + 1);
    // }
    // auto stop = high_resolution_clock::now();
    // timePushFront = (duration_cast<nanoseconds>(stop - start).count()) / 100; // Dzielimy przez 100, bo wykonaliśmy 100 operacji

    // // --- TEST 2: Dodawanie na końcu ---

    // start = high_resolution_clock::now();
    // for (int j{0}; j < 100; j++)
    // {
    //     myDoubleList[j].addElementAtEnd(rand() % 10000 + 1);
    // }
    // stop = high_resolution_clock::now();
    // timePushBack = (duration_cast<nanoseconds>(stop - start).count()) / 100; // Dzielimy przez 100, bo wykonaliśmy 100 operacji

    // // --- TEST 3: Dodawanie w losowym miejscu ---

    // start = high_resolution_clock::now();
    // for (int j{0}; j < 100; j++)
    // {
    //     myDoubleList[j].addElementAtPosition(rand() % 10000 + 1, rand() % size);
    // }
    // stop = high_resolution_clock::now();
    // timeInsertRandom = (duration_cast<nanoseconds>(stop - start).count()) / 100; // Dzielimy przez 100, bo wykonaliśmy 100 operacji

    // // --- TEST 4: Wyszukiwanie losowego elementu ---

    // // Pobieramy wartość, o której wiemy, że może tam być, lub losujemy
    // start = high_resolution_clock::now();
    // for (int j{0}; j < 100; j++)
    // {
    //     volatile bool found = myDoubleList[j].searchElement(myDoubleList[j].returnElementAtPosition(rand() % size)); // volatile zapobiega optymalizacji!
    // }
    // stop = high_resolution_clock::now();
    // timeSearch = (duration_cast<nanoseconds>(stop - start).count()) / 100; // Dzielimy przez 100, bo wykonaliśmy 100 operacji

    // 3. Wypisanie wyników (Format: Rozmiar, Czas_Front, Czas_Back, Czas_Random, Czas_Search)
    // cout << size << ","
    //      << timePushFront << ","
    //      << timePushBack << ","
    //      << timeInsertRandom << ","
    //      << timeSearch << endl;

    return 0;
}