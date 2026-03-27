#ifndef MENU_H
#define MENU_H

#include "DynamicTable.h"
#include "SinglyLinkedList.h"
#include "DoublyLinkedList.h"

class Menu
{
public:
    Menu();
    ~Menu();
    void displayMenu() const;
    void handleUserInput(DynamicTable &, SinglyLinkedList &, DoublyLinkedList &);
    void displayStructuresMenu() const;
};

#endif