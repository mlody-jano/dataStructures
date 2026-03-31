#include <iostream>
#include "Menu.h"
#include "Test.h"

int main()
{
    int mode;
    std::cout << "Wybierz tryb pracy:\n1. Menu interaktywne\n2. Testy wydajnosciowe\nWybor: ";
    std::cin >> mode;

    if (mode == 1)
    {
        DynamicTable table;
        SinglyLinkedList sList;
        DoublyLinkedList dList;
        Menu menu;
        menu.handleUserInput(table, sList, dList);
    }
    else if (mode == 2)
    {
        Test performanceTests;
        // Zakładamy, że masz plik "dane.txt" w folderze roboczym projektu
        performanceTests.runAllTests("../testing/data/data_1.txt", "../testing/data/wyniki.csv");
    }
    else
    {
        std::cout << "Nieznana opcja." << std::endl;
    }

    return 0;
}