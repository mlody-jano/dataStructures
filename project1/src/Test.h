#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <chrono>
#include <string>
#include <fstream>
#include "DynamicTable.h"
#include "SinglyLinkedList.h"
#include "DoublyLinkedList.h"

using namespace std;
using namespace chrono;

class Test
{
private:
    const int NUM_COPIES = 100;

    int *loadDataFromFile(const std::string &filename, int &outSize);

    void testDynamicTable(int *baseData, int dataSize, std::ofstream &csvFile);
    void testSinglyLinkedList(int *baseData, int dataSize, std::ofstream &csvFile);
    void testDoublyLinkedList(int *baseData, int dataSize, std::ofstream &csvFile);

    template <typename Container, typename Operation>
    void measureOperation(
        const string &containerName,
        const string &operationName,
        int *baseData,
        int dataSize,
        ofstream &csvFile,
        Operation op)
    {
        Container containers[NUM_COPIES];

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            for (int j = 0; j < dataSize; ++j)
            {
                containers[i].addElementAtEnd(baseData[j]);
            }
        }

        auto start = high_resolution_clock::now();

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            op(containers[i]);
        }

        auto end = high_resolution_clock::now();

        auto totalDuration = duration_cast<nanoseconds>(end - start).count();
        long long avgTime = totalDuration / NUM_COPIES;

        csvFile << containerName << ";" << operationName << ";" << dataSize << ";" << avgTime << "\n";
    }

    template <typename Container>
    void runTests(const string &ContainerName, int *baseData, int dataSize, ofstream &csvFile)
    {
        int testValue = 9999;
        int middle = dataSize / 2;
        int valueToSearch = baseData[middle];

        measureOperation<Container>(ContainerName, "AddAtBeginning", baseData, dataSize, csvFile, [&](Container &c)
                                    { c.addElementAtBeginning(testValue); });

        measureOperation<Container>(ContainerName, "AddAtEnd", baseData, dataSize, csvFile, [&](Container &c)
                                    { c.addElementAtEnd(testValue); });

        measureOperation<Container>(ContainerName, "AddAtPosition", baseData, dataSize, csvFile, [&](Container &c)
                                    { c.addElementAtPosition(testValue, middle); });

        measureOperation<Container>(ContainerName, "DeleteAtBeginning", baseData, dataSize, csvFile, [&](Container &c)
                                    { c.deleteElementAtBeginning(); });

        measureOperation<Container>(ContainerName, "DeleteAtEnd", baseData, dataSize, csvFile, [&](Container &c)
                                    { c.deleteElementAtEnd(); });

        measureOperation<Container>(ContainerName, "DeleteAtPosition", baseData, dataSize, csvFile, [&](Container &c)
                                    { c.deleteElementAtPosition(middle); });

        measureOperation<Container>(ContainerName, "Search", baseData, dataSize, csvFile, [&](Container &c)
                                    { c.searchElement(valueToSearch); });
    }

public:
    void runAllTests(const std::string &inputFilename, const std::string &outputCsvFilename);
};

#endif