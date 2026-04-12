#include "DynamicTable.h"
#include "SinglyLinkedList.h"
#include "DoublyLinkedList.h"
#include "Menu.h"
#include "Test.h"
#include <iostream>

/*
    Main function for the program
    It allows the user to choose between an interactive menu and performance tests
    Performance tests mode runs a series of tests on the three data structures and saves the results to a CSV file
    Interactive menu mode allows the user to interact with the three data structures and perform various operations on them
*/
int main(int argc, char* argv[])
{
    int mode;
    std::cout << "Wybierz tryb pracy:\n1. Menu interaktywne\n2. Testy wydajnosciowe\nWybor: ";
    std::cin >> mode;

    /*
        Interactive menu mode allows the user to interact with the three data structures and perform various operations on them
    */
    if (mode == 1)
    {
        DynamicTable                table;
        SinglyLinkedList            sList;
        DoublyLinkedList            dList;
        Menu                        menu;
        menu.handleUserInput(table, sList, dList);
    }
    /* 
        Performance tests mode runs a series of tests on the three data structures and saves the results to a CSV file 
    */
    else if (mode == 2)
    {
        Test                        performanceTests;
        performanceTests.runAllTests("../testing/data/data_1.txt", "../testing/data/wyniki.csv");
    }
    /* 
        Unknown option handling
    */
    else
    {
        std::cout << "Nieznana opcja." << std::endl;
    }
    return 0;
}