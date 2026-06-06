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
        Pair<V>     data;
        AVLNode*    left;
        AVLNode*    right;
        int         height;

    public:
        AVLNode(int k) : data(Pair<V>(k)), left(nullptr), right(nullptr), height(1) {}
        int                 getHeight()                                         const {return height;}
        void                setHeight(int newH)                                       {height = newH;}
        AVLNode*            leftChild()                                         const {return left;}
        AVLNode*            rightChild()                                        const {return right;}

    /** declare @class AVL as friend to provide acces to private fields */
    friend class AVL<V>;
};

/**
 * class AVL
 * represents the AVL tree data structure, using @class AVLNode to create a tree of nodes
 * @tparam V type of value in the node, key is set to be integer type
 */
template <typename V>
class AVL {
    private:
        AVLNode<V>* root;
        void                rotateLeft(AVLNode<V>*&);
        void                rotateRight(AVLNode<V>*&);
        AVLNode<V>*         minValueNode(AVLNode<V>*);
    protected:
        int                 balanceFactor(AVLNode<V>*&);
    public:
        AVL();
        AVL(const AVL<V>&);
        ~AVL();
        void                insert(AVLNode<V>*&, const int, const V&);
        void                remove(AVLNode<V>*&, const int);
        void                display()                                           const;
        void                inorderKeys(AVLNode<V>*, DynamicTable<int>&)        const;
        void                inorderValues(AVLNode<V>*, DynamicTable<V>&)        const;
        void                clear(AVLNode<V>*);
        V                   find(AVLNode<V>*, const int)                        const;
        AVLNode<V>*         getRoot()                                           const {return root;}
        AVL<V>&             operator=(const AVL<V>&);
};

/**
 * zero argument constructor of class AVL, sets root to nullptr
 * @tparam V type of value in the node
 */
template <typename V>
AVL<V>::AVL() : root(nullptr) {}

/**
 * copying constructor of class AVL
 * @tparam V type of value in the node
 */
template <typename V>
AVL<V>::AVL(const AVL<V>& other) : root(nullptr) { *this = other;}

/**
 * destructor of class AVL, deletes all nodes in post-order traversal
 * @tparam V type of value in the node
 */
template <typename V>
AVL<V>::~AVL() {
    std::function<void(AVLNode<V>*)> postOrder = [&](AVLNode<V>* node) {
        if (node == nullptr) { return; }
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
    
    // 
    if (current == nullptr) {
        current = new AVLNode<V>(key);
        current->data.setValue(value);
        current->setHeight(1);
        return;
    }

    // recursive search for valid position for new node
    if      (key < current->data.getKey()) {insert(current->left, key, value);}
    else if (key > current->data.getKey()) {insert(current->right, key, value);}

    // set new heights up the recursive call stack
    current->setHeight(1 + std::max(
        current->leftChild()  ? current->leftChild()->getHeight()  : 0,
        current->rightChild() ? current->rightChild()->getHeight() : 0
    ));

    // calculating balance factor up the recursive call stack
    int bf = balanceFactor(current);

    // if the balance factor is lower than -1 and the key is greater than the key of the right child, rotate left
    if      (bf < -1 && key > current->rightChild()->data.getKey()) {rotateLeft(current);}

    // if the balance factor is higher than 1 and the key is lower than the key of the left child, rotate right
    else if (bf > 1 && key < current->leftChild()->data.getKey()) {rotateRight(current);}

    // if the balance factor is higher than 1 and the key is greater than the key of the left child, rotate left on left child and then rotate right on current node
    else if (bf > 1 && key > current->leftChild()->data.getKey()) { rotateLeft(current->left); rotateRight(current);}

    // if the balance factor is lower than -1 and the key is lower than the key of the right child, rotate right on right child and then rotate left on current node
    else if (bf < -1 && key < current->rightChild()->data.getKey()) { rotateRight(current->right); rotateLeft(current);}
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
    
    if(current == nullptr) { return; }

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
            remove(current->right, temp->data.getKey());
        }
    }
    if(current == nullptr) { return; }

    // set new heights up the recursive call stack
    current->setHeight(1 + std::max(
        current->leftChild()  ? current->leftChild()->getHeight()  : 0,
        current->rightChild() ? current->rightChild()->getHeight() : 0
    ));

    // calculating balance factor up the recursive call stack
    int bf = balanceFactor(current);

    // if the balance factor is lower than -1 and the key is greater than the key of the right child, rotate left
    if      (bf < -1 && key > current->rightChild()->data.getKey()) {rotateLeft(current);}

    // if the balance factor is higher than 1 and the key is lower than the key of the left child, rotate right
    else if (bf > 1 && key < current->leftChild()->data.getKey()) {rotateRight(current);}

    // if the balance factor is higher than 1 and the key is greater than the key of the left child, rotate left on left child and then rotate right on current node
    else if (bf > 1 && key > current->leftChild()->data.getKey()) { rotateLeft(current->left); rotateRight(current);}

    // if the balance factor is lower than -1 and the key is lower than the key of the right child, rotate right on right child and then rotate left on current node
    else if (bf < -1 && key < current->rightChild()->data.getKey()) { rotateRight(current->right); rotateLeft(current);}
}

/**
 * method for displaying the contents of and AVL tree
 * @tparam V type of value in node
 * uses in-order traversal to showcase the contents of the tree
 */
template <typename V>
void AVL<V>::display() const {
    if (root == nullptr) { std::cout << "AVL tree is empty." << std::endl; return; }
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
AVLNode<V>* AVL<V>::minValueNode(AVLNode<V>* node) {
    AVLNode<V>* current = node;
    while (current->left != nullptr) { current = current->left; }
    return current;
}

/**
 * method used to calculate the balance factor of specific node
 * @tparam V type of value in node
 * @param node target node
 */
template <typename V>
int AVL<V>::balanceFactor(AVLNode<V>*& node) {
    if      (!node->leftChild())    { return -(node->rightChild()->getHeight()); }
    else if (!node->rightChild())   { return node->leftChild()->getHeight(); }
    else                            { return node->leftChild()->getHeight() - node->rightChild()->getHeight(); }
}

/**
 * private method used to recover the AVL tree property by shifting elements, so that the height of two subsequent subtrees differs by at most <-1,1>
 * @tparam V type of value in node
 * @param node node to perform rotation on
 */
template <typename V>
void AVL<V>::rotateLeft(AVLNode<V>*& node) {
    AVLNode<V>* rightChild = node->right;
    AVLNode<V>* rightLeftChild = rightChild->left;

    rightChild->left = node;
    node->right = rightLeftChild;

    // Aktualizacja wysokości — najpierw węzeł niżej
    node->setHeight(1 + std::max(
        node->left  ? node->left->getHeight()  : 0,
        node->right ? node->right->getHeight() : 0
    ));
    rightChild->setHeight(1 + std::max(
        rightChild->left  ? rightChild->left->getHeight()  : 0,
        rightChild->right ? rightChild->right->getHeight() : 0
    ));

    node = rightChild; // aktualizacja wskaźnika przez referencję
}

/**
 * private method used to recover the AVL tree property by shifting elements, so that the height of two subsequent subtrees differs by at most <-1,1>
 * @tparam V type of value in node
 * @param node node to perform rotation on
 */
template <typename V>
void AVL<V>::rotateRight(AVLNode<V>*& node) {
    AVLNode<V>* leftChild = node->left;
    AVLNode<V>* leftRightChild = leftChild->right;

    leftChild->right = node;
    node->left = leftRightChild;

    node->setHeight(1 + std::max(
        node->left  ? node->left->getHeight()  : 0,
        node->right ? node->right->getHeight() : 0
    ));
    leftChild->setHeight(1 + std::max(
        leftChild->left  ? leftChild->left->getHeight()  : 0,
        leftChild->right ? leftChild->right->getHeight() : 0
    ));

    node = leftChild;
}

/**
 * copying operator for AVL tree data structure
 * @tparam V type of value in node
 * @param other tree to be copied
 */
template <typename V>
AVL<V>& AVL<V>::operator=(const AVL<V>& other) {
    if (this != &other) {
        
        clear(root);
        root = nullptr;

        // helper function to copy nodes recursively
        std::function<AVLNode<V>*(AVLNode<V>*)> copyNodes = [&](AVLNode<V>* node) -> AVLNode<V>* {
            if (node == nullptr) { return nullptr; }
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

/**
 * method for clearing any nodes left in an AVL tree
 * @tparam V type of value in node
 * @param node to be cleared
 */
template <typename V>
void AVL<V>::clear(AVLNode<V>* node) {
    if (!node) { return; }
    clear(node->left);
    clear(node->right);
    delete node;
}

/**
 * method for returning a list of keys available in AVL tree
 * @tparam V
 * @param node starting node
 * @param keyList reference to result list
 */
template <typename V>
void AVL<V>::inorderKeys(AVLNode<V>* node, DynamicTable<int>& keyList) const {
    if (!node) { return; }
    inorderKeys(node->left,  keyList);
    keyList.pushBack(node->data.getKey());
    inorderKeys(node->right, keyList);
}

/**
 * method for returning a list of values available in AVL tree
 * @tparam V
 * @param node starting node
 * @param vList reference to result list
 */
template <typename V>
void AVL<V>::inorderValues(AVLNode<V>* node, DynamicTable<V>& vList) const {
    if (!node) { return; }
    inorderValues(node->left,  vList);
    vList.pushBack(node->data.getValue());
    inorderValues(node->right, vList);
}

/**
 * method for finding a value tied to a key passed along as argument
 * @tparam V type of value in node
 * @param node starting node
 * @param key key of pair to be found
 */
template <typename V>
V AVL<V>::find(AVLNode<V>* node, const int key) const {
    if (node == nullptr) { throw std::out_of_range("Key not found"); }
    
    if      (key < node->data.getKey()) return find(node->left,  key);
    else if (key > node->data.getKey()) return find(node->right, key);
    else                                return node->data.getValue();
}