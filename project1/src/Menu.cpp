#include "Menu.h"
#include "SinglyLinkedList.h"
#include "DoublyLinkedList.h"

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

void Menu::handleUserInput(DynamicTable& table, SinglyLinkedList& singlyList, DoublyLinkedList& doublyList)
{
    int structure, choice, element, position;

    while (true) {
        displayMenu();
        cout << "Enter structure: ";
        cin >> structure;

        switch (structure)
        {
        case 1:
            displayMenu();
            cout << "Enter choice: ";
            cin >> choice;
            switch (choice) {
            case 1:
                cout << "Enter element to add at the beginning: ";
                cin >> element;
                table.addElementAtBeginning(element);
                break;
            case 2:
                cout << "Enter element to add at the end: ";
                cin >> element;
                table.addElementAtEnd(element);
                break;
            case 3:
                cout << "Enter element to add: ";
                cin >> element;
                cout << "Enter position to add the element: ";
                cin >> position;
                table.addElementAtPosition(element, position);
                break;
            case 4:
                table.deleteElementAtBeginning();
                break;
            case 5:
                table.deleteElementAtEnd();
                break;
            case 6:
                cout << "Enter position to delete the element from: ";
                cin >> position;
                table.deleteElementAtPosition(position);
                break;
            case 7:
                cout << "Enter element to search for: ";
                cin >> element;
                table.searchElement(element);
                break;
            case 8:
                table.printTable();
                break;
            case 9:
                cout << "Exiting..." << endl;
                return;
            default:
                cerr << "Invalid choice. Please try again." << endl;
            }
            break;
        case 2:
            displayMenu();
            cout << "Enter choice: ";
            cin >> choice;
            switch (choice)
            {
            case 1:
                cout << "Enter element to add at the beginning: ";
                cin >> element;
                singlyList.addElementAtBeginning(element);
                break;
            case 2:
                cout << "Enter element to add at the end: ";
                cin >> element;
                singlyList.addElementAtEnd(element);
                break;
            case 3:
                cout << "Enter element to add: ";
                cin >> element;
                cout << "Enter position to add the element: ";
                cin >> position;
                singlyList.addElementAtPosition(element, position);
                break;
            case 4:
                singlyList.deleteElementAtBeginning();
                break;
            case 5:
                singlyList.deleteElementAtEnd();
                break;
            case 6:
                cout << "Enter position to delete the element from: ";
                cin >> position;
                singlyList.deleteElementAtPosition(position);
                break;
            case 7:
                cout << "Enter element to search for: ";
                cin >> element;
                singlyList.searchElement(element);
                break;
            
            default:
                break;
            }
            break;
        case 3:
            displayMenu();
            cout << "Enter choice: ";
            cin >> choice;
            switch (choice) {
            case 1:
                cout << "Enter element to add at the beginning: ";
                cin >> element;
                doublyList.addElementAtBeginning(element);
                break;
            case 2:
                cout << "Enter element to add at the end: ";
                cin >> element;
                doublyList.addElementAtEnd(element);
                break;
            case 3:
                cout << "Enter element to add: ";
                cin >> element;
                cout << "Enter position to add the element: ";
                cin >> position;
                doublyList.addElementAtPosition(element, position);
                break;
                case 4:
                singlyList.deleteElementAtBeginning();
                break;
            case 5:
                singlyList.deleteElementAtEnd();
                break;
            case 6:
                cout << "Enter position to delete the element from: ";
                cin >> position;
                singlyList.deleteElementAtPosition(position);
                break;
            case 7:
                cout << "Enter element to search for: ";
                cin >> element;
                singlyList.searchElement(element);
                break;
            
            default:
                break;
            }
            break;
        default:
            cerr << "Invalid structure. Please try again." << endl;
        }
    }
}