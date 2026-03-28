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

    void addElementAtBeginning(int value);
    void addElementAtEnd(int value);
    void deleteElementAtBeginning();
    void deleteElementAtEnd();
    void addElementAtPosition(int value, int position);
    void deleteElementAtPosition(int position);
    void searchElement(int value);
    void display() const;
};

#endif