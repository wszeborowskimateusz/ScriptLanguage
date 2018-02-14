#pragma once
#include <iostream>
#define MAX_SIZE 300

using namespace std;

class Int
{
public:
    int dlugosc_zmiennej;
    int wartosc;
    char*nazwa;
    //char nazwa[1001];
    bool isNull;
    Int(char *zn);
    ~Int();
    friend ostream & operator<< (ostream &wyjscie, const Int &s);

};