//
// Created by Dominik on 11.04.2018.
//

#ifndef LANGUAGE_INTERPRETER_MATRIXC_SCANNER_H
#define LANGUAGE_INTERPRETER_MATRIXC_SCANNER_H

#include <stdio.h>
#include <cstring>
#include <iostream>
#include <iomanip>
#include "src.h"

#define MAXIDLEN 50

enum SymType // Atomy leksykalne Mini Pascal'a
{
    mainsy, // 0
    functionsy, // 1
    matrixsy, // 2
    intsy, // 3
    returnsy, // 4
    whilesy,  // 5
    breaksy,  // 6
    continuesy,  // 7
    ifsy, // 8
    elsesy, // 9
    publicsy, // 10
    privatesy, //11 // Keywords
    NKEYS, // 12
    MAXKEY=privatesy, // 11
    ident, // 12
    intconst, //13
    times, // 14
    divop, // 15
    plusop, // 16
    minusop, // 17
    ltop, // 18
    leop, // 19
    gtop, // 20
    geop, // 21
    neop, // 22
    eqop, // 23
    lparent, // 24
    rparent, // 25
    lbracket, // 26
    rbracket, // 27
    semicolon, // 28
    colon, // 29
    becomes, // 30
    notop, // 31
    andop, // 32
    orop, // 33
    raccessop, // 34
    laccessop, // 35
    comma, // 36
    others, // 37
    MAXSYM=others+1 // 38
};

enum ScanErrors{ INTCONST2BIG=1, IDENTIFIER2LONG};

class Scan
{
    enum { FirstLexErr = 0 };
    struct KeyRec { // Deskryptor slowa kluczowego;
        const char *kw; // Slowo kluczowe
        SymType atom; // Odpowiadajacy mu atom
    };
    static KeyRec KT[NKEYS];// Keyword Table
    Source& src; // Strumien znaków
    int c; // Aktualny znak z src
    TextPos atompos; // Pozycja atomu w tekscie
    int intconstant; // Ostatnia stala
    char spell[MAXIDLEN+1];// Ostatni ident, przeliterowany
    void Nextc() { c=src.NextChar(); }
    static unsigned hash(char *s, unsigned int len);
    // Funkcja mieszajaca dla KT
public:
    Scan(Source &s):src(s) { Nextc(); }
    SymType NextSymbol();// Nastêpny atom
    void ScanError(int ecode, const char *mtxt=0, const char *atxt=0);
    int IntConst() { return intconstant; }
    const char* Spell(){ return spell; }
};



#endif //LANGUAGE_INTERPRETER_MATRIXC_SCANNER_H
