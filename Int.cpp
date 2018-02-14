#include "Int.h"







Int::Int(char* zn)
{

    nazwa = new char[MAX_SIZE];
    dlugosc_zmiennej = 0;

    int i = 0;
    while (zn[i] != '\0')
    {
        dlugosc_zmiennej++;
        nazwa[i] = zn[i];
        i++;
    }
    nazwa[i] = '\0';
    isNull = true;

}





Int::~Int()

{
    delete[] nazwa;
}



ostream & operator<<(ostream & wyjscie, const Int & s)
{

    if (s.isNull)wyjscie << "Nul";
    else wyjscie << s.wartosc;
    return wyjscie;

}