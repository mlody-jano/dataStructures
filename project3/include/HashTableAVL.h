#pragma once
#include "Dictionary.h"
#include "AVL.h"

template <typename V>
class HashTableAVL : public Dictionary<V> {

    private:
        AVL<V> *table;
        int hasFunction(const int&) const;

    public:
        HashTableAVL();
        ~HashTableAVL() override = default;

        void insert(const int&, const V&) override;
        void remove(const int&) override;
        void display() const override;
        bool empty() const override {;}
        bool exists(const int&) const override {;}
        int size() const override {;}
        V find(const int&) const override;
        DynamicTable<int> keys() const override;
        DynamicTable<V> values() const override;
};