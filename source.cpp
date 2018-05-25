//
// Created by Dominik on 11.04.2018.
//

#include "source.h"
/*
 * Moduł wczutujacy znaki z pliku.
 * Moduł określa bieżącą pozycję w pliku.
 */
Source::Source(const char *file)
{
    strcpy(fn = new char[strlen(file)+1], file);
    fp = fopen(fn, "r");
    if(!fp){
        cout<<"MatrixC: BLAD FATALNY - nie mozna otworzyc pliku \"" << fn << "\"\n";
        exit(1);
    }
    cout<<"\nMatrixC, v.1.0, (c) 2018 (PW)\n";
    cout<<"Raport kompilacji dla pliku: \"" << fn << "\"\n\n";
    etotal = einline = 0;
}
Source::~Source()
{
    cout << "\nMatrixC: koniec raportu. \nWykrytych bledow: "<< etotal <<'\n';
    fclose(fp);
    delete [] fn;
}
void Source::Error(int ec, const TextPos&tp, const char*mt="", const char*at="") // error code, textpos, message (dwie - obie opcjonalne, domylnie puste)
{
    etotal++;
    cout<<setw(5) << "Blad w wierszu " <<tpos.ln<<" :"<<Line;
    cout<<setw(tpos.cn) << "^" << endl;
    einline=1;
    cout<< "Kod bledu:" <<setw(2)<<ec<<"*** ";
    cout<<setw(tp.cn)<<setfill('-') <<setfill(' ') <<mt <<at <<'\n';
}
int Source::NextChar()
{
    int nextCharacter = fgetc(fp);
    if(tpos.ln==0) {
        tpos.ln++;
        tpos.cn=0;
        einline=0;
    } else {
        if (nextCharacter != EOF)
            tpos.ln++;
            tpos.cn=0;
            einline=0;
    }
    if(nextCharacter != EOF) {
        tpos.cn++;
        return nextCharacter;
    }
    else
        return EOF;
}

