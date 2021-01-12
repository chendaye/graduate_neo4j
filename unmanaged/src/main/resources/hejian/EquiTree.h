#pragma once
#include "EquiTrussAttribute.h"
class EquiTreeIndex
{
public:
    EquiTreeIndex() {}
    EquiTreeIndex(const EquiTrussAttribute& truss, const char* sNet_File, const char* sNetData_file, const char* sTreeAttr_file);

    int makeNum();
    bool LoadTreeGraph(const char* sNet_File, const char* sNetData_file, const char* sTreeAttr_file);
public:
    void makeSet(const EquiTrussAttribute & truss);

    void showSet();
    void showTree();

    void findSubTree(int k, set<int>& attr, const EquiTrussAttribute & truss, int num);
    
private:
    TPt <TNodeNet<TIntV> > Net;        //树结构

    TVec<int> trussness;

    int maxtness; int mintness;
    int currentid = 0;

    //这个不用本地，计算很快
    map<int, TIntV> truss2sid;        //int trussness值；tintv 具有该trussness值的超点集合

    //需要本地索引
    map<int, TIntV> truss2node; //int trussness值；tintv 具有该trussness值树节点集合

    map<int, set<Item>> tree2attr;        //int 树节点；tintv 该树节点包含的属性值，是超点属性值的并集！
protected:

};
