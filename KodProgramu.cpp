#include "KodProgramu.h"
#include <iostream>



using namespace std;





void KodProgramu::dodajInstrukcje(char * instrukcja, Int** zmienne, int &ile_zmiennych)
{
    Instrukcja* instr = new Instrukcja;
    instr->next = nullptr;

    for (int i = 0; i<MAX_SIZE; i++)

        instr->ins[i] = instrukcja[i];

    if (last != nullptr)
    {
        last->next = instr;
        last = instr;
    }

    else { first = instr; last = instr; }



    //Zliczanie ile poszczegolne cyfry maja znakow
    int z = 0, x = 0;
    bool zmiana = false;

    while (instrukcja[z] != '\0')
    {
        if (instrukcja[z] >= '0' && instrukcja[z] <= '9')
        {
            instr->liczby[x]++;
            zmiana = true;
        }

        else if (zmiana)
        {
            x++;
            zmiana = false;
        }
        z++;
    }



    int iterator = 0;
    int y = 0, y2 = 0;
    bool zmiana_zmienne = false;

    //Zapisywanie aktualnie uzywanych zmiennych w tablicy

    while (instrukcja[iterator] != '\0')
    {
        if ((instrukcja[iterator] >= 'a'&&instrukcja[iterator] <= 'z') || (instrukcja[iterator] >= 'A'&&instrukcja[iterator] <= 'Z'))
        {
            instr->nazwy_zmiennych[y][y2] = instrukcja[iterator];
            y2++;
            zmiana_zmienne = true;
        }

        else if (zmiana_zmienne)
        {
            instr->nazwy_zmiennych[y][y2] = '\0';
            y++;
            y2 = 0;
            zmiana_zmienne = false;
        }
        iterator++;
    }
    instr->nazwy_zmiennych[y][y2] = '\0';



    //Dodanie ewentualnych nowych zmiennych do listy zmiennych - liczby

    int dlugosci_nazw_zmiennych[MAX_SIZE] = {};
    int f = 0;

    for (int i = 0; i < MAX_SIZE; i++)
    {
        f = 0;
        while (instr->nazwy_zmiennych[i][f] != '\0')
        {
            dlugosci_nazw_zmiennych[i]++;
            f++;
        }
    }
    int i = 0;
    x = 0;
    bool czyIstniejeZmienna = false;
    bool identyczne = true;

    while (*(instr->nazwy_zmiennych[i]) != '\0')
    {
        czyIstniejeZmienna = false;
        identyczne = true;

        for (int j = 0; j < ile_zmiennych; j++)
        {

            identyczne = true;

            if (zmienne[j]->dlugosc_zmiennej != dlugosci_nazw_zmiennych[i])continue;

            while (zmienne[j]->nazwa[x] != '\0' && instr->nazwy_zmiennych[i][x] != '\0')
            {
                if (instr->nazwy_zmiennych[i][x] != zmienne[j]->nazwa[x]) { identyczne = false; break; }
                x++;
            }
            if (identyczne) { czyIstniejeZmienna = true; break; }
        }

        if (!czyIstniejeZmienna)
        {
            zmienne[ile_zmiennych++] = new Int(instr->nazwy_zmiennych[i]);
        }
        i++;
        x = 0;

    }

    //Zliczanie ilosci liczb
    for (int i = 0; i < MAX_SIZE; i++)
    {
        if (instr->liczby[i] == 0)break;
        instr->ile_liczb++;
    }

}



KodProgramu::KodProgramu()
{
    first = nullptr;
    last = nullptr;
}





KodProgramu::~KodProgramu()
{
}