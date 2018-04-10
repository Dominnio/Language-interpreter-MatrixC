enum SymType // Atomy leksykalne Mini Pascal'a
{
    mainsy, functionsy, matrixsy,
    intsy, returnsy, whilesy, breaksy, continuesy,
    ifsy, elsesy, publicsy, privatesy,// Keywords
    NKEYS, MAXKEY=privatesy,
    ident, intconst, times, div, plus, minus,
    ltop, leop, gtop, geop, neop, eqop,
    lparent, rparent, lbracket, rbracket,
    semicolon, colon, becomes, notop, andop, orop, raccessop, accessdot, laccessop,
    comma, others, MAXSYM=others+1
};

enum ScanErrors{ CARCONSTWRONG=1, ICONST2BIG };

class Scan
{
    enum { FirstLexErr = 0 };
    struct KeyRec { // Deskryptor s³owa kluczowego;
        char *kw; // S³owo kluczowe
        SymType atom; // Odpowiadaj¹cy mu atom
    };
    static KeyRec KT[NKEYS];// Keyword Table
    Source& src; // Strumieñ znaków
    int c; // Aktualny znak z src
    TextPos atompos; // Pozycja atomu w tekscie
    int intconstant; // Ostatnia sta³a
    char spell[MAXIDLEN+1];// Ostatni ident, przeliterowany
    void Nextc() { c=src.NextChar(); }
    static unsigned hash(char *s, unsigned int len);
    // Funkcja mieszaj¹ca dla KT
public:
    Scan(Source &s):src(s) { Nextc(); }
    SymType NextSymbol();// Nastêpny atom
    void ScanError(int ecode, const char *mtxt=0, const char *atxt=0);
    int IntConst() { return intconstant; }
    const char* Spell(){ return spell; }
};

// Implementacja klasy Scan.
char *AT[MAXSYM + 1] = // Atomy tekstowo
{
    "main", "function", "matrix",
    "int", "return", "while", "break", "continue",
    "if", "else", "public", "private",
    "ident", "intconst", "*", "/", "+", "-",
    "<", "<=", ">", ">=", "!=", "=",
    "(", ")", "{", "}",
    ";", ":", ":=", "!", "&", "|",
    "[", ".", "]", ","
};

Scan::KeyRec Scan::KT[NKEYS]= {
    // Keyword Atom hash(keyword)
    //-------------------------------------------
    { "main", mainsy }, // 0
    { "function", functionsy }, // 1
    { "matrix", matrixsy }, // 2
    { "int", intsy }, // 3
    { "return", returnsy }, // 4
    { "while", whilesy }, // 5
    { "break", breaksy }, // 6
    { "continue", continuesy }, // 7
    { "if", ifsy }, // 8
    { "else", elsesy }, // 9
    { "public", publicsy }, // 10
    { "private", privatesy }, // 11

};


unsigned Scan::hash(char *s, unsigned int len){
    //
    // Funkcja okreœlona eksperymentalnie: dla tabeli
    // NKEYS==18 elementowej jest 1 kolizja:
    // hash("not") == hash("else") == 17; brak kodu 0
    // Kolizjê usuniêto eksplicite.
    //
    unsigned h;
    if(strcmp(s, "not")==0) // Korekta kolizji
        return 0;
    h = (s[0]>>1) * s[1] * 1241;
    if(len>=3)
        h += s[len-1];
    return h % NKEYS;
}

SymType Scan::NextSymbol(){ // G³ówna us³uga scanera
do{
    while(isspace(c)) Nextc();// Pomiñ znaki bia³e
    if(c==EOF) return others;
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

    atompos=src.GetPos();
    //---IDENTYFIKATOR LUB S£OWO KLUCZOWE
    if(isalpha(c)){
        unsigned int len=0, h;
        do{  // pobieramy identyfikarot do tabeli spell
            if(len<MAXIDLEN) spell[len++]=c;
            Nextc();
        } while(isalnum(c));
        spell[len]='\0';    // koñczymy znakiem /0
        h = hash(spell,len);    // sprawdzamy czy jest to ident czy coœ innego
        if(strcmp(KT[h].kw, spell) == 0) return KT[h].atom; // jeœli jest to keyword to zwróæ to jakie
        else return ident;      // wpp jest to identyfikator
    } else { //---STA£A CA£KOWITA
        if(isdigit(c)){
            int big; long l=0;
            do{         // obliczamy jaka to liczba
                l = l*10+(c-'0');
                big = l>INT_MAX;
                Nextc();
            } while(isdigit(c));
            intconstant = (int)l;
            if(big) ScanError(ICONST2BIG);
            return intconst;
        }
    } else {
    // cd3 NextSymbol() ---Pozosta³e atomy
        switch(c){
            //----Sta³a znakowa
            case '"':
                Nextc();
                if(c=='"')
                { Nextc();
                if(c!='"') ScanError(CARCONSTWRONG);
                }
                intconstant=c; Nextc();
                if(c!='"') ScanError(CARCONSTWRONG,
                "B³êdna sta³a znakowa");
                else Nextc();
                return charconst;
            //----Operatory 2 i 1 znakowe
            case ':': Nextc();
                if (c=='=') { Nextc(); return becomes; }
                else return colon;
            case '.': Nextc();
                if(c=='.') { Nextc(); return thru;}
                else return period;
            case '<': Nextc();
                if(c=='=') { Nextc(); return leop;}
                else if(c=='>') { Nextc(); return neop; }
                else return ltop;
            case '>': Nextc();
                if(c=='=') { Nextc(); return geop; }
                else return gtop;
            //----Operatory 1 znakowe
            case '+': Nextc(); return plus;
            case '-': Nextc(); return minus;
            case '*': Nextc(); return times;
            case '=': Nextc(); return eqop;
            case '/': Nextc(); return divop;
            case '(': Nextc(); return lparent;
            case ')': Nextc(); return rparent;
            case '[': Nextc(); return lbracket;
            case ']': Nextc(); return rbracket;
            case ',': Nextc(); return comma;
            case ';': Nextc(); return semicolon;
            default : Nextc(); return others;
        }
    }
}
void Scan::ScanError(int ec, const char *mt, const char *at){
    static const char *ScnErr[] = {
        "Przekroczony zakres", // 0
        "B³êdna sta³a znakowa", // 1
    };
    if(mt)
        src.Error(ec, atompos, mt, at);
    else
        src.Error(FirstLexErr + ec, atompos,ScnErr[ec]);
}
