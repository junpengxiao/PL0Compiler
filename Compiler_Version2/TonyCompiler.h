//#define TONYDEBUG
#ifndef TONYCOMPILER_H
#define TONYCOMPILER_H
#include<sstream>
#include<iostream>
#include<fstream>
#include<string>
#include "TypeCode.h" // Define tokens code
//Global Variable Declaration
#define MaxN 1000
#ifdef TONYDEBUG
#define interResult cout
#else //intermediate result transmission
extern std::stringstream interResult;
extern std::stringstream optlv1;
#endif
extern std::string token;//processing symbol
extern Symbols symbol; //processing symbol type
extern int number; //integer or something 
extern int lineCounter; //count the line number of program
extern std::string strline;//processing line
extern int strlnPtr;//pointer of strline
extern char chNow;//processing char
extern int symbolCounter;
extern std::ifstream infile;//infile stream
extern std::ofstream oufile;//log stream
extern std::string output;
extern bool CompileOK;
//Declare functions
void GetNextSym();
char TouchNextChar();
std::string IntToString(int x);
void Program();
bool isNum(std::string tmp);
void ErrorHandler(std::string str);
#endif
