#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#pragma once

class DoublyLinkedList
{
private:
    struct Node
    {
        int data;
        Node *prev;
        Node *next;
    };

    Node *head;
    Node *tail;
    int size;

public:
    DoublyLinkedList();
    ~DoublyLinkedList();

    void addElementAtBeginning(int);
    void addElementAtEnd(int);
    void addElementAtPosition(int, int);

    void deleteElementAtBeginning();
    void deleteElementAtEnd();
    void deleteElementAtPosition(int);
    void deleteAllElements();

    volatile bool searchElement(int) const;

    int returnElementAtPosition(int) const;

    void display() const;
};

#endif