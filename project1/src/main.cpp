#include "DynamicTable.h"
#include "SinglyLinkedList.h"
#include "DoublyLinkedList.h"
#include "Menu.h"

int main()
{

    // Testing of the DynamicTable class
    Menu menu;
    DynamicTable myTable;
    SinglyLinkedList mySinglyList;
    DoublyLinkedList myDoublyList;

    menu.handleUserInput(myTable, mySinglyList, myDoublyList);

    return 0;
}