#include<stack>
#include<iostream>
#include <fstream>
#include<string>
#include "SymbolTable.h"
using namespace std;



SymbolItem::SymbolItem(std::string name,SymbolKind kind,SymbolType type,int value)
{
	this->name = name;
	this->kind = kind;
	this->type = type;
	this->value = value;
	this->offset = 0;
}
SymbolItem::SymbolItem(std::string name,SymbolKind kind,SymbolType type,string strvalue)
{
	this->name = name;
	this->kind = kind;
	this->type = type;
	this->strvalue = strvalue;
	this->offset = 0;
}
SymbolItem::SymbolItem()
{
    this->kind= constsk;
    this->name="";
    this->offset=0;
    this->size=0;
    this->strvalue="";
    this->type=integerst;
    this->value=0;
}
SymbolItem::~SymbolItem()
{
}
 void SymbolItem::SetOffset(int offset)
{
	this->offset = offset;
}
 int SymbolItem::GetOffset()
{
	return this->offset;
}
std::string SymbolItem::GetName()
{
	return this->name;
}
SymbolKind SymbolItem::GetKind()
{
	return this->kind;
}
 SymbolType SymbolItem::GetType()
{
	return this->type;
}
 int SymbolItem::GetValue()
{
	if (this->type != stringst) return this->value;
	//Throw Error of Trying to get integer value of a string object
    return 0;
}
 std::string SymbolItem::GetStrvalue()
{
	//if (this->type != stringst) ErrorHandler("this symbol is integer, but you try to read it as string");//throw try to get a sting value in integer value
	return this->strvalue;
}
 void SymbolItem::SetType(SymbolType type)
{
	this->type = type;
}
 int SymbolItem::GetSize()
{
	return this->size;
}
void SymbolItem::UpdateSize()
{
	if (type == integerst) this->size=4;
	if (type == charst) this->size=4;
	if (type == arrOfInt) this->size=this->value*4;
	if (type == arrOfCh) this->size=this->value*4;
	if (type == stringst) this->size=0;
}
void SymbolItem::UpdateSize(int n)
{
    this->size=n;
}
 void SymbolItem::SetValue(int value)
{
	if (this->kind!=constsk && this->type!=arrOfInt && this->type!=arrOfCh) ;//Warning : May assign value to a var
	this->value = value;
}
 void SymbolItem::SetValue(std::string strvalue)
{
	if (this->kind!=constsk) ;//Warning May assign a string to UN-string type;
	this->strvalue = strvalue;
}
void SymbolItem::SetName(string name)
{
    this->name=name;
}
void SymbolItem::SetKind(SymbolKind kind)
{
    this->kind=kind;
}
void SymbolItem::SetSize(int size)
{
    this->size=size;
}
SymbolTable::SymbolTable(int level,SymbolTable* father,string name,SymbolKind kind)
{
	this->level = level;
	this->father = father;
    this->kind = kind;
	if (level==0) this->father=NULL;
	this->label = 0;
    this->size = 0;
    this->name = name;
    this->setedtype=false;
    this->setedvalue=false;
    this->ptable=0;
    this->functype=integerst;
}
SymbolTable::SymbolTable()
{
    this->father=NULL;
    this->kind=procsk;
    this->functype=integerst;
    this->label=0;
    this->level=0;
    this->name="";
    this->paraNum=0;
    this->ptable=0;
    this->setedtype=false;
    this->setedvalue=false;
    this->size=0;
    this->strvalue="";
    this->type=integerst;
    this->value=0;
}
SymbolTable::~SymbolTable()
{
}
 int SymbolTable::GetLevel()
{
	return level;
}
 void SymbolTable::Push(std::string name,SymbolKind kind)
{
	if (!this->CheckingExist(name)) ErrorHandler("name already exist");//Throw name exists.
        SymbolItem temp(name,kind,integerst,0);
	this->push_back(temp);
}
 void SymbolTable::Push(std::string name,SymbolKind kind,SymbolType type,int value)
{
	if (!this->CheckingExist(name)) ErrorHandler("name already exist");//Throw name exists.
	SymbolItem temp(name,kind,type,value);
	this->push_back(temp);
} void SymbolTable::Push(std::string name,SymbolKind kind,SymbolType type,string strvalue)
{
	if (!this->CheckingExist(name)) ErrorHandler("name already exist");//Throw name exists.
	SymbolItem temp(name,kind,type,strvalue);
	this->push_back(temp);
}
 void SymbolTable::Pop()
{
	if (this->ptable==0) ErrorHandler("stack is empty(symbol table"); //Throw accessing undefined memory
    ptable--;
}
 SymbolItem* SymbolTable::Top()
{
	if (this->ptable==0) ErrorHandler("symbol table is empty");//throw accessing undefined memory;
	return (&table[ptable-1]);
}
 int SymbolTable::GetOffset(string name)
{
	for (int i=0;i!=this->ptable;++i) if (this->table[i].GetName()==name) return table[i].GetOffset();
	//if (this->father!=NULL) return this->father->GetOffset(name);
	//Throw can not find this name
    SymbolTable* table=this->father;
    int rect=-1;
    while (table->CheckingExist(name)) {
        rect--;
        if (table!=NULL) table=table->father;
        else ErrorHandler("Can not get the var offset: "+name);
    }
    return rect;
}
void SymbolTable::UpdateSize() {
    this->size=0;
    for (int i=0;i!=this->ptable;++i) size+=table[i].GetSize();
}
 void SymbolTable::SetType(SymbolType type)
{
	this->type=type;
    this->setedtype=true;
}
 void SymbolTable::SetValue(int value)
{
    this->setedvalue=true;
	this->value=value;
}
 void SymbolTable::SetValue(string strvalue)
{
	this->strvalue=strvalue;
    this->setedvalue=true;
}
 void SymbolTable::SetTypeValue(SymbolType type, int value)
{
    this->SetType(type);
    this->SetValue(value);
}
 void SymbolTable::SetTypeValue(SymbolType type, string value)
{
    this->SetType(type);
    this->SetValue(value);
}
void SymbolTable::SetParaNum(int num) {
    this->paraNum = num;
}

 void SymbolTable::FillBack()
{
    if (!setedtype) ErrorHandler("should set type before fill back");//Should set type before fill back
    for (;this->label != this->ptable;++(this->label))
	{
		this->table[label].SetType(this->type);
		if (this->type==stringst) this->table[label].SetValue(this->strvalue);
		else this->table[label].SetValue(this->value);
		if (label==0) table[label].SetOffset(4);
		else table[label].SetOffset(table[label-1].GetOffset()+table[label-1].GetSize());
		table[label].UpdateSize();
	}
    this->UpdateSize();
    setedtype=false;
    setedvalue=false;
}
void SymbolTable::ReInitial(int level,SymbolTable* father,std::string str,SymbolKind kind)
{
    SetLevel(level);
    SetFather(father);
    SetName(str);
    SetKind(kind);
}
 int SymbolTable::GetSize()
{
    return this->size;
}
bool SymbolTable::CheckingExist(string name) {
    for (int i=0;i!=this->ptable;++i) if (table[i].GetName()==name) return false;
    return true;
}
SymbolKind SymbolTable::GetKind() {
    return (this->kind);
}
SymbolItem* SymbolTable::GetItem(int i) {
    if (i<0) ErrorHandler("get item [i] i should >0");//Error index of get item should bigger than 1
    if (i>=this->ptable) ErrorHandler("get item [i] , ishould <ptable");//Error index overflow
    return (&(this->table[i]));
}
SymbolItem* SymbolTable::GetItem(string name) {
    for (int i=0;i!=this->ptable;++i) if (this->table[i].GetName()==name) return (&(this->table[i]));
    if (this->father!=NULL) return (this->father->GetItem(name));
    ErrorHandler("Can not find the var named "+name);
    return NULL ; //Error can not find a symbol named name...
}
string SymbolTable::GetName() {
    return this->name;
}
SymbolTable* SymbolTable::GetFather() {
    return this->father;
}
int SymbolTable::GetParaNum(){
    return  this->paraNum;
}
int SymbolTable::GetIndex(string name)
{
    for (int i=0;i!=ptable;++i) if (table[i].GetName()==name) return i;
    ErrorHandler("Can not find "+name);
    return -1;
}
void SymbolTable::DisplayTable() {
    cout<<"---------------------DisplayTable "<<this->name<<" -----------------------------"<<endl;
    cout<<"Level: "<<level<<"\t\tSize: "<<size<<"\t\tNum of para: "<<paraNum<<endl<<"\tFunctype: "<<this->functype<<endl;
    cout<<"Father: "<<(father==NULL?"NULL":father->GetName())<<"\t\tIs Func? "<< (kind==funcsk?"Yes":"No")<<"\t\tNum of Item: "<<this->ptable<<endl;
    for (int i=0;i!=this->ptable;++i) {
	cout<<"Item: "<<table[i].GetName()<<"    Kind: ";
	switch (table[i].GetKind()) {
	    case constsk: cout<<"const"; break;
	    case varsk: cout<<"var"; break;
	    case parask: cout<<"para"; break;
	    case funcsk: cout<<"funct"; break;
	    case procsk: cout<<"proce"; break;
	}
	cout<<"    Type: ";
	switch (table[i].GetType()) {
	    case integerst: cout<<"int"; break;
	    case charst: cout<<"char"; break;
	    case stringst: cout<<"string"; break;
	    case arrOfInt: cout<<"array of int"; break;
	    case arrOfCh: cout<<"array of char"; break;
	}
	if (table[i].GetType()==stringst) cout<<"    Value: "<<table[i].GetStrvalue();
	else cout<<"    Value: "<<table[i].GetValue();
	cout<<"    Size: "<<table[i].GetSize()<<"    Offset: "<<table[i].GetOffset()<<endl;
    }
}
void SymbolTable::SetName(string str)
{
    this->name=str;
}
void SymbolTable::SetLevel(int level)
{
    this->level=level;
}
void SymbolTable::SetFuncType(SymbolType type)
{
    this->functype=type;
}
void SymbolTable::SetFather(SymbolTable* father)
{
    this->father=father;
}
void SymbolTable::SetKind(SymbolKind kind)
{
    this->kind=kind;
}
SymbolType SymbolTable::GetFuncType()
{
    return this->functype;
}
int SymbolTable::GetTail()
{
    return this->ptable;
}
void SymbolTable::push_back(SymbolItem item)
{
    table[ptable].SetOffset(item.GetOffset());
    table[ptable].SetType(item.GetType());
    table[ptable].SetValue(item.GetValue());
    table[ptable].SetValue(item.GetStrvalue());
    table[ptable].SetName(item.GetName());
    table[ptable].SetSize(item.GetSize());
    table[ptable].SetKind(item.GetKind());
    ptable++;
}