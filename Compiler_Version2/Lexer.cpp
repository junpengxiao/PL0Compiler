#include "TonyCompiler.h"
#include<iostream>
#include<fstream>
#include<string>
using namespace std;


//Define variabbles

//ofstream interResult("output.txt");
string token;//processing symbol
Symbols symbol; //processing symbol type
int number; //integer or something
int lineCounter; //count the line number of program
std::string strline;//processing line
int strlnPtr;//pointer of strline
char chNow;//processing char
int symbolCounter;
ifstream infile("IfTest.pas");//infile stream
ofstream oufile("AsOutput.txt");//log stream
stringstream interResult,optlv1;
void ErrorHandler(string str)
{
    cout<<"Error at line" << lineCounter<<" The "<<strlnPtr<<" Char with reason : "<<str<<endl;
    exit(-1);
}
char TouchNextChar() { return ((unsigned int)strlnPtr<strline.length())?strline[strlnPtr]:'\0'; }
bool isSpace(char ch) { return ch==' '; }
bool isNewLine(char ch) {return ch=='\n' || ch=='\r'; }
bool isTab(char ch) { return ch=='\t'; }
char GetNextChar()
{
	while (strlnPtr==strline.length()) {
		if (!getline(infile,strline)) ErrorHandler("end of file"); //Throw end of file
        while (strline[0]=='#') {
            getline(infile,strline);
        }
		strlnPtr=0;
		lineCounter++;
	}
	chNow = strline[strlnPtr++];
	return chNow;
}
void GetCharSymbol()
{
	if (!isalnum(GetNextChar())) ErrorHandler("can not get char while need be get a char");//Throw can not get char const
	symbol=CHARCON;
	token=chNow;
    if (GetNextChar()!='\'') ErrorHandler("can not find the \' while reading a char"); //Throw can not find end symbol
}
void GetStringSymbol()
{
	token.clear();
    while (true) {
        GetNextChar();
        if (chNow=='\"') break;
        if (chNow>=32 && chNow<=126 && chNow!=34) token+=chNow;
        else ; //Throw String isn't fit syntax
    }
	symbol=STRCON;
}

#define keywordsNumber 21
const string keywords[keywordsNumber] = 	{"const","integer","char","var","array","of","repeat","until","if","then","else","do","for","to","downto","procedure","function","read","write","begin","end"};
const Symbols keywordsTK[keywordsNumber] =
	{CONSTTK,INTTK,CHARTK,VARTK,ARRAYTK,OFTK,REPTTK,UNLTK,IFTK,THENTK,ELSETK,DOTK,FORTK,TOTK,DOWNTOTK,PROCETK,FUNCTK,READTK,WRITETK,BEGINTK,ENDTK,};
void GetNextSym()
{
	++symbolCounter;
	token.clear();
	GetNextChar();
	while (isSpace(chNow) || isNewLine(chNow) || isTab(chNow)) GetNextChar();
	token+=chNow;
	switch (chNow) // All kinds of special symbol
	{
        case '+': symbol=PLUS; return;
        case '-': symbol=MINU; return;
        case '*': symbol=MULT; return;
        case '/': symbol=DIV; return;
        case '<' :
		switch (TouchNextChar())
        {
            case '=': symbol=LEQ; token+=GetNextChar(); break;
            case '>': symbol=NEQ; token+=GetNextChar(); break;
            default:
                symbol=LSS; break;
		}
            return;
        case '>' :
            if (TouchNextChar()=='=') { symbol=GEQ; token+=GetNextChar();}
            else symbol=GRE;
            return;
        case '=': symbol=EQL; return;
        case ';': symbol=SEMICN; return;
        case ',': symbol=COMMA; return;
        case '.': symbol=PERIOD; return;
        case ':':
            if (TouchNextChar()=='=') {symbol=ASSIGN; token+=GetNextChar();}
            else symbol=COLON;
            return;
            case '\'': return GetCharSymbol();
        case '\"': return GetStringSymbol();
        case '(': symbol=LPARENT; return;
        case ')': symbol=RPARENT; return;
        case '[': symbol=LBRACK; return;
        case ']': symbol=RBRACK; return;
        case '{': symbol=LBRACE; return;
        case '}': symbol=RBRACE; return;
    }
	while (isalnum(TouchNextChar())) token+=GetNextChar();
	if (isalpha(token[0])) {
		for (int i=0;i!=keywordsNumber;++i) if (token==keywords[i]) {
            symbol = keywordsTK[i];
            return;
		}
		symbol = IDEN;
		return;
	}
	if (isdigit(token[0])) {
		number=0;
		symbol=INTCON;
		for (int i=0;i!=token.length();++i) {
			if (!isdigit(token[i])) ErrorHandler("it's a number but with alpha");//Throw an non number value in number string
			number=number*10+token[i]-'0';
		}
		return;
	}
    //Throw not expect run to this point
}
