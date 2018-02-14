#pragma once
#include "Int.h"

class StosElement_Int
{
public:
    int data;
    Int* zmienna;
    StosElement_Int* next;
    StosElement_Int();
    ~StosElement_Int();
};