#include "Test.h"
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

int *Test::loadDataFromFile(const string &filename, int &outSize)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Nie udalo sie otworzyc pliku: " << filename << endl;
        outSize = 0;
        return nullptr;
    }

    // 1. Zliczanie elementów
    int count = 0;
    int tempValue;
    while (file >> tempValue)
    {
        count++;
    }

    // 2. Powrót na początek pliku
    file.clear();
    file.seekg(0, ios::beg);

    // 3. Alokacja tablicy i właściwe wczytanie danych
    int *dataArray = new int[count];
    for (int i = 0; i < count; i++)
    {
        file >> dataArray[i];
    }

    file.close();
    outSize = count;
    return dataArray;
}

void Test::testDynamicTable(int *baseData, int dataSize, ofstream &csvFile)
{
    int testValue = 9999; // Przykładowa wartość używana do dodawania/wyszukiwania

    // ==========================================
    // OPERACJA: Dodawanie na początek
    // ==========================================
    {
        DynamicTable tables[NUM_COPIES]; // Tworzy 100 pustych tablic

        // Wypełnienie wszystkich 100 tablic danymi z pliku
        for (int i = 0; i < NUM_COPIES; ++i)
        {
            for (int j = 0; j < dataSize; ++j)
            {
                tables[i].addElementAtEnd(baseData[j]);
            }
        }

        auto start = high_resolution_clock::now(); // START ZEGARA

        // Wykonanie operacji na każdej tablicy
        for (int i = 0; i < NUM_COPIES; ++i)
        {
            tables[i].addElementAtBeginning(testValue);
        }

        auto end = high_resolution_clock::now(); // STOP ZEGARA

        // Obliczenia
        auto totalDuration = duration_cast<nanoseconds>(end - start).count();
        long long avgTime = totalDuration / NUM_COPIES;

        // Zapis do CSV
        csvFile << "DynamicTable;AddAtBeginning;" << dataSize << ";" << avgTime << "\n";
    } // <-- Tutaj tablica 100 obiektów 'tables' jest usuwana, pamięć czyszczona automatycznie

    // ==========================================
    // OPERACJA: Dodawanie na koniec
    // ==========================================

    {
        DynamicTable tables[NUM_COPIES]; // Tworzy 100 pustych tablic

        // Wypełnienie wszystkich 100 tablic danymi z pliku
        for (int i = 0; i < NUM_COPIES; ++i)
        {
            for (int j = 0; j < dataSize; ++j)
            {
                tables[i].addElementAtEnd(baseData[j]);
            }
        }

        auto start = high_resolution_clock::now(); // START ZEGARA

        // Wykonanie operacji na każdej tablicy
        for (int i = 0; i < NUM_COPIES; ++i)
        {
            tables[i].addElementAtEnd(testValue);
        }

        auto end = high_resolution_clock::now(); // STOP ZEGARA

        // Obliczenia
        auto totalDuration = duration_cast<nanoseconds>(end - start).count();
        long long avgTime = totalDuration / NUM_COPIES;

        // Zapis do CSV
        csvFile << "DynamicTable;AddAtEnd;" << dataSize << ";" << avgTime << "\n";
    }

    // ==========================================
    // OPERACJA: Dodawanie na losową pozycję
    // ==========================================

    {
        DynamicTable tables[NUM_COPIES]; // Tworzy 100 pustych tablic

        // Wypełnienie wszystkich 100 tablic danymi z pliku
        for (int i = 0; i < NUM_COPIES; ++i)
        {
            for (int j = 0; j < dataSize; ++j)
            {
                tables[i].addElementAtEnd(baseData[j]);
            }
        }

        auto start = high_resolution_clock::now(); // START ZEGARA

        // Wykonanie operacji na każdej tablicy
        for (int i = 0; i < NUM_COPIES; ++i)
        {
            tables[i].addElementAtPosition(testValue, dataSize / 2); // Dodajemy na środek tablicy
        }

        auto end = high_resolution_clock::now(); // STOP ZEGARA

        // Obliczenia
        auto totalDuration = duration_cast<nanoseconds>(end - start).count();
        long long avgTime = totalDuration / NUM_COPIES;

        // Zapis do CSV
        csvFile << "DynamicTable;AddAtPosition;" << dataSize << ";" << avgTime << "\n";
    }

    // ==========================================
    // OPERACJA: Usuwanie z początku
    // ==========================================

    {
        DynamicTable tables[NUM_COPIES]; // Znowu tworzymy 100 świeżych tablic

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            for (int j = 0; j < dataSize; ++j)
            {
                tables[i].addElementAtEnd(baseData[j]);
            }
        }

        auto start = high_resolution_clock::now();

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            tables[i].deleteElementAtBeginning();
        }

        auto end = high_resolution_clock::now();

        auto totalDuration = duration_cast<nanoseconds>(end - start).count();
        long long avgTime = totalDuration / NUM_COPIES;

        csvFile << "DynamicTable;DeleteAtBeginning;" << dataSize << ";" << avgTime << "\n";
    }

    // ==========================================
    // OPERACJA: Usuwanie z końca
    // ==========================================
    {
        DynamicTable tables[NUM_COPIES]; // Znowu tworzymy 100 świeżych tablic

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            for (int j = 0; j < dataSize; ++j)
            {
                tables[i].addElementAtEnd(baseData[j]);
            }
        }

        auto start = high_resolution_clock::now();

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            tables[i].deleteElementAtEnd();
        }

        auto end = high_resolution_clock::now();

        auto totalDuration = duration_cast<nanoseconds>(end - start).count();
        long long avgTime = totalDuration / NUM_COPIES;

        csvFile << "DynamicTable;DeleteAtEnd;" << dataSize << ";" << avgTime << "\n";
    }

    // ==========================================
    // OPERACJA: Usuwanie z losowej pozycji
    // ==========================================

    {
        DynamicTable tables[NUM_COPIES]; // Znowu tworzymy 100 świeżych tablic

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            for (int j = 0; j < dataSize; ++j)
            {
                tables[i].addElementAtEnd(baseData[j]);
            }
        }

        auto start = high_resolution_clock::now();

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            tables[i].deleteElementAtPosition(dataSize / 2); // Usuwamy element ze środka tablicy
        }

        auto end = high_resolution_clock::now();

        auto totalDuration = duration_cast<nanoseconds>(end - start).count();
        long long avgTime = totalDuration / NUM_COPIES;

        csvFile << "DynamicTable;DeleteAtPosition;" << dataSize << ";" << avgTime << "\n";
    }

    // ==========================================
    // OPERACJA: Szukanie elementu (załóżmy, że szukamy ostatniego)
    // ==========================================
    {
        DynamicTable tables[NUM_COPIES];
        int valueToSearch = baseData[dataSize - 1]; // Szukamy ostatniego elementu (pesymistyczny przypadek)

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            for (int j = 0; j < dataSize; ++j)
            {
                tables[i].addElementAtEnd(baseData[j]);
            }
        }

        auto start = high_resolution_clock::now();

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            tables[i].searchElement(valueToSearch);
        }

        auto end = high_resolution_clock::now();

        auto totalDuration = duration_cast<nanoseconds>(end - start).count();
        long long avgTime = totalDuration / NUM_COPIES;

        csvFile << "DynamicTable;Search;" << dataSize << ";" << avgTime << "\n";
    }
}

void Test::testSinglyLinkedList(int *baseData, int dataSize, ofstream &csvFile)
{
    int testValue = 9999;

    // ==========================================
    // OPERACJA: Dodawanie na początek
    // ==========================================
    {
        SinglyLinkedList lists[NUM_COPIES];

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            for (int j = 0; j < dataSize; ++j)
            {
                lists[i].addElementAtEnd(baseData[j]);
            }
        }

        auto start = high_resolution_clock::now();

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            lists[i].addElementAtBeginning(testValue);
        }

        auto end = high_resolution_clock::now();

        auto totalDuration = duration_cast<nanoseconds>(end - start).count();
        long long avgTime = totalDuration / NUM_COPIES;

        csvFile << "SinglyLinkedList;AddAtBeginning;" << dataSize << ";" << avgTime << "\n";
    }

    // ==========================================
    // OPERACJA: Dodawanie na koniec
    // ==========================================

    {
        SinglyLinkedList lists[NUM_COPIES];

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            for (int j = 0; j < dataSize; ++j)
            {
                lists[i].addElementAtEnd(baseData[j]);
            }
        }

        auto start = high_resolution_clock::now();

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            lists[i].addElementAtEnd(testValue);
        }

        auto end = high_resolution_clock::now();

        auto totalDuration = duration_cast<nanoseconds>(end - start).count();
        long long avgTime = totalDuration / NUM_COPIES;

        csvFile << "SinglyLinkedList;AddAtEnd;" << dataSize << ";" << avgTime << "\n";
    }

    // ==========================================
    // OPERACJA: Dodawanie na losową pozycję
    // ==========================================

    {
        SinglyLinkedList lists[NUM_COPIES];

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            for (int j = 0; j < dataSize; ++j)
            {
                lists[i].addElementAtEnd(baseData[j]);
            }
        }

        auto start = high_resolution_clock::now();

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            lists[i].addElementAtPosition(testValue, dataSize / 2); // Dodajemy na środek listy
        }

        auto end = high_resolution_clock::now();

        auto totalDuration = duration_cast<nanoseconds>(end - start).count();
        long long avgTime = totalDuration / NUM_COPIES;

        csvFile << "SinglyLinkedList;AddAtPosition;" << dataSize << ";" << avgTime << "\n";
    }

    // ==========================================
    // OPERACJA: Usuwanie z początku
    // ==========================================
    {
        SinglyLinkedList lists[NUM_COPIES];

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            for (int j = 0; j < dataSize; ++j)
            {
                lists[i].addElementAtEnd(baseData[j]);
            }
        }

        auto start = high_resolution_clock::now();

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            lists[i].deleteElementAtBeginning();
        }

        auto end = high_resolution_clock::now();

        auto totalDuration = duration_cast<nanoseconds>(end - start).count();
        long long avgTime = totalDuration / NUM_COPIES;

        csvFile << "SinglyLinkedList;DeleteAtBeginning;" << dataSize << ";" << avgTime << "\n";
    }

    // ==========================================
    // OPERACJA: Usuwanie z końca
    // ==========================================

    {
        SinglyLinkedList lists[NUM_COPIES];

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            for (int j = 0; j < dataSize; ++j)
            {
                lists[i].addElementAtEnd(baseData[j]);
            }
        }

        auto start = high_resolution_clock::now();

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            lists[i].deleteElementAtEnd();
        }

        auto end = high_resolution_clock::now();

        auto totalDuration = duration_cast<nanoseconds>(end - start).count();
        long long avgTime = totalDuration / NUM_COPIES;

        csvFile << "SinglyLinkedList;DeleteAtEnd;" << dataSize << ";" << avgTime << "\n";
    }

    // ==========================================
    // OPERACJA: Usuwanie z losowej pozycji
    // ==========================================

    {
        SinglyLinkedList lists[NUM_COPIES];

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            for (int j = 0; j < dataSize; ++j)
            {
                lists[i].addElementAtEnd(baseData[j]);
            }
        }

        auto start = high_resolution_clock::now();

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            lists[i].deleteElementAtPosition(dataSize / 2); // Usuwamy element ze środka listy
        }

        auto end = high_resolution_clock::now();

        auto totalDuration = duration_cast<nanoseconds>(end - start).count();
        long long avgTime = totalDuration / NUM_COPIES;

        csvFile << "SinglyLinkedList;DeleteAtPosition;" << dataSize << ";" << avgTime << "\n";
    }

    // ==========================================
    // OPERACJA: Szukanie elementu (załóżmy, że szukamy ostatniego)
    // ==========================================

    {
        SinglyLinkedList lists[NUM_COPIES];
        int valueToSearch = baseData[dataSize - 1]; // Szukamy ostatniego elementu (pesymistyczny przypadek)

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            for (int j = 0; j < dataSize; ++j)
            {
                lists[i].addElementAtEnd(baseData[j]);
            }
        }

        auto start = high_resolution_clock::now();

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            lists[i].searchElement(valueToSearch);
        }

        auto end = high_resolution_clock::now();

        auto totalDuration = duration_cast<nanoseconds>(end - start).count();
        long long avgTime = totalDuration / NUM_COPIES;

        csvFile << "SinglyLinkedList;Search;" << dataSize << ";" << avgTime << "\n";
    }
}

void Test::testDoublyLinkedList(int *baseData, int dataSize, ofstream &csvFile)
{
    int testValue = 9999;

    // ==========================================
    // OPERACJA: Dodawanie na początek
    // ==========================================

    {
        DoublyLinkedList lists[NUM_COPIES];

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            for (int j = 0; j < dataSize; ++j)
            {
                lists[i].addElementAtEnd(baseData[j]);
            }
        }

        auto start = high_resolution_clock::now();

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            lists[i].addElementAtBeginning(testValue);
        }

        auto end = high_resolution_clock::now();

        auto totalDuration = duration_cast<nanoseconds>(end - start).count();
        long long avgTime = totalDuration / NUM_COPIES;

        csvFile << "DoublyLinkedList;AddAtBeginning;" << dataSize << ";" << avgTime << "\n";
    }

    // ==========================================
    // OPERACJA: Dodawanie na koniec
    // ==========================================
    {
        DoublyLinkedList lists[NUM_COPIES];

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            for (int j = 0; j < dataSize; ++j)
            {
                lists[i].addElementAtEnd(baseData[j]);
            }
        }

        auto start = high_resolution_clock::now();

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            lists[i].addElementAtEnd(testValue);
        }

        auto end = high_resolution_clock::now();

        auto totalDuration = duration_cast<nanoseconds>(end - start).count();
        long long avgTime = totalDuration / NUM_COPIES;

        csvFile << "DoublyLinkedList;AddAtEnd;" << dataSize << ";" << avgTime << "\n";
    }

    // ==========================================
    // OPERACJA: Dodawanie na losową pozycję
    // ==========================================

    {
        DoublyLinkedList lists[NUM_COPIES];

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            for (int j = 0; j < dataSize; ++j)
            {
                lists[i].addElementAtEnd(baseData[j]);
            }
        }

        auto start = high_resolution_clock::now();

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            lists[i].addElementAtPosition(testValue, dataSize / 2); // Dodajemy na środek listy
        }

        auto end = high_resolution_clock::now();

        auto totalDuration = duration_cast<nanoseconds>(end - start).count();
        long long avgTime = totalDuration / NUM_COPIES;

        csvFile << "DoublyLinkedList;AddAtPosition;" << dataSize << ";" << avgTime << "\n";
    }

    // ==========================================
    // OPERACJA: Usuwanie z początku
    // ==========================================

    {
        DoublyLinkedList lists[NUM_COPIES];

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            for (int j = 0; j < dataSize; ++j)
            {
                lists[i].addElementAtEnd(baseData[j]);
            }
        }

        auto start = high_resolution_clock::now();

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            lists[i].deleteElementAtBeginning();
        }

        auto end = high_resolution_clock::now();

        auto totalDuration = duration_cast<nanoseconds>(end - start).count();
        long long avgTime = totalDuration / NUM_COPIES;

        csvFile << "DoublyLinkedList;DeleteAtBeginning;" << dataSize << ";" << avgTime << "\n";
    }

    // ==========================================
    // OPERACJA: Usuwanie z końca
    // ==========================================

    {
        DoublyLinkedList lists[NUM_COPIES];

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            for (int j = 0; j < dataSize; ++j)
            {
                lists[i].addElementAtEnd(baseData[j]);
            }
        }

        auto start = high_resolution_clock::now();

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            lists[i].deleteElementAtEnd();
        }

        auto end = high_resolution_clock::now();

        auto totalDuration = duration_cast<nanoseconds>(end - start).count();
        long long avgTime = totalDuration / NUM_COPIES;

        csvFile << "DoublyLinkedList;DeleteAtEnd;" << dataSize << ";" << avgTime << "\n";
    }

    // ==========================================
    // OPERACJA: Usuwanie z losowej pozycji
    // ==========================================

    {
        DoublyLinkedList lists[NUM_COPIES];

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            for (int j = 0; j < dataSize; ++j)
            {
                lists[i].addElementAtEnd(baseData[j]);
            }
        }

        auto start = high_resolution_clock::now();

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            lists[i].deleteElementAtPosition(dataSize / 2); // Usuwamy element ze środka listy
        }

        auto end = high_resolution_clock::now();

        auto totalDuration = duration_cast<nanoseconds>(end - start).count();
        long long avgTime = totalDuration / NUM_COPIES;

        csvFile << "DoublyLinkedList;DeleteAtPosition;" << dataSize << ";" << avgTime << "\n";
    }

    // ==========================================
    // OPERACJA: Szukanie elementu (załóżmy, że szukamy ostatniego)
    // ==========================================

    {
        DoublyLinkedList lists[NUM_COPIES];
        int valueToSearch = baseData[dataSize - 1]; // Szukamy ostatniego elementu (pesymistyczny przypadek)

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            for (int j = 0; j < dataSize; ++j)
            {
                lists[i].addElementAtEnd(baseData[j]);
            }
        }

        auto start = high_resolution_clock::now();

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            lists[i].searchElement(valueToSearch);
        }

        auto end = high_resolution_clock::now();

        auto totalDuration = duration_cast<nanoseconds>(end - start).count();
        long long avgTime = totalDuration / NUM_COPIES;

        csvFile << "DoublyLinkedList;Search;" << dataSize << ";" << avgTime << "\n";
    }
}

void Test::runAllTests(const string &inputFilename, const string &outputCsvFilename)
{
    cout << "Wczytywanie danych z pliku: " << inputFilename << " ..." << endl;

    int dataSize = 0;
    int *baseData = loadDataFromFile(inputFilename, dataSize);

    if (baseData == nullptr || dataSize == 0)
    {
        cerr << "Brak danych do testow. Przerwano." << endl;
        return;
    }

    ofstream csvFile(outputCsvFilename);
    if (!csvFile.is_open())
    {
        cerr << "Nie udalo sie otworzyc pliku CSV do zapisu!" << endl;
        delete[] baseData;
        return;
    }

    // Nagłówki w pliku CSV
    csvFile << "DataStructure;Operation;Size;AverageTime_ns\n";
    int testSizes[] = {20000, 40000, 60000, 80000};
    int numberOfSizes = 4;

    for (int i = 0; i < numberOfSizes; ++i)
    {
        int currentSize = testSizes[i];
        cout << "Rozpoczynam testy DynamicTable dla rozmiaru: " << currentSize << endl;
        testDynamicTable(baseData, currentSize, csvFile);

        cout << "Rozpoczynam testy SinglyLinkedList dla rozmiaru: " << currentSize << endl;
        testSinglyLinkedList(baseData, currentSize, csvFile);

        cout << "Rozpoczynam testy DoublyLinkedList dla rozmiaru: " << currentSize << endl;
        testDoublyLinkedList(baseData, currentSize, csvFile);
    }

    csvFile.close();
    cout << "Testy zakonczone! Wyniki zapisano w pliku: " << outputCsvFilename << endl;

    // Zwalniamy tablicę z danymi z pliku
    delete[] baseData;
}