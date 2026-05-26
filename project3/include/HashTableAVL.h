#pragma once
#include "Dictionary.h"

template <typename K, typename V>
class HashTableAVL : public Dictionary<K, V> {

    private:
        int hasFunction(const K&) const;

    public:
        HashTableAVL();
        ~HashTableAVL() override = default;

        void insert(const K&, const V&) override;
        void remove(const K&) override;
        void display() const override;
        bool empty() const override {;}
        bool exists(const K&) const override {;}
        int size() const override {;}
        V find(const K&) const override;
        DynamicTable<K> keys() const override;
        DynamicTable<V> values() const override;
};