//
//  OptLv1.cpp
//  Compiler_Version2
//
//  Created by 肖俊鹏 on 1/1/14.
//  Copyright (c) 2014 肖俊鹏. All rights reserved.
//  针对汇编代码优化，类型：窥孔优化。

#include "OptLv1.h"
#include "TonyCompiler.h"
#include "string"
using namespace std;
string q[MaxL];
bool u[MaxL]={true,true,true,true};
int p=0;
int t[MaxL];
bool UpdateQ()
{
    for (int i=p;i!=MaxL;++i) if (u[i]) {
        u[i]=false;
        if (!getline(optlv1, q[i])) return false;
        cout<<q[i]<<endl;
    }
    for (int i=0;i!=p;++i) if (u[i]) {
        u[i]=false;
        if (!getline(optlv1, q[i])) return false;
        cout<<q[i]<<endl;
    }
    return true;
}
void dq()
{
    for (int i=0;i!=MaxL;++i) cout<<q[i]<<endl;
}
void Sett()
{
    for (int i=0;i!=MaxL;++i) t[i]=(p+i)%MaxL;
}
bool Rull1()//remove: same level, but using esi
{
    Sett();
    if (q[t[0]]!="\tmov esi,ebp") return false;
    if ((q[t[1]].find("\tsub esi,")==-1)&&(q[t[1]].find("\tadd esi")==-1)) return false;
    if (!isNum(q[t[1]].substr(q[t[1]].find(',')+1,q[t[1]].length()))) return false;
    if (q[t[2]].find("[esi]")==-1) return false;
    for (int i=0;i!=3;++i) u[t[i]]=true;
    p=t[2]+1;
    p%=MaxL;
    output=q[t[2]].substr(0,q[t[2]].find('['));
    output+="[ebp";
    if (~q[t[1]].find("sub")) output+='-';
    else output+='+';
    output+=q[t[1]].substr(q[t[1]].find(',')+1,q[t[1]].length());
    output+=q[t[2]].substr(q[t[2]].find(']'),q[t[2]].length());
    oufile<<output<<endl;
    return true;
}
void OptLv1()
{
    while (UpdateQ()) {
        if (Rull1()) continue;
        if (!u[p]) {
            u[p]=true;
            oufile<<q[p++]<<endl;
            p%=MaxL;
        }
    }
    for (int i=p;i!=MaxL;++i) if (!u[i]) {
        u[i]=true;
        oufile<<q[i]<<endl;
    }
    for (int i=0;i!=p;++i) if (!u[i]) {
        u[i]=true;
        oufile<<q[i]<<endl;
    }
    oufile.close();
}