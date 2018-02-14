#pragma once
#include "Instrukcja.h"

class WhileIf;
class KodWhileIf
{
public:
    bool isIf;
    bool isWhile;
    Instrukcja* instrukcja;
    //Nested while or If
    WhileIf* whileif;
    KodWhileIf*next;
    KodWhileIf();
    ~KodWhileIf();
};