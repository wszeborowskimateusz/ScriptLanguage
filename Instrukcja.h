#pragma once
#define MAX_SIZE 300

class Instrukcja
{
public:
    //Instrukcja zapisana w ONP
    char*ins;
    Instrukcja *next;
    //Tablica z dlugoscia kazdej liczby w instrukcji
    int liczby[MAX_SIZE];
    //Nazwy wszstkich zmiennych z instrukcji
    char**nazwy_zmiennych;
    int ile_liczb;
    void zmien_Ins(char* newIns);
    Instrukcja();
    ~Instrukcja();
};