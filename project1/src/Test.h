#ifndef TEST_H
#define TEST_H

#include <string>
#include <fstream>
#include "DynamicTable.h"
#include "SinglyLinkedList.h"
#include "DoublyLinkedList.h"

class Test
{
private:
    const int NUM_COPIES = 100;

    // Funkcja ładująca dane z pliku do tablicy int*. Zwraca wskaźnik, a rozmiar przekazuje przez referencję.
    int *loadDataFromFile(const std::string &filename, int &outSize);

    // Funkcje testujące poszczególne struktury
    void testDynamicTable(int *baseData, int dataSize, std::ofstream &csvFile);
    void testSinglyLinkedList(int *baseData, int dataSize, std::ofstream &csvFile);
    void testDoublyLinkedList(int *baseData, int dataSize, std::ofstream &csvFile);

public:
    // Główna metoda uruchamiająca wszystkie testy
    void runAllTests(const std::string &inputFilename, const std::string &outputCsvFilename);
};

#endif