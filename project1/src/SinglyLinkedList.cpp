#include "SinglyLinkedList.h"
#include <iostream>

SinglyLinkedList::SinglyLinkedList()
{
    head = nullptr;
    tail = nullptr;
    size = 0;
}

SinglyLinkedList::~SinglyLinkedList()
{
    Node *current = head;
    while (current != nullptr)
    {
        Node *temp = current;
        current = current->next;
        delete temp;
    }
}

void SinglyLinkedList::pushFront(int value)
{
    Node *newNode = new Node{value, head};
    head = newNode;
    if (head == nullptr)
    {
        tail = newNode;
    }
    size++;
}

void SinglyLinkedList::popFront()
{
    if (head == nullptr)
    {
        return;
    }

    Node *temp = head;
    head = head->next;
    delete temp;

    if (head == nullptr)
    {
        tail = nullptr;
    }

    size--;
}

void SinglyLinkedList::pushBack(int value)
{
    Node *newNode = new Node{value, nullptr};

    if (tail != nullptr)
    {
        tail->next = newNode;
    }
    tail = newNode;

    if (head == nullptr)
    {
        head = newNode;
    }

    size++;
}

void SinglyLinkedList::popBack()
{
    if (head == nullptr)
    {
        return;
    }

    if (head == tail)
    {
        delete head;
        head = nullptr;
        tail = nullptr;
        return;
    }

    Node *current = head;
    while (current->next != tail)
    {
        current = current->next;
    }

    delete tail;
    tail = current;
    tail->next = nullptr;

    size--;
}

void SinglyLinkedList::insert(int value, int position)
{
    if (position < 0)
    {
        return;
    }

    if (position == 0)
    {
        pushFront(value);
        return;
    }

    Node *current = head;
    for (int i = 0; i < position - 1 && current != nullptr; ++i)
    {
        current = current->next;
    }

    if (current == nullptr)
    {
        return;
    }

    Node *newNode = new Node{value, nullptr};
    newNode->next = current->next;
    current->next = newNode;

    if (newNode->next == nullptr)
    {
        tail = newNode;
    }

    size++;
}

void SinglyLinkedList::remove(int position)
{
    if (position < 0 || head == nullptr)
    {
        return;
    }

    if (position == 0)
    {
        popFront();
        return;
    }

    Node *current = head;
    for (int i = 0; i < position - 1 && current != nullptr; ++i)
    {
        current = current->next;
    }

    if (current == nullptr || current->next == nullptr)
    {
        return;
    }

    Node *temp = current->next;
    current->next = temp->next;
    if (temp == tail)
    {
        tail = current;
    }
    delete temp;
}

void SinglyLinkedList::find(int value)
{
    Node *current = head;
    int position = 0;
    bool found = false;
    while (current != nullptr)
    {
        if (current->data == value)
        {
            std::cout << "Value " << value << " found at position " << position << std::endl;
            found = true;
        }
        current = current->next;
        position++;
    }
    if (!found)
    {
        std::cout << "Value " << value << " not found in the list." << std::endl;
    }
}

void SinglyLinkedList::display() const
{
    Node *current = head;
    while (current != nullptr)
    {
        std::cout << current->data << " ";
        current = current->next;
    }
    std::cout << std::endl;
}