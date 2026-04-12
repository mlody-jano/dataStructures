#include "Test.h"
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

/*
    Load data from a file into an array
    Reads integers from the specified file and stores them in a dynamically allocated array
    Returns a pointer to the array and sets the output parameter outSize to the number of elements read
*/
int *Test::loadDataFromFile(const string &filename, int &outSize)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Nie udalo sie otworzyc pliku: " << filename << endl;
        outSize = 0;
        return nullptr;
    }

    int count = 0;
    int tempValue;
    while (file >> tempValue)
    {
        count++;                                // Count the number of integers in the file
    }

    file.clear();                               // Clear the error state caused by reaching the end of the file
    file.seekg(0, ios::beg);                    // Reset the file stream to the beginning of the file

    int *dataArray = new int[count];
    for (int i = 0; i < count; i++)             // Use the count to read the integers into the array
    {
        file >> dataArray[i];
    }

    file.close();
    outSize = count;
    return dataArray;
}


/*
    Test functions for each data structure
    Each function runs a series of tests on the specified data structure and records the results in the provided CSV file
*/
void Test::testDynamicTable(int *baseData, int dataSize, ofstream &csvFile)
{
    runTests<DynamicTable>("DynamicTable", baseData, dataSize, csvFile);
}

void Test::testSinglyLinkedList(int *baseData, int dataSize, ofstream &csvFile)
{
    runTests<SinglyLinkedList>("SinglyLinkedList", baseData, dataSize, csvFile);
}

void Test::testDoublyLinkedList(int *baseData, int dataSize, ofstream &csvFile)
{
    runTests<DoublyLinkedList>("DoublyLinkedList", baseData, dataSize, csvFile);
}


/*
    Run all tests for the three data structures
    Loads data from the specified input file, runs tests for each data structure at various sizes, and saves the results to the specified CSV file
*/
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

    /*
        Open the CSV file for writing test results
        Writes the header row to the CSV file and prepares to record results for each test
    */
    ofstream csvFile(outputCsvFilename);
    if (!csvFile.is_open())
    {
        cerr << "Nie udalo sie otworzyc pliku CSV do zapisu!" << endl;
        delete[] baseData;
        return;
    }

    csvFile << "DataStructure;Operation;Size;AverageTime_ns\n";
    int testSizes[] = {20000, 40000, 60000, 80000, 100000, 120000, 140000, 160000};                     // Define the sizes for which to run the tests
    int numberOfSizes = sizeof(testSizes) / sizeof(testSizes[0]);                                       // Calculate the number of sizes to test


    /*
        Main testing loop
        Iterates through each defined size, runs tests for each data structure, and records the results in the CSV file
    */
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

    delete[] baseData;
}