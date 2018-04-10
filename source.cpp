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
Source::Source(const char *file)
{
    strcpy(fn = new char[strlen(file)+1], file);
    fp = fopen(fn, "r");
    if(!fp){
        cout<<"MatrixC: B£¥D FATALNY - nie mo¿na otworzyæ pliku \"" << fn << "\"\n";
        exit(1);
    }
    cout<<"\nMatrixC, v.1.0, (c) 2018 (PW)\n";
    cout<<" raport kompilacji dla pliku: \"" << fnam << "\"\n\n";
    etotal = einline = 0;
}
Source::~Source()
{
    cout << "\nMatrixC: koniec raportu. \nWykrytych b³êdów: "<< etotal <<'\n';
    fclose(fp);
    delete [] fn;
}
void Source::Error(int ec, const TextPos&tp, const char*mt="", const char*at="") // error code, textpos, message (dwie - obie opcjonalne, domyœlnie puste)
{
    etotal++;
    if((options&NOLIST) && einline==0)// B³¹d #1 w wierszu // jeœli jest odpowiednia opcja kompilatora
        cout<<setw(5)<<tpos.ln<<' '<<Line;
    einline=1;
    cout<<setw(2)<<ec<<"*** ";
    cout<<setw(tp.cn)<<setfill('-')
    <<'^' <<setfill(' ') <<mt <<at <<'\n';
}
int Source::NextChar()
{
    int r=1;
    if(tpos.ln==0)          // jeœli ¿adna linia nie by³a brana z pliku to weŸ now¹ liniê (piêrsz¹)
        r=NextLine();
    else
        if(!Line[tpos.cn])  // jeœli to by³ ostatni znak z linni to weŸ now¹ liniê
            r=NextLine();
    if(r)                   // jeœli r = 0 tzn. nie ma ju¿ nowych linii, wpp weŸ nastêpny znak z tej¿e linii
        return Line[tpos.cn++];
    else
        return EOF;
}
int Source::NextLine()
{
    if(!fgets(Line, MAXLIN, fp))    // jeœli ostatnia brana linia to zwróæ 0 (nie ma ju¿ lini)
        return 0; // EOF
    tpos.ln++; // Nastêpny wiersz,
    tpos.cn=0; // Ustaw pozycjê na przed pierwszym znakiem (jeszcze go nie pobrano)
    while(Line[tpos.cn]==' ')       // jeœli jest jakaœ spacja to dodaj pozycjê
        tpos.cn++;
    if((options&NOLIST)==0 )    // wypisz wiersz, jeœli jest zaznaczona odpowiednia opcja kompilatora
        PrntLine();
    einline=0; // 0 b³êdów w nowym wierszu
    return 1;
}
