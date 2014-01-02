//
//  Parser.cpp
//  Compiler_Version2
//
//  Created by 肖俊鹏 on 11/29/13.
//  Copyright (c) 2013 肖俊鹏. All rights reserved.
//

#include "Parser.h"
#include "SymbolTable.h"
#include "Emit.h"
#include <string>
using namespace std;
SymbolTable symbolTableList[MaxN];
int ptable=0;
int tempGenerator(0);
string GetArrayOffset(SymbolTable &table)
{
    Emit tmp1,tmp2;
    string desttmp;
    tmp1.SetS1(token);
    tmp1.SetTP(assignet);
    GetNextSym();
    if (symbol!=LBRACK) ErrorHandler("At array offset need [");//Error need a [
    GetNextSym();
    tmp2.SetTP(assignet);
    tmp2.SetS1(Expression(table));
    desttmp=NextTempName(table);
    tmp2.SetD1(desttmp);
    tmp2.emitResult();
    if (symbol!=RBRACK) ErrorHandler("At Array offset need ]");//Error need a ]
    
    /*
     if (table.GetItem(tmp1.GetS1())->GetType()== arrOfInt) {
        tmp2.SetTP( multet);
        tmp2.SetS2("4");
        desttmp = NextTempName(table);
        tmp2.SetD1(desttmp);
        tmp2.emitResult();
        tmp1.SetS2(desttmp);
    } else tmp1.SetS2(tmp2.GetS1());
     */
    tmp1.SetS1(tmp1.GetS1()+"["+desttmp+"]");
    return tmp1.GetS1();
}
bool CheckItem(string str,SymbolKind kind,SymbolType type,SymbolTable &table)
{
    SymbolItem* item = table.GetItem(str);
    if (item->GetKind()== parask && kind== varsk) return true;//if var is para then return true
    if (kind== varsk && table.GetName()==str && table.GetKind()== funcsk) return true;//if var is the name of this table, and current table is a function then return true.
    if (item->GetKind()!=kind) return false;
    if (item->GetType()!=type) return false;
    return true;
}
bool CheckItem(string str,SymbolKind kind,SymbolTable &table)
{
    SymbolItem* item = table.GetItem(str);
    if (item->GetKind()!=kind) return false;
    return true;
}
bool CheckItem(string str,bool isArray,SymbolTable &table)
{
    SymbolItem* item = table.GetItem(str);
    if (isArray) return (item->GetKind()== varsk && IsArrayVar(*item));
    return false;
}
bool IsBasicVar(SymbolItem &item,SymbolTable &table){
    bool rect=false;
    rect=(item.GetKind()== varsk || item.GetKind()== parask);
    rect=rect&&(item.GetType()== charst || item.GetType()== integerst);
    rect = rect ||(table.GetKind()== funcsk && table.GetName()==item.GetName() && TouchNextChar()!='(');
    return rect;
}
bool IsArrayVar(SymbolItem &item){
    return (item.GetType()== arrOfCh || item.GetType()== arrOfInt);
}
bool IsFunctVar(SymbolItem &item) {
    return (item.GetKind()== funcsk && TouchNextChar()=='(');
}
bool IsProceVar(SymbolItem &item){
    return (item.GetKind()==procsk && TouchNextChar()=='(');
}
string IntToString(int x){
    string tmp="",rect="";
    if (x<0) rect="-";
    do {
        tmp+=('0'+x%10);
        x/=10;
    }while(x);
    for (int i=0;i!=tmp.length();++i) rect+=tmp[tmp.length()-i-1];
    return rect;
}
SymbolTable* GetTableByName(string name)
{
    for (int i=0;i!=ptable;++i) if (symbolTableList[i].GetName()==name) return &(symbolTableList[i]);
    //Error can not find table of a certain name
    return NULL;
}
string NextTempName()
{
    string rect="ttemp";
    return rect+IntToString(tempGenerator++);
}
string NextTempName(SymbolTable &table)
{
    string rect="ttemp";
    rect=rect+IntToString(tempGenerator++);
    table.Push(rect, varsk);
    table.SetType(charst);
    table.FillBack();
    return rect;
}
void DebugPrintListNum()
{
    cout<<ptable<<endl;
}
void Program()
{
    symbolTableList[ptable].ReInitial(1, NULL, "main", procsk);
    symbolTableList[ptable].SetParaNum(0);
    ptable++;
    SubProgram(symbolTableList[ptable-1]);
    if (symbol!=PERIOD) ErrorHandler("Program should end with .");//Throw program not end
}
void SubProgram(SymbolTable &table)
{
    if (symbol==CONSTTK) ConstDeclaration(table);
    if (symbol==VARTK) VarDeclaration(table);
    while (symbol==PROCETK || symbol==FUNCTK) {
        if (symbol==PROCETK) ProcDeClaration(table);
        else if (symbol==FUNCTK) FuncDeClaration(table);
    }
    if (symbol!=BEGINTK) ErrorHandler("SubProgram should begin with begin"); //Throw Error while define Subprogram, can not find begin tk.
    if (table.GetKind()==procsk) interResult<<"Proc ";
    if (table.GetKind()==funcsk) interResult<<"Func ";
    interResult<<table.GetName();
    for (int i=0;i!=ptable;++i) if ((&symbolTableList[i])==&table) {
        interResult<<' '<<i<<endl;
        break;
    }
    CompoundStatement(table);
    table.Push(table.GetName(), varsk);
    table.SetType(table.GetFuncType());
    table.FillBack();
}
void ConstDeclaration(SymbolTable &table)
{
    if (symbol!=CONSTTK) ErrorHandler("const declaration should begin with const");//Throw Error while define const list, can not find consttk head
    GetNextSym();
    ConstDefinition(table);
    while (symbol!=SEMICN) {
        GetNextSym();
        ConstDefinition(table);
    }
    GetNextSym();
}
void ConstDefinition(SymbolTable &table)
{
    if (symbol!=IDEN) ErrorHandler("Const defination shoule begin with iden");//Throw Error while define const value
    table.Push(token,  constsk);
    GetNextSym();
    if (symbol!=EQL) ErrorHandler("Const defination should use = as symbol");//Throw Error while define const value
    GetNextSym();
    if (symbol==CHARCON) {
      table.SetTypeValue( charst, (int)token[0]);
      table.FillBack();
      GetNextSym();
      return;
    }
    if (symbol==MINU) {
        GetNextSym();
        number*=-1;
    } else if (symbol==PLUS) GetNextSym();
    if (symbol!=INTCON) ErrorHandler("Const should be integer or char");//throw Error while define int const
    table.SetTypeValue( integerst, number);
    table.FillBack();
    GetNextSym();
}
void VarDeclaration(SymbolTable &table)
{
    if (symbol!=VARTK) ErrorHandler("var declaration should begin with car"); //Error while define var
    GetNextSym();
    VarDefinition(table);
    while (symbol==SEMICN) {
        GetNextSym();
        if (symbol!=IDEN) break;
        VarDefinition(table);
    }
}
void VarDefinition(SymbolTable &table)
{
    if (symbol!=IDEN) ErrorHandler("Error defination shold begin with iden");//Throw Error while define var, expect iden
    table.Push(token,  varsk);
    do {
        GetNextSym();
        if (symbol==COLON) break;
        GetNextSym();
        if (symbol!=IDEN) ErrorHandler("Error defination should begin with iden"); //throw error while define var
        table.Push(token,  varsk);
    } while (true);
    GetNextSym();
    if (symbol==INTTK) table.SetType( integerst);
    if (symbol==CHARTK) table.SetType( charst);
    //table.SetKind(varsk);
    if (symbol==ARRAYTK) {
        GetNextSym();
        if (symbol!=LBRACK) ErrorHandler("Error while define arr,miss [");//Throw Error expect [
        GetNextSym();
        if (symbol!=INTCON) ErrorHandler("[k] k should be number");// Throw Error expect number
        table.SetTypeValue( arrOfCh, number);
        GetNextSym();
        if (symbol!=RBRACK) ErrorHandler("define arr, miss ]");//Throw Error expect ]
        GetNextSym();
        if (symbol!=OFTK) ErrorHandler("define arr miss of");//Throw Error expect of
        GetNextSym();
        if (symbol!=INTTK && symbol!=CHARTK) ErrorHandler("define arr miss char or integer"); //Throw error expect char or integer
        if (symbol==INTTK) table.SetType( arrOfInt);
    }
    table.FillBack();
    GetNextSym();
}
void ProcDeClaration(SymbolTable &table)
{
    int backup = (int)ptable; // Backup, for updating the size of this table later
    ProcHeader(table);
    SubProgram(symbolTableList[ptable-1]);
    symbolTableList[backup].UpdateSize();
    //table.Top()->UpdateSize(symbolTableList[backup].GetSize());
    table.Top()->UpdateSize(4);
    if (symbol!=SEMICN) ErrorHandler("proc shold end with '"); //Error procedure should end with ;
    GetNextSym();
}
void ProcHeader(SymbolTable &table)
{
    if (symbol!=PROCETK) ErrorHandler("Proc should begin with procedure");//Error procedure should begin with procedure
    GetNextSym();
    if (symbol!=IDEN) ErrorHandler("procedue should have iden as name");//Error procedure should have iden
    symbolTableList[ptable].ReInitial(table.GetLevel()+1,&table,token, procsk);
    ptable++;
    table.Push(token,  procsk);
    GetNextSym();
    if (symbol!=LPARENT) ErrorHandler("proc define should have (");//Error procedure should have (
    int num=0;//The number of para
    GetNextSym();
    if (symbol!=RPARENT) num=GetParaList(symbolTableList[ptable-1]);
    symbolTableList[ptable-1].SetParaNum(num);
    table.SetTypeValue( integerst, num);
    table.FillBack();
    if (symbol!=RPARENT) ErrorHandler("Proc define should have )") ;//Error procedure should have )
    GetNextSym();
    if (symbol!=SEMICN) ErrorHandler("Proc head def should end with ;");//Error Procedure should end with ;
    GetNextSym();
}
int GetParaList(SymbolTable &table)
{
    bool ispara=false;
    int num=0;//The number of para
    if (symbol==VARTK) {
        ispara=true;
        GetNextSym();
    }
    do {
        if (symbol!=IDEN) ErrorHandler("paralist should split vars via ,");//Error while define paralist ,should have iden
        if (ispara) table.Push(token,  parask);
        else table.Push(token,  varsk);
        num++;
        GetNextSym();
        if (symbol==COLON) break;
        if (symbol!=COMMA) ErrorHandler("para list should split vars via ,");//Error iden should be split by comma
        GetNextSym();
    } while (true);
    GetNextSym();
    if (symbol==INTTK) table.SetType( integerst);
    else if (symbol==CHARTK) table.SetType( charst);
    else ;//Error should have basic type
    table.FillBack();
    GetNextSym();
    if (symbol==SEMICN) {
        GetNextSym();
        num+=GetParaList(table);
    }
    return num;
}
void FuncDeClaration(SymbolTable &table)
{
    int backup = (int)ptable;
    FuncHeader(table);
    SubProgram(symbolTableList[ptable-1]);
    symbolTableList[backup].UpdateSize();
    //table.Top()->UpdateSize(symbolTableList[backup].GetSize());
    table.Top()->UpdateSize(4);
    if (symbol!=SEMICN) ErrorHandler("func declaration should end with ;");//Error function should end with ;
    GetNextSym();
}
void FuncHeader(SymbolTable &table)
{
    if (symbol!=FUNCTK) ErrorHandler("func should begin with function");//Error symbol should be func
    GetNextSym();
    if (symbol!=IDEN) ErrorHandler("function should have iden as name");//Error function should have iden
    symbolTableList[ptable].ReInitial(table.GetLevel()+1,&table,token, funcsk);
    ptable++;
    table.Push(token,  funcsk);
    GetNextSym();
    if (symbol!=LPARENT) ErrorHandler("function define should have ( ");//Error function should have (
    int num=0;
    GetNextSym();
    if (symbol!=RPARENT) num=GetParaList(symbolTableList[ptable-1]);
    symbolTableList[ptable-1].SetParaNum(num);
    if (symbol!=RPARENT) ErrorHandler("function defien should have )");//Error function should have )
    GetNextSym();
    if (symbol!=COLON) ErrorHandler("function define should have :");//Error fcuntion should have return value
    GetNextSym();
    if (symbol==INTTK) table.SetType( integerst);
    else if (symbol==CHARTK) {
        table.SetType( charst);
        symbolTableList[ptable-1].SetFuncType(charst);
    }
    else ;//Error function should have return value;
    table.SetValue(num);
    table.FillBack();
    GetNextSym();
    if (symbol!=SEMICN) ErrorHandler("function define should have integer or char as type");//Error function should end with ;
    GetNextSym();
}
void CompoundStatement(SymbolTable &table)
{
    if (symbol!=BEGINTK) ErrorHandler("statement should begin with begin"); //Error statement should begin with begin
    do {
        GetNextSym();
        Statement(table);
    } while (symbol==SEMICN);
    if (symbol!=ENDTK) ErrorHandler("statement shold end with end or You miss a ';'");//Erro Statement should end with end
    GetNextSym();
}
void Statement(SymbolTable &table)
{
    switch (symbol) {
        case IFTK:
            IfStatement(table); break;
        case REPTTK:
            RepeatStatement(table); break;
        case BEGINTK:
            CompoundStatement(table); break;
        case READTK:
            ReadStatement(table); break;
        case WRITETK:
            WriteStatement(table); break;
        case FORTK:
            ForStatement(table); break;
        case ENDTK: break;
        case IDEN:
            if (!IsFunctVar(*table.GetItem(token)) && !IsProceVar(*table.GetItem(token))) AssignStatement(table);
            else FunctionCall(table);
            break;
        default: break;//Warning : find empty statement. programer should comnfirm twice.
    }
}
void AssignStatement(SymbolTable &table)
{
    Emit tmp;
    tmp.SetTP( assignet);
    if (symbol!=IDEN) ErrorHandler("Assign should have iden to be assigned");//Error assign should have iden
    SymbolItem* item = table.GetItem(token);
    if (item==NULL && token!=table.GetName()) ErrorHandler("iden not exist while assign");//Error iden not exist;
    if (IsBasicVar(*item, table)) tmp.SetD1(token);
    else if (IsArrayVar(*item)) tmp.SetD1(GetArrayOffset(table));
    else ;//Error This must be somthing I don't expect.
    GetNextSym();
    if (symbol!=ASSIGN) ErrorHandler("assign must have :=") ;//Error should be assign
    GetNextSym();
    tmp.SetS1(Expression(table));
    tmp.emitResult();
}
string Expression(SymbolTable &table)
{
    string desttmp;
    Emit tmp;
    if (symbol!=MINU) {
        if (symbol==PLUS) GetNextSym();
        desttmp=Term(table);
    } else {
        tmp.SetTB(&table);
        desttmp=NextTempName(table);
        tmp.SetTP( negiet);
        GetNextSym();
        tmp.SetD1(desttmp);
        tmp.SetS1(Term(table));
        tmp.emitResult();
    }
    while (symbol==PLUS || symbol==MINU) {
        if (symbol==PLUS) tmp.SetTP(addet);
        if (symbol==MINU) tmp.SetTP(subet);
        GetNextSym();
        tmp.SetS1(desttmp);
        desttmp = NextTempName(table);
        tmp.SetS2(Term(table));
        tmp.SetD1(desttmp);
        tmp.emitResult();
    }
    return desttmp;
}
string Term(SymbolTable &table)
{
    Emit tmp;
    tmp.SetTB(&table);
    string desttmp=Factor(table);
    while (symbol==MULT || symbol==DIV) {
        if (symbol==MULT) tmp.SetTP( multet);
        if (symbol==DIV) tmp.SetTP( divet);
        GetNextSym();
        tmp.SetS2(Factor(table));
        tmp.SetS1(desttmp);
        desttmp = NextTempName(table);
        tmp.SetD1(desttmp);
        tmp.emitResult();
    }
    return desttmp;
}
string Factor(SymbolTable &table)
{
    string rect="",desttmp;
    Emit tmp;
    tmp.SetTB(&table);
    if (symbol==INTCON) {
        rect=token;
        GetNextSym();
        return rect;
    }
    if (symbol==CONSTTK) {
        ErrorHandler("Can not use char as factor");
    }
    if (symbol==LPARENT)
    {
        GetNextSym();
        rect=Expression(table);
        if (symbol!=RPARENT) ErrorHandler("if factor start with ( then it should end with )");//Error should have )
        GetNextSym();
        return rect;
    }
    if (symbol==IDEN){
        SymbolItem* item = table.GetItem(token);
        bool isfunc=false;//if it is func then should not get next symbol
        if (item==NULL) ;//Error
        if (IsArrayVar(*item)) desttmp=GetArrayOffset(table);
        else if (IsFunctVar(*item)) {
            desttmp=FunctionCall(table);
            isfunc=true;
        }
        else if (IsBasicVar(*item, table)) desttmp=token;
        else if (item->GetKind()==constsk) desttmp=token;
        else ;//Error do not know what it is
        if (!isfunc) GetNextSym();
        return desttmp;
    }
    //Error is not a factor
    return "ERROR";
}
string FunctionCall(SymbolTable &table)
{
    Emit tmp;
    SymbolTable* targetfunc=NULL;
    for (int i=0;i!=ptable;++i) if (symbolTableList[i].GetFather()==&table&&symbolTableList[i].GetName()==token) {
        targetfunc=&symbolTableList[i];
        break;
    }
    tmp.SetTB(targetfunc);
    string desttmp;
    tmp.SetS1(token);
    GetNextSym();
    if (symbol!=LPARENT) ErrorHandler("function call should have (");//Error function call should have (
    SymbolTable* target = GetTableByName(tmp.GetS1());
    SymbolItem* item = table.GetItem(tmp.GetS1());
    GetNextSym();
    CheckParalist(*target,item->GetValue(),table);
    tmp.SetTP( callet);
    tmp.SetS2(IntToString(item->GetValue()));
    desttmp = NextTempName(table);
    tmp.SetD1(desttmp);
    tmp.emitResult();
    return desttmp;
}
void CheckParalist(SymbolTable &table,int num,SymbolTable &callTable)
{
    Emit tmp;
    SymbolItem* item;
    for (int i=0;i!=num;++i) {
        item=table.GetItem(i);
        if (symbol==RPARENT) ErrorHandler("function call doesn't fit defination");//Error paralist doesn't fit defination
        if (item->GetKind()== parask) {
            SymbolItem* tmpitem=callTable.GetItem(token);
            if (!(IsBasicVar(*tmpitem, callTable) || IsArrayVar(*tmpitem))) ErrorHandler("var pass address not expression");//Error var should pass address
            if (IsBasicVar(*tmpitem, callTable)) tmp.SetD1(token);
            else if (IsArrayVar(*tmpitem)) tmp.SetD1(GetArrayOffset(callTable));
            else ;//Error var pare should have iden to be parameter
            GetNextSym();
            if (symbol!=COMMA && symbol!=RPARENT) ErrorHandler("para not fit defination");//Error para should have iden yo be parameter
            tmp.SetTP( pushet);
            tmp.SetD1(tmp.GetD1()+"[");
            tmp.emitResult();
            GetNextSym();
            continue;
        }
        tmp.SetTP( pushet);
        tmp.SetD1(Expression(callTable));
        tmp.emitResult();
        if (symbol!=COMMA && symbol!=RPARENT) ErrorHandler("para not fit defination");//Error para doesn't fit the defination
        GetNextSym();
    }
    if (num==0) {
        GetNextSym();
    }
}
void IfStatement(SymbolTable &table)
{
    if (symbol!=IFTK) ErrorHandler("if tk should begin with if");//Error if statement should begin with if
    GetNextSym();
    string desttmp = Condition(table);
    if (symbol!=THENTK) ErrorHandler("if should have then tk");//Error if should have then tk
    Emit tmp;
    string thenlabel = "@" + NextTempName(table);
    string elselabel = "@" + NextTempName(table);
    string afterlabel= "@" + NextTempName(table);
    tmp.SetTP( jzet);
    tmp.SetS1(desttmp);
    tmp.SetD1(thenlabel);
    tmp.emitResult();
    tmp.SetTP( jmpet);
    tmp.SetD1(elselabel);
    tmp.emitResult();
    interResult<<thenlabel<<endl; //print out label
    GetNextSym();
    Statement(table);
    tmp.SetTP( jmpet);
    tmp.SetD1(afterlabel);
    tmp.emitResult();
    if (symbol==ELSETK) {
        interResult<<elselabel<<endl;//print else label
        GetNextSym();
        Statement(table);
        interResult<<afterlabel<<endl;
    } else {
        interResult<<elselabel<<endl;
        interResult<<afterlabel<<endl;
    }
}
string Condition(SymbolTable &table)
{
    Emit tmp;
    tmp.SetS1(Expression(table));
    switch (symbol) {
        case LSS:
            tmp.SetTP( lsset);
            break;
        case LEQ:
            tmp.SetTP( leqet);
            break;
        case GRE:
            tmp.SetTP( greet);
            break;
        case GEQ:
            tmp.SetTP( geqet);
            break;
        case EQL:
            tmp.SetTP( eqlet);
            break;
        case NEQ:
            tmp.SetTP( neqet);
            break;
        default:
            //Error expect a symbol
            break;
    }
    GetNextSym();
    tmp.SetS2(Expression(table));
    tmp.SetD1(NextTempName(table));
    tmp.emitResult();
    return tmp.GetD1();
}
void RepeatStatement(SymbolTable &table)
{
    if (symbol!=REPTTK) ErrorHandler("repear should begin with repeat");//Error repeat should begin with repeat
    string labelsta = "@"+NextTempName(table);
    string labelend = "@"+NextTempName(table);
    interResult<<labelsta<<endl;
    GetNextSym();
    Statement(table);
    if (symbol!=UNLTK) ErrorHandler("repeat should have until");//Error should end with until;
    GetNextSym();
    string desttmp = Condition(table);
    Emit tmp;
    tmp.SetTP( jzet);
    tmp.SetS1(desttmp);
    tmp.SetD1(labelend);
    tmp.emitResult();
    tmp.SetTP(jmpet);
    tmp.SetD1(labelsta);
    tmp.emitResult();
    interResult<<labelend<<endl;
}
void ReadStatement(SymbolTable &table)
{
    Emit tmp;
    if (symbol!=READTK) ErrorHandler("read should begin with read");//Error should begin with read
    GetNextSym();
    if (symbol!=LPARENT) ErrorHandler("read should have (");//Error read should have (
    GetNextSym();
    if (symbol!=IDEN) ErrorHandler("read should have at least 1 iden");//Error read should read as least 1 iden
    while (symbol!=RPARENT) {
        if (symbol==LBRACK || symbol==LBRACE || symbol==LPARENT) ErrorHandler("according to syntax, can not read into array or function or procedure");
        if (!IsBasicVar(*table.GetItem(token), table)) ErrorHandler("should read at least 1 symbol");//Error should be iden recorded in symbol list
        //if (table.GetName()==token) ;//Error can not read vaule to a function !!!! This is very important....can function symbol be read when excute this function?
        tmp.SetTP( readet);
        tmp.SetD1(token);
        tmp.emitResult();
        GetNextSym();
        if (symbol==COMMA) GetNextSym();
        if (symbol!=IDEN && symbol!=RPARENT) ErrorHandler("Error while reading");//Error something unknown in read statement
    }
    GetNextSym();
}
void WriteStatement(SymbolTable &table)
{
    Emit tmp;
    if (symbol!=WRITETK) ErrorHandler("write should begin with write");//Error write should begin with write
    GetNextSym();
    if (symbol!=LPARENT) ErrorHandler("write should have (");//Error write list should begin with (
    GetNextSym();
    if (symbol==STRCON) {
        tmp.SetTP( writeet);
        tmp.SetD1("$"+token);//I will use $ to denote it's a string
        tmp.emitResult();
        GetNextSym();
        if (symbol==COMMA) {
            GetNextSym();
            tmp.SetTP( writeet);
            if (TouchNextChar()==')') {
                tmp.SetD1(token);
                tmp.emitResult();
                GetNextSym();
            } else {
                tmp.SetD1(Expression(table));
                tmp.emitResult();
            }
        }
        if (symbol!=RPARENT) ErrorHandler("write should end with )");//Error Should end with )
        GetNextSym();
        return;
    }
    tmp.SetTP( writeet);
    if (TouchNextChar()==')') {
        tmp.SetD1(token);
        tmp.emitResult();
        GetNextSym();
    } else {
        tmp.SetD1(Expression(table));
        tmp.emitResult();
    }
    if (symbol!=RPARENT) ErrorHandler("write should end with )");//Error should end with )
    GetNextSym();
}
void ForStatement(SymbolTable &table)
{
    Emit tmp;
    if (symbol!=FORTK) ErrorHandler("for should begin with for");//Error for statement should begin with for
    GetNextSym();
    if (symbol!=IDEN) ErrorHandler("for should have iden as i");//Error for should have an iden to be iterator
    if (!IsBasicVar(*table.GetItem(token), table)) ErrorHandler("iden in for not exist");//Error for iden isn't int the table
    //if (token==table.GetName()) ;//Error function name can not be a iterator. Ok..I will use function name to be a variable
    tmp.SetTP( assignet);
    tmp.SetD1(token);
    string iter(token);
    GetNextSym();
    if (symbol!=ASSIGN) ErrorHandler("for should have :=") ;//Error iden should be assign
    GetNextSym();
    tmp.SetS1(Expression(table));
    tmp.emitResult();
    if (symbol!=TOTK && symbol!=DOWNTOTK) ErrorHandler("for should have to downto");//Error for should have to/downto
    string val = (symbol==TOTK?"1":"-1");
    GetNextSym();
    string cond = Expression(table);
    string labelbegin = "@" + NextTempName(table);
    string labelend = "@" +NextTempName(table);
    interResult<<labelbegin<<endl;
    if (val=="1") tmp.SetTP(greet);
    else tmp.SetTP(lsset);
    tmp.SetS2(cond);
    string desttmp = NextTempName(table);
    tmp.SetS1(tmp.GetD1());
    tmp.SetD1(desttmp);
    tmp.emitResult();
    tmp.SetTP( jzet);
    tmp.SetS1(desttmp);
    tmp.SetD1(labelend);
    tmp.emitResult();
    if (symbol!=DOTK) ErrorHandler("for should have do");//Error for should have do
    GetNextSym();
    Statement(table);
    tmp.SetTP(addet);
    tmp.SetS1(iter);
    tmp.SetS2(val);
    tmp.SetD1(iter);
    tmp.emitResult();
    tmp.SetTP( jmpet);
    tmp.SetD1(labelbegin);
    tmp.emitResult();
    interResult<<labelend<<endl;
}







