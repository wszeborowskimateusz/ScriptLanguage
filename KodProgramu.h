#pragma once
#define MAX_SIZE 300
#include "Instrukcja.h"
#include "Int.h"

class KodProgramu
{
public:
    Instrukcja *first;
    Instrukcja *last;
    void dodajInstrukcje(char* instrukcja, Int** zmienne, int& ile_zmiennych);
    KodProgramu();
    ~KodProgramu();
};
