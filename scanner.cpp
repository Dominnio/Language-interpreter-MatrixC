//
// Created by Dominik on 11.04.2018.
//

#include "scanner.h"

/*
 * Główny moduł leksera.
 * Interpretuje symbole, pryzpisuje im tokeny.
 */
const char *AT[MAXSYM + 1] =
        {
            "function", "matrix", "int", "return", "while",
            "if", "else", "public",
            "private", "ident", "intconst",
            "*", "/", "+", "-", "<", "<=", ">", ">=", "!=", "=",
            "(", ")", "{", "}", ";", ":", ":=", "!", "&", "|", "[", "]", ",", "others", "EndOfFile"
        };


Scan::KeyRec Scan::KT[NKEYS]= {
        // Keyword Atom hash(keyword)
        //-------------------------------------------
        { "function", functionsy }, // 0
        { "matrix", matrixsy }, // 1
        { "int", intsy }, // 2
        { "return", returnsy }, // 3
        { "while", whilesy }, // 4
        { "if", ifsy }, // 5
        { "else", elsesy }, // 6
        { "public", publicsy }, // 7
        { "private", privatesy }, // 8

};

unsigned Scan::hash(char *s, unsigned int len){
    //
    // Funkcja okrelona eksperymentalnie: dla tabeli
    // NKEYS==18 elementowej jest 1 kolizja:
    // hash("not") == hash("else") == 17; brak kodu 0
    // Kolizjê usuniêto eksplicite.
    //
    unsigned h;
    if(strcmp(s, "function")==0)
        return 0;
    if(strcmp(s, "matrix")==0)
        return 1;
    if(strcmp(s, "int")==0)
        return 2;
    if(strcmp(s, "return")==0)
        return 3;
    if(strcmp(s, "while")==0)
        return 4;
    if(strcmp(s, "if")==0)
        return 5;
    if(strcmp(s, "else")==0)
        return 6;
    if(strcmp(s, "public")==0)
        return 7;
    if(strcmp(s, "private")==0)
        return 8;

    h = (s[0]>>1) * s[1] * 1241;
    if(len>=3)
        h += s[len-1];
    return h % NKEYS;
}


bool Scan::SkipWhiteSpaceOrComment() {
    do{
        while(isspace(c)) Nextc();// Pomiñ znaki bia³e
        if(c=='#'){ // komentarz w stylu #...#
            do Nextc(); while(c!='#');
            Nextc();
        } else
        if(c=='/'){     // komentarz w stylu //
            Nextc();
            if(c!='/') return divop;
            else do Nextc(); while(c!='\n');
        }
    } while(isspace(c) || c=='#' || c=='/');
}

SymType Scan::NextSymbol(){ // G³ówna us³uga scanera

    SkipWhiteSpaceOrComment();

    if(c==EOF) return EndOfFile;
    atompos=src.GetPos();
    //---IDENTYFIKATOR LUB SLOWO KLUCZOWE
    if(isalpha(c)){
        unsigned int len=0, h;
        do{  // pobieramy identyfikarot do tabeli spell
            if(len<MAXIDLEN) spell[len++]=c;
            else ScanError(IDENTIFIER2LONG, "Error : Przekroczono maksymalna dlugosc identyfikatora");
            Nextc();
        } while(isalnum(c));
        spell[len]='\0';    // koñczymy znakiem /0
        h = hash(spell,len);    // sprawdzamy czy jest to ident czy co innego
        if(strcmp(KT[h].kw, spell) == 0) return KT[h].atom; // jesli jest to keyword to zwróc to jakie
        else return ident;      // wpp jest to identyfikator
    } else {  //---STA£A CA£KOWITA
        if (isdigit(c)) {
            bool big = false;
            long l = 0;
            do {         // obliczamy jaka to liczba
                l = l * 10 + (c - '0');
                if(l > INT_MAX || l < 0) {
                    big = true;
                    break;
                }
                Nextc();
            } while (isdigit(c));
            intconstant = (int) l;
            if (big == true) ScanError(INTCONST2BIG, "Error : Przekroczono zakres int!");
            return intconst;
        } else {
            // ---Pozostale atomy
            switch (c) {
                    //----Operatory 2 i 1 znakowe
                case ':':
                    Nextc();
                    if (c == '=') {
                        Nextc();
                        return becomes;
                    }
                    else return colon;
                case '<':
                    Nextc();
                    if (c == '=') {
                        Nextc();
                        return leop;
                    }
                    else return ltop;
                case '>':
                    Nextc();
                    if (c == '=') {
                        Nextc();
                        return geop;
                    }
                    else return gtop;
                case '!':
                    Nextc();
                    if (c == '=') {
                        Nextc();
                        return neop;
                    }
                    else return notop;                    //----Operatory 1 znakowe
                case '+':
                    Nextc();
                    return plusop;
                case '-':
                    Nextc();
                    return minusop;
                case '*':
                    Nextc();
                    return times;
                case '=':
                    Nextc();
                    return eqop;
                case '/':
                    Nextc();
                    return divop;
                case '(':
                    Nextc();
                    return lparent;
                case ')':
                    Nextc();
                    return rparent;
                case '{':
                    Nextc();
                    return lbracket;
                case '}':
                    Nextc();
                    return rbracket;
                case '[':
                    Nextc();
                    return laccessop;
                case ']':
                    Nextc();
                    return raccessop;
                case ',':
                    Nextc();
                    return comma;
                case ';':
                    Nextc();
                    return semicolon;
                case '&':
                    Nextc();
                    return andop;
                case '|':
                    Nextc();
                    return orop;
                default :
                    Nextc();
                    ScanError(NOTRECOGNISED, "Error : Nierozpoznany symbol");
                    return others;
            }
        }
    }
}
void Scan::ScanError(int ecode, const char *mt, const char *at){
    src.Error(ecode, atompos, mt, at);
}
