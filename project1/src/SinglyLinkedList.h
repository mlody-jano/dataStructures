#ifndef SINGLYLINKEDLIST_H
#define SINGLYLINKEDLIST_H

#pragma once

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
    void searchElement(int value);
    void display() const;
};

#endif