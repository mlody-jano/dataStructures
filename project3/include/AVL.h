#pragma once
#include "Pair.h"

/**
 * class AVLNode
 * represents a node in an AVL tree, using @class Pair to store the key-value pair, pointers to left and right children, representing subtrees, and height of node
 * @tparam V type of value in the node, key is set to be integer type
 */
template <typename V>
class AVLNode {
    private:
        Pair<V> data;
        AVLNode* left;
        AVLNode* right;
        int height;

    public:
        AVLNode(int k) : data(Pair<V>(k)), left(nullptr), right(nullptr), height(1) {}
        int getHeight() const {return height;}
        void setHeight(int newH) {height = newH;}
        AVLNode* leftChild() const {return left;}
        AVLNode* rightChild() const {return right;}
};

/**
 * class AVL
 * represents the AVL tree data structure, using @class AVLNode to create a tree of nodes
 * @tparam V type of value in the node, key is set to be integer type
 * 
 */
template <typename V>
class AVL {
    private:
        AVLNode<V>* root;
        void rotateLeft(AVLNode<V>*&);
        void rotateRight(AVLNode<V>*&);
        AVLNode<V>* minValueNode(AVLNode<V>*&);
    protected:
        int balanceFactor(AVLNode<V>*&);
    public:
        AVL();
        ~AVL();
        void insert(AVLNode<V>*&, const int, const V&);
        void remove(AVLNode<V>*&, const int);
        void display() const;
        AVLNode<V>* getRoot() const {return root;}
        AVL<V>& operator=(const AVL<V>&);
        friend class AVLNode;
};

/**
 * zero argument constructor of class AVL, sets root to nullptr
 * @tparam V type of value in the node
 */
template <typename V>
AVL<V>::AVL() : root(nullptr) {}

/**
 * destructor of class AVL, deletes all nodes in post-order traversal
 * @tparam V type of value in the node
 */
template <typename V>
AVL<V>::~AVL() {
    std::function<void(AVLNode<V>*)> postOrder = [&](AVLNode<V>* node) {
        if (node == nullptr) return;
        postOrder(node->leftChild());
        postOrder(node->rightChild());
        delete node;
    };
    postOrder(root);
}

/**
 * method to insert a new node to AVL tree
 * @tparam V type of value in node
 * @param current pointer to starting node
 * @param key new key
 * @param value new value
 * method uses recursion to find the correct position for new node, sets heights and checks balance factors up the recursive call stack, and performs rotations to keep the property of an AVL tree
 * expected time complexity of this implementation is O(log n) on average, and worst case; best case is O(1) when tree is empty
 */
template <typename V>
void AVL<V>::insert(AVLNode<V>*& current, const int key, const V& value) {
    AVLNode<V>* newNode = new AVLNode<V>(key);
    newNode->data.setValue(value);

    // if tree is empty, set new node as root
    if (current == nullptr) {newNode->setHeight(1); current = newNode; return;}

    // recursive search for valid position for new node
    if      (key < current->data.getKey()) {insert(current->left, key, value);}
    else if (key > current->data.getKey()) {insert(current->right, key, value);}

    // set new heights up the recursive call stack
    current->setHeight(1 + std::max(current->leftChild().getHeight(), current->rightChild().getHeight()));

    // calculating balance factor up the recursive call stack
    int bf = balanceFactor(current);

    // if the balance factor is lower than -1 and the key is greater than the key of the right child, rotate left
    if(bf < -1 && key > current->rightChild()->data.getKey()) {rotateLeft(current);}

    // if the balance factor is higher than 1 and the key is lower than the key of the left child, rotate right
    if(bf > 1 && key < current->leftChild().data.getKey()) {rotateRight(current);}

    // if the balance factor is higher than 1 and the key is greater than the key of the left child, rotate left on left child and then rotate right on current node
    if(bf > 1 && key > current->leftChild().data.getKey()) { rotateRight(current->left); rotateLeft(current);}

    // if the balance factor is lower than -1 and the key is lower than the key of the right child, rotate right on right child and then rotate left on current node
    if(bf < -1 && key < current->rightChild().data.getKey()) { rotateLeft(current->right); rotateRight(current);}
}

/**
 * method to remove a node from AVL tree
 * @tparam V type of value in node
 * @param current pointer to starting node
 * @param key new key
 * method uses recursion to find the node to be removed, removes it, updates heights and checks balance factors up the recursive call stack, performing rotation to keep the property of an AVL tree
 * expected time complexity of this implementation is O(log n) on average, and worst case; best case is O(1) when tree has only one node
 */
template <typename V>
void AVL<V>::remove(AVLNode<V>*& current, const int key) {
    
    if(current == nullptr) {return;}

    if      (key < current->data.getKey()) {remove(current->left, key);}
    else if (key > current->data.getKey()) {remove(current->right, key);}
    else {
        if(!current->leftChild()) {
            AVLNode<V>* temp = current->rightChild();
            delete current;
            current = temp;
        }
        else if(!current->rightChild()) {
            AVLNode<V>* temp = current->leftChild();
            delete current;
            current = temp;
        }
        else {
            AVLNode<V>* temp = minValueNode(current->rightChild());
            current->data.setKey(temp->data.getKey());
            current->data.setValue(temp->data.getValue());
            remove(current->rightChild(), temp->data.getKey());
        }
    }

    // set new heights up the recursive call stack
    current->setHeight(1 + std::max(current->leftChild().getHeight(), current->rightChild().getHeight()));

    // calculating balance factor up the recursive call stack
    int bf = balanceFactor(current);

    // if the balance factor is lower than -1 and the key is greater than the key of the right child, rotate left
    if(bf < -1 && key > current->rightChild().data.getKey()) {rotateLeft(current);}

    // if the balance factor is higher than 1 and the key is lower than the key of the left child, rotate right
    if(bf > 1 && key < current->leftChild().data.getKey()) {rotateRight(current);}

    // if the balance factor is higher than 1 and the key is greater than the key of the left child, rotate left on left child and then rotate right on current node
    if(bf > 1 && key > current->leftChild().data.getKey()) { rotateRight(current->left); rotateLeft(current);}

    // if the balance factor is lower than -1 and the key is lower than the key of the right child, rotate right on right child and then rotate left on current node
    if(bf < -1 && key < current->rightChild().data.getKey()) { rotateLeft(current->right); rotateRight(current);}
}

/**
 * method for displaying the contents of and AVL tree
 * @tparam V type of value in node
 * uses in-order traversal to showcase the contents of the tree
 */
template <typename V>
void AVL<V>::display() const {
    if (root == nullptr) {std::cout << "AVL tree is empty." << std::endl; return;}
    std::cout << "AVL tree contents: " << std::endl;
    std::function<void(AVLNode<V>*)> inOrder = [&](AVLNode<V>* node) {
        if (node != nullptr) {
            inOrder(node->leftChild());
            std::cout << "Key: " << node->data.getKey() << ", Value: " << node->data.getValue() << std::endl;
            inOrder(node->rightChild());
        }
    };
    inOrder(root);
}

/**
 * method for finding a minimal successor of a node
 * @tparam V type of value in node
 * @param node starting node
 */
template <typename V>
AVLNode<V>* AVL<V>::minValueNode(AVLNode<V>*& node) {
    AVLNode<V>* current = node;
    while (current->left != nullptr) {current = current->left;}
    return current;
}

template <typename V>
int AVL<V>::balanceFactor(AVLNode<V>*& node) {
    if      (!node->leftChild())    { return -(node->rightChild()->getHeight());}
    else if (!node->rightChild())   { return node->leftChild()->getHeight();}
    else                            { return node->leftChild()->getHeight() - node->rightChild()->getHeight();}
}

/**
 * copying operator for AVL tree data structure
 * @tparam V type of value in node
 * @param other tree to be copied
 */
template <typename V>
AVL<V>& AVL<V>::operator=(const AVL<V>& other) {
    if (this != &other) {
        // Clear the current tree
        this->~AVL();

        // Helper function to copy nodes recursively
        std::function<AVLNode<V>*(AVLNode<V>*)> copyNodes = [&](AVLNode<V>* node) -> AVLNode<V>* {
            if (node == nullptr) {return nullptr;}
            AVLNode<V>* newNode = new AVLNode<V>(node->data.getKey());
            newNode->data.setValue(node->data.getValue());
            newNode->setHeight(node->getHeight());
            newNode->left = copyNodes(node->leftChild());
            newNode->right = copyNodes(node->rightChild());
            return newNode;
        };

        // Copy the nodes from the other tree
        root = copyNodes(other.root);
    }
    return *this;
}