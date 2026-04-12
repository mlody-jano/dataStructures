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

void SinglyLinkedList::addElementAtBeginning(int value)
{
    Node *newNode = new Node{value, head};
    head = newNode;
    if (tail == nullptr)
    {
        tail = newNode;
    }
    size++;
}

void SinglyLinkedList::deleteElementAtBeginning()
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

void SinglyLinkedList::addElementAtEnd(int value)
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

void SinglyLinkedList::deleteElementAtEnd()
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
        size--;
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

void SinglyLinkedList::addElementAtPosition(int value, int position)
{
    if (position < 0)
    {
        return;
    }

    if (position == 0)
    {
        addElementAtBeginning(value);
        return;
    }

    else if (position == size)
    {
        addElementAtEnd(value);
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

    size++;
}

void SinglyLinkedList::deleteElementAtPosition(int position)
{
    if (position < 0 || head == nullptr)
    {
        return;
    }

    if (position == 0)
    {
        deleteElementAtBeginning();
        return;
    }

    if (position == size - 1)
    {
        deleteElementAtEnd();
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
    delete temp;
    size--;
}

void SinglyLinkedList::deleteAllElements()
{
    Node *current = head;
    while (current != nullptr)
    {
        Node *temp = current;
        current = current->next;
        delete temp;
    }
    SinglyLinkedList(); // Reinitializing the list to its default state
}

volatile bool SinglyLinkedList::searchElement(int value)
{
    Node *current = head;
    int position = 0;
    while (current != nullptr)
    {
        if (current->data == value)
        {
            // std::cout << "Value " << value << " found at position " << position << std::endl;
            return true;
        }
        current = current->next;
        position++;
    }
    // std::cout << "Value " << value << " not found in the list." << std::endl;
    return false;
}

void SinglyLinkedList::display() const
{
    Node *current = head;

    std::cout << "Singly Linked List: " << std::endl;
    for (int i = 0; i < size; i++)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    while (current != nullptr)
    {
        std::cout << current->data << " ";
        current = current->next;
    }
    std::cout << std::endl;
}

int SinglyLinkedList::returnElementAtPosition(int position) const
{
    if (position < 0 || position >= size)
    {
        std::cerr << "Invalid position. Returning -1." << std::endl;
        return -1; // Return -1 to indicate an error
    }

    Node *current = head;
    for (int i = 0; i < position; i++)
    {
        current = current->next;
    }
    return current->data;
}