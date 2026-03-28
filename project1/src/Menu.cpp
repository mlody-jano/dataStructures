#include "Menu.h"
#include "DynamicTable.h"
#include "SinglyLinkedList.h"
#include "DoublyLinkedList.h"

Menu::Menu()
{
}

Menu::~Menu()
{
}

void Menu::displayMenu() const
{
    cout << "Menu:" << endl;
    cout << "1. Add element at the beginning" << endl;
    cout << "2. Add element at the end" << endl;
    cout << "3. Add element at a specific position" << endl;
    cout << "4. Delete element from the beginning" << endl;
    cout << "5. Delete element from the end" << endl;
    cout << "6. Delete element from a specific position" << endl;
    cout << "7. Search for an element" << endl;
    cout << "8. Print list contents" << endl;
    cout << "9. Exit" << endl;
}

void Menu::displayStructuresMenu() const
{
    cout << "Select a data structure:" << endl;
    cout << "1. Dynamic Table" << endl;
    cout << "2. Singly Linked List" << endl;
    cout << "3. Doubly Linked List" << endl;
}

void Menu::handleUserInput(DynamicTable &table, SinglyLinkedList &singlyList, DoublyLinkedList &doublyList)
{
    int structure;

    while (true)
    {
        displayStructuresMenu();
        cout << "Enter structure: ";
        cin >> structure;

        switch (structure)
        {
        case 1:
            displaySubMenu(table);
            break;
        case 2:
            displaySubMenu(singlyList);
            break;
        case 3:
            displaySubMenu(doublyList);
            break;
        default:
            cerr << "Invalid choice. Please try again." << endl;
        }
    }
}