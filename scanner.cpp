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
            "(", ")", "{", "}", ";", ":", ":=", "!", "&", "|", "[", "]", ",", "@", "others", "EndOfFile"
        };

SymType Scan::number() {
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
    if (big) ScanError(INTCONST2BIG, "Error : Przekroczono zakres int!");
    return intconst;
}

SymType Scan::oper(){
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
        case '@':
            Nextc();
            return printsy;
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


SymType Scan::word(){
    unsigned int len=0, h;
    spell.clear();
    do{  // pobieramy identyfikarot do tabeli spell
        if(len<MAXIDLEN) {
            len++;
            spell.push_back(c);
        }
        else ScanError(IDENTIFIER2LONG, "Error : Przekroczono maksymalna dlugosc identyfikatora");
        Nextc();
    } while(isalnum(c));
    
    if(spell.compare("function")==0)
        return functionsy;
    if(spell.compare("matrix")==0)
        return matrixsy;
    if(spell.compare("int")==0)
        return intsy;
    if(spell.compare("return")==0)
        return returnsy;
    if(spell.compare("while")==0)
        return whilesy;
    if(spell.compare("if")==0)
        return ifsy;
    if(spell.compare("else")==0)
        return elsesy;
    if(spell.compare("public")==0)
        return publicsy;
    if(spell.compare("private")==0)
        return privatesy;
    return ident;
}

bool Scan::SkipWhiteSpaceOrComment() {
    do{
        while(isspace(c)) Nextc();// Pomiñ znaki biala
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
    if(isalpha(c)){
        return word();
    } else {
        if (isdigit(c)) {
            return number();
        } else {
            return oper();
        }
    }
}

void Scan::ScanError(int ecode, const char *mt, const char *at){
    src.Error(ecode, atompos, mt, at);
}
