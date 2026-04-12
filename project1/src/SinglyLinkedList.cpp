#include "SinglyLinkedList.h"
#include <iostream>

/*
    Constructor for SinglyLinkedList
    Initializes the head and tail pointers to nullptr and size to 0
*/
SinglyLinkedList::SinglyLinkedList() : head{nullptr}, tail{nullptr}, size{0} {}

/*
    Destructor for SinglyLinkedList
    Deallocates the memory used by the nodes in the list
*/
SinglyLinkedList::~SinglyLinkedList() {
    Node *current = head;
    while (current != nullptr)
    {
        Node *temp = current;
        current = current->next;
        delete temp;
    }
}

/*
    Add an element at the beginning of the list
    Creates a new node with the given value and sets it as the new head of the list
    Is O(1) since it only involves updating a few pointers and does not require traversing the list
*/
void SinglyLinkedList::addElementAtBeginning(int value) {
    Node *newNode = new Node{value, head};
    head = newNode;
    if (tail == nullptr)
    {
        tail = newNode;
    }
    size++;
}

/*
    Delete an element from the beginning of the list
    Updates the head pointer to point to the next node and deallocates the memory used by the old head node
    Is O(1) since it only involves updating a few pointers and does not require traversing the list
*/
void SinglyLinkedList::deleteElementAtBeginning() {
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

/*
    Add an element at the end of the list
    Creates a new node with the given value and sets it as the new tail of the list
    Is O(1) since it only involves updating a few pointers and does not require traversing the list
*/
void SinglyLinkedList::addElementAtEnd(int value) {
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

/*
    Delete an element from the end of the list
    Updates the tail pointer to point to the previous node and deallocates the memory used by the old tail node
    Is O(n) since it requires traversing the list to find the second-to-last node
*/
void SinglyLinkedList::deleteElementAtEnd() {
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

/*
    Add an element at a specific position in the list
    Creates a new node with the given value and inserts it at the specified position in the list
    Is O(n) since it may require traversing the list to find the correct position for insertion
*/
void SinglyLinkedList::addElementAtPosition(int value, int position) {
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

/*
    Delete an element from a specific position in the list
    Updates the pointers to bypass the node at the specified position
    Is O(n) since it may require traversing the list to find the correct position for deletion
*/
void SinglyLinkedList::deleteElementAtPosition(int position) {
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

/*
    Delete all elements from the list
    Deallocates the memory used by all nodes in the list and resets the head and tail pointers to nullptr
    Is O(n) since it requires traversing the entire list to delete all nodes
*/
void SinglyLinkedList::deleteAllElements() {
    Node *current = head;
    while (current != nullptr)
    {
        Node *temp = current;
        current = current->next;
        delete temp;
    }
    SinglyLinkedList(); // Reinitializing the list to its default state
}


/*
    Search for an element in the list
    Traverses the list to find the specified value and returns true if found, false otherwise
    Is O(n) since it may require traversing the entire list in the worst case
*/
volatile bool SinglyLinkedList::searchElement(int value) {
    Node *current = head;
    bool found = false;
    int position = 0;
    while (current != nullptr)
    {
        if (current->data == value) {
            std::cout << "Value " << value << " found at position " << position << std::endl;
            found = true;
        }
        current = current->next;
        position++;
    }
    if (!found)
    {
        std::cout << "Element " << value << " not found in the list." << std::endl;
    }

    return found;
}

/*
    Display the contents of the list
    Traverses the list and prints the value of each node to the console
    Is O(n) since it requires traversing the entire list to display all elements
*/
void SinglyLinkedList::display() const {
    Node *current = head;

    std::cout << "Singly Linked List: " << std::endl;
    while (current != nullptr)
    {
        std::cout << current->data << " ";
        current = current->next;
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
    Return the element at a specific position in the list
    Traverses the list to find the node at the specified position and returns its value
    Is O(n) since it may require traversing the entire list in the worst case
*/
int SinglyLinkedList::returnElementAtPosition(int position) const {
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