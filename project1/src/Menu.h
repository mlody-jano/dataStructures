#ifndef MENU_H
#define MENU_H

#include "DynamicTable.h"
#include "SinglyLinkedList.h"
#include "DoublyLinkedList.h"

/*
    Class representing the menu for the program
    It allows the user to interact with the three data structures and perform various operations on them
    The menu is implemented as a class with member functions for displaying the menu options and handling user input
*/
class Menu
{
private:
    /*
        Display the submenu options for a specific data structure
        Using a template function to allow for handling user input for all three data structures without needing to duplicate code for each data structure
    */
    template <typename T>
    void displaySubMenu(T &dataStructure)
    {
        int choice, value, position;
        bool exitSubMenu = false;
        while (!exitSubMenu)
        {
            displayMenu();
            cout << "Enter choice: ";
            cin >> choice;
            switch (choice)
            {
            case 1:
                cout << "Enter element to add at the beginning: ";
                cin >> value;
                dataStructure.addElementAtBeginning(value);
                break;
            case 2:
                cout << "Enter element to add at the end: ";
                cin >> value;
                dataStructure.addElementAtEnd(value);
                break;
            case 3:
                cout << "Enter element to add: ";
                cin >> value;
                cout << "Enter position to add the element: ";
                cin >> position;
                dataStructure.addElementAtPosition(value, position);
                break;
            case 4:
                dataStructure.deleteElementAtBeginning();
                break;
            case 5:
                dataStructure.deleteElementAtEnd();
                break;
            case 6:
                cout << "Enter position to delete the element from: ";
                cin >> position;
                dataStructure.deleteElementAtPosition(position);
                break;
            case 7:
                cout << "Enter element to search for: ";
                cin >> value;
                dataStructure.searchElement(value);
                break;
            case 8:
                dataStructure.display();
                break;
            case 9:
                cout << "Exiting..." << endl;
                exitSubMenu = true;
                break;
            default:
                cerr << "Invalid choice. Please try again." << endl;
            }
        }
    }

public:
    Menu();
    ~Menu();
    void displayMenu() const;
    void handleUserInput(DynamicTable &, SinglyLinkedList &, DoublyLinkedList &);
    void displayStructuresMenu() const;
};

#endif