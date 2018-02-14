#pragma once
#include "Stos.h"
#include <iostream>

using namespace std;


Stos::Stos()
{
    first = NULL;
    size = 0;
}



Stos::~Stos()
{
}


void Stos::Push(char data)
{
    StosElement* element = new StosElement;
    element->next = first;
    element->data = data;
    first = element;
    size++;
}





char Stos::Pop()
{
    if (first != NULL)
    {
        char data = first->data;
        first = first->next;
        size--;
        return data;
    }
    return ' ';
}