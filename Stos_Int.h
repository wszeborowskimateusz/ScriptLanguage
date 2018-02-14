#pragma once
#include "StosElement_Int.h"
#include "Int.h"

class Stos_Int
{
    unsigned int size;
    StosElement_Int* first;
public:
    Stos_Int();
    ~Stos_Int();
    void Push(int data);
    void Push(Int *zmienna);
    int getSize() { return size; }
    int Pop();
    Int*PoP();
    bool isZmienna() { if (first != NULL &&first->zmienna != NULL)return true; else return false; }
    StosElement_Int *getFirst() { return first; }
};