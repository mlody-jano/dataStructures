#ifndef MENU_H
#define MENU_H

#include "DynamicTable.h"

class Menu {
public:
    Menu();
    ~Menu();
    void displayMenu() const;
    void handleUserInput(DynamicTable&, SinglyLinkedList&, DoublyLinkedList&);
};

#endif