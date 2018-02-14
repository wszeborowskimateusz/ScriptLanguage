#define MAX_SIZE 300
#include "Int.h"
#include "Stos.h"
#include "StosElement.h"
#include "Stos_Int.h"
#include "StosElement_Int.h"
#include "Instrukcja.h"
#include "KodProgramu.h"
#include "WhileIf.h"
#include "KodWhileIf.h"
#include <assert.h>
#include <iostream>
#include <math.h>

using namespace std;

void Wczytaj(KodProgramu&pr, Int** zmienne, int& ile_zmiennych, int &licznik_operacji, int max_operacji);
void If(Int**zmienne, int& licznik_operacji, int max_operacji, int &ile_zmiennych, char kod);
void petla(Int**zmienne, int& licznik_operacji, int max_operacji, int &ile_zmiennych, char kod);
char*  wczytajWarunek(char &kod);

//Funkcja zwraca priorytet operatora od najmniejszego 1 do najewiększego 8
//Funkcja zwaraca -1 gdy operator nie znajduje się na liście operatorów lub gdy podano niewłaściwą wartość operatora
int getPriority(char op)
{
    if (op == '=')return 1;
    else if (op == '|')return 2;
    else if (op == '&')return 3;
    else if (op == '~' || op == '`')return 4;
    else if (op == '<' || op == '>' || op == '^' || op == '$')return 5;
    else if (op == '+' || op == '-')return 6;
    else if (op == '/' || op == '*' || op == '%')return 7;
    else if (op == '!' || op == '#')return 8;
    else return -1;
}

bool isWhiteSign(char zn)
{
    if (zn == ' ' || zn == '\n' || zn == '\t')return true;
    else return false;
}

bool isOperand(char zn)
{
    if (zn == '=' || zn == '|' || zn == '&' || /*zn == "!=" || zn == "==" ||zn==">="||zn=="<="*/ zn == '<' || zn == '>' || zn == '+' || zn == '-' || zn == '/'
        || zn == '*' || zn == '%' || zn == '!' || zn == '(' || zn == ')' || zn == '{' || zn == '}' || zn == '#' || zn == '~' || zn == '^' || zn == '$' || isWhiteSign(zn) || zn == '`')return false;
    else return true;
}

int Calculate(int op1, int op2, char token, Int* zmienna1, Int* zmienna2, bool& isNul)
{
    int result = 0;

    if (zmienna1 != NULL && zmienna2 != NULL && token == '=' && zmienna2->isNull)isNul = true;
    if (zmienna1 != NULL /*&& token != '='*/)op1 = zmienna1->wartosc;
    if (zmienna2 != NULL /*&& token != '='*/)op2 = zmienna2->wartosc;
    if (((zmienna2 != NULL&&zmienna2->isNull) || (zmienna1 != NULL&&zmienna1->isNull)) && token != '=' && token != '!')
    {
        isNul = true;
    }

    switch (token)
    {
        case '|':
            if ((zmienna1 != nullptr && zmienna1->isNull) && (zmienna2 != nullptr && zmienna2->isNull))
            {
                isNul = true;
                result = 1;
            }
            else if (zmienna1 == nullptr&&zmienna2 == nullptr && ((op1 != 0 && op1 != 1) || (op2 != 0 && op2 != 1)))result = 0;
            else if (op1 == 0 || op2 == 0)result = 0;
            else result = 1;
            break;
        case '&':
            result = -1;
            if ((zmienna1 != nullptr && zmienna1->isNull) || (zmienna2 != nullptr && zmienna2->isNull))
                isNul = true;
            else if ((zmienna1 != nullptr && !zmienna1->isNull &&zmienna2->wartosc == 0) && (zmienna2 != nullptr && !zmienna2->isNull&&zmienna1->wartosc == 0))result = 0;
            else if (op1 == 0 && op2 == 0)result = 0;
            else result = 1;
            break;
        case '$':
            if (op1 <= op2)result = 0;
            else result = 1;
            break;
        case '>':
            if (op1 > op2)result = 0;
            else result = 1;
            break;
        case '<':
            if (op1 < op2)result = 0;
            else result = 1;
            break;
        case '^':
            if (op1 >= op2)result = 0;
            else result = 1;
            break;
        case '+':result = op1 + op2; break;
        case '-':result = op1 - op2; break;
        case '/':if (op2 != 0)result = op1 / op2; else isNul = true; break;
        case '*':result = op1 * op2; break;
        case '%':result = abs(op1 % op2); break;
        case '!':
            if (zmienna1 != nullptr && !zmienna1->isNull)
            {
                isNul = true;
                result = 1;
            }
            else if (zmienna1 != nullptr && zmienna1->isNull)
                result = 0;
            else
                result = !op1;
            break;
        case'#':result = -op1; break;
        case'~':if (op1 == op2)result = 0; else result = 1; break;
        case '`':if (op1 != op2)result = 0; else result = 1; break;
        case '=':
        {
            if (zmienna1 != NULL)
            {
                zmienna1->wartosc = op2;
                if (!isNul)
                    zmienna1->isNull = false;
                else zmienna1->isNull = true;
                result = op2;
            }
        }break;
        default:result = 0; break;
    }
    return result;
}

//Funkcja konwertuje otrzymane wyrazenie na Odwrotną Notację Polską #-znak negacji (jednoargumentowy) ~-== ` - != $ <= ^ >=
char* ONPConv(char* linia_programu2)
{
    Stos stos;
    char* onp = new char[MAX_SIZE];
    char* linia_programu = new char[MAX_SIZE];
    int i = 0, j = 0;
    if (linia_programu[0] == '-')linia_programu[0] = '#';
    int x = 0, z = 0;
    char zn = ' ', zn_before = ' ';

    while (linia_programu2[x] != '\0')
    {
        if (!isWhiteSign(linia_programu2[x]))linia_programu[j++] = linia_programu2[x];
        x++;
    }

    linia_programu[j] = '\0';
    x = 0;

    while (linia_programu[x] != '\0')
    {
        zn = linia_programu[x];
        zn_before = linia_programu[x - 1];
        if (zn == '-' && !isOperand(zn_before) && zn_before != ')')linia_programu[x] = '#';
        if (zn == '='&&zn_before == '=') { linia_programu[x] = '~'; linia_programu[x - 1] = ' '; }
        if (zn == '='&&zn_before == '!') { linia_programu[x] = '`'; linia_programu[x - 1] = ' '; }
        if (zn == '='&&zn_before == '<') { linia_programu[x] = '$'; linia_programu[x - 1] = ' '; }
        if (zn == '='&&zn_before == '>') { linia_programu[x] = '^'; linia_programu[x - 1] = ' '; }
        x++;
    }

    j = 0; i = 0;

    while (linia_programu[i] != '\0')
    {
        char token = linia_programu[i];
        if (isWhiteSign(token)) { i++; continue; }
        if (isOperand(token)) { onp[j] = token; j++; }
        else if (token == '(')stos.Push(token);
        else if (token == ')')
        {
            StosElement*tmp = stos.getFirst();

            while (tmp != NULL)
            {
                token = stos.Pop();
                if (token == '(')break;
                else
                {
                    onp[j] = token;
                    j++;
                }
                tmp = tmp->next;
            }
        }
        else
        {
            bool lewostronnie_laczny = true;
            if (token == '=')lewostronnie_laczny = false;
            int priority = getPriority(token);
            StosElement*tmp = stos.getFirst();
            char top = ' ';
            if (stos.getSize() != 0)
            {
                top = stos.getFirstData();
                char zn = ' ';
                while ((lewostronnie_laczny&&getPriority(top) >= priority) || (!lewostronnie_laczny&&getPriority(top) > priority))
                {
                    if (stos.getSize() == 0)break;
                    zn = stos.Pop();
                    onp[j] = zn; j++;
                    top = stos.getFirstData();
                }
            }
            stos.Push(token);
        }
        i++;
    }

    StosElement*tmp2 = stos.getFirst();

    while (tmp2 != NULL)
    {
        char item = stos.Pop();
        onp[j] = item;
        j++;
        tmp2 = tmp2->next;
    }
    onp[j] = '\0';
    //delete[]linia_programu;
    return onp;
}


//Funkcja oblicza wartos wyrażenie linia_programu w postaci ONP
void ONPCalc(char* linia_programu_onp, Int**zmienne, int* liczby, int ile_liczb, int& licznik_operacji, int max_operacji, int ile_zmiennych, char**nazwy_zmiennych
        , Int* wynik1 = NULL, Int* wynik2 = NULL)
{
    if (licznik_operacji >= max_operacji)return;
    //Przenoszenie sie nula w wyrazeniu
    bool isNul = false;
    int ktora_zmienna = 0;
    Stos_Int result;
    int j = 0, ktora_liczba = 0;
    while (linia_programu_onp[j] != '\0')
    {
        char token = linia_programu_onp[j];
        if (isOperand(token))
        {
            //Zamiana charów na inta, wartości liczbowe (nie zmienne)
            if (token >= '0'&&token <= '9')
            {
                int liczba_liter = liczby[ktora_liczba];
                int operand = 0;
                if (liczba_liter == 1)operand = (int)token - 48;
                else operand = pow(10.0, (int)((liczba_liter - 1)))*((int)token - 48);
                while (liczba_liter > 1)
                {
                    j++;
                    token = linia_programu_onp[j];
                    liczba_liter--;
                    operand += pow(10.0, (int)(liczba_liter - 1))*((int)token - 48);
                }
                ktora_liczba++;
                result.Push(operand);
            }
                //Zmienne
            else if (!isWhiteSign(token))
            {
                int dlugosc = 0, dlugosc_stala = 0;
                int ktora_cyfra = 0;
                //Wyznaczanie dlugosci zmiennej (ilosc znakow)
                while (nazwy_zmiennych[ktora_zmienna][ktora_cyfra] != '\0') { dlugosc++; ktora_cyfra++; }
                //Dlugosc nazwy zmiennej
                dlugosc_stala = dlugosc;
                //Wczytywanie znakow skladajacych sie na zmienna
                while (dlugosc>1)
                {
                    token = linia_programu_onp[++j];
                    dlugosc--;
                }
                int j = 0;
                bool sukces = true;
                //Wyszukiwanie zmiennej w tablicy
                for (int i = 0; i < ile_zmiennych; i++)
                {
                    j = 0;
                    sukces = true;
                    if (zmienne[i]->dlugosc_zmiennej != dlugosc_stala) { sukces = false; continue; }
                    while (nazwy_zmiennych[ktora_zmienna][j] != '\0' || zmienne[i]->nazwa[j] != '\0')
                    {
                        if (zmienne[i]->nazwa[j] != nazwy_zmiennych[ktora_zmienna][j])sukces = false;
                        j++;
                    }
                    if (sukces) { result.Push(zmienne[i]); break; }
                }
                ktora_zmienna++;
            }
        }
        else if (!isWhiteSign(token))
        {
            if (token != '!' && token != '#')
            {
                int operand1 = 0, operand2 = 0;
                Int *zmienna1 = NULL, *zmienna2 = NULL;
                if (result.isZmienna())zmienna1 = result.PoP();
                else operand1 = result.Pop();
                if (result.isZmienna())zmienna2 = result.PoP();
                else operand2 = result.Pop();
                result.Push(Calculate(operand2, operand1, token, zmienna2, zmienna1, isNul));
            }
            else
            {
                int operand = 0;
                Int* zmienna1 = NULL;
                if (result.isZmienna())
                {
                    zmienna1 = result.PoP();
                }
                else
                {
                    operand = result.Pop();
                    // minus przed liczba nie jest unarny
                    if (token == '#')
                    {
                        licznik_operacji--;
                    }
                }
                result.Push(Calculate(operand, 0, token, zmienna1, NULL, isNul));
            }
            if (licznik_operacji >= max_operacji)break;
            licznik_operacji++;
            if (licznik_operacji >= max_operacji)break;
        }
        j++;
    }
    if (!(wynik1 == NULL&&wynik2 == NULL))
    {
        int wynik = 0;
        bool isNul = false;
        Int*wynik3 = NULL;
        if (result.isZmienna()) { wynik3 = result.PoP(); wynik = wynik3->wartosc; isNul = wynik3->isNull; }
        else wynik = result.Pop();

        if (wynik1 != NULL) { wynik1->wartosc = wynik; wynik1->isNull = isNul; }
        if (wynik2 != NULL) { wynik2->wartosc = wynik; wynik2->isNull = isNul; }
    }
}

//Funkcja zwraca 0, gdy warunek jest spełniony oraz 1 gdy niespełniony
int checkWarunek(char *warunek, Int** zmienne, int&ile_zmiennych, int&licznik_operacji, int max_operacji)
{
    KodProgramu war;

    war.dodajInstrukcje(warunek, zmienne, ile_zmiennych);
    Int *wynik1 = new Int("");
    wynik1->isNull = true;
    char* warunek_ONP = new char[MAX_SIZE];
    warunek_ONP = ONPConv(warunek);
    bool isThereOperator = false;
    int iter = 0;
    while (warunek_ONP[iter] != '\0')
    {
        if (!isOperand(warunek_ONP[iter]))
        {
            isThereOperator = true; break;
        }
        iter++;
    }
    war.last->zmien_Ins(warunek_ONP);
    ONPCalc(war.last->ins, zmienne, war.last->liczby, war.last->ile_liczb, licznik_operacji, max_operacji, ile_zmiennych, war.last->nazwy_zmiennych, wynik1);
    if (!isThereOperator)
    {
        if (wynik1->isNull)return 1;
        else return 0;
    }
    if (!wynik1->isNull && wynik1->wartosc == 0)return 0;
    else return 1;
}

void wczytajKod(WhileIf &whorif, char &kod, Int** zmienne, int &ile_zmiennych, int max_operacji, int& licznik_operacji, bool zapis = true)
{
    bool koniec_instruckji = false;
    bool isWhile = false, isIf = false;
    char kod_before = '+';
    char pomoc = '+';
    char*obecna_linia = new char[MAX_SIZE];
    for (int i = 0; i < MAX_SIZE; i++) { obecna_linia[i] = '\0'; }
    int i = 0;
    while (kod != '{')kod = getchar();
    kod = getchar();
    i = 0;
    //Wczytywanie instrukcji wewnatrz petli
    if (zapis)
    {
        while (kod != '}')
        {
            if (licznik_operacji >= max_operacji)return;
            if (kod == EOF)break;
            if (isWhile || kod == '@')
            {
                whorif.addWhile();
                whorif.last->whileif->setWarunek(wczytajWarunek(kod));
                wczytajKod(*(whorif.last->whileif), kod, zmienne, ile_zmiennych, max_operacji, licznik_operacji);
                isWhile = false;
                i = 0;
            }
            else if (isIf || kod == '?')
            {
                whorif.addIf();
                whorif.last->whileif->setWarunek(wczytajWarunek(kod));
                wczytajKod(*(whorif.last->whileif), kod, zmienne, ile_zmiennych, max_operacji, licznik_operacji);
                isIf = false;
                i = 0;
            }
            else if (isOperand(kod_before))
            {
                if (isWhiteSign(kod))
                {
                    while (isWhiteSign(kod)) { kod = getchar(); }
                    if (kod == '}' || kod == EOF)break;
                    if (isOperand(kod) || kod == '@' || kod == '?' || kod == '(') { koniec_instruckji = true; pomoc = kod; if (kod == '@')isWhile = true; else if (kod == '?')isIf = true; }
                    else obecna_linia[i] = kod;
                }
                else if (kod == ')')
                {
                    kod = getchar();
                    while (isWhiteSign(kod))
                    {
                        kod = getchar();
                    }
                    if (kod == '}' || kod == EOF)break;
                    if (isOperand(kod) || kod == '@' || kod == '?') {
                        koniec_instruckji = true; pomoc = kod; obecna_linia[i] = ')'; if (kod == '@')isWhile = true; else if (kod == '?')isIf = true;
                    }
                    else obecna_linia[i] = kod;
                }
                else { obecna_linia[i] = kod; }
            }
            else  obecna_linia[i] = kod;
            i++;
            //Koniec jednej instrukcji
            if (koniec_instruckji)
            {
                obecna_linia[i] = '\0';
                char*linia_programu_ONP = new char[MAX_SIZE];
                linia_programu_ONP = ONPConv(obecna_linia);
                whorif.addIns(obecna_linia, zmienne, ile_zmiennych);
                whorif.last->instrukcja->zmien_Ins(linia_programu_ONP);
                i = 0;
                koniec_instruckji = false;
                for (int i = 0; i < MAX_SIZE; i++)obecna_linia[i] = '\0';
                obecna_linia[i++] = pomoc;
            }
            kod_before = kod;
            kod = getchar();
        }
        obecna_linia[i] = '\0';
        char*linia_programu_ONP = new char[MAX_SIZE];
        linia_programu_ONP = ONPConv(obecna_linia);
        whorif.addIns(obecna_linia, zmienne, ile_zmiennych);
        whorif.last->instrukcja->zmien_Ins(linia_programu_ONP);
        //delete[]linia_programu_ONP;
        delete[]obecna_linia;
    }
    else
    {
        int ile_nawiasow = 0;
        while (kod != '}' && kod != EOF)
        {
            if (kod == '{')ile_nawiasow++;
            kod = getchar();
            if (kod == '}' && ile_nawiasow != 0)
            {
                ile_nawiasow--;
                kod = getchar();
            }
        }
    }
}

//Wczytywanie warunku
char*  wczytajWarunek(char &kod)
{
    char* warunek = new char[MAX_SIZE];
    for (int i = 0; i < MAX_SIZE; i++) { warunek[i] = '\0'; }
    int i = 0;
    while (kod != '(')kod = getchar();
    kod = getchar();
    int ile_nawiasow = 0;
    //Wczytywanie warunku
    while (kod != ')')
    {
        if (isWhiteSign(kod)) { kod = getchar(); continue; }
        if (kod == '(')ile_nawiasow++;

        warunek[i++] = kod;
        kod = getchar();
        if (kod == ')' && ile_nawiasow != 0)
        {
            warunek[i++] = kod;
            kod = getchar();
        }
    }

    return warunek;
}

void wykonajWhileIf(WhileIf& begin, Int**zmienne, int& licznik_operacji, int max_operacji, int &ile_zmiennych, char kod)
{

    KodWhileIf* tmp = begin.first;
    while (tmp != nullptr)
    {
        if (tmp->isIf)
        {
            if (licznik_operacji >= max_operacji)return;
            licznik_operacji++;
            if (licznik_operacji >= max_operacji)return;

            if (checkWarunek(tmp->whileif->warunek, zmienne, ile_zmiennych, licznik_operacji, max_operacji) == 0)
            {
                wykonajWhileIf(*tmp->whileif, zmienne, licznik_operacji, max_operacji, ile_zmiennych, kod);
            }
        }
        else if (tmp->isWhile)
        {
            if (licznik_operacji >= max_operacji)return;
            licznik_operacji++;
            if (licznik_operacji >= max_operacji)return;

            while (checkWarunek(tmp->whileif->warunek, zmienne, ile_zmiennych, licznik_operacji, max_operacji) == 0)
            {
                if (licznik_operacji >= max_operacji)return;
                licznik_operacji++;
                if (licznik_operacji >= max_operacji)return;

                wykonajWhileIf(*tmp->whileif, zmienne, licznik_operacji, max_operacji, ile_zmiennych, kod);
            }
        }
        else
        {
            ONPCalc(tmp->instrukcja->ins, zmienne, tmp->instrukcja->liczby, tmp->instrukcja->ile_liczb, licznik_operacji, max_operacji, ile_zmiennych, tmp->instrukcja->nazwy_zmiennych);
        }
        tmp = tmp->next;
    }
}

//while  @(test){wyrazenia}
void petla(Int**zmienne, int& licznik_operacji, int max_operacji, int &ile_zmiennych, char kod)
{
    if (licznik_operacji >= max_operacji)return;
    licznik_operacji++;
    if (licznik_operacji >= max_operacji)return;

    WhileIf While;
    char*warunek = new char[MAX_SIZE];
    for (int i = 0; i < MAX_SIZE; i++)warunek[i] = '\0';
    warunek = wczytajWarunek(kod);
    While.setWarunek(warunek);

    int j = 0;
    bool warunek_w = false;

    while (checkWarunek(warunek, zmienne, ile_zmiennych, licznik_operacji, max_operacji) == 0)
    {
        warunek_w = true;
        if (licznik_operacji >= max_operacji)break;
        if (j == 0)
            wczytajKod(While, kod, zmienne, ile_zmiennych, max_operacji, licznik_operacji);
        j++;

        wykonajWhileIf(While, zmienne, licznik_operacji, max_operacji, ile_zmiennych, kod);

        if (licznik_operacji >= max_operacji)break;
        licznik_operacji++;
        if (licznik_operacji >= max_operacji)break;
    }
    if (!warunek_w)wczytajKod(While, kod, zmienne, ile_zmiennych, max_operacji, licznik_operacji, false);
    delete[]warunek;
}

//If  ?(test){wyrazenia}
void If(Int**zmienne, int& licznik_operacji, int max_operacji, int &ile_zmiennych, char kod)
{
    if (licznik_operacji >= max_operacji)return;
    licznik_operacji++;
    if (licznik_operacji >= max_operacji)return;

    WhileIf If;
    char*warunek = new char[MAX_SIZE];
    warunek = wczytajWarunek(kod);
    If.setWarunek(warunek);

    if (checkWarunek(warunek, zmienne, ile_zmiennych, licznik_operacji, max_operacji) == 0)
    {
        wczytajKod(If, kod, zmienne, ile_zmiennych, max_operacji, licznik_operacji);
        wykonajWhileIf(If, zmienne, licznik_operacji, max_operacji, ile_zmiennych, kod);
    }
    else
        wczytajKod(If, kod, zmienne, ile_zmiennych, max_operacji, licznik_operacji, false);

    delete[]warunek;
}

//Wczytywanie linii programu
void Wczytaj(KodProgramu&pr, Int** zmienne, int& ile_zmiennych, int &licznik_operacji, int max_operacji)
{
    int i = 0;
    char pomoc = '_';
    bool koniec_instruckji = false;
    bool isWhile = false, isIf = false;
    char*obecna_linia = new char[MAX_SIZE];
    for (int i = 0; i < MAX_SIZE; i++)obecna_linia[i] = '\0';
    char kod = getchar();
    char kod_before = '+';
    while (kod != EOF)
    {
        if(kod=='g')break;
        if (i >= MAX_SIZE)break;
        if (licznik_operacji >= max_operacji)return;
        if (isWhile || kod == '@')
        {
            isWhile = false;
            petla(zmienne, licznik_operacji, max_operacji, ile_zmiennych, kod);
        }
        else if (isIf || kod == '?')
        {
            isIf = false;
            If(zmienne, licznik_operacji, max_operacji, ile_zmiennych, kod);
        }
        else if (isOperand(kod_before))
        {
            if (isWhiteSign(kod))
            {
                while (isWhiteSign(kod))kod = getchar();
                if (kod == EOF)break;
                if (isOperand(kod) || kod == '@' || kod == '?' || kod == '(') { koniec_instruckji = true; pomoc = kod; if (kod == '@')isWhile = true; else if (kod == '?')isIf = true; }
                else obecna_linia[i] = kod;
            }
            else if (kod == ')')
            {
                kod = getchar();
                while (isWhiteSign(kod))
                {
                    kod = getchar();
                }
                if (kod == EOF)break;
                if (isOperand(kod) || kod == '@' || kod == '?') { koniec_instruckji = true; pomoc = kod; obecna_linia[i] = ')'; if (kod == '@')isWhile = true; else if (kod == '?')isIf = true; }
                else { obecna_linia[i++] = ')'; obecna_linia[i] = kod; }
            }
            else { if (i<MAX_SIZE)obecna_linia[i] = kod; }
        }
        else  obecna_linia[i] = kod;
        i++;
        //Koniec jednej instrukcji
        if (koniec_instruckji)
        {
            obecna_linia[i] = '\0';
            char*linia_programu_ONP = new char[MAX_SIZE];
            linia_programu_ONP = ONPConv(obecna_linia);
            pr.dodajInstrukcje(obecna_linia, zmienne, ile_zmiennych);
            pr.last->zmien_Ins(linia_programu_ONP);
            ONPCalc(pr.last->ins, zmienne, pr.last->liczby, pr.last->ile_liczb, licznik_operacji, max_operacji, ile_zmiennych, pr.last->nazwy_zmiennych);
            i = 0;
            koniec_instruckji = false;
            for (int i = 0; i < MAX_SIZE; i++)obecna_linia[i] = '\0';
            obecna_linia[i++] = pomoc;
        }
        kod_before = kod;
        kod = getchar();
    }

    //Wczytanie ostatniej linii
    obecna_linia[i] = '\0';
    char*linia_programu_ONP = new char[MAX_SIZE];
    linia_programu_ONP = ONPConv(obecna_linia);
    pr.dodajInstrukcje(obecna_linia, zmienne, ile_zmiennych);
    pr.last->zmien_Ins(linia_programu_ONP);
    ONPCalc(pr.last->ins, zmienne, pr.last->liczby, pr.last->ile_liczb, licznik_operacji, max_operacji, ile_zmiennych, pr.last->nazwy_zmiennych);
    delete[]obecna_linia;
    delete[]linia_programu_ONP;
}


int main()
{
    //Tablica z informacjami ile poszczegolne cyfry maja znaków
    int *liczby = new int[MAX_SIZE];
    int ile_liczb = 0;
    bool zmiana = false;
    bool zmiana_zmienne = false;
    char**nazwy_zmiennych = new char*[MAX_SIZE];

    for (int i = 0; i < MAX_SIZE; i++)
    {
        liczby[i] = 0;
        nazwy_zmiennych[i] = new char[MAX_SIZE];
        for (int j = 0; j < MAX_SIZE; j++)nazwy_zmiennych[i][j] = '\0';
    }
    //iteratory
    int iterator = 0;
    int i = 0, j = 0, z = 0;
    int licznik_operacji = 0;
    int ile_zmiennych = 0;
    int ile_zmiennych_do_wypisania = 0;

    //zmienna pomocnicza do wczytania niepotrzebnego znaku konca linii
    char znak_nowej_lini = ' ';
    //nazwa zminnej
    char* slowo = new char[MAX_SIZE];
    //Tablica wskaznikow na zmienne
    Int** zmienne = new Int*[MAX_SIZE];
    //Zmienne do wypisania
    Int** zmienne_odczyt = new Int*[MAX_SIZE];
    zmienne_odczyt = zmienne;
    int max_operacji = 0;
    std::cin >> max_operacji;
    licznik_operacji = 0;
    //Upewnienie sie zepodano wlasciwa liczbe operacji
    assert(max_operacji > 0);
    //Pobranie znaku nowej linii po wczytaniu max_operacji
    znak_nowej_lini = getchar();

    //Pobieranie nazw zmiennych
    char zn = getchar();
    while (zn != '\n')
    {
        while (zn != ' ')
        {
            slowo[i] = zn;
            zn = getchar();
            i++;
            if (zn == '\n')break;
        }
        slowo[i] = '\0';
        zmienne[j] = new Int(slowo);
        ile_zmiennych++;
        i = 0;
        j++;
        if (zn == '\n')break;
        zn = getchar();
    }

    ile_zmiennych_do_wypisania = ile_zmiennych;
    /*----------------------------------------------------------------------------------------------------*/

    KodProgramu pr;
    Wczytaj(pr, zmienne, ile_zmiennych, licznik_operacji, max_operacji);

    /*----------------------------------------------------------------------------------------------------*/

    //Wypisywanie licznika operacji
    std::cout << licznik_operacji << endl;
    //Wypisywanie wartosci zmiennych
    for (int i = 0; i < ile_zmiennych_do_wypisania; i++)
    {
        std::cout << zmienne_odczyt[i]->nazwa << " " << *zmienne_odczyt[i] << endl;
    }
    getchar();
    delete[]liczby;
    delete[]slowo;
    for (int i = 0; i < ile_zmiennych; i++)
    {
        delete zmienne[i];
    }
    delete[]zmienne;
    for (int i = 0; i < MAX_SIZE; i++)delete[]nazwy_zmiennych[i];
    delete[]nazwy_zmiennych;
    return 0;
}