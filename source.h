//
// Created by Dominik on 11.04.2018.
//


#ifndef LANGUAGE_INTERPRETER_MATRIXC_SRC_H
#define LANGUAGE_INTERPRETER_MATRIXC_SRC_H

// Klasa Source.
// sekcja #include
#include <cstdio>
#include <cstring>
#include <iostream>
#include <iomanip>
#define MAXLIN 100

using namespace std;

extern int options; // Definicja w MP1SORC.CPP
struct TextPos
{
    int ln, cn; // Numer wiersza, znaku (od 1)
    TextPos(int l=0, int c=0): ln(l), cn(c) {}
};
class Source
{
    char *fn; // Nazwa pliku
    FILE *fp;
    TextPos tpos; // Bie¿¹ca pozycja w pliku
    int etotal, einline; // Liczniki b³êdów:
    char Line[MAXLIN+1]; // Bufor bie¿¹cego wiersza
    void PrntLine(); // Druk bie¿¹cego wiersza
    int NextLine(); // Nastêpny wiersz do bufora
public:
    Source(const char *file);
    int NextChar();
    void Error(int ec,const TextPos&tp, const char*mt,const char*at);
    const TextPos& GetPos() const { return tpos; }
    ~Source();
};

#endif //LANGUAGE_INTERPRETER_MATRIXC_SRC_H
