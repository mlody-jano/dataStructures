#include "DoublyLinkedList.h"
#include <iostream>

/*
    Constructor for DoublyLinkedList
*/
DoublyLinkedList::DoublyLinkedList() : head{nullptr}, tail{nullptr}, size{0} {}

/*
    Destructor for DoublyLinkedList
*/
DoublyLinkedList::~DoublyLinkedList()
{
    Node *current = head;
    while (current != nullptr)
    {
        Node *temp = current;
        current = current->next;
        delete temp;
    }
}

/*
    Adding the element at the beggining of the list
    Is O(1) due to the head pointer and the fact we dont need to copy and shift the elements like in an array
*/
void DoublyLinkedList::addElementAtBeginning(int value)
{
    Node *newNode = new Node{value, nullptr, head};
    if (head != nullptr)
    {
        head->prev = newNode;
    }
    head = newNode;
    if (tail == nullptr)
    {
        tail = newNode;
    }
    size++;
}

/*
    Deleting the element at the beggining of the list
    Is O(1) due to the head pointer and the fact we dont need to copy and shift the elements like in an array
*/
void DoublyLinkedList::deleteElementAtBeginning()
{
    if (head == nullptr)
    {
        return;
    }

    Node *temp = head;
    head = head->next;
    if (head != nullptr)
    {
        head->prev = nullptr;
    }
    else
    {
        tail = nullptr;
    }
    delete temp;
    size--;
}

/*
    Adding the element at the end of the list
    Is O(1) due to the tail pointer and the fact we dont need to copy and shift the elements like in an array
*/
void DoublyLinkedList::addElementAtEnd(int value)
{
    Node *newNode = new Node{value, tail, nullptr};
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

/*
    Deleting the element at the end of the list
    Is O(1) due to the tail pointer and the fact we dont need to copy and shift the elements like in an array
*/
void DoublyLinkedList::deleteElementAtEnd()
{
    if (tail == nullptr)
    {
        return;
    }

    Node *temp = tail;
    tail = tail->prev;
    if (tail != nullptr)
    {
        tail->next = nullptr;
    }
    else
    {
        head = nullptr;
    }
    delete temp;
    size--;
}

/*
    Adding the element at the given position of the list
    Is O(n) due to the fact we need to traverse the list to find the position where we want to add the element
*/
void DoublyLinkedList::addElementAtPosition(int value, int position)
{
    if (position < 0 || position > size)
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

    Node *current;
    if (position < size / 2)
    {
        current = head;
        for (int i = 0; i < position && current != nullptr; ++i)
        {
            current = current->next;
        }
    }
    else
    {
        current = tail;
        for (int i = size - 1; i > position && current != nullptr; --i)
        {
            current = current->prev;
        }
    }
    Node *newNode = new Node{value, current->prev, current};
    if (current->prev != nullptr)
    {
        current->prev->next = newNode;
    }
    current->prev = newNode;

    size++;
}

/*
    Deleting the element at the given position of the list
    Is O(n) due to the fact we need to traverse the list to find the position where we want to delete the element
*/
void DoublyLinkedList::deleteElementAtPosition(int position)
{
    if (position < 0 || position >= size)
    {
        return;
    }

    if (position == 0)
    {
        deleteElementAtBeginning();
        return;
    }
    else if (position == size - 1)
    {
        deleteElementAtEnd();
        return;
    }

    Node *current;
    if (position < size / 2)
    {
        current = head;
        for (int i = 0; i < position && current != nullptr; ++i)
        {
            current = current->next;
        }
    }
    else
    {
        current = tail;
        for (int i = size - 1; i > position && current != nullptr; --i)
        {
            current = current->prev;
        }
    }

    current->prev->next = current->next;
    current->next->prev = current->prev;
    delete current;

    size--;
}

/*
    Deleting all elements from the list
    Is O(n) due to the fact we need to traverse the list to delete all the elements
    After deleting all elements we reinitialize the list to its default state
*/
void DoublyLinkedList::deleteAllElements()
{
    Node *current = head;
    while (current != nullptr)
    {
        Node *temp = current;
        current = current->next;
        delete temp;
    }
    DoublyLinkedList(); // Reinitializing the list to its default state
}

/*
    Searching for an element in the list
    Is O(n) due to the fact we need to traverse the list to find the element
*/
volatile bool DoublyLinkedList::searchElement(int value) const
{
    Node *current = head;
    int position = 0;
    bool found = false;
    while (current != nullptr)
    {
        if (current->data == value)
        {
            // std::cout << "Value " << value << " found at position " << position << std::endl;
            found = true;
        }
        current = current->next;
        position++;
    }
    if (!found)
    {
        // std::cout << "Value " << value << " not found in the list." << std::endl;
    }
    return found;
}

/*
    Displaying the elements of the list
    Is O(n) due to the fact we need to traverse the list to display all the elements
*/
void DoublyLinkedList::display() const
{
    Node *current = head;

    std::cout << "Doubly Linked List: " << std::endl;
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

/*
    Returning the element at the given position of the list
    Is O(n) due to the fact we need to traverse the list to find the position where we want to return the element
*/
int DoublyLinkedList::returnElementAtPosition(int position) const
{
    if (position < 0 || position >= size)
    {
        throw std::out_of_range("Position out of range");
    }

    Node *current;
    if (position < size / 2)
    {
        current = head;
        for (int i = 0; i < position && current != nullptr; ++i)
        {
            current = current->next;
        }
    }
    else
    {
        current = tail;
        for (int i = size - 1; i > position && current != nullptr; --i)
        {
            current = current->prev;
        }
    }
    return current->data;
}