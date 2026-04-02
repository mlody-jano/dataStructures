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

    int count = 0;
    int tempValue;
    while (file >> tempValue)
    {
        count++;
    }

    file.clear();
    file.seekg(0, ios::beg);

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

    csvFile << "DataStructure;Operation;Size;AverageTime_ns\n";
    int testSizes[] = {20000, 40000, 60000, 80000, 100000, 120000, 140000, 160000};
    int numberOfSizes = sizeof(testSizes) / sizeof(testSizes[0]);

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