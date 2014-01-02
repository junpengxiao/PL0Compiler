#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include<iostream>
#include<string>
#include<stack>
#include "TonyCompiler.h"
enum SymbolKind {
	constsk,varsk,parask,funcsk,procsk
};
enum SymbolType {
	integerst,charst,stringst,arrOfInt,arrOfCh
};
class SymbolItem {
private:
	std::string name;
	SymbolKind kind;
	SymbolType type;
	int value,size;
	std::string strvalue;
	int offset;
public:
	SymbolItem(std::string name,SymbolKind kind,SymbolType type,int value);
	SymbolItem(std::string name,SymbolKind kind,SymbolType type,std::string strvalue);
    SymbolItem();
    ~SymbolItem();
	void SetOffset(int offset);
	void SetType(SymbolType type);
	void SetValue(int value);
	void SetValue(std::string strvalue);
	int GetOffset();
    std::string GetName();
	SymbolKind GetKind();
	SymbolType GetType();
	int GetValue();
	std::string GetStrvalue();
	int GetSize();
    void SetName(std::string name);
    void SetKind(SymbolKind kind);
    void SetSize(int size);
	void UpdateSize();
    void UpdateSize(int n);
};
class SymbolTable {
private:
	int level,size,paraNum;
    bool setedvalue,setedtype;
	SymbolTable* father;
	SymbolItem table[MaxN];
    int ptable;
	SymbolType type,functype;
	int value,label;
	std::string strvalue;
    std::string name;
    SymbolKind kind;
public:
	SymbolTable(int level,SymbolTable* father,std::string name,SymbolKind kind);
    SymbolTable();
	bool CheckingExist(std::string name);
    ~SymbolTable();
	SymbolTable* GetFather();
	int GetLevel();
	void Push(std::string name,SymbolKind kind,SymbolType type,int value);
	void Push(std::string name,SymbolKind kind,SymbolType type,std::string value);
	void Push(std::string name,SymbolKind kind);
    void push_back(SymbolItem item);
	void Pop();//Bounder checking
	SymbolItem* Top();//Bounder checking
	void SetType(SymbolType type);
	void SetValue(int value);
	void SetValue(std::string strvalue);
    void SetTypeValue(SymbolType type,int value);
    void SetFuncType(SymbolType type);
    void SetLevel(int level);
    void SetName(std::string str);
    void SetFather(SymbolTable* father);
    void SetKind(SymbolKind kind);
    void SetTypeValue(SymbolType type,std::string value);
    void ReInitial(int level,SymbolTable* father,std::string str,SymbolKind kind);
	void FillBack();
	void SetParaNum(int num);
    int GetSize();
    int GetParaNum();
    SymbolType GetFuncType();
    int GetOffset(std::string name);
    int GetIndex(std::string name);
    int GetTail();
    std::string GetName();
    SymbolKind GetKind();
    SymbolItem* GetItem(std::string name);
    SymbolItem* GetItem(int index);
    void UpdateSize();
    void DisplayTable();
};
extern SymbolTable symbolTableList[MaxN];
extern int ptable;
#endif
