#pragma once
#define MAX_SIZE 300
#include "KodWhileIf.h"
#include "Int.h"


class WhileIf
{
public:
    char*warunek;
    KodWhileIf* first;
    KodWhileIf* last;
    char* getWarunek() { return warunek; }
    void setWarunek(char* war);
    void addWhile();
    void addIf();
    void addIns(char * instrukcja, Int** zmienne, int &ile_zmiennych);
    WhileIf();
    ~WhileIf();
};