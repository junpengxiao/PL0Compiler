//
//  Assembly.h
//  Compiler_Version2
//
//  Created by 肖俊鹏 on 12/31/13.
//  Copyright (c) 2013 肖俊鹏. All rights reserved.
//

#ifndef __Compiler_Version2__Assembly__
#define __Compiler_Version2__Assembly__

#include <iostream>

void AsHead();
void AsTail();
void AsProcSta();
void AsProcEnd();
void AsRead(int offset);
void CalleeSta(int totOffset);
void CalleeEnd();
void CallerSta();
void CallerEnd();
void Assembly();
void AsArray();
#endif /* defined(__Compiler_Version2__Assembly__) */
