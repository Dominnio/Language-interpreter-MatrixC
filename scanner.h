//
// Created by Dominik on 11.04.2018.
//

#ifndef LANGUAGE_INTERPRETER_MATRIXC_SCANNER_H
#define LANGUAGE_INTERPRETER_MATRIXC_SCANNER_H

#include <stdio.h>
#include <cstring>
#include <iostream>
#include <iomanip>
#include "source.h"

#define MAXIDLEN 50

enum SymType // Atomy leksykalne
{
    functionsy,
    matrixsy,
    intsy,
    returnsy,
    whilesy,
    ifsy,
    elsesy,
    publicsy,
    privatesy,  // Keywords
    NKEYS,
    MAXKEY=privatesy,
    ident,
    intconst,
    times,
    divop,
    plusop,
    minusop,
    ltop,
    leop,
    gtop,
    geop,
    neop,
    eqop,
    lparent,
    rparent,
    lbracket,
    rbracket,
    semicolon,
    colon,
    becomes,
    notop,
    andop,
    orop,
    laccessop,
    raccessop,
    comma,
    printsy,
    others,
    EndOfFile,
    MAXSYM=others+1,
};

enum ScanErrors{ INTCONST2BIG=1, IDENTIFIER2LONG, NOTRECOGNISED};

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
    bool SkipWhiteSpaceOrComment();
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
