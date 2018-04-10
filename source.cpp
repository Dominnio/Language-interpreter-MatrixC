// Klasa Source.
// sekcja #include
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
    TextPos tpos; // Bie��ca pozycja w pliku
    int etotal, einline; // Liczniki b��d�w:
    char Line[MAXLIN+1]; // Bufor bie��cego wiersza
    void PrntLine(); // Druk bie��cego wiersza
    int NextLine(); // Nast�pny wiersz do bufora
public:
    Source(const char *file);
    int NextChar();
    void Error(int ec,const TextPos&tp, const char*mt,const char*at);
    const TextPos& GetPos() const { return tpos; }
    ~Source();
};
Source::Source(const char *file)
{
    strcpy(fn = new char[strlen(file)+1], file);
    fp = fopen(fn, "r");
    if(!fp){
        cout<<"MatrixC: B��D FATALNY - nie mo�na otworzy� pliku \"" << fn << "\"\n";
        exit(1);
    }
    cout<<"\nMatrixC, v.1.0, (c) 2018 (PW)\n";
    cout<<" raport kompilacji dla pliku: \"" << fnam << "\"\n\n";
    etotal = einline = 0;
}
Source::~Source()
{
    cout << "\nMatrixC: koniec raportu. \nWykrytych b��d�w: "<< etotal <<'\n';
    fclose(fp);
    delete [] fn;
}
void Source::Error(int ec, const TextPos&tp, const char*mt="", const char*at="") // error code, textpos, message (dwie - obie opcjonalne, domy�lnie puste)
{
    etotal++;
    if((options&NOLIST) && einline==0)// B��d #1 w wierszu // je�li jest odpowiednia opcja kompilatora
        cout<<setw(5)<<tpos.ln<<' '<<Line;
    einline=1;
    cout<<setw(2)<<ec<<"*** ";
    cout<<setw(tp.cn)<<setfill('-')
    <<'^' <<setfill(' ') <<mt <<at <<'\n';
}
int Source::NextChar()
{
    int r=1;
    if(tpos.ln==0)          // je�li �adna linia nie by�a brana z pliku to we� now� lini� (pi�rsz�)
        r=NextLine();
    else
        if(!Line[tpos.cn])  // je�li to by� ostatni znak z linni to we� now� lini�
            r=NextLine();
    if(r)                   // je�li r = 0 tzn. nie ma ju� nowych linii, wpp we� nast�pny znak z tej�e linii
        return Line[tpos.cn++];
    else
        return EOF;
}
int Source::NextLine()
{
    if(!fgets(Line, MAXLIN, fp))    // je�li ostatnia brana linia to zwr�� 0 (nie ma ju� lini)
        return 0; // EOF
    tpos.ln++; // Nast�pny wiersz,
    tpos.cn=0; // Ustaw pozycj� na przed pierwszym znakiem (jeszcze go nie pobrano)
    while(Line[tpos.cn]==' ')       // je�li jest jaka� spacja to dodaj pozycj�
        tpos.cn++;
    if((options&NOLIST)==0 )    // wypisz wiersz, je�li jest zaznaczona odpowiednia opcja kompilatora
        PrntLine();
    einline=0; // 0 b��d�w w nowym wierszu
    return 1;
}
