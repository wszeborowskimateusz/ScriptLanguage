#pragma once
#include "Stos_Int.h"
#include <iostream>

using namespace std;


Stos_Int::Stos_Int()
{
    first = NULL;
    size = 0;
}





Stos_Int::~Stos_Int()
{
}





void Stos_Int::Push(int data)
{
    StosElement_Int* element = new StosElement_Int;
    element->zmienna = NULL;
    element->next = first;
    element->data = data;
    first = element;
    size++;
}



void Stos_Int::Push(Int* zmienna)
{
    StosElement_Int* element = new StosElement_Int;
    element->next = first;
    element->zmienna = zmienna;
    first = element;
    size++;
}





int Stos_Int::Pop()
{
    if (first != NULL)
    {
        int data = first->data;
        first = first->next;
        size--;
        return data;
    }
    return -1;
}



Int * Stos_Int::PoP()
{
    if (first != NULL)
    {
        Int* zmienna = first->zmienna;
        first = first->next;
        size--;
        return zmienna;
    }
    return NULL;
}