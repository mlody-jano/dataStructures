#ifndef SINGLYLINKEDLIST_H
#define SINGLYLINKEDLIST_H

#pragma once

/*
    Class representing a singly linked list data structure
    It allows for adding and deleting elements at the beginning, end, and specific positions in the list
    It also provides functions for searching for an element, returning an element at a specific position, and displaying the contents of the list
*/
class SinglyLinkedList
{
private:
    struct Node
    {
        int data;
        Node *next;
    };

    Node *head;
    Node *tail;
    int size;

public:
    SinglyLinkedList();
    ~SinglyLinkedList();

    void addElementAtBeginning(int value);
    void addElementAtEnd(int value);

    void deleteElementAtBeginning();
    void deleteElementAtEnd();
    void addElementAtPosition(int value, int position);
    void deleteElementAtPosition(int position);
    void deleteAllElements();

    volatile bool searchElement(int value);
    int returnElementAtPosition(int position) const;
    void display() const;
};

#endif