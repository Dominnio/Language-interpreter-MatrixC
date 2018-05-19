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
        cout<<"MatrixC: B£¥D FATALNY - nie mozna otworzyc pliku \"" << fn << "\"\n";
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
    int r=1;
    if(tpos.ln==0)          // jeli ¿adna linia nie by³a brana z pliku to we now¹ liniê (piêrsz¹)
        r=NextLine();
    else
    if(!Line[tpos.cn])  // jeli to by³ ostatni znak z linni to we now¹ liniê
        r=NextLine();
    if(r)                   // jeli r = 0 tzn. nie ma ju¿ nowych linii, wpp we nastêpny znak z tej¿e linii
        return Line[tpos.cn++];
    else
        return EOF;
}
int Source::NextLine()
{
    if(!fgets(Line, MAXLIN, fp))    // jeli ostatnia brana linia to zwróæ 0 (nie ma ju¿ lini)
        return 0; // EOF
    tpos.ln++; // Nastêpny wiersz,
    tpos.cn=0; // Ustaw pozycjê na przed pierwszym znakiem (jeszcze go nie pobrano)
    while(Line[tpos.cn]==' ')       // jeli jest jaka spacja to dodaj pozycjê
        tpos.cn++;
    //if((options&NOLIST)==0 )    // wypisz wiersz, jeli jest zaznaczona odpowiednia opcja kompilatora
    //PrntLine();
    einline=0; // 0 blêdów w nowym wierszu
    return 1;
}
