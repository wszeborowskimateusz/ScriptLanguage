#include "Instrukcja.h"







void Instrukcja::zmien_Ins(char * newIns)
{

    for (int i = 0; i < MAX_SIZE; i++)
    {
        ins[i] = newIns[i];
        if (newIns[i] == '\0')break;
    }
}



Instrukcja::Instrukcja()
{

    ins = new char[MAX_SIZE];
    nazwy_zmiennych = new char*[MAX_SIZE];

    for (int i = 0; i < MAX_SIZE; i++)

    {
        ins[i] = '\0';
        liczby[i] = 0;
        nazwy_zmiennych[i] = new char[MAX_SIZE];

        for (int j = 0; j < MAX_SIZE; j++)
        {
            nazwy_zmiennych[i][j] = '\0';
        }
    }
    ile_liczb = 0;
}





Instrukcja::~Instrukcja()
{
    for (int i = 0; i < MAX_SIZE; i++)delete[]nazwy_zmiennych[i];
    delete[]nazwy_zmiennych;
    delete[]ins;
}
