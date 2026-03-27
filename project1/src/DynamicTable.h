#ifndef DYNAMICTABLE_H
#define DYNAMICTABLE_H

#pragma once

class DynamicTable
{
public:
    DynamicTable();
    ~DynamicTable();

private:
    int *table;
    int capacity;
    int size;
};

#endif