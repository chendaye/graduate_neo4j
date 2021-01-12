#pragma once
#include "stdafx.h"
#include "Snap.h"
#include "dblp.h"
#include "network.h"
#include "wgtnet.h"
#include <vector>
#include <random>
#include <iostream>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <map>
#include <string>
#include <cstring>
#include <string.h>
#include <list>
#include <queue>
#include <iterator>
#include <fstream>
#include <sstream>
#include "fptree.hpp"
#define max_memory 20000000
using namespace std;

typedef pair<int, int> edgePair;    // 边对
typedef map<edgePair, int> edgeMap; // 边map容器
typedef set<pair<int, int> > SEType;// 边set容器
typedef vector<edgePair> comType;   // 社区
typedef int attrType;                // 属性关键字类型
constexpr int attrTypeFlag = 1;               // 需要加个关键字类型标记，1表示int，0表示string
//typedef PUNGraph comType;

struct SuperNode{
    int snID;     //supernode ID
    int k;          //supernode trussness
    vector<edgePair> SNEdgeList; //supernode include edge pair
};

class EquiTrussAttribute {

public:
    //int sup[105][105];
    edgeMap sup; //边的支持度
    edgeMap global_edgeTrussness; //边的trussness
    unordered_map<int, int> global_nodeTrussness;    //点的trussness
    unordered_map<int, set<int> > vertexSNIndex;
    set<pair<int, edgePair> > nonDecSup;  //非递减排序支持度，从小到大排序，前面是支持度，后面是边，利用set自动排序
    unordered_map<int, vector<attrType> > attributesOfVertex;  //顶点的属性集，修改set为vector，因为数据经过处理进行了频繁排序2018/4/27
    unordered_map<int, set<attrType> > attributesOfVertex_set; //添加set版本，这样求交集的时候就不用转换了2018/5/9
    //unordered_map<int, set<int> > attributesOfVertex_set_int;  //倒排索引2018/5/11
    //map<string, int> attributesCount;   //有多少的顶点拥有这个属性，即某个属性的数量

    vector<SuperNode> superNode;
    SEType superEdge;
    PUNGraph superGraph;
    unordered_map<int, set<attrType> > SNAttributes;  //每个超点的属性集
    set<attrType> global_psi;        //频繁集
    int global_psi_size;
    set<int> global_psi_int;        //频繁集
    //bool label = false;   //测试输出标签
    double extra_time = 0;  //读取文件的额外时间

    unordered_map<attrType, vector<int> > vertices_in_attribute;  //倒排索引顶点，可以是vector
    map<attrType, set<int> > vertices_in_supernode;             //倒排索引超点，只能是set去重
    unordered_map<int, string> attr_int2str;   //每个数字对应一个字符属性关键字
    map<int, set<int> > attr_truss;    //属性包含那些顶点之内
    //vector<int> v(2);  //这里直接分配空间是不允许的https://blog.csdn.net/ky_heart/article/details/72857012

    unordered_map<int, vector<edgePair> > attr_com;   //属性truss社团
    unordered_map<int, set<int> > attr_com_vertex;

public:
    EquiTrussAttribute();    //构造函数
    virtual ~EquiTrussAttribute() {};    //虚函数  析构函数

    void EdgeSupport(PUNGraph graph);  //求边的支持度
    int TrussDecomposition(PUNGraph graph, edgeMap &edgeTrussness, unordered_map<int, int> &nodeTrussness);        //算法1：求点和边的trussness
    edgePair EdgePair(int u, int v);    //无向图存储边的时候前面的顶点小于等于后面的顶点

    PUNGraph EquiTrussIndex(PUNGraph &graph, const char* superNode_file, const char* superEdge_file, const char* nodeTruss_file, const char* edgeTruss_file);
    bool LoadSuperGraph(const char* superNode_file, const char* superEdge_file, const char* nodeTruss_file, const char* edgeTruss_file);
    void ProcessEdge(int u, int v, int k, map<edgePair, bool> &processed, queue<edgePair> &Q, map<edgePair, vector<int> > &SNIdentifyList, int snID);
    map<int, set<pair<set<attrType>, uint64_t> > > FP_Growth(set<int> &nodes, int support, set<int> &psiInt, set<attrType> &SATR);
    map<int, set<pair<set<attrType>, uint64_t> > > FP_Growth_1(int support, set<int> &psiInt, set<attrType> &SATR);
    void Vertex_Attribute(const char* fileName, const char* attr_int2str_fileName, PUNGraph G);
    bool ContainsAll(int node);
    unordered_map<int, set<int> > VertexSNIndex();
    void GetSNAttributes();
    bool FindSubgraph(vector<edgePair> &H, PUNGraph &subgraph, int k, int q, vector<int> &meet);
    bool FindSubgraph_by_index(vector<edgePair> &H, PUNGraph &subgraph, int k, int q, vector<int> &meet);
    bool ContainsAll_by_int(int node);
    void Rand_300_Vertives(PUNGraph &graph, char* output, int max_vertex_id);
    void Find_Attribute_Truss(int attr_max_id, PUNGraph G, const char* output_file);
    void KindexInc_DFS(int psiAttrCnt, int sortedPsiAttr[], int index, int combinePsi[], int psiLevel, bool ifSelect, int k, int q, map<string, bool> &ifExistPsi);
    vector<comType > ATCIndex_Advanced(PUNGraph G, int k, int q, set<attrType> &SATR);

    //Algorithm 1: Attributed Truss Community Search (basic)  ATCBasic
    vector<comType > ATCBasic(PUNGraph graph, int k, int q, set<attrType> &SATR, vector<string> &comS);
    vector<comType > ATCBasic_1(PUNGraph graph, int k, int q, set<attrType> &SATR, vector<string> &comS);

    //Algorithm 2: FindKTrussCom
    void FindKTrussCom(PUNGraph G, int k, int q, vector<comType> &C);

    //Algorithm 3: Attributed Truss Community Search algorithm(+index)  ATCIndex
    vector<comType > ATCIndex(PUNGraph graph, int k, int q, set<attrType> &SATR);
    vector<comType > ATCIndex_1(PUNGraph graph, int k, int q, set<attrType> &SATR);
    
    //Algorithm 4: kTruss - Summary(G, vq, k)
    vector<edgePair> KTrussSummary(unordered_map<int, bool> &processed, int SN_id, int k, int q, set<attrType> &SATR);

    //Algorithm 5: Attributed Truss Community Search algorithm (+index)
    vector<comType > ATCImprove(PUNGraph graph, int k, int q, set<attrType> &SATR);

    vector<comType > KIndexDec(PUNGraph &G, int k, int q, set<attrType> &SATR, const char* load_file);
    vector<comType > KIndexInc(PUNGraph &G, int k, const int q, set<attrType> &SATR, const char* load_file);
    vector<comType > ACCore(PUNGraph G, int k, int q, set<attrType> &SATR, vector<string> &comS);
    void FindKCoreCom(PUNGraph G, int k, int q, vector<comType> &C);
    vector<comType > NCTruss(PUNGraph G, int k, int q);
    vector<edgePair> NAKTrussSummary(unordered_map<int, bool> &processed, int SN_id, int k, int q);

protected:

private:


};


