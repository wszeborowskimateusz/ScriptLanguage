#pragma once
#include "StosElement.h"


class Stos
{
    unsigned int size;
    StosElement* first;

public:
    Stos();
    ~Stos();
    void Push(char data);
    int getSize() { return size; }
    int getFirstData() { if (first != nullptr) return first->data; else return '1'; }
    char Pop();
    StosElement *getFirst() { return first; }
};