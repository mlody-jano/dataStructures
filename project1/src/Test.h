#ifndef TEST_H
#define TEST_H

#pragma once

#include "DynamicTable.h"
#include "SinglyLinkedList.h"
#include "DoublyLinkedList.h"

#include <vector>
#include <string>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <functional>

struct TestResult
{
    std::string structure;
    std::string operation;
    int dataSize;
    long long avgTimeNs;
};

class Test
{
public:
    Test(const std::string &dataFilePath,
         int repetitions = 100,
         int startSize = 2000,
         int sizeStep = 2000,
         int numberOfSizes = 5);

    ~Test() = default;

    void runAll();
    void saveToCSV(const std::string &outputFilePath) const;

private:
    std::string dataFilePath;
    int repetitions;
    int startSize;
    int sizeStep;
    int numberOfSizes;

    std::vector<TestResult> results;

    // Pozycja i wartość do wyszukiwania — losowane raz przy starcie
    int fixedPosition;
    int fixedSearchValue;

    // Wczytuje dokładnie n liczb z pliku
    std::vector<int> loadData(int n) const;

    // Wypełnia strukturę danymi (nie mierzony czas)
    template <typename T>
    void fillStructure(T &structure, const std::vector<int> &data) const;

    // Tworzy wektor 'repetitions' kopii struktury wypełnionej danymi
    template <typename T>
    std::vector<T *> createCopies(const std::vector<int> &data) const;

    // Zwalnia pamięć kopii
    template <typename T>
    void deleteCopies(std::vector<T *> &copies) const;

    // Mierzy średni czas operacji na 'repetitions' kopiach [ns]
    template <typename T>
    long long measure(std::vector<T *> &copies,
                      std::function<void(T &)> operation) const;

    // Uruchamia wszystkie operacje dla jednej struktury i jednego rozmiaru
    template <typename T>
    void runForStructure(const std::string &structureName,
                         const std::vector<int> &data,
                         int dataSize);
};

#endif