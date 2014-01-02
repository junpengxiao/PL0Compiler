//
//  main.cpp
//  Compiler_Version2
//
//  Created by 肖俊鹏 on 11/29/13.
//  Copyright (c) 2013 肖俊鹏. All rights reserved.
//

#include <iostream>
#include "TonyCompiler.h"
#include "Assembly.h"
//Define variables

int main(int argc, const char * argv[])
{
    std::string tmp;
    std::cin>>tmp;
    infile.close();
    infile.clear();
    infile.open(tmp);
    GetNextSym();
    Program();
    Assembly();
    return 0;
}

