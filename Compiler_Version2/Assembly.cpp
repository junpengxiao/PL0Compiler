//
//  Assembly.cpp
//  Compiler_Version2
//
//  Created by 肖俊鹏 on 12/31/13.
//  Copyright (c) 2013 肖俊鹏. All rights reserved.
//
#include<string>
#include "Assembly.h"
#include "TonyCompiler.h"
#include "SymbolTable.h"
#include "OptLv1.h"
using namespace std;

string output="";
string tmp;
string ttmp;
bool retract=false;
int pointer;
SymbolTable* table=NULL;
string strlist[MaxN];
int pstrlist=0;
void DpR()
{
    ofstream tmpou("MidCode.txt");
    string tmpst=interResult.str();
    interResult.str("");
    interResult.clear();
    stringstream tmpstrsm("");
    tmpstrsm<<tmpst;
    while (getline(tmpstrsm, tmpst)) {
        if (tmpst.find("WRITE")==-1) interResult<<tmpst<<endl;
        else if (tmpst.find('$')==-1) interResult<<tmpst<<endl;
        else {
            strlist[pstrlist]=tmpst.substr(tmpst.find('$')+1,tmpst.length());
            interResult<<"WRITE $constr"<<pstrlist<<endl;
            ++pstrlist;
        }
    }
//    while (tmpstrsm>>tmpst) {
//        if (tmpst=="WRITE") {
//            interResult<<tmpst<<endl;
//            tmpstrsm>>tmpst;
//            if (tmpst[0]=='$') {
//                strlist[pstrlist]=tmpst.substr(1,tmpst.length());
//                interResult<<'$constr'<<pstrlist<<endl;
//                ++pstrlist;
//            } else interResult<<tmpst<<endl;
//        } else interResult<<tmpst<<endl;
//    }
    tmpou<<tmpst;
    tmpou.close();
}
bool isNum(string tmp)
{
    for (int i=1;i!=tmp.length();++i) if (!isnumber(tmp[i])) return false;
    if ((!isnumber(tmp[0]))&&(tmp[0]!='-')&&(tmp[0]!='+')) return  false;
    if ((!isnumber(tmp[0]))&&tmp.length()<=1) return false;
    return true;
}
void Flush()
{
    optlv1<<output<<endl;
    if (retract) output="\t";
    else output="";
}
void Set(string tmp)
{
    if ((output==""||output=="\t")&&(tmp[0]==';'||tmp.back()==':'||tmp[0]=='@')) {
        if (tmp[0]!='@') optlv1<<endl;
        output="";
    }
    if (output!=""&&output!="\t") {
        output+=' ';
    }
    output+=tmp;
}
bool isPara(string name)
{
    SymbolTable* ttable=table;
    while (ttable->GetOffset(name)<0) ttable=ttable->GetFather();
    return ttable->GetParaNum()>ttable->GetIndex(name);
}
bool isVar(string name)
{
    return table->GetItem(name)->GetKind()==parask;
}
int _AsOffset(string name) {
    if (isNum(name)) {
        int rect=0;
        for (int i=0;i!=name.length();++i) rect+=(rect*10+name[i]-'0');
        return rect;
    }
    SymbolTable* ttable=table;
    while (ttable->GetOffset(name)<0) ttable=ttable->GetFather();
    if (isPara(name)) return (ttable->GetLevel()+ttable->GetParaNum()-ttable->GetIndex(name))*4;
    return (ttable->GetOffset(name));
}
void AsOffset()
{
    int arrp=0;
    string arrstring;
    if (tmp.back()==']') {
        arrp=(int)tmp.find('[');
        arrstring=tmp.substr(arrp+1,tmp.length()-2-arrp);
        tmp=tmp.substr(0,arrp);
    }
    int offset=table->GetOffset(tmp);
    if (offset<0) Set("mov esi,[ebp+"+IntToString(abs(offset-1)*4)+"]");
    else Set("mov esi,ebp");
    Flush();
    if (isPara(tmp)) Set("add esi,"+IntToString(_AsOffset(tmp)));
    else Set("sub esi,"+IntToString(_AsOffset(tmp)));
    Flush();
    if (isVar(tmp)) {
        Set("mov esi,[esi]");
        Flush();
    }
    if (arrp!=0) {
        if (isNum(arrstring)) {
            int trans=0;
            for (int i=0;i!=arrstring.length();++i) trans=trans*10+arrstring[i]-'0';
            trans*=4;
            Set("sub esi,"+IntToString(trans));
        } else {
            Set("mov edi,[ebp-"+IntToString(table->GetOffset(arrstring))+']');
            Flush();
            Set("shl edi,2");
            Flush();
            Set("sub esi,edi");
        }
        Flush();
    }
}
void AsHead()
{
    Set(".386"); Flush();
    Set(".model flat,stdcall"); Flush();
    Set("option casemap:none"); Flush();
    Set(";Include resource"); Flush();
    Set("include msvcrt.inc"); Flush();
    Set("include kernel32.inc"); Flush();
    Set("includelib kernel32.lib"); Flush();
    Set("includelib msvcrt.lib"); Flush();
    Set(";Define C-type IO"); Flush();
    Set("printf PROTO C:ptr sbyte, :vararg"); Flush();
    Set("scanf PROTO C:ptr sbyte, :vararg"); Flush();
    Flush(); Set(".data"); Flush();
    Set("$IOChar db \"%c\",0"); Flush();
    Set("$IOInt db \"%d\",0"); Flush();
    Set("$Return db 10,13,0");Flush();
    for (int i=0;i!=pstrlist;++i) {
        Set("$constr"+IntToString(i)+" db \""+strlist[i]+"\",0");
        Flush();
    }
    Flush(); Set(".code");Flush();
}
void AsTail()
{
    Set("start:"); Flush();
    CalleeSta(0);
    Set("call @main0");Flush();
    Set("call ExitProcess"); Flush();
    Set("end start"); Flush();
}
void LoadConst()
{
    for (int i=0;i!=table->GetTail();++i) if (table->GetItem(i)->GetKind()==constsk) {
        Set("mov dword ptr [ebp-"+IntToString(table->GetItem(i)->GetOffset())+"],"+IntToString(table->GetItem(i)->GetValue()));
        Flush();
    }
}
void AsProcSta()
{
    if (table!=NULL) AsProcEnd();
    interResult>>tmp;
    interResult>>pointer;
    table=&symbolTableList[pointer];
    Flush(); output=""; Set("@"+tmp+IntToString(pointer)+" proc"); Flush();
    CalleeSta(table->GetSize());
    LoadConst();
}
void AsProcEnd()
{
    Set("mov eax,");
    Set("[ebp-"+IntToString(table->GetOffset(table->GetName()))+"]");
    Flush();
    Set(";Subroutine Epilogue"); Flush();
    Set("pop esi");Flush();
    Set("pop edi");Flush();
    Set("mov esp,ebp");Flush();
    Set("pop ebp");Flush();
    Set("ret");Flush();
    output="";
    Set("@"+table->GetName()+IntToString(pointer)+" endp");
    Flush();
}
void AsArray()
{
    string tmp1,tmp2;
    int t=(int)tmp.find('[');
    tmp1=tmp.substr(0,t);
    tmp2=tmp.substr(t+1,tmp.length()-2-t);
    Set("mov esi,[ebp-"+IntToString(table->GetOffset(tmp.substr(t+1,tmp.length()-2-t)))+"]");
    Flush();
    Set("shl esi,2");
    Flush();
    Set("add esi,"+IntToString(table->GetOffset(tmp.substr(0,t))));
    Flush();
    Set("sub esi,ebp");
    Flush();
    Set("neg esi");
    Flush();
}
void AsAssign()
{
    interResult>>tmp;
    if (isNum(tmp)) {
        ttmp=tmp;
        interResult>>tmp;
        AsOffset();
        Set("mov");
        Set("dword ptr [esi],");
        Set(ttmp);
        Flush();
        return;
    }
    AsOffset();
    Set("mov eax,");
    Set("dword ptr [esi]");
    Flush();
    interResult>>tmp;
    AsOffset();
    Set("mov dword ptr [esi],");
    Set("eax");
    Flush();
}
void AsRead()
{
    interResult>>tmp;
    SymbolItem *item=table->GetItem(tmp);
    AsOffset();
    Set("lea eax,");
    Set("dword ptr [esi]");
    Flush();
    Set("push eax");
    Flush();
    if (item->GetType()==integerst) Set("push offset $IOInt");
    else Set("push offset $IOChar");
    Flush();
    Set("call scanf");
    Flush();
}
void AsWrite()
{
    interResult>>tmp;
    if (tmp[0]=='$') {
        Set("push offset "+tmp);
        Flush();
        Set("call printf");
        Flush();
        return;
    }
    AsOffset();
    SymbolItem* item=table->GetItem(tmp);
    Set("mov eax,[esi]");
    Flush();
    Set("push eax");
    Flush();
    Set("push offset");
    if (item->GetType()==integerst||item->GetType()==arrOfInt) Set("$IOInt");
    if (item->GetType()==charst||item->GetType()==arrOfCh) Set("$IOChar");
    Flush();
    Set("call printf");
    Flush();
    
//    //---------------------RETURN
//    Set("push offset $Return");
//    Flush();
//    Set("call printf");
//    Flush();
}
void CalleeSta(int totOffset)
{
    Set(";Subroutine Prologue"); Flush();
    Set("push ebp"); Flush();
    Set("mov ebp,esp"); Flush();
    Set("sub esp,"+IntToString(totOffset)); Flush();
    Set("push edi"); Flush();
    Set("push esi"); Flush();
    Set(";Subroutine Body"); Flush();
}
void AsCondi(string type)
{
    interResult>>tmp;
    if (!isNum(tmp)) {
        AsOffset();
        Set("mov eax,[esi]");
    } else Set("mov eax,"+tmp);
    Flush();
    interResult>>tmp;
    interResult>>ttmp>>ttmp;
    if (ttmp!="JZ") ErrorHandler("Assembly: What's the f**k!!!A condition must followed by JZ. I'm CRAZZZZZZZY");
    interResult>>ttmp>>ttmp;
    if (!isNum(tmp)) {
        AsOffset();
        Set("cmp eax,[esi]");
    } else Set("cmp eax,"+tmp);
    Flush();
    if (type=="LSS") Set("jl");
    if (type=="LEQ") Set("jle");
    if (type=="GRE") Set("jg");
    if (type=="GEQ") Set("jge");
    if (type=="EQL") Set("je");
    if (type=="NEQ") Set("jne");
    Set(ttmp);
    Flush();
//    interResult>>tmp;
//    if (tmp!="JMP") ErrorHandler("Assembly: What's the f**k!!!A condition must followed by JZ. I'm CRAZZZZZZZY");
//    interResult>>ttmp;
//    Set("jmp "+ttmp);
//    Flush();
}
void AsLabel()
{
    Set(tmp+':');
    Flush();
}
void AsJMP()
{
    interResult>>tmp;
    Set("jmp "+tmp);
    Flush();
}
void AsAlge()
{
    ttmp=tmp;
    interResult>>tmp;
    if (isNum(tmp)) {
        Set("mov eax,"+tmp);
    } else {
        AsOffset();
        Set("mov eax,[esi]");
    }
    Flush();
    interResult>>tmp;
    string oper="";
    if (ttmp=="ADD") oper="add eax,";
    if (ttmp=="SUB") oper="sub eax,";
    if (ttmp=="MUL") oper="imul eax,";
    if (ttmp!="DIV") {
        if (isNum(tmp)) {
            Set(oper);
            Set(tmp);
        } else {
            AsOffset();
            Set(oper);
            Set("[esi]");
        }
        Flush();
        //dest
        interResult>>tmp;
        AsOffset();
        Set("mov [esi],");
        Set("eax");
        Flush();
        return;
    }
    if (isNum(tmp)){
        Set("cdq");
        Flush();
        Set("mov edi,"+tmp);
        Flush();
        Set("idiv edi");
    }else {
        AsOffset();
        Set("cdq");
        Flush();
        Set("idiv");
        Set("dword ptr [esi]");
    }
    Flush();
    //dest
    interResult>>tmp;
    AsOffset();
    Set("mov [esi],");
    Set("eax");
    Flush();
}
void AsCall()
{
    int index;
    interResult>>tmp;
    for (index=0;index!=ptable;++index) if (symbolTableList[index].GetName()==tmp) break;
    if (table->GetLevel()+1<symbolTableList[index].GetLevel()) ErrorHandler("Assembly: unlegel function call");
    if (table->GetLevel()+1==symbolTableList[index].GetLevel()) {
        for (int i=0;i!=table->GetLevel()-1;++i) {
            Set("push [ebp+"+IntToString((table->GetLevel()-i)*4)+"]");
            Flush();
        }
        Set("push ebp");
        Flush();
    } else {
        for (int i=0;i!=symbolTableList[index].GetLevel()-1;++i) {
            Set("push [ebp+"+IntToString((table->GetLevel()-i)*4)+"]");
            Flush();
        }
    }
    int pointer=0;
    SymbolTable* ttable=table;
    while (ttable->GetOffset(tmp)<0) {
        ttable=ttable->GetFather();
    }
    for (int i=0;i!=ptable;++i) if ((&symbolTableList[i])==ttable) {
        pointer=i;
        break;
    }
    ttable=&symbolTableList[pointer];
    for (int i=0;i!=ptable;++i) if ((symbolTableList[i].GetFather())==ttable&&symbolTableList[i].GetName()==tmp) {
        pointer=i;
        break;
    }
    Set("call @"+tmp+IntToString(pointer)); Flush();
    interResult>>tmp;
    interResult>>tmp;
    Set("mov");
    Set("dword ptr [ebp-"+IntToString(table->GetOffset(tmp))+"],");//Special-------------------------------->New Added, Think twice before modify
    Set("eax");
    Flush();
    Set("add esp,"+IntToString((symbolTableList[index].GetParaNum()+symbolTableList[index].GetLevel()-1)*4));
    Flush();
}
void AsPush()
{
    interResult>>tmp;
    if (isNum(tmp)) Set("push "+tmp);
    else {
        if (tmp.back()!='[') {
            AsOffset();
            Set("push [esi]");
        } else  {
            tmp.pop_back();
            AsOffset();
            Set("push esi");
        }
    }
    Flush();
//    
//    if (tmp.back()!='[') {
//        if (isNum(tmp)) Set("push "+tmp);
//        else {
//            AsOffset();
//            Set("push esi");
//        }
//    } else {
//        tmp.pop_back();
//        AsOffset();
//        Set("push");
//        Set("esi");
//    }
//    Flush();
}
void Assembly()
{
    DpR();
    AsHead();
    retract=true;
    while (interResult>>tmp) {
        if (tmp=="Proc"||tmp=="Func") {
            AsProcSta();
            continue;
        }
        if (tmp=="ASSIGN") {
            AsAssign();
            continue;
        }
        if (tmp=="WRITE") {
            AsWrite();
            continue;
        }
        if (tmp=="READ") {
            AsRead();
            continue;
        }
        if (tmp=="EQL"||tmp=="LSS"||tmp=="LEQ"||tmp=="GRE"||tmp=="GEQ"||tmp=="NEQ") {
            AsCondi(tmp);
            continue;
        }
        if (tmp[0]=='@') {
            AsLabel();
            continue;
        }
        if (tmp=="JMP") {
            AsJMP();
            continue;
        }
        if (tmp=="ADD" || tmp=="SUB" || tmp=="MUL" || tmp=="DIV") {
            AsAlge();
            continue;
        }
        if (tmp=="CALL") {
            AsCall();
            continue;
        }
        if (tmp=="PUSH") {
            AsPush();
            continue;
        }
        
    }
    if (table!=NULL) AsProcEnd();
    retract=false;
    AsTail();
    OptLv1();
}
