//
//  Emit.cpp
//  Compiler_Version2
//
//  Created by 肖俊鹏 on 11/29/13.
//  Copyright (c) 2013 肖俊鹏. All rights reserved.
//

#include "Emit.h"
using namespace std;

Emit::Emit()
{
    this->setd1=false;
    this->sets1=false;
    this->sets2=false;
    this->settp=false;
    this->table=NULL;
    this->sour1="ERROR";
    this->sour2="ERROR";
    this->dest1="ERROR";
}
void Emit::SetS1(string str) {
    this->sour1=str;
    sets1=true;
}
void Emit::SetS2(string str) {
    this->sour2=str;
    sets2=true;
}
void Emit::SetD1(string str) {
    this->dest1 = str;
    setd1=true;
}
void Emit::SetTP(EmitType type) {
    this->type=type;
    settp=true;
}
string Emit::GetD1() {
    return dest1;
}
string Emit::GetS1() {
    return sour1;
}
string Emit::GetS2() {
    return sour2;
}
void Emit::SetTB(SymbolTable* tb)
{
    this->table=tb;
}
void _emit(string type,string s1,string s2,string d1){
    interResult<<type<<' '<<s1<<' '<<s2<<' '<<d1<<endl;
}
SymbolType Emit::GetType(string str)
{
    SymbolTable* ttable=table;
    if (str.find('[')!=-1) str=str.substr(0,str.find('['));
    while (ttable!=NULL && ttable->CheckingExist(str)) ttable=ttable->GetFather();
    if (ttable==NULL) return charst;
    return ttable->GetItem(str)->GetType();
}
void Emit::SetType(SymbolType type)
{
    SymbolTable* ttable=table;
    while (ttable!=NULL && ttable->CheckingExist(dest1)) ttable=ttable->GetFather();
    ttable->GetItem(dest1)->SetType(type);
    return;
}
void Emit::Check(int num) {
    if (table==NULL) return;
    if (dest1.substr(0,5)!="ttemp") return;
    if (num==2 && GetType(sour1)==charst) return SetType(charst);
    if (num==3 && (GetType(sour1)==charst || GetType(sour2)==charst)) return SetType(charst);
    return SetType(integerst);
}
void Emit::UpGradeType()
{
    switch (type) {
        case negiet:
            return Check(2);
        case assignet:
            return Check(2);
    }
    switch (type) {
        case addet:
            return Check(3);
        case subet:
            return Check(3);
            break;
        case multet:
            return Check(3);
        case divet:
            return Check(3);
        case callet:
            return Check(3);
    }
}
void Emit::emitResult()
{
    //Check
    if (table!=NULL) UpGradeType();
    if (!settp) ErrorHandler("emit should be set type");//Error nott set type
    settp=false;
    if (type==noptp) return _emit("NOP","","","");
    if (type==endet) return _emit("END", "", "", "");
    if (!setd1) ErrorHandler("emit should have d1");//Error not set t1
    setd1=false;
    switch (type) {
        case jmpet:
            return _emit("JMP", "", "", dest1);
            break;
        case readet:
            return _emit("READ", "", "", dest1);
            break;
        case writeet:
            return _emit("WRITE", "", "", dest1);
            break;
        case pushet:
            return _emit("PUSH", "", "", dest1);
            break;
        case popet:
            return _emit("POP", "", "", dest1);
            break;
        case retet:
            return _emit("RET", "", "", dest1);
            break;
    }
    if (!sets1) ErrorHandler("emit should have s1");//Error not set s1
    sets1=false;
    switch (type) {
        case negiet:
            return _emit("NEG", sour1, "", dest1);
            break;
        case jzet:
            return _emit("JZ", sour1, "", dest1);
            break;
        case assignet:
            return _emit("ASSIGN", sour1, "", dest1);
            break;
    }
    if (!sets2) ErrorHandler("emit should have s2");//Error not set s2
    sets2=false;
    switch (type) {
        case addet:
            return _emit("ADD", sour1, sour2, dest1);
            break;
        case subet:
            return _emit("SUB", sour1, sour2, dest1);
            break;
        case multet:
            return _emit("MUL", sour1, sour2, dest1);
            break;
        case divet:
            return _emit("DIV", sour1, sour2, dest1);
            break;
        case greet:
            return _emit("GRE",sour1,sour2,dest1);
            break;
        case lsset:
            return _emit("LSS", sour1, sour2, dest1);
            break;
        case geqet:
            return _emit("GEQ", sour1, sour2, dest1);
            break;
        case leqet:
            return _emit("LEQ", sour1, sour2, dest1);
            break;
        case neqet:
            return _emit("NEQ", sour1, sour2, dest1);
            break;
        case callet:
            return _emit("CALL", sour1, sour2, dest1);
            break;
        case eqlet:
            return _emit("EQL", sour1, sour2, dest1);
            break;
    }
    //Error should not reach to this place
}
