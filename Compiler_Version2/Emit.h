//
//  Emit.h
//  Compiler_Version2
//
//  Created by 肖俊鹏 on 11/29/13.
//  Copyright (c) 2013 肖俊鹏. All rights reserved.
//

#ifndef __Compiler_Version2__Emit__
#define __Compiler_Version2__Emit__

#include <iostream>
#include "TonyCompiler.h"
#include "SymbolTable.h"
enum EmitType {
    assignet,addet,negiet,subet,multet,divet,pushet,popet,callet,jmpet,jzet,
    lsset,leqet,greet,geqet,neqet,eqlet,readet,writeet,noptp,retet,endet
};
class Emit
{
private:
    EmitType type;
    std::string sour1,sour2,dest1;
    bool sets1,sets2,setd1,settp;
    SymbolTable* table;
    void UpGradeType();
    SymbolType GetType(std::string str);
    void SetType(SymbolType type);
    void Check(int num);
public:
    Emit();
    void emitResult();// if a term emit a result ,it should reset the bool label
    void SetS1(std::string str);
    void SetS2(std::string str);
    void SetTP(EmitType type);
    void SetD1(std::string str);
    void SetTB(SymbolTable* tb);
    std::string GetS1();
    std::string GetS2();
    std::string GetD1();
};
#endif /* defined(__Compiler_Version2__Emit__) */
