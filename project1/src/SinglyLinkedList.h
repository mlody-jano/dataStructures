#ifndef SINGLYLINKEDLIST_H
#define SINGLYLINKEDLIST_H

#pragma once

struct Node
{
    int data;
    Node *next;
};

class SinglyLinkedList
{
private:
    Node *head;
    Node *tail;
    int size;

public:
    SinglyLinkedList();
    ~SinglyLinkedList();

    void pushFront(int value);
    void pushBack(int value);
    void popFront();
    void popBack();
    void insert(int value, int position);
    void remove(int position);
    void find(int value);
    void display() const;
};

#endif