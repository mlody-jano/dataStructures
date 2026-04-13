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

/*
    Class representing the test suite for the three data structures
    It provides functions for loading data from a file, running tests for each data structure, and recording the results in a CSV file
    The tests include adding and deleting elements at various positions, as well as searching for an element in the data structure
    The results of each test are recorded in a CSV file with the format: DataStructure;Operation;Size;AverageTime_ns
*/
class Test
{
private:
    const int NUM_COPIES = 100; // Number of iterations to run each test for averaging the results

    int *loadDataFromFile(const std::string &filename, int &outSize);

    void testDynamicTable(int *baseData, int dataSize, std::ofstream &csvFile);
    void testSinglyLinkedList(int *baseData, int dataSize, std::ofstream &csvFile);
    void testDoublyLinkedList(int *baseData, int dataSize, std::ofstream &csvFile);

    /*
        Measure the time taken to perform a specific operation on a container
        This function takes a container type, an operation to perform, and the data to use for
        Using a template function to unify the testing process for all three data structures
    */
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

        auto start = high_resolution_clock::now(); // Start the timer before performing the operation on all copies of the container

        for (int i = 0; i < NUM_COPIES; ++i)
        {
            op(containers[i]); // Perform the specified operation on each copy of the container
        }

        auto end = high_resolution_clock::now(); // Stop the timer after performing the operation on all copies of the container

        auto totalDuration = duration_cast<nanoseconds>(end - start).count();
        long long avgTime = totalDuration / NUM_COPIES; // Calculate the average time

        csvFile << containerName << ";" << operationName << ";" << dataSize << ";" << avgTime << "\n";
    }

    template <typename Container>
    void runTests(const string &ContainerName, int *baseData, int dataSize, ofstream &csvFile)
    {
        int testValue = 9999;
        int pos = rand() % (dataSize - 1); // Randomize position selection
        int valueToSearch = baseData[pos];

        measureOperation<Container>(ContainerName, "AddAtBeginning", baseData, dataSize, csvFile, [&](Container &c)
                                    { c.addElementAtBeginning(testValue); });

        measureOperation<Container>(ContainerName, "AddAtEnd", baseData, dataSize, csvFile, [&](Container &c)
                                    { c.addElementAtEnd(testValue); });

        measureOperation<Container>(ContainerName, "AddAtPosition", baseData, dataSize, csvFile, [&](Container &c)
                                    { c.addElementAtPosition(testValue, pos); });

        measureOperation<Container>(ContainerName, "DeleteAtBeginning", baseData, dataSize, csvFile, [&](Container &c)
                                    { c.deleteElementAtBeginning(); });

        measureOperation<Container>(ContainerName, "DeleteAtEnd", baseData, dataSize, csvFile, [&](Container &c)
                                    { c.deleteElementAtEnd(); });

        measureOperation<Container>(ContainerName, "DeleteAtPosition", baseData, dataSize, csvFile, [&](Container &c)
                                    { c.deleteElementAtPosition(pos); });

        measureOperation<Container>(ContainerName, "Search", baseData, dataSize, csvFile, [&](Container &c)
                                    { c.searchElement(valueToSearch); });
    }

public:
    void runAllTests(const std::string &inputFilename, const std::string &outputCsvFilename);
};

#endif