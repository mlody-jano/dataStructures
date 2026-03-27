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
};

#endif