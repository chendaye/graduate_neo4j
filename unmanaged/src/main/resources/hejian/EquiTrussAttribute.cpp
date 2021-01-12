/***********************************************************************************************

                    Copyright(C), 2017-2018, HanKin. All rights reserved

************************************************************************************************
File Name:    EquiTrussAttribute.cpp : 定义控制台应用程序的入口点。
Version:
Author: HanKin
Created: 2017.12.01

university: WhuHan University
language: C++
other: Snap
************************************************************************************************/

#include "stdafx.h"
#include <random>
#include "EquiTrussAttribute.h"
#include <unordered_set>
#include <unordered_map>
#include "SetIter.h"

vector<int> is_meet(max_memory);

/**
*类的构造函数
*/
EquiTrussAttribute::EquiTrussAttribute()
{

}

/**
*重载==给超点结构体
*/
bool operator == (const SuperNode & A, const SuperNode & B) //重载 “==” 操作符，函数最后的 const 别忘了，否则会报错。
{
    return A.k == B.k && A.snID == B.snID && A.SNEdgeList.size() == B.SNEdgeList.size();
}

/**
*存储边时，前面的顶点大小小于后面的顶点
*/
edgePair EquiTrussAttribute::EdgePair(int u, int v)
{
    if (u < v) return make_pair(u, v);
    return make_pair(v, u);
}

/**
*判断两个属性集合中是否包含对方的属性
*/
bool EquiTrussAttribute::ContainsAll(int node)
{
    //if (v.size() < s.size()) return false;
    //set<string>::iterator its = s.begin();
    //for (; its != s.end(); its++) {
    //    string str = *its;
    //    set<string>::iterator itv = find(v.begin(), v.end(), str);
    //    if (itv == v.end()) return false;
    //}
    set<attrType> temp;
    set_intersection(attributesOfVertex_set[node].begin(), attributesOfVertex_set[node].end(), global_psi.begin(), global_psi.end(), inserter(temp, temp.begin()));
    if (temp.size() != global_psi.size()) return false;
    return true;
}

/*************************************************************************************************
函 数 名: ContainsAll_by_int
功能描述: 判断两个属性集合中是否包含对方的属性
输入参数:
输出参数:
返 回 值:
调用函数:
被调函数:
修改历史:
    1.日期: 2018年09月22日
      作者: 何健
      修改: 创建函数
*************************************************************************************************/
bool EquiTrussAttribute::ContainsAll_by_int(int node)
{
    set<int> temp;
    //set_intersection(attributesOfVertex_set_int[node].begin(), attributesOfVertex_set_int[node].end(), global_psi_int.begin(), global_psi_int.end(), inserter(temp, temp.begin()));
    if (temp.size() != global_psi.size()) return false;
    return true;
}

/*************************************************************************************************
函 数 名: SplitString
功能描述: 根据分隔符c进行字符串分割
输入参数:
输出参数:
返 回 值:
调用函数:
被调函数:
修改历史:
    1.日期: 2018年09月22日
      作者: 何健
      修改: 创建函数
*************************************************************************************************/
void SplitString(const string& s, vector<string>& v, const string& c)
{
    string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while (string::npos != pos2) {
        v.push_back(s.substr(pos1, pos2 - pos1));
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if (pos1 != s.length()) v.push_back(s.substr(pos1));
    return;
}

/*************************************************************************************************
函 数 名: SplitInt
功能描述: 根据分隔符c进行字符串分割
输入参数:
输出参数:
返 回 值:
调用函数:
被调函数:
修改历史:
    1.日期: 2019年01月19日
      作者: 何健
      修改: 创建函数
*************************************************************************************************/
void SplitInt(const string& s, vector<int>& v, const string& c)
{
    string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while (string::npos != pos2) {
        int num = stoi(s.substr(pos1, pos2 - pos1), nullptr, 10);
        v.push_back(num);
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if (pos1 != s.length()) {
        int num = stoi(s.substr(pos1), nullptr, 10);
        v.push_back(num);
    }
    return;
}

/*************************************************************************************************
函 数 名: Vertex_Attribute
功能描述: 读取属性数据集，存储顶点对应的属性，并建立属性索引
输入参数: fileName属性数据文件、G图
输出参数:
返 回 值: 无
调用函数:
被调函数:
修改历史:
    1.日期: 2018年09月22日
      作者: 何健
      修改: 创建函数
*************************************************************************************************/
void EquiTrussAttribute::Vertex_Attribute(const char* fileName, const char* attr_int2str_fileName, PUNGraph G)
{
    int attr_index = 1;
    int line = 0;
    FILE *attributeF = fopen(fileName, "r");
    while (!feof(attributeF)) {
        int node;
        char attr[20005];
        fscanf(attributeF, "%d\t%s\n", &node, &attr);

        // 这里需要修改（对于顶点伸缩2018.9.22），属性索引不应该是整个大图的点
        if (!G->IsNode(node)) continue;

        vector<attrType> vec;
        string str = string(attr);   // char转化为string
        if (str.size() == 0) continue;
        if (attrTypeFlag) SplitInt(str, vec, ","); // 好像修改容器里的类型很难。。。。
        for (int i = 0; i < vec.size(); i++) {
            attributesOfVertex[node].push_back(vec[i]);    //这个有频繁性，有重复的存储
            attributesOfVertex_set[node].insert(vec[i]);   //这个是上面的set存储
            // 如果把str转换成int（这里有将属性关键字转换成数字）有个问题：查找的时候会进行匹配，从根源修改好些
            // 关键字转化为了int，就不需要这里（20190220）
            //if (attribute_index[vec[i]] == 0) attribute_index[vec[i]] = attr_index++;

            attr_truss[vec[i]].insert(node);   //属性在哪些顶点中
            vertices_in_attribute[vec[i]].push_back(node);
            for (int sn : vertexSNIndex[node]) {
                vertices_in_supernode[vec[i]].insert(sn);
            }
        }
        //if ((line++) % 1000 == 0) printf("%d\n", line);
    }
    FILE *file = fopen(attr_int2str_fileName, "r");
    while (!feof(file)) {
        int id;
        char attr[20005];
        fscanf(file, "%s\t%d\n", &attr, &id);
        attr_int2str[id] = string(attr);
    }
    fclose(attributeF);
    fclose(file);
    //map<int, set<string> >::iterator it = attributesOfVertex.begin();
    //for (; it != attributesOfVertex.end(); it++) {
    //    printf("%d\n", it->second.size());
    //}
    return;
}

/*************************************************************************************************
函 数 名: Find_Attribute_Truss
功能描述: 对属性进行预处理，找出属性形成的truss

attr_id -1    主键
sn_size -2  超点数量
k       -3  当前超点的k
-1 -1    结束

输入参数: attr_max_id属性最大id、G图
输出参数: output_file输出文件
返 回 值: 无
调用函数: 无
被调函数: main
修改历史:
    1.日期: 2018年09月22日
      作者: 何健
      修改: 创建函数
*************************************************************************************************/
void EquiTrussAttribute::Find_Attribute_Truss(int attr_max_id, PUNGraph G, const char* output_file)
{
    ifstream file(output_file);
    if (file) return;   //文件存在就不需要继续处理了

    clock_t tim = clock();
    FILE *fout = fopen(output_file, "w");

    cout << "attr_max_id = " << attr_max_id << endl;
    for (int i = 1; i <= attr_max_id; i++) {
        if (attr_truss[i].size() <= 2) continue;   //单一社区不存储，k=2就是图本身，两个点一定不能形成k=3

        PUNGraph graph = TUNGraph::New();
        for (int node : attr_truss[i]) {   //注意，顶点不能重复添加
            if (!graph->IsNode(node)) graph->AddNode(node);
        }
        //cout << "size = " << attr_truss[i].size() << endl;
        if (int(attr_truss[i].size()) > 4000) {
            for (TUNGraph::TEdgeI EI = G->BegEI(); EI < G->EndEI(); EI++) {
                int u = EI.GetDstNId(), v = EI.GetSrcNId();
                if (graph->IsNode(u) && graph->IsNode(v)) graph->AddEdge(u, v);
            }
        }
        else {
            for (set<int>::iterator u = attr_truss[i].begin(); u != attr_truss[i].end(); u++) {
                for (set<int>::iterator v = attr_truss[i].begin(); v != attr_truss[i].end(); v++) {
                    if (G->IsEdge(*u, *v)) graph->AddEdge(*u, *v);   //边也一样
                }
            }
        }
        // initialization初始化
        clock_t startTime = clock();
        EdgeSupport(graph);
        //printf("Run Time:%f s\n\n", ((double)(clock() - startTime) / CLOCKS_PER_SEC));

        edgeMap edgeTrussness; //边的trussness
        unordered_map<int, int> nodeTrussness;    //点的trussness
        int kMax = TrussDecomposition(graph, edgeTrussness, nodeTrussness);    //返回边的最大trussness
        //printf("kMax = %d\n", kMax);
        //printf("Run Time:%f s\n\n", ((double)(clock() - startTime) / CLOCKS_PER_SEC));
        fprintf(fout, "%d\t%d\n", i, -1);  //attr_id 插入属性ID

        map<edgePair, bool> processed;        //边是否处理
        unordered_map<int, set<edgePair> > sameTruss;    //相同的edgeTrussness集合
        queue<edgePair> Q;                    //BFS
        map<edgePair, vector<int> > SNIdentifyList;    //边存储snID集合

        for (TUNGraph::TEdgeI EI = graph->BegEI(); EI < graph->EndEI(); EI++) {    //遍历边
            edgePair e = EdgePair(EI.GetSrcNId(), EI.GetDstNId());
            processed[e] = false;
            //SNIdentifyList[e].clear();
            sameTruss[edgeTrussness[e]].insert(e);
        }
        //printf("sameTruss's size:%d\n", sameTruss.size());
        int snID = 0;    //Super-node ID initialized to 0不过是从1开始，记录超点数量

        vector<SuperNode> SNode;
        SEType SEdge;
        SuperNode t;
        SNode.push_back(t); //This initiallized to 1。因为snID从1开始的
                            // Index Construction
        for (int k = 3; k <= kMax; k++) {
            while (!sameTruss[k].empty()) {
                edgePair edge = *sameTruss[k].begin();
                processed[edge] = true;
                snID++;
                SuperNode sv;    //Create a super-node v with v.snID <-- ++snID
                sv.k = k, sv.snID = snID;
                SNode.push_back(sv);    //A new super-node for Ce

                                        //traverse G in BFS to identify all the edges k-truss equivalent to e by exploring its incident k-triangles
                Q.push(edge);
                while (!Q.empty()) {
                    edgePair edgeUV = Q.front();
                    Q.pop();
                    SNode[snID].SNEdgeList.push_back(edgeUV);

                    //Create a super-edge (v, u) where u is an existing super - node with u.snID = id;
                    for (vector<int>::iterator it = SNIdentifyList[edgeUV].begin(); it != SNIdentifyList[edgeUV].end(); it++) {
                        int su = *it;
                        SEdge.insert(EdgePair(sv.snID, su));    //Add super-edge
                    }

                    int u = edgeUV.first, v = edgeUV.second;
                    int temp;
                    if (graph->GetNI(u).GetDeg() > graph->GetNI(v).GetDeg()) {
                        temp = u, u = v, v = temp;
                    }

                    for (int i = 0; i < graph->GetNI(u).GetDeg(); i++) {
                        int w = graph->GetNI(u).GetNbrNId(i);    //the same neighbor of u and v
                        if (edgeTrussness[EdgePair(w, v)] >= k &&edgeTrussness[EdgePair(w, u)] >= k) {
                            ProcessEdge(u, w, k, processed, Q, SNIdentifyList, snID);
                            ProcessEdge(v, w, k, processed, Q, SNIdentifyList, snID);
                        }
                    }
                    //sameTruss[k].remove(edgeUV);  //这一步超级费时间，原先是list容器，现在改为set容器
                    sameTruss[k].erase(edgeUV);
                    graph->DelEdge(edgeUV.first, edgeUV.second);
                }
            }
        }
        //printf("superNode's size:%d\n", SNode.size() - 1);
        fprintf(fout, "%d\t%d\n", SNode.size() - 1, -2);   //插入有多少超点个数

        bool FLAG = true;
        for (SuperNode sn : SNode) {
            if (FLAG) {   //从第二个开始
                FLAG = false;
                continue;
            }
            //printf("superNode:: v%d   k = %d\n", sn.snID, sn.k);
            fprintf(fout, "%d\t%d\n", sn.k, -3);   //插入k值
            for (edgePair edge : sn.SNEdgeList) {
                fprintf(fout, "%d\t%d\n", edge.first, edge.second);
            }
            fprintf(fout, "%d\t%d\n", -1, -1);
        }
        edgeTrussness.clear(), nodeTrussness.clear();
        graph->Clr();
    }
    cout << "time = " << double(clock() - tim) / CLOCKS_PER_SEC << 's' << endl << endl;
    return;
}

/**
*计算dblp属性频繁概率
*/
void Compute_Probability(char* fin, char* fout)
{
    FILE *attribute_fin = fopen(fin, "r");
    map<string, int> attributesCount;   //有多少的顶点拥有这个属性，即某个属性的数量
    unordered_set<string> attributes;            //存储属性
    set<int> nodes;
    while (!feof(attribute_fin)) {
        int node;
        char attr[200005];   //这里容易出现内存分配太小，出现段错误
        fscanf(attribute_fin, "%d\t%s\n", &node, &attr);

        vector<string> vec;
        SplitString(attr, vec, ",");
        for (int i = 0; i < vec.size(); i++) {
            attributesCount[vec[i]]++;
            attributes.insert(vec[i]);
        }
        nodes.insert(node);
    }
    fclose(attribute_fin);

    unordered_set<string> pro3, pro6, pro9, pro12, pro15;
    for (auto it = attributes.begin(); it != attributes.end(); it++) {
        double probability = attributesCount[*it] * 1.0 / nodes.size();
        if (probability >= 0.0025 && probability <= 0.0035) pro3.insert(*it);
        if (probability >= 0.0055 && probability <= 0.0065) pro6.insert(*it);
        if (probability >= 0.0085 && probability <= 0.0095) pro9.insert(*it);
        if (probability >= 0.0115 && probability <= 0.0125) pro12.insert(*it);
        if (probability >= 0.0145 && probability <= 0.0155) pro15.insert(*it);
    }

    FILE *attribute_out = fopen(fout, "w");
    fprintf(attribute_out, "*******************\n");
    fprintf(attribute_out, "概率为0.003\n");
    fprintf(attribute_out, "*******************\n");
    for (string elem : pro3) fprintf(attribute_out, "%s, ", elem.data());

    fprintf(attribute_out, "\n*******************\n");
    fprintf(attribute_out, "概率为0.006\n");
    fprintf(attribute_out, "*******************\n");
    for (string elem : pro6) fprintf(attribute_out, "%s, ", elem.data());

    fprintf(attribute_out, "\n*******************\n");
    fprintf(attribute_out, "概率为0.009\n");
    fprintf(attribute_out, "*******************\n");
    for (string elem : pro9) fprintf(attribute_out, "%s, ", elem.data());

    fprintf(attribute_out, "\n*******************\n");
    fprintf(attribute_out, "概率为0.012\n");
    fprintf(attribute_out, "*******************\n");
    for (string elem : pro12) fprintf(attribute_out, "%s, ", elem.data());

    fprintf(attribute_out, "\n*******************\n");
    fprintf(attribute_out, "概率为0.015\n");
    fprintf(attribute_out, "*******************\n");
    for (string elem : pro15) fprintf(attribute_out, "%s, ", elem.data());
    fclose(attribute_out);
    return;
}

/**
*Source：Truss-equivalence
*Algorithm 0：Edge Support
*Input：A graph G(V, E)
*Output：The edge support sup(e) for each e->E
*/
void EquiTrussAttribute::EdgeSupport(PUNGraph graph)
{
    sup.clear(), nonDecSup.clear();
    for (TUNGraph::TEdgeI EI = graph->BegEI(); EI < graph->EndEI(); EI++) {    //遍历边
        int src = EI.GetSrcNId(), dst = EI.GetDstNId();    //边的两个端点
        /*
        set<int> temp, u, v;
        for(int i = 0; i < graph->GetNI(src).GetDeg(); i++) u.insert(graph->GetNI(src).GetNbrNId(i));
        for(int i = 0; i < graph->GetNI(dst).GetDeg(); i++) v.insert(graph->GetNI(dst).GetNbrNId(i));
        set_intersection(u.begin(), u.end(), v.begin(), v.end(), inserter(temp, temp.begin()));
        int support = temp.size();
        temp.clear(), u.clear(), v.clear();
        */
        //遍历邻居节点数量少的端点src，然后判断是否和另外一个端点存在边，即共同的邻居节点数量
        int temp;
        if (graph->GetNI(src).GetDeg() > graph->GetNI(dst).GetDeg()) {
            //swap(src, dst);
            temp = src, src = dst, dst = temp;
        }

        int support = 0;    //当前边的支持度，初始化为0，两个端点有多少共同的邻居节点即支持度
        for (int i = 0; i < graph->GetNI(src).GetDeg(); i++) {
            int nbr = graph->GetNI(src).GetNbrNId(i);
            if (graph->IsEdge(nbr, dst)) support++;
            //为啥正常了？if (src == 19 && dst == 18) cout << graph->GetNI(src).GetDeg() << " hj " << support << " " << nbr << endl;
        }

        edgePair edge = EdgePair(src, dst);
        sup[edge] = support;    //存储支持度
        nonDecSup.insert(make_pair(support, edge));
    }
    return;
}

/**
*Source：Truss-equivalence
*Algorithm 1：Truss Decomposition
*Input：A graph G(V, E)
*Output：The edge trussness T(e) for each e->E
*/
int EquiTrussAttribute::TrussDecomposition(PUNGraph graph, edgeMap &edgeTrussness, unordered_map<int, int> &nodeTrussness)
{
    //clock_t tim = clock();
    nodeTrussness.clear(), edgeTrussness.clear();
    //printf("clear_Time Run Time:%f s\n\n", ((double)(clock() - tim) / CLOCKS_PER_SEC));
    //EdgeSupport(graph);        //Computer sup(e) for each edge e
    int k = 2;
    while (!nonDecSup.empty()) {
        while (!nonDecSup.empty() && nonDecSup.begin()->first <= k - 2) {
            int s = nonDecSup.begin()->first;        //当前边的支持度（最小）
            edgePair e = nonDecSup.begin()->second; //当前边

            int u = e.first;
            int v = e.second;    //u < v
                                //d(u) < d(v)
            if (graph->GetNI(u).GetDeg() > graph->GetNI(v).GetDeg()) swap(u, v);

            for (int i = 0; i < graph->GetNI(u).GetDeg(); i++) {
                int nbr = graph->GetNI(u).GetNbrNId(i);
                if (sup.find(EdgePair(nbr, v)) != sup.end() && sup.find(EdgePair(nbr, u)) != sup.end()) {
                    edgePair edge = EdgePair(nbr, v);
                    nonDecSup.erase(make_pair(sup[edge], edge));
                    sup[edge] -= 1;
                    nonDecSup.insert(make_pair(sup[edge], edge));

                    edge = EdgePair(nbr, u);
                    nonDecSup.erase(make_pair(sup[edge], edge));
                    sup[edge] -= 1;
                    nonDecSup.insert(make_pair(sup[edge], edge));
                }
            }
            //要删除当前边e，所以边两端的顶点最大truss值就是k，一直更新下去
            nodeTrussness[e.first] = k;  //思考：以前只写了first点的trussness值
            nodeTrussness[e.second] = k; //即没有这句代码
            edgeTrussness[e] = k;
            sup.erase(e);
            nonDecSup.erase(make_pair(s, e));
        }
        k += 1;
    }
    return k - 1;
}

void EquiTrussAttribute::Rand_300_Vertives(PUNGraph &graph, char* output, int max_vertex_id)
{
    clock_t tim = clock();
    cout << "1、Compute Edge Support......" << endl;
    EdgeSupport(graph);
    printf("Run Time:%f s\n\n", ((double)(clock() - tim) / CLOCKS_PER_SEC));

    tim = clock();
    cout << "2、Compute Edge Truss......" << endl;
    int kMax = TrussDecomposition(graph, global_edgeTrussness, global_nodeTrussness);    //返回边的最大trussness
    printf("Run Time:%f s\n\n", ((double)(clock() - tim) / CLOCKS_PER_SEC));

    tim = clock();
    cout << "3、Load Vertex Atrributes...." << endl;
    //Vertex_Attribute("./DataSets/tencent_nltk_attributes.txt", "./DataSets/tencent_String2Int.txt", graph);   //读取属性数据集，存储到内存中
    Vertex_Attribute("./DataSets/dblp_attributes_int.txt", "./DataSets/dblp_String2Int.txt", graph);
    printf("Run Time:%f s\n\n", ((double)(clock() - tim) / CLOCKS_PER_SEC));
    
    //生成300个随机顶点，满足k>=6
    cout << "max_vertex_id = " << max_vertex_id << endl;
    //cout << RAND_MAX << endl;  //windows上32767，Linux上2147483647
    cout << "start random 300 vertices which k >= 6......." << endl;
    if (kMax < 6) return;    // 寻找k值最小为6的顶点

    int seed = time(0); srand(seed);
    mt19937 mt_time(seed);
    uniform_int_distribution<int> dist(1, max_vertex_id);  //[1, num_max]间的随机整数
    FILE* fout = fopen(output, "w");
    set<int> nodes;
    for (int i = 1; i <= 300; i++) {
        if (i % 30 == 0) cout << "finish " << i << endl;
        //int kmax[12] = { 6, 10, 10, 10, 10, 10, 7, 7, 10, 10, 10, 10 };
        //int kmax[5] = { 6, 10, 6, 10, 10 };
        while (1) {
            int node = dist(mt_time); //[1, num_max]间的随机整数
            if (global_nodeTrussness[node] >= 6 && attributesOfVertex_set[node].size() >= 5) {
                auto it = find(nodes.begin(), nodes.end(), node);
                if (it == nodes.end()) {
                    nodes.insert(node);
                    fprintf(fout, "%d\t%d\n", node, global_nodeTrussness[node]); //会生成301行，最后一行为空行
                    break;
                }
            }
            //cout << node << ' ' << nodeTrussness[node] << endl;
        }
    }
    fclose(fout);
    cout << "vertices_rand300_6truss_.txt finished!" << endl;
    return;
}

/*************************************************************************************************
函 数 名: EquiTrussIndex
功能描述: 给图建立truss equience索引
输入参数: G图
输出参数: 超图、超边、超点
返 回 值: 无
调用函数:
被调函数:
修改历史:
    1.日期: 2018年12月22日
      作者: 何健
      修改: 创建函数
*************************************************************************************************/
bool EquiTrussAttribute::LoadSuperGraph(const char* superNode_file, const char* superEdge_file, const char* nodeTruss_file, const char* edgeTruss_file)
{
    FILE *Fedge = fopen(superEdge_file, "r");
    FILE *Fnode = fopen(superNode_file, "r");
    FILE *FnodeTruss = fopen(nodeTruss_file, "r");
    FILE *FedgeTruss = fopen(edgeTruss_file, "r");

    int edgeCnt = 0;
    SuperNode sv;
    while (!feof(Fnode)) {
        int id, k;
        if (edgeCnt <= 0) {
            superNode.push_back(sv);   // 刚刚好第一个是空超点，没有错
            sv.SNEdgeList.clear();     // 一直往vector里面添加会爆内存，直到电脑死机
            fscanf(Fnode, "%d\t%d\t%d\n", &id, &k, &edgeCnt);
            sv.k = k, sv.snID = id;
        }
        else {
            int u, v;
            fscanf(Fnode, "%d\t%d\n", &u, &v);
            sv.SNEdgeList.push_back(make_pair(u, v));
            edgeCnt--;
        }
    }
    superNode.push_back(sv);
    cout << "加载超点完成!!!" << endl;
    while (!feof(Fedge)) {
        int u, v;
        fscanf(Fedge, "%d\t%d\n", &u, &v);
        superEdge.insert(make_pair(u, v));
    }
    cout << "加载超边完成!!!" << endl;

    while (!feof(FnodeTruss)) {
        int node, truss;
        fscanf(FnodeTruss, "%d\t%d\n", &node, &truss);
        global_nodeTrussness[node] = truss;
    }
    while (!feof(FedgeTruss)) {
        int u, v, truss;
        fscanf(FedgeTruss, "%d\t%d\t%d\n", &u, &v, &truss);
        global_edgeTrussness[make_pair(u, v)] = truss;
    }
    cout << "加载truss ness完成!!!" << endl;

    fclose(Fnode);
    fclose(Fedge);
    fclose(FedgeTruss);
    fclose(FnodeTruss);
    return true;
}

PUNGraph EquiTrussAttribute::EquiTrussIndex(PUNGraph &graph, const char* superNode_file, const char* superEdge_file, const char* nodeTruss_file, const char* edgeTruss_file)
{
    PUNGraph SG = TUNGraph::New();
    ifstream file(superNode_file);  // 判断一个即可
    if (file)
    {
        cout << "本地有索引文件，加载中...." << endl;
        LoadSuperGraph(superNode_file, superEdge_file, nodeTruss_file, edgeTruss_file);
        vector<SuperNode>::iterator node_it = superNode.begin();
        node_it += 1;
        for (; node_it != superNode.end(); node_it++) {
            SG->AddNode(node_it->snID);
        }
        set<pair<int, int>>::iterator it = superEdge.begin();
        for (; it != superEdge.end(); it++) {
            SG->AddEdge(it->first, it->second);
        }
        printf("superNode's size:%d\n", superNode.size() - 1);
        printf("superEdge's size:%d\n", superEdge.size());
        cout << "超图点数 = " << SG->GetNodes() << endl;
        cout << "超图边数 = " << SG->GetEdges() << endl;
        return SG;   //文件存在就不需要继续处理了
    }
    // initialization初始化
    clock_t startTime = clock();
    printf("==========2.Compute the Support of Edges==========\n");
    EdgeSupport(graph);
    printf("Run Time:%f s\n\n", ((double)(clock() - startTime) / CLOCKS_PER_SEC));

    printf("==========3.Compute the Trussness of Edges==========\n");
    int kMax = TrussDecomposition(graph, global_edgeTrussness, global_nodeTrussness);    //返回边的最大trussness
    printf("kMax = %d\n", kMax);

    // 20190322添加查询最大和最小truss值
    int maxNodeTruss = -1, minNodeTruss = 10000, maxEdgeTruss = -1, minEdgeTruss = 10000;
    for (TUNGraph::TNodeI NI = graph->BegNI(); NI < graph->EndNI(); NI++) {
        int node = NI.GetId();
        int truss = global_nodeTrussness[node];
        if (truss > maxNodeTruss) maxNodeTruss = truss;
        if (truss < minNodeTruss) minNodeTruss = truss;
    }
    for (TUNGraph::TEdgeI EI = graph->BegEI(); EI < graph->EndEI(); EI++) {
        int u = EI.GetDstNId(), v = EI.GetSrcNId();
        if (u > v) swap(u, v);
        int truss = global_edgeTrussness[make_pair(u, v)];
        if (truss > maxEdgeTruss) maxEdgeTruss = truss;
        if (truss < minEdgeTruss) minEdgeTruss = truss;
    }
    cout << "Node = " << minNodeTruss << ' ' << maxNodeTruss << endl;
    cout << "Edge = " << minEdgeTruss << ' ' << maxEdgeTruss << endl;
    printf("Run Time:%f s\n\n", ((double)(clock() - startTime) / CLOCKS_PER_SEC));

    printf("==========4.Index Construction for EquiTruss==========\n");

    map<edgePair, bool> processed;        //边是否处理
    unordered_map<int, set<edgePair> > sameTruss;    //相同的edgeTrussness集合
    queue<edgePair> Q;                    //BFS
    map<edgePair, vector<int> > SNIdentifyList;    //边存储snID集合

    for (TUNGraph::TEdgeI EI = graph->BegEI(); EI < graph->EndEI(); EI++) {    //遍历边
        edgePair e = EdgePair(EI.GetSrcNId(), EI.GetDstNId());
        processed[e] = false;
        //SNIdentifyList[e].clear();
        sameTruss[global_edgeTrussness[e]].insert(e);
    }
    //printf("sameTruss's size:%d\n", sameTruss.size());
    int snID = 0;    //Super-node ID initialized to 0
    SuperNode t;
    superNode.push_back(t); //This initiallized to 1。因为snID从1开始的

    // Index Construction
    for (int k = 3; k <= kMax; k++) {
        while (!sameTruss[k].empty()) {
            edgePair edge = *sameTruss[k].begin();
            processed[edge] = true;
            snID++;
            SuperNode sv;    //Create a super-node v with v.snID <-- ++snID
            sv.k = k, sv.snID = snID;
            superNode.push_back(sv);    //A new super-node for Ce
            SG->AddNode(snID);

            //traverse G in BFS to identify all the edges k-truss equivalent to e by exploring its incident k-triangles
            Q.push(edge);
            while (!Q.empty()) {
                edgePair edgeUV = Q.front();
                Q.pop();
                superNode[snID].SNEdgeList.push_back(edgeUV);

                //Create a super-edge (v, u) where u is an existing super - node with u.snID = id;
                for (vector<int>::iterator it = SNIdentifyList[edgeUV].begin(); it != SNIdentifyList[edgeUV].end(); it++) {
                    int su = *it;
                    superEdge.insert(EdgePair(sv.snID, su));    //Add super-edge
                    if (!SG->IsEdge(sv.snID, su)) {
                        SG->AddEdge(sv.snID, su);
                    }
                }

                int u = edgeUV.first, v = edgeUV.second;
                int temp;
                if (graph->GetNI(u).GetDeg() > graph->GetNI(v).GetDeg()) {
                    temp = u, u = v, v = temp;
                }

                for (int i = 0; i < graph->GetNI(u).GetDeg(); i++) {
                    int w = graph->GetNI(u).GetNbrNId(i);    //the same neighbor of u and v
                    if (global_edgeTrussness[EdgePair(w, v)] >= k &&global_edgeTrussness[EdgePair(w, u)] >= k) {
                        ProcessEdge(u, w, k, processed, Q, SNIdentifyList, snID);
                        ProcessEdge(v, w, k, processed, Q, SNIdentifyList, snID);
                    }
                }
                //sameTruss[k].remove(edgeUV);  //这一步超级费时间，原先是list容器，现在改为set容器
                sameTruss[k].erase(edgeUV);
                graph->DelEdge(edgeUV.first, edgeUV.second);
            }
        }
    }

    // 20190325添加存储到本地的功能
    FILE *Fnode = fopen(superNode_file, "w");
    FILE *Fedge = fopen(superEdge_file, "w");
    FILE *FnodeTruss = fopen(nodeTruss_file, "w");
    FILE *FedgeTruss = fopen(edgeTruss_file, "w");
    printf("superNode's size:%d\n", superNode.size() - 1);
    vector<SuperNode>::iterator node_it = superNode.begin();
    node_it += 1;
    for (; node_it != superNode.end(); node_it++) {
    //for (SuperNode sn : superNode) {  超点是从1开始计算的，第一个是空的
        //printf("superNode:: v%d   k = %d\n", sn.snID, sn.k);
        fprintf(Fnode, "%d\t%d\t%d\n", node_it->snID, node_it->k, int(node_it->SNEdgeList.size()));
        for (edgePair ep : node_it->SNEdgeList) {
            //printf("(%d, %d)\n", ep.first, ep.second);
            fprintf(Fnode, "%d\t%d\n", ep.first, ep.second);
        }
    }
    cout << "超点存储到本地完成！！！" << endl;
    /*
    TIntStrH Name;
    for (int i = 1; i < superNode.size(); i++) {
    Name.AddDat(i) = to_string(i).data();
    }
    TSnap::DrawGViz<PUNGraph>(superGraph, gvlDot, "HanKin.png", "", Name);*/
    printf("superEdge's size:%d\n", superEdge.size());
    set<pair<int, int>>::iterator it = superEdge.begin();
    for (; it != superEdge.end(); it ++) {
        //printf("superEdge:: (v%d  v%d)\n", it ->first, it ->second);
        fprintf(Fedge, "%d\t%d\n", it->first, it->second);
    }
    cout << "超边存储到本地完成！！！" << endl;

    map<edgePair, int>::iterator edgeTruss_it = global_edgeTrussness.begin();
    for (; edgeTruss_it != global_edgeTrussness.end(); edgeTruss_it++) {
        fprintf(FedgeTruss, "%d\t%d\t%d\n", edgeTruss_it->first.first, edgeTruss_it->first.second, edgeTruss_it->second);
    }
    unordered_map<int, int>::iterator nodeTruss_it = global_nodeTrussness.begin();
    for (; nodeTruss_it != global_nodeTrussness.end(); nodeTruss_it++) {
        fprintf(FnodeTruss, "%d\t%d\n", nodeTruss_it->first, nodeTruss_it->second);
    }
    cout << "全局trussness存储到本地完成!!!" << endl;

    cout << "超图点数 = " << SG->GetNodes() << endl;
    cout << "超图边数 = " << SG->GetEdges() << endl;
    //CommunitySearchOnEquiTruss(4, 252326, superGraph);
    //边点的truss容器如果不清理，后面再次调用时会先清理，这样就会花费时间
    sup.clear(), nonDecSup.clear(); //因为在177行的时候已经慢慢的删除了，所以可以不用清理回收
    fclose(Fedge);
    fclose(Fnode);
    fclose(FedgeTruss);
    fclose(FnodeTruss);
    return SG;
}

unordered_map<int, set<int> > EquiTrussAttribute::VertexSNIndex()
{
    unordered_map<int, set<int> > vertex_supernode;
    for (vector<SuperNode>::iterator it = superNode.begin() + 1; it != superNode.end(); it++) {
        for (edgePair edge : it->SNEdgeList) {
            vertex_supernode[edge.first].insert(it->snID);
            vertex_supernode[edge.second].insert(it->snID);
        }
    }
    return vertex_supernode;
}

/**
*EquiTrussIndex函数的添加超边
*/
void EquiTrussAttribute::ProcessEdge(int u, int v, int k, map<edgePair, bool> &processed, queue<edgePair> &Q, map<edgePair, vector<int> > &SNIdentifyList, int snID)
{
    edgePair edge = EdgePair(u, v);
    if (global_edgeTrussness[edge] == k) {    //k-trangle connectivity
        if (processed[edge] == false) {
            processed[edge] = true;
            Q.push(edge);
        }
    }
    else {    //edgeTrussness[edge] > k
        vector<int>::iterator it = find(SNIdentifyList[edge].begin(), SNIdentifyList[edge].end(), snID);
        if (it == SNIdentifyList[edge].end()) {
            SNIdentifyList[edge].push_back(snID);
        }
    }
    return;
}

/**
*关联分析中，频繁项集的挖掘中的FP-Growth算法
*/
map<int, set<pair<set<attrType>, uint64_t> > > EquiTrussAttribute::FP_Growth(set<int> &nodes, int FPsupport, set<int> &psiInt, set<attrType> &SATR)
{
    vector<Transaction> transactions;
    set<int>::iterator it = nodes.begin();
    for (; it != nodes.end(); it++) {
        set<attrType> vec_to_set(attributesOfVertex[*it].begin(), attributesOfVertex[*it].end());
        //优化，去掉不包含查询点的其他属性
        set<attrType> intersection; intersection.clear();
        set_intersection(vec_to_set.begin(), vec_to_set.end(), SATR.begin(), SATR.end(), inserter(intersection, intersection.begin()));
        Transaction temp = intersection;
        transactions.push_back(temp);
    }

    const uint64_t minimum_support_threshold = FPsupport;

    const FPTree fptree{ transactions, minimum_support_threshold };

    const set<Pattern> patterns = fptree_growth(fptree);

    //cout << "在超点中FP总共的pair对数量（psi1 + psi2 + 。。） = " << patterns.size() << endl;
    set<Pattern>::iterator iter = patterns.begin();
    map<int, set<Pattern> > psi;
    for (; iter != patterns.end(); iter++) {
        int n = iter->first.size();
        psi[n].insert(*iter);
        psiInt.insert(n);
    }
    return psi;
}

//FP-Growth算法 不使用PROPERTY 2(Neighborhood Constraint)
map<int, set<pair<set<attrType>, uint64_t> > > EquiTrussAttribute::FP_Growth_1(int FPsupport, set<int> &psiInt, set<attrType> &SATR)
{
    vector<Transaction> transactions;
    
    //枚举SATR的所有非空子集，存入transactions
    vector<attrType> vSATR(SATR.begin(), SATR.end());
    for(int subsetSize=1; subsetSize<=vSATR.size(); subsetSize++){
        vector<attrType> subset(subsetSize);
        CSetIter iter(vSATR.size());
        iter.Init(subsetSize, CSetIter::SINGLE_DISORDER);
        
        while (iter.GetNextSubset(subset)){
            set<attrType> intersection; intersection.clear();
            for (int j=0; j<subsetSize; j++){ intersection.insert(vSATR[subset[j]]); }
            Transaction temp = intersection;
            transactions.push_back(temp);
        }
    }
    
    const uint64_t minimum_support_threshold = FPsupport;

    const FPTree fptree{ transactions, minimum_support_threshold };

    const set<Pattern> patterns = fptree_growth(fptree);

    //cout << "在超点中FP总共的pair对数量（psi1 + psi2 + 。。） = " << patterns.size() << endl;
    set<Pattern>::iterator iter = patterns.begin();
    map<int, set<Pattern> > psi;
    for (; iter != patterns.end(); iter++) {
        int n = iter->first.size();
        psi[n].insert(*iter);
        psiInt.insert(n);
    }
    return psi;
}

/**
*Algorithm 1: Attributed Truss Community Search (basic)  ATCBasic
*Input :A graph G, a vertex q, and an integer k
*Output:All the attributed truss communities containing q
*/
vector<comType > EquiTrussAttribute::ATCBasic(PUNGraph G, int k, int q, set<attrType> &SATR, vector<string> &comS)
{
    clock_t origin = clock();
    vector<comType > R;    //R是本函数返回的结果
    set<int> NbrNodes;
    //需要加一步判断邻居点中是否有k个邻居有共同的查找属性
    for (int i = 0; i < G->GetNI(q).GetDeg(); i++) {
        int w = G->GetNI(q).GetNbrNId(i);
        NbrNodes.insert(w);
    }

    //cout << "NbrNodes = " << NbrNodes.size() << endl;
    clock_t FP_Growth_Time = clock();
    //int表示的psi的子集长度的大小，外面的set有多少个当前长度的子集，里面的set就是int个属性，uint64_t没有用到
    map<int, set<pair<set<attrType>, uint64_t> > > psi;
    //第一个参数是邻居点集合，第二个参数是支持度，即频繁性，第三个参数存储频繁性的种类，即有2个长度的，4个长度的，第四个参数是对哪些属性进行频繁挖掘
    set<int> psiInt;
    psi = FP_Growth(NbrNodes, k - 1, psiInt, SATR);
    //cout << "psi.size = " << psi.size() << endl;
    //cout << "NbrNodes.size = " << NbrNodes.size() << endl;
    //printf("FP_Growth_Time Run Time:%f s\n\n", ((double)(clock() - FP_Growth_Time) / CLOCKS_PER_SEC));
    int i = psi.size();
    vector<int> psiDex;  //psiInt是set型，从小到大排列，而psiDex用vector可以使用下标倒序访问
    for (set<int>::iterator it = psiInt.begin(); it != psiInt.end(); it++) psiDex.push_back(*it);

    //double find_subgraph_time = 0, find_truss_time = 0, bfs_time = 0, del_time = 0, degree_time = 0;
    //clock_t all_time = clock();
    while (i >= 1) {
        set<pair<set<attrType>, uint64_t> >::iterator it_PSI = psi[psiDex[i - 1]].begin();
        //pis[l]存储的是{x,y},{x,z},{y,z}这样的序列对
        for (; it_PSI != psi[psiDex[i - 1]].end(); it_PSI++) {
            global_psi.clear();
            global_psi = it_PSI->first;
            //global_psi_int.clear();
            //for (string str : global_psi) global_psi_int.insert(attribute_index[str]);
            //很重要，需要判断这些属性集查询点q是否也包含，index版本是因为有truss
            //if (!ContainsAll(SATR, W_)) continue;

            //遍历整个图查询满足的顶点这部分最耗费时间
            clock_t tim = clock();
            //vector<int> is_meet(max_memory);  //判断点是否满足，0未访问，1满足，2不满足
            is_meet.assign(max_memory, 0);      //改成全局变量了，记得清理，clear()并没有什么卵用，这个应该当作初始化
            PUNGraph GRAPH = TUNGraph::New();
            for (TUNGraph::TEdgeI EI = G->BegEI(); EI < G->EndEI(); EI++) { //Find G[W_] from the subgraph from Hi
                int node_u = EI.GetDstNId(), node_v = EI.GetSrcNId();
                if (is_meet[node_u] == 2 || is_meet[node_v] == 2) continue;
                if (is_meet[node_u] == 1 && is_meet[node_v] == 1) {
                    if (!GRAPH->IsNode(node_u)) GRAPH->AddNode(node_u);
                    if (!GRAPH->IsNode(node_v)) GRAPH->AddNode(node_v);
                    GRAPH->AddEdge(node_u, node_v);
                }
                if (is_meet[node_u] == 0) {
                    if (ContainsAll(node_u)) is_meet[node_u] = 1;
                    else {
                        is_meet[node_u] = 2;
                        continue;
                    }
                }
                if (is_meet[node_v] == 0) {
                    if (ContainsAll(node_v)) is_meet[node_v] = 1;
                    else {
                        is_meet[node_v] = 2;
                        continue;
                    }
                }
                if (!GRAPH->IsNode(node_u)) GRAPH->AddNode(node_u);
                if (!GRAPH->IsNode(node_v)) GRAPH->AddNode(node_v);
                GRAPH->AddEdge(node_u, node_v);
            }

            if (!GRAPH->IsNode(q)) {
                GRAPH.Clr();
                continue;
            }
            tim = clock();
            vector<comType> H;
            FindKTrussCom(GRAPH, k, q, H);
            //find_truss_time += clock() - tim;

            tim = clock();
            GRAPH.Clr();
            //del_time += clock() - tim;
            if (H.size() != 0) {
                printf("(%d)", H.size());
                cout << "当前选择的最多共有属性：";
                string SS = "#";
                for (attrType str : it_PSI->first) {   // 还没改成字符属性关键字（20190220）
                    SS += str;
                    SS += "#";
                    cout << str << ' ';
                }
                cout << endl;
                for (comType com : H) R.push_back(com);
                comS.push_back(SS);
                H.clear();
            }
        }
        if (R.size() == 0) i--;   // R就是返回结果，当不为0时就是最大社区
        else break;
    }
    printf("实际时间(去掉传递时间) Run Time:%f s\n\n", ((double)(clock() - origin) / CLOCKS_PER_SEC));
    //printf("all_time Run Time:%f s\n", ((double)(clock() - all_time) / CLOCKS_PER_SEC));
    //printf("FindsubGraph Run Time:%f s\n", (find_subgraph_time / CLOCKS_PER_SEC));
    //printf("bfs Run Time:%f s\n", (bfs_time / CLOCKS_PER_SEC));
    //printf("degree Run Time:%f s\n", (degree_time / CLOCKS_PER_SEC));
    //printf("del Run Time:%f s\n", (del_time / CLOCKS_PER_SEC));
    //printf("FindKTrussCom Run Time:%f s\n\n", (find_truss_time / CLOCKS_PER_SEC));
    return R;
}

vector<comType > EquiTrussAttribute::ATCBasic_1(PUNGraph G, int k, int q, set<attrType> &SATR, vector<string> &comS)
{
    clock_t origin = clock();
    vector<comType > R;    //R是本函数返回的结果
    set<int> GNodes;
    for (auto NI = G->BegNI(); NI < G->EndNI(); NI++) {
        int w = NI.GetId();
        GNodes.insert(w);
    }

    //cout << "NbrNodes = " << NbrNodes.size() << endl;
    clock_t FP_Growth_Time = clock();
    //int表示的psi的子集长度的大小，外面的set有多少个当前长度的子集，里面的set就是int个属性，uint64_t没有用到
    map<int, set<pair<set<attrType>, uint64_t> > > psi;
    //第一个参数是邻居点集合，第二个参数是支持度，即频繁性，第三个参数存储频繁性的种类，即有2个长度的，4个长度的，第四个参数是对哪些属性进行频繁挖掘
    set<int> psiInt;
    //psi = FP_Growth_1(k - 1, psiInt, SATR);
    psi = FP_Growth(GNodes, k - 1, psiInt, SATR);
    //cout << "psi.size = " << psi.size() << endl;
    //cout << "GNodes.size = " << GNodes.size() << endl;
    //printf("FP_Growth_Time Run Time:%f s\n\n", ((double)(clock() - FP_Growth_Time) / CLOCKS_PER_SEC));
    int i = psi.size();
    vector<int> psiDex;  //psiInt是set型，从小到大排列，而psiDex用vector可以使用下标倒序访问
    for (set<int>::iterator it = psiInt.begin(); it != psiInt.end(); it++) psiDex.push_back(*it);

    //double find_subgraph_time = 0, find_truss_time = 0, bfs_time = 0, del_time = 0, degree_time = 0;
    //clock_t all_time = clock();
    while (i >= 1) {
        set<pair<set<attrType>, uint64_t> >::iterator it_PSI = psi[psiDex[i - 1]].begin();
        //pis[l]存储的是{x,y},{x,z},{y,z}这样的序列对
        for (; it_PSI != psi[psiDex[i - 1]].end(); it_PSI++) {
            global_psi.clear();
            global_psi = it_PSI->first;
            //global_psi_int.clear();
            //for (string str : global_psi) global_psi_int.insert(attribute_index[str]);
            //很重要，需要判断这些属性集查询点q是否也包含，index版本是因为有truss
            //if (!ContainsAll(SATR, W_)) continue;

            //遍历整个图查询满足的顶点这部分最耗费时间
            clock_t tim = clock();
            //vector<int> is_meet(max_memory);  //判断点是否满足，0未访问，1满足，2不满足
            is_meet.assign(max_memory, 0);      //改成全局变量了，记得清理，clear()并没有什么卵用，这个应该当作初始化
            PUNGraph GRAPH = TUNGraph::New();
            for (TUNGraph::TEdgeI EI = G->BegEI(); EI < G->EndEI(); EI++) { //Find G[W_] from the subgraph from Hi
                int node_u = EI.GetDstNId(), node_v = EI.GetSrcNId();
                if (is_meet[node_u] == 2 || is_meet[node_v] == 2) continue;
                if (is_meet[node_u] == 1 && is_meet[node_v] == 1) {
                    if (!GRAPH->IsNode(node_u)) GRAPH->AddNode(node_u);
                    if (!GRAPH->IsNode(node_v)) GRAPH->AddNode(node_v);
                    GRAPH->AddEdge(node_u, node_v);
                }
                if (is_meet[node_u] == 0) {
                    if (ContainsAll(node_u)) is_meet[node_u] = 1;
                    else {
                        is_meet[node_u] = 2;
                        continue;
                    }
                }
                if (is_meet[node_v] == 0) {
                    if (ContainsAll(node_v)) is_meet[node_v] = 1;
                    else {
                        is_meet[node_v] = 2;
                        continue;
                    }
                }
                if (!GRAPH->IsNode(node_u)) GRAPH->AddNode(node_u);
                if (!GRAPH->IsNode(node_v)) GRAPH->AddNode(node_v);
                GRAPH->AddEdge(node_u, node_v);
            }

            if (!GRAPH->IsNode(q)) {
                GRAPH.Clr();
                continue;
            }
            tim = clock();
            vector<comType> H;
            FindKTrussCom(GRAPH, k, q, H);
            //find_truss_time += clock() - tim;

            tim = clock();
            GRAPH.Clr();
            //del_time += clock() - tim;
            if (H.size() != 0) {
                printf("(%d)", H.size());
                cout << "当前选择的最多共有属性：";
                string SS = "#";
                for (attrType str : it_PSI->first) {   // 还没改成字符属性关键字（20190220）
                    SS += str;
                    SS += "#";
                    cout << str << ' ';
                }
                cout << endl;
                for (comType com : H) R.push_back(com);
                comS.push_back(SS);
                H.clear();
            }
        }
        if (R.size() == 0) i--;   // R就是返回结果，当不为0时就是最大社区
        else break;
    }
    printf("实际时间(去掉传递时间) Run Time:%f s\n\n", ((double)(clock() - origin) / CLOCKS_PER_SEC));
    //printf("all_time Run Time:%f s\n", ((double)(clock() - all_time) / CLOCKS_PER_SEC));
    //printf("FindsubGraph Run Time:%f s\n", (find_subgraph_time / CLOCKS_PER_SEC));
    //printf("bfs Run Time:%f s\n", (bfs_time / CLOCKS_PER_SEC));
    //printf("degree Run Time:%f s\n", (degree_time / CLOCKS_PER_SEC));
    //printf("del Run Time:%f s\n", (del_time / CLOCKS_PER_SEC));
    //printf("FindKTrussCom Run Time:%f s\n\n", (find_truss_time / CLOCKS_PER_SEC));
    return R;
}


/**
*Algorithm 2: FindKTrussCom
*input :A graph G′, a vertex vq, and an integer k
*output:All the k-truss communities containing q
*/
void EquiTrussAttribute::FindKTrussCom(PUNGraph G, int k, int q, vector<comType> &C)
{
    //clock_t tim = clock();
    EdgeSupport(G);
    //if(label) printf("support_Time Run Time:%f s\n", ((double)(clock() - tim) / CLOCKS_PER_SEC));
    //tim = clock();
    map<edgePair, int> edgeTrussness;
    unordered_map<int, int> nodeTrussness;
    int kMax = TrussDecomposition(G, edgeTrussness, nodeTrussness);    //返回边的最大trussness
    //if (label) printf("trussdecomposition_Time Run Time:%f s\n", ((double)(clock() - tim) / CLOCKS_PER_SEC));
    //if (label) cout << "Max Trussness = " << kMax << endl;

    //tim = clock();
    map<edgePair, bool> visited;
    for (TUNGraph::TEdgeI EI = G->BegEI(); EI < G->EndEI(); EI++) {
        edgePair edge = EdgePair(EI.GetSrcNId(), EI.GetDstNId());
        visited[edge] = false;
        //cout << "(" << edge.first << "," << edge.second << ") = " << edgeTrussness[edge] << endl;
    }
    //if (label) printf("标记 Time Run Time:%f s\n", ((double)(clock() - tim) / CLOCKS_PER_SEC));

    //tim = clock();
    //double insec_time = 0, bfs_time = 0, sum_time = 0;

    for (int i = 0; i < G->GetNI(q).GetDeg(); i++) {
        int nq = G->GetNI(q).GetNbrNId(i);   //遍历q的邻居顶点
        edgePair edge = EdgePair(nq, q);
        clock_t t = clock();
        if (edgeTrussness[edge] >= k && visited[edge] == false) {
            comType temp;

            //clock_t tom = clock();
            queue<edgePair> Q;
            Q.push(edge);
            visited[edge] = true;
            while (!Q.empty()) {
                edge = Q.front(); Q.pop();
                temp.push_back(edge);
                int u = edge.first, v = edge.second;
                int tmp;
                if (G->GetNI(u).GetDeg() > G->GetNI(v).GetDeg()) {
                    tmp = u, u = v, v = tmp;
                }

                for (int j = 0; j < G->GetNI(u).GetDeg(); j++) {
                    int w = G->GetNI(u).GetNbrNId(j);    //the same neighbor of u and v
                    if (edgeTrussness[EdgePair(w, v)] >= k && edgeTrussness[EdgePair(w, u)] >= k) {
                        if (visited[EdgePair(w, v)] == false) {
                            visited[EdgePair(w, v)] = true;
                            Q.push(EdgePair(w, v));
                        }
                        if (visited[EdgePair(w, u)] == false) {
                            visited[EdgePair(w, u)] = true;
                            Q.push(EdgePair(w, u));
                        }
                    }
                }
            }
            //bfs_time += clock() - tom;

            //tom = clock();
            //怎么算最大社区，算全部呗
            if (int(temp.size()) != 0) {
                C.push_back(temp);
                temp.clear();
            }
            //insec_time += clock() - tom;
        }
        //sum_time += clock() - t;
    }
    edgeTrussness.clear(), nodeTrussness.clear();
    return;
}

/**
*Algorithm 3: Attributed Truss Community Search algorithm (+index)  ATCIndex
*Input :A graph G, a vertex q, and an integer k
*Output:All the attributed truss communities containing q;
*/
vector<comType > EquiTrussAttribute::ATCIndex(PUNGraph G, int k, int q, set<attrType> &SATR)
{
    clock_t origin = clock();
    vector<comType > R;    //R是本函数返回的结果
    set<int> NbrNodes;
    map<attrType, int> attr_nbr_cnt;
    for (attrType elem : SATR) attr_nbr_cnt[elem] = 1;   //其他默认为0，查询点的属性初始为1，记得减一
    int max_value = 0;  //属性的最大占有值
                        //需要加一步判断邻居点中是否有k个邻居有共同的查找属性
    for (int i = 0; i < G->GetNI(q).GetDeg(); i++) {
        int w = G->GetNI(q).GetNbrNId(i);
        if (global_edgeTrussness[EdgePair(w, q)] >= k) {   //判断这条边是否满足k
            NbrNodes.insert(w);
            for (attrType elem : attributesOfVertex[w]) {
                if (attr_nbr_cnt[elem] != 0) {
                    if (attr_nbr_cnt[elem] > max_value) max_value = attr_nbr_cnt[elem];  //这样就不用减一
                    attr_nbr_cnt[elem] += 1;
                }
            }
        }
    }
    

    
    //k-truss is (k - 1) kore
    if (max_value < k - 1) {
        printf("实际时间(去掉传递时间)---找不到 Run Time:%f s\n\n", ((double)(clock() - origin) / CLOCKS_PER_SEC));
        return R;
    }
    //NbrNodes.insert(q);
    clock_t FP_Growth_Time = clock();
    set<int> psiInt;
    map<int, set<pair<set<attrType>, uint64_t> > > psi;
    psi = FP_Growth(NbrNodes, k - 1, psiInt, SATR);
    /*int psi_cnt = 0;
    for (auto it = psi.begin(); it != psi.end(); it++) {
    psi_cnt += it->second.size();
    }
    cout << "psi的集合长度种类 = " << psi.size() << endl;
    cout << "psi的数量 = " << psi_cnt << endl;
    cout << "NbrNodes.size = " << NbrNodes.size() << endl;
    printf("FP_Growth_Time Run Time:%f s\n\n", ((double)(clock() - FP_Growth_Time) / CLOCKS_PER_SEC));*/
    if (psi.size() == 0) return R;

    //Find all super-nodes which its k >= k containing q and put them into S(q)
    vector<int> S;   //改为存储超点id
    for (set<int>::iterator id = vertexSNIndex[q].begin(); id != vertexSNIndex[q].end(); id++) {
        if (superNode[*id].k >= k) {
            S.push_back(superNode[*id].snID);
        }
    }
    if (S.size() == 0) {
        printf("实际时间(去掉传递时间)---找不到 Run Time:%f s\n\n", ((double)(clock() - origin) / CLOCKS_PER_SEC));
        return R;
    }
    //printf("Run Time:%f s\n\n", ((double)(clock() - t) / CLOCKS_PER_SEC));
    //printf("S(q)'s size = %d\n", S.size());
    unordered_map<int, bool> processed;    //超点是否处理，访问
    vector<SuperNode>::iterator it = superNode.begin() + 1;
    for (; it != superNode.end(); it++) {
        processed[it->snID] = false;
    }

    //KTrussSummary中的属性集参数应该是psi集合的并集
    set<attrType> SATR_temp;
    for (map<int, set<pair<set<attrType>, uint64_t> > >::iterator it1 = psi.begin(); it1 != psi.end(); it1++) {
        for (set<pair<set<attrType>, uint64_t> >::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
            for (attrType elem : it2->first) {
                SATR_temp.insert(elem);
            }
        }
    }

    vector<vector<edgePair> > H;
    for (vector<int>::iterator it_S = S.begin(); it_S != S.end(); it_S++) {
        //SuperNode s = ;
        vector<edgePair> As = KTrussSummary(processed, *it_S, k, q, SATR_temp); //As是EquiTruss返回的结果
        if (As.size() == 0) continue;
        H.push_back(As);
    }
    int i = psi.size();
    int l = H.size();
    vector<int> psiDex;
    for (set<int>::iterator it = psiInt.begin(); it != psiInt.end(); it++) psiDex.push_back(*it);

    //double find_truss_time = 0, find_subgraph_time = 0;
    //double insert_subgraph_time = 0, x = 0;
    //int cnt = 0;
    //clock_t find_time = clock();
    //printf("\n==========6.Community Search Based on EquiTruss==========\n");
    while (i >= 1) {
        set<pair<set<attrType>, uint64_t> >::iterator it_PSI = psi[psiDex[i - 1]].begin();
        //pis[l]存储的是{x,y,z},{x,z,w},{x,y,z}这样的集合，这里l的值为3，全部为长度为3的集合
        for (; it_PSI != psi[psiDex[i - 1]].end(); it_PSI++) {
            //set<string> W_ = it_PSI->first;
            global_psi.clear();
            global_psi = it_PSI->first;
            global_psi_size = global_psi.size();
            clock_t ok = clock();
            //vector<int> is_meet(max_memory);  //判断点是否满足
            is_meet.assign(max_memory, 0);
            for (attrType attr : global_psi) {
                for (int elem : vertices_in_attribute[attr]) {
                    is_meet[elem] += 1;
                }
            }
            //x += clock() - ok;
            for (int j = 1; j <= l; j++) {  //后面会-1
                                            //很重要，需要判断这些属性集查询点q是否也包含，index版本是因为有truss
                                            //if (!ContainsAll(SATR, it_PSI->first)) continue;
                clock_t tim = clock();
                PUNGraph GRAPH = TUNGraph::New();
                bool flag = FindSubgraph_by_index(H[j - 1], GRAPH, k, q, is_meet);
                //find_subgraph_time += clock() - tim;
                if (!flag) continue;

                tim = clock();
                //find k-truss from G[W_]
                vector<comType> A;
                FindKTrussCom(GRAPH, k, q, A);

                //find_truss_time += clock() - tim;
                if (A.size() != 0) {
                    printf("(%d)", A.size());
                    cout << "当前选择的最多共有属性：";
                    for (attrType str : it_PSI->first) cout << str << ' ';
                    cout << endl;
                    for (comType com : A) R.push_back(com);
                    A.clear();
                }
                GRAPH.Clr();
            }
        }
        if (R.size() == 0) i--;
        else break;
    }
    //cout << "时间 = " << x / CLOCKS_PER_SEC << endl;
    printf("实际时间(去掉传递时间) Run Time:%f s\n\n", ((double)(clock() - origin) / CLOCKS_PER_SEC));
    //printf("find_Time Run Time:%f s\n", ((double)(clock() - find_time) / CLOCKS_PER_SEC));
    //printf("find subgraph Time Run Time:%f s\n", (find_subgraph_time / CLOCKS_PER_SEC));
    //printf("find truss Time Run Time:%f s\n\n", (find_truss_time / CLOCKS_PER_SEC));
    return R;
}

vector<comType > EquiTrussAttribute::ATCIndex_1(PUNGraph G, int k, int q, set<attrType> &SATR)
{
    clock_t origin = clock();
    vector<comType > R;    //R是本函数返回的结果
    set<int> GNodes;
    map<attrType, int> attr_nbr_cnt;
    for (attrType elem : SATR) attr_nbr_cnt[elem] = 1;   //其他默认为0，查询点的属性初始为1，记得减一
    int max_value = 0;  //属性的最大占有值
    for (auto NI = G->BegNI(); NI < G->EndNI(); NI++) {
        int w = NI.GetId();
        if (global_edgeTrussness[EdgePair(w, q)] >= k) {   //判断这条边是否满足k
            GNodes.insert(w);
            for (attrType elem : attributesOfVertex[w]) {
                if (attr_nbr_cnt[elem] != 0) {
                    if (attr_nbr_cnt[elem] > max_value) max_value = attr_nbr_cnt[elem];  //这样就不用减一
                    attr_nbr_cnt[elem] += 1;
                }
            }
        }
    }
    

    
    //k-truss is (k - 1) kore
    if (max_value < k - 1) {
        printf("实际时间(去掉传递时间)---找不到 Run Time:%f s\n\n", ((double)(clock() - origin) / CLOCKS_PER_SEC));
        return R;
    }
    //NbrNodes.insert(q);
    clock_t FP_Growth_Time = clock();
    set<int> psiInt;
    map<int, set<pair<set<attrType>, uint64_t> > > psi;
    //psi = FP_Growth_1(k - 1, psiInt, SATR);
    psi = FP_Growth(GNodes, k - 1, psiInt, SATR);
    /*int psi_cnt = 0;
    for (auto it = psi.begin(); it != psi.end(); it++) {
    psi_cnt += it->second.size();
    }
    cout << "psi的集合长度种类 = " << psi.size() << endl;
    cout << "psi的数量 = " << psi_cnt << endl;
    cout << "NbrNodes.size = " << NbrNodes.size() << endl;
    printf("FP_Growth_Time Run Time:%f s\n\n", ((double)(clock() - FP_Growth_Time) / CLOCKS_PER_SEC));*/
    if (psi.size() == 0) return R;

    //Find all super-nodes which its k >= k containing q and put them into S(q)
    vector<int> S;   //改为存储超点id
    for (set<int>::iterator id = vertexSNIndex[q].begin(); id != vertexSNIndex[q].end(); id++) {
        if (superNode[*id].k >= k) {
            S.push_back(superNode[*id].snID);
        }
    }
    if (S.size() == 0) {
        printf("实际时间(去掉传递时间)---找不到 Run Time:%f s\n\n", ((double)(clock() - origin) / CLOCKS_PER_SEC));
        return R;
    }
    //printf("Run Time:%f s\n\n", ((double)(clock() - t) / CLOCKS_PER_SEC));
    //printf("S(q)'s size = %d\n", S.size());
    unordered_map<int, bool> processed;    //超点是否处理，访问
    vector<SuperNode>::iterator it = superNode.begin() + 1;
    for (; it != superNode.end(); it++) {
        processed[it->snID] = false;
    }

    //KTrussSummary中的属性集参数应该是psi集合的并集
    set<attrType> SATR_temp;
    for (map<int, set<pair<set<attrType>, uint64_t> > >::iterator it1 = psi.begin(); it1 != psi.end(); it1++) {
        for (set<pair<set<attrType>, uint64_t> >::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
            for (attrType elem : it2->first) {
                SATR_temp.insert(elem);
            }
        }
    }

    vector<vector<edgePair> > H;
    for (vector<int>::iterator it_S = S.begin(); it_S != S.end(); it_S++) {
        //SuperNode s = ;
        vector<edgePair> As = KTrussSummary(processed, *it_S, k, q, SATR_temp); //As是EquiTruss返回的结果
        if (As.size() == 0) continue;
        H.push_back(As);
    }
    int i = psi.size();
    int l = H.size();
    vector<int> psiDex;
    for (set<int>::iterator it = psiInt.begin(); it != psiInt.end(); it++) psiDex.push_back(*it);

    //double find_truss_time = 0, find_subgraph_time = 0;
    //double insert_subgraph_time = 0, x = 0;
    //int cnt = 0;
    //clock_t find_time = clock();
    //printf("\n==========6.Community Search Based on EquiTruss==========\n");
    while (i >= 1) {
        set<pair<set<attrType>, uint64_t> >::iterator it_PSI = psi[psiDex[i - 1]].begin();
        //pis[l]存储的是{x,y,z},{x,z,w},{x,y,z}这样的集合，这里l的值为3，全部为长度为3的集合
        for (; it_PSI != psi[psiDex[i - 1]].end(); it_PSI++) {
            //set<string> W_ = it_PSI->first;
            global_psi.clear();
            global_psi = it_PSI->first;
            global_psi_size = global_psi.size();
            clock_t ok = clock();
            //vector<int> is_meet(max_memory);  //判断点是否满足
            is_meet.assign(max_memory, 0);
            for (attrType attr : global_psi) {
                for (int elem : vertices_in_attribute[attr]) {
                    is_meet[elem] += 1;
                }
            }
            //x += clock() - ok;
            for (int j = 1; j <= l; j++) {  //后面会-1
                                            //很重要，需要判断这些属性集查询点q是否也包含，index版本是因为有truss
                                            //if (!ContainsAll(SATR, it_PSI->first)) continue;
                clock_t tim = clock();
                PUNGraph GRAPH = TUNGraph::New();
                bool flag = FindSubgraph_by_index(H[j - 1], GRAPH, k, q, is_meet);
                //find_subgraph_time += clock() - tim;
                if (!flag) continue;

                tim = clock();
                //find k-truss from G[W_]
                vector<comType> A;
                FindKTrussCom(GRAPH, k, q, A);

                //find_truss_time += clock() - tim;
                if (A.size() != 0) {
                    printf("(%d)", A.size());
                    cout << "当前选择的最多共有属性：";
                    for (attrType str : it_PSI->first) cout << str << ' ';
                    cout << endl;
                    for (comType com : A) R.push_back(com);
                    A.clear();
                }
                GRAPH.Clr();
            }
        }
        if (R.size() == 0) i--;
        else break;
    }
    //cout << "时间 = " << x / CLOCKS_PER_SEC << endl;
    printf("实际时间(去掉传递时间) Run Time:%f s\n\n", ((double)(clock() - origin) / CLOCKS_PER_SEC));
    //printf("find_Time Run Time:%f s\n", ((double)(clock() - find_time) / CLOCKS_PER_SEC));
    //printf("find subgraph Time Run Time:%f s\n", (find_subgraph_time / CLOCKS_PER_SEC));
    //printf("find truss Time Run Time:%f s\n\n", (find_truss_time / CLOCKS_PER_SEC));
    return R;
}



vector<comType > EquiTrussAttribute::ATCIndex_Advanced(PUNGraph G, int k, int q, set<attrType> &SATR)
{
    //cout << "新版查看倒排索引时间，去除重复" << endl;
    clock_t origin = clock();
    vector<comType > R;    //R是本函数返回的结果
    set<int> NbrNodes;
    map<attrType, int> attr_nbr_cnt;
    for (attrType elem : SATR) attr_nbr_cnt[elem] = 1;   //其他默认为0，查询点的属性初始为1，记得减一
    int max_value = 0;  //属性的最大占有值
                        //需要加一步判断邻居点中是否有k个邻居有共同的查找属性
    for (int i = 0; i < G->GetNI(q).GetDeg(); i++) {
        int w = G->GetNI(q).GetNbrNId(i);
        if (global_edgeTrussness[EdgePair(w, q)] >= k) {   //判断这条边是否满足k
            NbrNodes.insert(w);
            for (attrType elem : attributesOfVertex[w]) {
                if (attr_nbr_cnt[elem] != 0) {
                    if (attr_nbr_cnt[elem] > max_value) max_value = attr_nbr_cnt[elem];  //这样就不用减一
                    attr_nbr_cnt[elem] += 1;
                }
            }
        }
    }
    //k-truss is (k - 1) kore
    if (max_value < k - 1) {
        printf("实际时间(去掉传递时间)---找不到 Run Time:%f s\n\n", ((double)(clock() - origin) / CLOCKS_PER_SEC));
        return R;
    }
    //NbrNodes.insert(q);
    clock_t FP_Growth_Time = clock();
    set<int> psiInt;
    map<int, set<pair<set<attrType>, uint64_t> > > psi;
    psi = FP_Growth(NbrNodes, k - 1, psiInt, SATR);
    int psi_cnt = 0;
    for (auto it = psi.begin(); it != psi.end(); it++) {
        psi_cnt += it->second.size();
    }
    //cout << "psi的集合长度种类 = " << psi.size() << endl;
    //cout << "psi的数量 = " << psi_cnt << endl;
    //cout << "NbrNodes.size = " << NbrNodes.size() << endl;
    //printf("FP_Growth_Time Run Time:%f s\n\n", ((double)(clock() - FP_Growth_Time) / CLOCKS_PER_SEC));
    if (psi.size() == 0) return R;

    //KTrussSummary中的属性集参数应该是psi集合的并集
    set<attrType> SATR_temp;
    for (map<int, set<pair<set<attrType>, uint64_t> > >::iterator it1 = psi.begin(); it1 != psi.end(); it1++) {
        for (set<pair<set<attrType>, uint64_t> >::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
            for (attrType elem : it2->first) {
                SATR_temp.insert(elem);
            }
        }
    }
    //Find all super-nodes which its k >= k containing q and put them into S(q)
    unordered_map<int, bool> processed;    //超点是否处理，访问
    vector<SuperNode>::iterator it = superNode.begin() + 1;
    for (; it != superNode.end(); it++) {
        processed[it->snID] = false;
    }

    vector<edgePair> As;
    for (set<int>::iterator id = vertexSNIndex[q].begin(); id != vertexSNIndex[q].end(); id++) {
        if (superNode[*id].k >= k) {
            vector<edgePair> com = KTrussSummary(processed, superNode[*id].snID, k, q, SATR_temp); //As是EquiTruss返回的结果
            if (com.size() == 0) continue;
            for (edgePair edge : com) {
                As.push_back(edge);   //会不会有重复？？？
            }
        }
    }
    if (As.size() == 0) {
        printf("实际时间(去掉传递时间)---找不到 Run Time:%f s\n\n", ((double)(clock() - origin) / CLOCKS_PER_SEC));
        return R;
    }
    //vector<int>().swap(is_meet);  //创建临时变量交换销毁

    int i = psi.size();
    vector<int> psiDex;
    for (set<int>::iterator it = psiInt.begin(); it != psiInt.end(); it++) psiDex.push_back(*it);

    while (i >= 1) {
        set<pair<set<attrType>, uint64_t> >::iterator it_PSI = psi[psiDex[i - 1]].begin();
        //pis[l]存储的是{x,y,z},{x,z,w},{x,y,z}这样的集合，这里l的值为3，全部为长度为3的集合
        for (; it_PSI != psi[psiDex[i - 1]].end(); it_PSI++) {
            //set<string> W_ = it_PSI->first;
            global_psi.clear();
            global_psi = it_PSI->first;
            global_psi_size = global_psi.size();   //这个会在FindSubgraph_by_index里用到，所以这个很重要
                                                   //vector<int> is_meet(max_memory);  //判断点是否满足
                                                   //unordered_map<int, int> is_meet;
            is_meet.assign(max_memory, 0);
            for (attrType attr : global_psi) {
                for (int elem : vertices_in_attribute[attr]) {
                    is_meet[elem] += 1;
                }
            }
            //删除超点分块合并
            clock_t tim = clock();
            PUNGraph GRAPH = TUNGraph::New();
            bool flag = FindSubgraph_by_index(As, GRAPH, k, q, is_meet);
            //find_subgraph_time += clock() - tim;
            if (!flag) continue;

            tim = clock();
            //find k-truss from G[W_]
            vector<comType> A;
            FindKTrussCom(GRAPH, k, q, A);

            //find_truss_time += clock() - tim;
            if (A.size() != 0) {
                printf("(%d)", A.size());
                cout << "当前选择的最多共有属性：";
                for (attrType str : it_PSI->first) cout << str << ' ';
                cout << endl;
                for (comType com : A) R.push_back(com);
                A.clear();
            }
            GRAPH.Clr();
        }
        if (R.size() == 0) i--;
        else break;
    }
    //cout << "时间 = " << x / CLOCKS_PER_SEC << endl;
    printf("实际时间(去掉传递时间) Run Time:%f s\n\n", ((double)(clock() - origin) / CLOCKS_PER_SEC));
    //printf("find_Time Run Time:%f s\n", ((double)(clock() - find_time) / CLOCKS_PER_SEC));
    //printf("find subgraph Time Run Time:%f s\n", (find_subgraph_time / CLOCKS_PER_SEC));
    //printf("find truss Time Run Time:%f s\n\n", (find_truss_time / CLOCKS_PER_SEC));
    return R;
}


void EquiTrussAttribute::GetSNAttributes()
{
    for (SuperNode sn : superNode) {
        for (edgePair edge : sn.SNEdgeList) {
            for (attrType attr : attributesOfVertex[edge.first]) {
                SNAttributes[sn.snID].insert(attr);
            }
            for (attrType attr : attributesOfVertex[edge.second]) {
                SNAttributes[sn.snID].insert(attr);
            }
        }
    }
    return;
}

/**
*Algorithm 4: kTruss-Summary (G,vq,k)
*Input :A summary graph G, a vertex vq, and an integer k
*Output:All the k-truss communities containing vq
*/
vector<edgePair> EquiTrussAttribute::KTrussSummary(unordered_map<int, bool> &processed, int SN_id, int k, int q, set<attrType> &SATR)
{
    clock_t sum = clock();
    int l = 0;    //the number of communities
    vector<edgePair> As;  //As set is superNode set of including query vertex q

    if (processed[SN_id] == false) {
        //global_psi_size = SATR.size();
        vector<int> is_vis(max_memory);  //判断点是否满足，0未访问，1满足，2不满足
        for (attrType attr : SATR) {
            for (int elem : vertices_in_supernode[attr]) {
                is_vis[elem] += 1;
            }
        }

        processed[SN_id] = true;
        As.clear();
        queue<int> Q;    //Q可以只放int型id
        Q.push(SN_id);

        //clock_t WhileQ_Time = clock();
        //int WhileQ_Cnt = 0;
        //double Q1_Time = 0, Q2_Time = 0, insec_time = 0;
        while (!Q.empty()) {
            //WhileQ_Cnt++;
            SN_id = Q.front(); Q.pop();
            //printf("v%d\n", v.list.size());
            //添加超边的时候，第二个超点只添加了k和snID，list是空的
            clock_t tim = clock();
            vector<edgePair>::iterator iter = superNode[SN_id].SNEdgeList.begin();
            for (; iter != superNode[SN_id].SNEdgeList.end(); iter++) {
                As.push_back(*iter);
            }
            //Q1_Time += clock() - tim;
            tim = clock();
            int vid = SN_id;
            for (int i = 0; i < superGraph->GetNI(vid).GetDeg(); i++) {
                int nbr = superGraph->GetNI(vid).GetNbrNId(i);
                clock_t tx = clock();
                int uid = superNode[nbr].snID;
                if (superNode[nbr].k >= k && processed[uid] == false) {
                    processed[uid] = true;
                    if (is_vis[uid] != 0) Q.push(nbr);
                }
            }
            //Q2_Time += clock() - tim;
        }
        is_vis.clear();
    }
    //printf("KTS_Time Run Time:%f s\n", ((clock() - sum) / CLOCKS_PER_SEC));
    return As;
}

bool EquiTrussAttribute::FindSubgraph_by_index(vector<edgePair> &H, PUNGraph &subgraph, int k, int q, vector<int> &meet)
{
    for (edgePair edge : H) {
        int u = edge.first, v = edge.second;
        if (meet[u] == global_psi_size && meet[v] == global_psi_size) {
            if (!subgraph->IsNode(u)) subgraph->AddNode(u);
            if (!subgraph->IsNode(v)) subgraph->AddNode(v);
            subgraph->AddEdge(u, v);
        }
    }
    //cout << "原子图 nodes = " << subgraph->GetNodes() << endl;
    //cout << "原子图 edges = " << subgraph->GetEdges() << endl << endl;

    //多次去度会增加时间，一次就好
    for (TUNGraph::TNodeI NI = subgraph->BegNI(); NI < subgraph->EndNI(); NI++) {
        int node = NI.GetId();
        if (NI.GetDeg() < k - 1) subgraph->DelNode(node);
    }

    if (!subgraph->IsNode(q)) {
        subgraph.Clr();
        return false;  //找不到满足子图
    }
    //cout << "degree剪枝后 nodes = " << subgraph->GetNodes() << endl;
    //cout << "degree剪枝后 edges = " << subgraph->GetEdges() << endl << endl;

    queue<int> Q;
    Q.push(q);
    PUNGraph GRAPH_q = TUNGraph::New();
    GRAPH_q->AddNode(q);
    while (!Q.empty()) {
        int cur = Q.front();  Q.pop();
        for (int i = 0; i < subgraph->GetNI(cur).GetDeg(); i++) {
            int nbr = subgraph->GetNI(cur).GetNbrNId(i);
            if (!GRAPH_q->IsNode(nbr)) {
                Q.push(nbr);
                GRAPH_q->AddNode(nbr);
            }
            GRAPH_q->AddEdge(cur, nbr);
        }
    }
    *subgraph = *GRAPH_q;
    //cout << "component剪枝后 nodes = " << subgraph->GetNodes() << endl;
    //cout << "component剪枝后 edges = " << subgraph->GetEdges() << endl << endl;
    //for (TUNGraph::TNodeI NI = subgraph->BegNI(); NI < subgraph->EndNI(); NI++) {
    //    int node = NI.GetId();
    //    cout << node << ' ';
    //}
    //cout << endl;
    return true;
}

bool EquiTrussAttribute::FindSubgraph(vector<edgePair> &H, PUNGraph &subgraph, int k, int q, vector<int> &meet)
{
    double bfs_time = 0, degree_time = 0;
    clock_t tim = clock();
    for (vector<edgePair>::iterator edge = H.begin(); edge != H.end(); edge++) { //Find G[W_] from the subgraph from Hi
        int node_u = edge->first, node_v = edge->second;
        if (meet[node_u] == 2 || meet[node_v] == 2) continue;
        if (meet[node_u] == 1 && meet[node_v] == 1) {
            if (!subgraph->IsNode(node_u)) subgraph->AddNode(node_u);
            if (!subgraph->IsNode(node_v)) subgraph->AddNode(node_v);
            subgraph->AddEdge(node_u, node_v);
        }
        if (meet[node_u] == 0) {
            if (ContainsAll_by_int(node_u)) meet[node_u] = 1;
            else {
                meet[node_u] = 2;
                continue;
            }
        }
        if (meet[node_v] == 0) {
            if (ContainsAll_by_int(node_v)) meet[node_v] = 1;
            else {
                meet[node_v] = 2;
                continue;
            }
        }
        if (!subgraph->IsNode(node_u)) subgraph->AddNode(node_u);
        if (!subgraph->IsNode(node_v)) subgraph->AddNode(node_v);
        subgraph->AddEdge(node_u, node_v);
    }
    //cout << "原始的子图 nodes = " << subgraph->GetNodes() << endl;
    //cout << "原始的子图 edges = " << subgraph->GetEdges() << endl << endl;

    tim = clock();
    //多次去度会增加时间，一次就好
    for (TUNGraph::TNodeI NI = subgraph->BegNI(); NI < subgraph->EndNI(); NI++) {
        int node = NI.GetId();
        if (NI.GetDeg() < k - 1) subgraph->DelNode(node);
    }
    degree_time += clock() - tim;

    if (!subgraph->IsNode(q)) {
        subgraph.Clr();
        return false;  //找不到满足子图
    }
    //cout << "degree剪枝后 nodes = " << subgraph->GetNodes() << endl;
    //cout << "degree剪枝后 edges = " << subgraph->GetEdges() << endl << endl;

    tim = clock();
    queue<int> Q;
    Q.push(q);
    PUNGraph GRAPH_q = TUNGraph::New();
    GRAPH_q->AddNode(q);
    while (!Q.empty()) {
        int cur = Q.front();  Q.pop();
        for (int i = 0; i < subgraph->GetNI(cur).GetDeg(); i++) {
            int nbr = subgraph->GetNI(cur).GetNbrNId(i);
            if (!GRAPH_q->IsNode(nbr)) {
                Q.push(nbr);
                GRAPH_q->AddNode(nbr);
            }
            GRAPH_q->AddEdge(cur, nbr);
        }
    }
    *subgraph = *GRAPH_q;
    bfs_time += clock() - tim;
    //cout << "component剪枝后 nodes = " << GRAPH_q->GetNodes() << endl;
    //cout << "component剪枝后 edges = " << subgraph->GetEdges() << endl << endl;
    //printf("bfs Run Time:%f s\n", (bfs_time / CLOCKS_PER_SEC));
    //printf("degree Run Time:%f s\n", (degree_time / CLOCKS_PER_SEC));
    return true;
}

/*************************************************************************************************
函 数 名: ATCImprove
功能描述: 自下而上的寻找，从单个属性向多个属性进行组合
输入参数: fileName属性数据文件、G图
输出参数:
返 回 值: 无
调用函数:
被调函数:
修改历史:
    1.日期: 2018年09月22日
    作者: 何健
    修改: 创建函数
*************************************************************************************************/
vector<comType > EquiTrussAttribute::ATCImprove(PUNGraph G, int k, int q, set<attrType> &SATR)
{
    clock_t origin = clock();
    vector<comType > R;    //R是本函数返回的结果
    set<int> NbrNodes;
    unordered_map<attrType, int> attr_nbr_cnt;
    for (attrType elem : SATR) attr_nbr_cnt[elem] = 1;   //其他默认为0，查询点的属性初始为1，记得减一
    int max_value = 0;  //属性的最大占有值
                        //需要加一步判断邻居点中是否有k个邻居有共同的查找属性
    for (int i = 0; i < G->GetNI(q).GetDeg(); i++) {
        int w = G->GetNI(q).GetNbrNId(i);
        if (global_edgeTrussness[EdgePair(w, q)] >= k) {   //判断这条边是否满足k
            NbrNodes.insert(w);
            for (attrType elem : attributesOfVertex[w]) {
                if (attr_nbr_cnt[elem] != 0) {
                    if (attr_nbr_cnt[elem] > max_value) max_value = attr_nbr_cnt[elem];  //这样就不用减一
                    attr_nbr_cnt[elem] += 1;
                }
            }
        }
    }
    if (max_value < k - 1) {
        printf("实际时间(去掉传递时间)--找不到 Run Time:%f s\n\n", ((double)(clock() - origin) / CLOCKS_PER_SEC));
        return R;
    }

    //Find all super-nodes which its k >= k containing q and put them into S(q)
    vector<int> SuperNode_S;

    //cout << "查询点所在的超点有：";
    for (set<int>::iterator id = vertexSNIndex[q].begin(); id != vertexSNIndex[q].end(); id++) {
        if (superNode[*id].k >= k) {
            SuperNode_S.push_back(*id);
            //cout << *id << ' ';
        }
    }
    //cout << endl;
    if (SuperNode_S.size() == 0) return R;

    //printf("\n==========6.Community Search Based on EquiTruss==========\n");
    vector<vector<edgePair> > H;
    vector<set<int> > psiInt;
    vector<map<int, set<pair<set<attrType>, uint64_t> > > > psi;

    unordered_map<int, bool> processed, VIS;        //超点是否处理，访问
    for (vector<SuperNode>::iterator it = superNode.begin() + 1; it != superNode.end(); it++) {
        processed[it->snID] = false;
        VIS[it->snID] = false;
    }
    printf("2 Run Time:%f s\n\n", ((double)(clock() - origin) / CLOCKS_PER_SEC));
    int maxn = 0, j;
    double KTSummary_Time = 0; int KTSummaryCnt = 0;
    double MergeAndComputeNodes_Time = 0, FP_Growth_Time = 0, PsiSize = 0, NodesSize = 0;
    //cout << "顶点q所在的超点数 = " << SuperNode_S.size() << endl;
    for (vector<int>::iterator it_S = SuperNode_S.begin(); it_S != SuperNode_S.end(); it_S++) {
        //SuperNode s = ;   去掉这种单独拿出来的做法，会浪费时间
        clock_t tim = clock();
        vector<edgePair> As = KTrussSummary(processed, *it_S, k, q, SATR); //As是EquiTruss返回的结果
                                                                           //if(KTSummaryCnt == 0 ) printf("Only One KTSummary Run Time:%f s\n\n", (double)(clock() - tim) / CLOCKS_PER_SEC);
        KTSummaryCnt++;
        KTSummary_Time += clock() - tim;
        if (As.size() == 0) continue;

        tim = clock();
        vector<int> mergeS; mergeS.clear();
        for (vector<int>::iterator id = SuperNode_S.begin(); id != SuperNode_S.end(); id++) {
            if (processed[*id] == true && VIS[*id] == false) {
                mergeS.push_back(*id);
                VIS[*id] = true;
            }
        }

        //这个真的搞笑，以前真是想到啥就咋做，现在优化起来真要命
        set<int> nodes; nodes.clear();
        for (int node : NbrNodes) {
            for (int sn : vertexSNIndex[node]) {
                auto it = find(mergeS.begin(), mergeS.end(), sn);
                if (it != mergeS.end()) nodes.insert(node);
            }
        }
        MergeAndComputeNodes_Time += clock() - tim;
        H.push_back(As);
        set<int> psiInt_;
        map<int, set<pair<set<attrType>, uint64_t> > > psi_;

        tim = clock();
        psi_ = FP_Growth(nodes, k - 1, psiInt_, SATR);  //psiInt_表示这个{}集合里面有psiInt_个属性。
        PsiSize += psi_.size();
        NodesSize += nodes.size();
        FP_Growth_Time += clock() - tim;
        psiInt.push_back(psiInt_), psi.push_back(psi_);
        if (psiInt_.size() != 0 && *(--psiInt_.end()) > maxn) {
            maxn = *(--psiInt_.end());
            j = psiInt_.size();
        }
    }
    //cout << "KTSummaryCnt = " << KTSummaryCnt << endl;
    //printf("KTrussSummary_Time Run Time:%f s\n", (KTSummary_Time / CLOCKS_PER_SEC));
    //printf("MergeAndComputeNodes_Time Run Time:%f s\n", (MergeAndComputeNodes_Time / CLOCKS_PER_SEC));
    //cout << "psi.size = " << PsiSize << endl;
    //cout << "nodes.size = " << NodesSize << endl;
    //cout << "psiInt.size = " << psiInt.size() << endl;
    printf("FP_Growth_Time Run Time:%f s\n", (FP_Growth_Time / CLOCKS_PER_SEC));
    printf("1 Run Time:%f s\n\n", ((double)(clock() - origin) / CLOCKS_PER_SEC));
    int l = H.size();
    clock_t whi = clock();
    double find_truss_time = 0, find_subgraph_time = 0;
    while (j >= 1) {   //j表示最大的频繁集size
        for (int i = 1; i <= l; i++) {
            if (psiInt[i - 1].size() != 0 && *(--psiInt[i - 1].end()) >= j) {
                vector<int> psiDex;
                for (set<int>::iterator it = psiInt[i - 1].begin(); it != psiInt[i - 1].end(); it++) {
                    psiDex.push_back(*it);
                }
                set<pair<set<attrType>, uint64_t> >::iterator it_PSI = psi[i - 1][psiDex[j - 1]].begin();
                //pis[l]存储的是{x,y},{x,z},{y,z}这样的序列对
                for (; it_PSI != psi[i - 1][psiDex[j - 1]].end(); it_PSI++) {
                    //set<string> W_ = ;
                    global_psi.clear();
                    global_psi = it_PSI->first;
                    global_psi_int.clear();
                    global_psi_size = global_psi.size();
                    //vector<int> is_meet(max_memory);  //判断点是否满足，0未访问，1满足，2不满足
                    is_meet.assign(max_memory, 0);
                    for (attrType attr : global_psi) {
                        for (int elem : vertices_in_attribute[attr]) {
                            is_meet[elem] += 1;
                        }
                    }
                    //for (string str : global_psi) global_psi_int.insert(attribute_index[str]);
                    //很重要，需要判断这些属性集查询点q是否也包含，index版本是因为有truss
                    //if (!ContainsAll(SATR, it_PSI->first)) continue;

                    clock_t tim = clock();
                    PUNGraph GRAPH = TUNGraph::New();
                    bool flag = FindSubgraph_by_index(H[i - 1], GRAPH, k, q, is_meet);
                    find_subgraph_time += clock() - tim;
                    if (!flag) continue;

                    //find k-truss from G[W_]  GRAPH
                    tim = clock();
                    vector<comType> A;
                    FindKTrussCom(GRAPH, k, q, A);
                    find_truss_time += clock() - tim;
                    if (A.size() != 0) {
                        printf("(%d)", A.size());
                        cout << "当前选择的最多共有属性：";
                        for (attrType str : it_PSI->first) cout << str << ' ';
                        cout << endl;
                        for (comType com : A) R.push_back(com);
                        A.clear();
                    }
                    GRAPH.Clr();
                }
            }
        }
        if (R.size() == 0) j--;
        else break;
    }
    //printf("WhileSearch_Time Run Time:%f s\n", ((double)(clock() - whi) / CLOCKS_PER_SEC));
    printf("实际时间(去掉传递时间) Run Time:%f s\n\n", ((double)(clock() - origin) / CLOCKS_PER_SEC));
    //printf("Find subgraph Run Time:%f s\n", (find_subgraph_time / CLOCKS_PER_SEC));
    //printf("FindKTrussCom Run Time:%f s\n\n", (find_truss_time / CLOCKS_PER_SEC));
    return R;
}

struct node {
    int id;
    int comEdgeCnt;
};

bool cmp(node a, node b) {
    return a.comEdgeCnt < b.comEdgeCnt;
}

/*************************************************************************************************
函 数 名: KIndexDec
功能描述: 自上而下的寻找，从最大频繁集出发
输入参数: G图、k-truss值、查询点q、q的属性关键字、属性关键字的truss索引文件
输出参数: 无
返 回 值: 社区R
调用函数: 无
被调函数: main
修改历史:
    1.日期: 2018年09月22日
    作者: 何健
    修改: 创建函数
*************************************************************************************************/
vector<comType > EquiTrussAttribute::KIndexDec(PUNGraph &G, int k, int q, set<attrType> &SATR, const char* load_file)
{
    clock_t origin = clock();
    vector<comType > R;    //R是本函数返回的结果
    set<int> NbrNodes;
    unordered_map<attrType, int> attr_nbr_cnt;
    for (attrType elem : SATR) attr_nbr_cnt[elem] = 1;   //其他默认为0，查询点的属性初始为1，记得减一
    int max_value = 0;  //属性的最大占有值
                        //需要加一步判断邻居点中是否有k个邻居有共同的查找属性
    for (int i = 0; i < G->GetNI(q).GetDeg(); i++) {
        int w = G->GetNI(q).GetNbrNId(i);
        if (global_edgeTrussness[EdgePair(w, q)] >= k) {   //判断这条边是否满足k
            NbrNodes.insert(w);
            for (attrType elem : attributesOfVertex[w]) {
                if (attr_nbr_cnt[elem] != 0) {
                    if (attr_nbr_cnt[elem] > max_value) max_value = attr_nbr_cnt[elem];  //这样就不用减一
                    attr_nbr_cnt[elem] += 1;
                }
            }
        }
    }
    if (max_value < k - 1) {
        extra_time = 0;
        printf("实际时间(去掉传递时间)--找不到 Run Time:%f s\n\n", ((double)(clock() - origin) / CLOCKS_PER_SEC));
        return R;
    }
    //------上面仅仅是快速结束查询不到--------

    //-------加载属性truss子图并排序(按边)-------  同一个属性合并truss子图
    clock_t tim = clock();
    node at[505];
    int dex = 0;
    FILE *fin = fopen(load_file, "r");
    set<int> set_attr;
    for (attrType elem : SATR) {
        set_attr.insert(elem);
    }
    bool flag = false, is_read = false;
    int attr_id, supernode_cnt, cnt = SATR.size();
    while (!feof(fin)) {
        int u, v;
        fscanf(fin, "%d\t%d\n", &u, &v);
        if (v == -2) supernode_cnt = u;
        else if (u != -1 && v == -1) {                //读取属性id
            auto it = find(set_attr.begin(), set_attr.end(), u);  //属性在查询点里
            if (it != set_attr.end()) {
                flag = true;   //仅仅是满足查询的属性
                attr_id = u;
                cnt--;
                continue;
            }
        }
        else if (u == -1 && v == -1) {
            supernode_cnt--;
            if (supernode_cnt <= 0) {
                if (cnt <= 0) break;  //查询点的所有属性读取完毕
                flag = false;
                is_read = false;
            }
            continue;
        }
        else if (u != -1 && v == -3) {
            if (flag) {
                if (u >= k) {
                    is_read = true;  //满足k值条件
                }
            }
        }
        if (is_read  && supernode_cnt) {
            attr_com_vertex[attr_id].insert(u);
            attr_com_vertex[attr_id].insert(v);
            attr_com[attr_id].push_back(EdgePair(u, v));   //存储边
        }
    }
    fclose(fin);
    printf("Load attribute_truss time = %f  s\n\n", (double)(clock() - tim) / CLOCKS_PER_SEC);
    extra_time = clock() - tim;

    //-------遍历频繁集(自上而下)--------
    clock_t FP_Growth_Time = clock();
    set<int> psiInt;
    map<int, set<pair<set<attrType>, uint64_t> > > psi;
    psi = FP_Growth(NbrNodes, k - 1, psiInt, SATR);
    if (psi.size() == 0) return R;    //没有满足的频繁集直接返回
                                      //-------求频繁集------------

    clock_t find_time = clock();
    int i = psi.size();    //最好不要使用i，会出大问题的
    vector<int> psiDex;

    for (set<int>::iterator it = psiInt.begin(); it != psiInt.end(); it++) psiDex.push_back(*it);
    while (i >= 1) {
        set<pair<set<attrType>, uint64_t> >::iterator it_PSI = psi[psiDex[i - 1]].begin();
        //pis[l]存储的是{x,y,z},{x,z,w},{x,y,z}这样的集合，这里l的值为3，全部为长度为3的集合
        for (; it_PSI != psi[psiDex[i - 1]].end(); it_PSI++) {   //遍历频繁集中的每个组合
            node attr[55];
            int tag = 0;
            for (attrType at : it_PSI->first) {
                attr[tag].id = at;
                attr[tag++].comEdgeCnt = attr_com[at].size();     //思考是比较边还是顶点
            }
            sort(attr, attr + tag, cmp);
            //想想有没有这个查询点，应该有吧比较是查询点生成的频繁集

            PUNGraph subgraph = TUNGraph::New();
            for (edgePair edge : attr_com[attr[0].id]) {
                int u = edge.first, v = edge.second;
                if (!subgraph->IsNode(u)) subgraph->AddNode(u);
                if (!subgraph->IsNode(v)) subgraph->AddNode(v);
                subgraph->AddEdge(u, v);
            }
            //cout << "原子图 nodes = " << subgraph->GetNodes() << endl;
            //cout << "原子图 edges = " << subgraph->GetEdges() << endl << endl;

            for (int x = 1; x < tag; x++) {
                if (attr_com[attr[x].id].size() == 0) {
                    subgraph->Clr();
                    break;
                }

                //unordered_map<int, int> is_meet;
                //vector<int> is_meet(max_memory);
                is_meet.assign(max_memory, 0);
                for (int ver : attr_com_vertex[attr[x].id]) {
                    is_meet[ver] = 1;
                }
                for (TUNGraph::TNodeI NI = subgraph->BegNI(); NI < subgraph->EndNI(); NI++) {
                    int node = NI.GetId();
                    if (is_meet[node] != 1) subgraph->DelNode(node);
                }
            }

            //多次去度会增加时间，一次就好
            for (TUNGraph::TNodeI NI = subgraph->BegNI(); NI < subgraph->EndNI(); NI++) {
                int node = NI.GetId();
                if (NI.GetDeg() < k - 1) subgraph->DelNode(node);
            }

            if (!subgraph->IsNode(q)) {
                subgraph.Clr();
                continue;  //找不到满足子图
            }
            //cout << "degree剪枝后 nodes = " << subgraph->GetNodes() << endl;
            //cout << "degree剪枝后 edges = " << subgraph->GetEdges() << endl << endl;

            queue<int> Q;
            Q.push(q);
            PUNGraph GRAPH_q = TUNGraph::New();
            GRAPH_q->AddNode(q);
            while (!Q.empty()) {
                int cur = Q.front();  Q.pop();
                for (int x = 0; x < subgraph->GetNI(cur).GetDeg(); x++) {
                    int nbr = subgraph->GetNI(cur).GetNbrNId(x);
                    if (!GRAPH_q->IsNode(nbr)) {
                        Q.push(nbr);
                        GRAPH_q->AddNode(nbr);
                    }
                    GRAPH_q->AddEdge(cur, nbr);
                }
            }
            //cout << "component剪枝后 nodes = " << subgraph->GetNodes() << endl;
            //cout << "component剪枝后 edges = " << subgraph->GetEdges() << endl << endl;

            //find k-truss from G[W_]
            vector<comType> A;
            FindKTrussCom(GRAPH_q, k, q, A);

            if (A.size() != 0) {
                printf("(%d)", A.size());
                cout << "当前选择的最多共有属性：";
                for (attrType str : it_PSI->first) cout << str << ' ';
                cout << endl;
                for (comType com : A) R.push_back(com);
                A.clear();
            }
            GRAPH_q->Clr();
            subgraph->Clr();
        }
        if (R.size() == 0) i--;
        else break;
    }
    printf("实际时间(去掉传递时间) Run Time:%f s\n\n", ((double)(clock() - origin) / CLOCKS_PER_SEC));
    attr_com_vertex.clear();
    attr_com.clear();
    //unordered_map<int, vector<edgePair> >().swap(attr_com);   //只能设置成全局变量了
    //unordered_map<int, set<int> >().swap(attr_com_vertex);
    //printf("find_Time Run Time:%f s\n", ((double)(clock() - find_time) / CLOCKS_PER_SEC));
    //printf("find subgraph Time Run Time:%f s\n", (find_subgraph_time / CLOCKS_PER_SEC));
    //printf("find truss Time Run Time:%f s\n\n", (find_truss_time / CLOCKS_PER_SEC));
    return R;
}

/*************************************************************************************************
函 数 名: KIndexInc
功能描述: 从单个属性向多个属性进行组合查询
输入参数: G-图数据
输出参数:
返 回 值: 无
调用函数:
被调函数:
修改历史:
    1.日期: 2018年09月22日
    作者: 何健
    修改: 创建函数
*************************************************************************************************/
int maxMatchPsi = 0;        //当前最大的一个查询结果psi
vector<comType > RRR;    //R是本函数返回的结果
vector<comType > EquiTrussAttribute::KIndexInc(PUNGraph &G, int k, const int q, set<attrType> &SATR, const char* load_file)
{
    clock_t origin = clock();
    vector<comType > R;    //R是本函数返回的结果
    set<int> NbrNodes;
    unordered_map<attrType, int> attr_nbr_cnt;
    for (attrType elem : SATR) {
        attr_nbr_cnt[elem] = 1;   //其他默认为0，查询点的属性初始为1，记得减一
    }
    int max_value = 0;  //属性的最大占有值
    //需要加一步判断邻居点中是否有k个邻居有共同的查找属性
    for (int i = 0; i < G->GetNI(q).GetDeg(); i++) {
        int w = G->GetNI(q).GetNbrNId(i);
        if (global_edgeTrussness[EdgePair(w, q)] >= k) {   //判断这条边是否满足k
            NbrNodes.insert(w);
            for (attrType elem : attributesOfVertex[w]) {
                if (attr_nbr_cnt[elem] != 0) {
                    if (attr_nbr_cnt[elem] > max_value) max_value = attr_nbr_cnt[elem];  //这样就不用减一
                    attr_nbr_cnt[elem] += 1;
                }
            }
        }
    }
    //------快速结束查询不到--------
    if (max_value < k - 1) {
        extra_time = 0;
        printf("实际时间(去掉传递时间)--找不到 Run Time:%f s\n\n", ((double)(clock() - origin) / CLOCKS_PER_SEC));
        return R;
    }
    
    //-------求频繁集------------
    clock_t FP_Growth_Time = clock();
    set<int> psiInt;
    map<int, set<pair<set<attrType>, uint64_t> > > psi;
    psi = FP_Growth(NbrNodes, k - 1, psiInt, SATR);
    if (psi.size() == 0) return R;    //没有满足的频繁集直接返回
    vector<int> psiDex;
    for (set<int>::iterator it = psiInt.begin(); it != psiInt.end(); it++) psiDex.push_back(*it);

    //-------加载属性truss子图并排序(按边)-------  同一个属性合并truss子图
    clock_t tim = clock();
    node at[505];
    int dex = 0;
    FILE *fin = fopen(load_file, "r");
    if (fin == NULL) {
        cout << "load file not exist!!!" << endl;
        return R;
    }
    set<int> set_attr;
    for (attrType elem : SATR) {
        set_attr.insert(elem);
    }

    bool flag = false, is_read = false;
    int attr_id, supernode_cnt, cnt = SATR.size();
    //cout << "set_attr = " << set_attr.size() << endl;
    while (!feof(fin)) {
        int u, v;
        fscanf(fin, "%d\t%d\n", &u, &v);
        if (v == -2) supernode_cnt = u;
        else if (u != -1 && v == -1) {                //读取属性id
            auto it = find(set_attr.begin(), set_attr.end(), u);  //属性在查询点里
            if (it != set_attr.end()) {
                flag = true;   //仅仅是满足查询的属性
                attr_id = u;
                cnt--;
                continue;
            }
        }
        else if (u == -1 && v == -1) {
            supernode_cnt--;
            if (supernode_cnt <= 0) {
                if (cnt <= 0) break;  //查询点的所有属性读取完毕
                flag = false;
                is_read = false;
            }
            continue;
        }
        else if (u != -1 && v == -3) {
            if (flag) {
                if (u >= k) {
                    is_read = true;  //满足k值条件
                }
            }
        }
        if (is_read  && supernode_cnt) {
            attr_com_vertex[attr_id].insert(u);
            attr_com_vertex[attr_id].insert(v);
            attr_com[attr_id].push_back(EdgePair(u, v));   //存储边
        }
    }
    fclose(fin);
    printf("Load attribute_truss time = %f  s\n\n", (double)(clock() - tim) / CLOCKS_PER_SEC);
    extra_time = clock() - tim;

    //-------通过属性形成组合--------
    //1、先找出频繁集有哪些属性，可以减少一些组合
    set<attrType> SATR_temp;
    map<string, bool> ifExistPsi;
    for (map<int, set<pair<set<attrType>, uint64_t> > >::iterator it1 = psi.begin(); it1 != psi.end(); it1++) {
        for (set<pair<set<attrType>, uint64_t> >::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
            string skr = "";
            set<int> tmp;
            for (attrType elem : it2->first) {
                SATR_temp.insert(elem);
                tmp.insert(elem);
            }
            for (int attr : tmp) {
                skr += to_string(attr) + "_";
            }
            ifExistPsi[skr] = true;
        }
    }
    node attr[1005];
    int psiAttrCnt = 0;
    for (attrType at : SATR_temp) {
        attr[psiAttrCnt].id = at;
        attr[psiAttrCnt++].comEdgeCnt = attr_com[at].size();     //思考是比较边还是顶点
    }
    sort(attr, attr + psiAttrCnt, cmp);  //比较边
    int sortedPsiAttr[1005];
    for (int i = 0; i < psiAttrCnt; i++) {
        sortedPsiAttr[i] = attr[i].id;
    }
    tim = clock();
    RRR.clear();
    maxMatchPsi = 0;  //目前找到的最大匹配psi
    int combinePsi[1005];
    KindexInc_DFS(psiAttrCnt, sortedPsiAttr, 0, combinePsi, 0, false, k, q, ifExistPsi);
    printf("dfs time = %f  s\n", (double)(clock() - tim) / CLOCKS_PER_SEC);
    attr_com_vertex.clear();
    attr_com.clear();
    return RRR;
}

/*************************************************************************************************
函 数 名: KindexInc_DFS
功能描述: 使用深搜策略进行自下而上的寻找，并且进行优化剪枝
输入参数: psiAttrCnt属性数量、sortedPsiAttr排好序的单个属性、index、psiLevel、
输出参数: 无
返 回 值: 无
调用函数: 无
被调函数: KindexInc()
修改历史:
    1.日期: 2018年09月22日
    作者: 何健
    修改: 创建函数
*************************************************************************************************/
void EquiTrussAttribute::KindexInc_DFS(int psiAttrCnt, int sortedPsiAttr[], int index, int combinePsi[], int psiLevel, bool ifSelect, int k, int q, map<string, bool> &ifExistPsi)
{
    if (psiLevel >= maxMatchPsi && psiLevel && ifSelect) {   //会有重复，如a,b,c;选取a,b;然而在a,b输出一次，选取和不选取c的时候会再次输出a,b一次
        PUNGraph subgraph = TUNGraph::New();
        if (attr_com[combinePsi[0]].size() == 0) return;

        string skr = "";
        set<int> tmp;
        for (int i = 0; i < psiLevel; i++) tmp.insert(combinePsi[i]);
        for (int attr : tmp) {
            skr += to_string(attr) + "_";
        }
        if (!ifExistPsi[skr]) return;  //如果没有在psi集中就返回

        for (edgePair edge : attr_com[combinePsi[0]]) {
            int u = edge.first, v = edge.second;
            if (!subgraph->IsNode(u)) subgraph->AddNode(u);
            if (!subgraph->IsNode(v)) subgraph->AddNode(v);
            subgraph->AddEdge(u, v);
        }
        //cout << "原子图 nodes = " << subgraph->GetNodes() << endl;
        //cout << "原子图 edges = " << subgraph->GetEdges() << endl << endl;

        //从小到大进行属性交集
        for (int x = 1; x < psiLevel; x++) {
            if (attr_com[combinePsi[x]].size() == 0) return;
            //unordered_map<int, int> is_met;
            is_meet.assign(max_memory, 0);
            for (int ver : attr_com_vertex[combinePsi[x]]) {
                is_meet[ver] = 1;
            }
            for (TUNGraph::TNodeI NI = subgraph->BegNI(); NI < subgraph->EndNI(); NI++) {
                int node = NI.GetId();
                if (is_meet[node] != 1) subgraph->DelNode(node);
            }
            if (!subgraph->IsNode(q)) return;   //可以返回，当前组合无法形成子图，没有这种情况吧
        }
        //多次去度会增加时间，一次就好
        for (TUNGraph::TNodeI NI = subgraph->BegNI(); NI < subgraph->EndNI(); NI++) {
            int node = NI.GetId();
            if (NI.GetDeg() < k - 1) subgraph->DelNode(node);
        }
        //cout << "degree剪枝后 nodes = " << subgraph->GetNodes() << endl;
        //cout << "degree剪枝后 edges = " << subgraph->GetEdges() << endl << endl;
        if (!subgraph->IsNode(q)) return;   //可以返回，当前组合无法形成子图，度剪枝后可能会不存在查询q

        queue<int> Q;
        Q.push(q);
        PUNGraph GRAPH_q = TUNGraph::New();
        GRAPH_q->AddNode(q);
        while (!Q.empty()) {
            int cur = Q.front();  Q.pop();
            for (int i = 0; i < subgraph->GetNI(cur).GetDeg(); i++) {
                int nbr = subgraph->GetNI(cur).GetNbrNId(i);
                if (!GRAPH_q->IsNode(nbr)) {
                    Q.push(nbr);
                    GRAPH_q->AddNode(nbr);
                }
                GRAPH_q->AddEdge(cur, nbr);
            }
        }
        //subgraph->Clr();
        //cout << "component剪枝后 nodes = " << subgraph->GetNodes() << endl;
        //cout << "component剪枝后 edges = " << subgraph->GetEdges() << endl << endl;

        //find k-truss from G[W_]
        vector<comType> A;
        FindKTrussCom(GRAPH_q, k, q, A);
        if (A.size() != 0) {        //找到
            printf("(%d)", A.size());
            cout << "当前选择的最多共有属性：";   //这里就没法输出了，只能存储起来
            for (int str = 0; str < psiLevel; str++) {
                cout << combinePsi[str] << ' ';
                break;
            }
            cout << endl;
            if (psiLevel > maxMatchPsi) RRR.clear();    //把先前的清理
            for (comType com : A) RRR.push_back(com);
            maxMatchPsi = psiLevel;
            A.clear();
        }
        //return;   这里不能返回，因为我是在组合过程中的判断，组合还需要继续
    }
    if (index >= psiAttrCnt) return;
    combinePsi[psiLevel] = sortedPsiAttr[index];
    KindexInc_DFS(psiAttrCnt, sortedPsiAttr, index + 1, combinePsi, psiLevel + 1, true, k, q, ifExistPsi);  //选择当前属性
    KindexInc_DFS(psiAttrCnt, sortedPsiAttr, index + 1, combinePsi, psiLevel, false, k, q, ifExistPsi);        //不选择当前属性
    return;
}

/*************************************************************************************************
函 数 名: ACCore
功能描述: 基于core进行社区发现，只会有一个社区返回
输入参数: G-图数据、k-kore值、q-查询点、STAR-查询点q的属性
输出参数: comS-社区中返回的共同属性
返 回 值: 无
调用函数: 无
被调函数: main
修改历史:
    1.日期: 2018年10月02日
    作者: 何健
    修改: 创建函数
*************************************************************************************************/
vector<comType > EquiTrussAttribute::ACCore(PUNGraph G, int k, int q, set<attrType> &SATR, vector<string> &comS)
{
    clock_t startTime = clock();
    vector<comType > R;    //R是本函数返回的结果
    set<int> NbrNodes;
    //需要加一步判断邻居点中是否有k个邻居有共同的查找属性
    for (int i = 0; i < G->GetNI(q).GetDeg(); i++) {
        int w = G->GetNI(q).GetNbrNId(i);
        if (G->GetNI(w).GetDeg() >= k) NbrNodes.insert(w);  // core必须大于等于k
    }

    map<int, set<pair<set<attrType>, uint64_t> > > psi;
    set<int> psiInt;
    psi = FP_Growth(NbrNodes, k - 1, psiInt, SATR);
    int i = psi.size();
    vector<int> psiDex;  //psiInt是set型，从小到大排列，而psiDex用vector可以使用下标倒序访问
    for (set<int>::iterator it = psiInt.begin(); it != psiInt.end(); it++) psiDex.push_back(*it);
    while (i >= 1) {
        set<pair<set<attrType>, uint64_t> >::iterator it_PSI = psi[psiDex[i - 1]].begin();
        //pis[l]存储的是{x,y},{x,z},{y,z}这样的序列对
        for (; it_PSI != psi[psiDex[i - 1]].end(); it_PSI++) {
            global_psi.clear();
            global_psi = it_PSI->first;
            //遍历整个图查询满足的顶点这部分最耗费时间
            clock_t tim = clock();
            //vector<int> is_meet(max_memory);  //判断点是否满足，0未访问，1满足，2不满足
            is_meet.assign(max_memory, 0);      //改成全局变量了，记得清理，clear()并没有什么卵用，这个应该当作初始化
            PUNGraph GRAPH = TUNGraph::New();
            for (TUNGraph::TEdgeI EI = G->BegEI(); EI < G->EndEI(); EI++) { //Find G[W_] from the subgraph from Hi
                int node_u = EI.GetDstNId(), node_v = EI.GetSrcNId();
                if (G->GetNI(node_u).GetDeg() < k || G->GetNI(node_v).GetDeg() < k) continue;
                if (is_meet[node_u] == 2 || is_meet[node_v] == 2) continue;
                if (is_meet[node_u] == 1 && is_meet[node_v] == 1) {
                    if (!GRAPH->IsNode(node_u)) GRAPH->AddNode(node_u);
                    if (!GRAPH->IsNode(node_v)) GRAPH->AddNode(node_v);
                    GRAPH->AddEdge(node_u, node_v);
                }
                if (is_meet[node_u] == 0) {
                    if (ContainsAll(node_u)) is_meet[node_u] = 1;
                    else {
                        is_meet[node_u] = 2;
                        continue;
                    }
                }
                if (is_meet[node_v] == 0) {
                    if (ContainsAll(node_v)) is_meet[node_v] = 1;
                    else {
                        is_meet[node_v] = 2;
                        continue;
                    }
                }
                if (!GRAPH->IsNode(node_u)) GRAPH->AddNode(node_u);
                if (!GRAPH->IsNode(node_v)) GRAPH->AddNode(node_v);
                GRAPH->AddEdge(node_u, node_v);
            }

            if (!GRAPH->IsNode(q)) {
                GRAPH.Clr();
                continue;
            }
            vector<comType> H;
            FindKCoreCom(GRAPH, k, q, H);
            GRAPH.Clr();
            if (H.size() != 0) {
                printf("社区数量 = (%d)", H.size());
                cout << "当前选择的最多共有属性：";
                string SS = "#";
                for (attrType str : it_PSI->first) {
                    SS += str;
                    SS += "#";
                    cout << str << ' ';
                }
                cout << endl;
                for (comType com : H) R.push_back(com);
                comS.push_back(SS);
                H.clear();
            }
        }
        if (R.size() == 0) i--;   // R就是返回结果，当不为0时就是最大社区
        else break;
    }
    printf("实际时间(去掉传递时间) Run Time:%f s\n\n", ((double)(clock() - startTime) / CLOCKS_PER_SEC));
    return R;
}

/*************************************************************************************************
函 数 名: FindKCoreCom
功能描述: 在图中寻找k-core子图
输入参数: G图
输出参数:
返 回 值: 无
调用函数: 无
被调函数: ACCore()
修改历史:
    1.日期: 2018年10月02日
    作者: 何健
    修改: 创建函数
*************************************************************************************************/
void EquiTrussAttribute::FindKCoreCom(PUNGraph G, int k, int q, vector<comType> &C)
{
    if (G->GetNI(q).GetDeg() < k) return;

    PUNGraph subGraph = TUNGraph::New();

    is_meet.assign(max_memory, 0);           // 改成全局变量了，记得清理，clear()并没有什么卵用，这个应该当作初始化
    queue<int> Q;
    Q.push(q);
    while (!Q.empty()) {
        int node = Q.front();
        Q.pop();
        is_meet[node] = 1;

        if (!subGraph->IsNode(node)) subGraph->AddNode(node);
        for (int i = 0; i < G->GetNI(node).GetDeg(); i++) {
            int w = G->GetNI(node).GetNbrNId(i);
            if (G->GetNI(w).GetDeg() < k || is_meet[w]) continue;
            Q.push(w);
        }
    }
    for (TUNGraph::TEdgeI EI = G->BegEI(); EI < G->EndEI(); EI++) {
        int u = EI.GetDstNId(), v = EI.GetSrcNId();
        if (subGraph->IsNode(u) && subGraph->IsNode(v) && !subGraph->IsEdge(u, v)) subGraph->AddEdge(u, v);
    }

    while (subGraph->GetNodes()) {
        bool flag = false;
        vector<int> delNodes;
        for (TUNGraph::TNodeI NI = subGraph->BegNI(); NI < subGraph->EndNI(); NI++) {
            if (NI.GetDeg() < k) {
                //subGraph->DelNode(NI.GetId());  //直接删除可能出现迭代器丢失
                delNodes.push_back(NI.GetId());
                flag = true;
            }
        }
        if (!flag) break;
        for (int node : delNodes) subGraph->DelNode(node);
        delNodes.clear();
    }

    //一定最多只能返回一个社区
    comType com;
    for (TUNGraph::TEdgeI EI = subGraph->BegEI(); EI < subGraph->EndEI(); EI++) {
        int u = EI.GetDstNId(), v = EI.GetSrcNId();
        com.push_back(EdgePair(u, v));
    }
    if (com.size()) C.push_back(com);
    return;
}

/*************************************************************************************************
函 数 名: NCTruss
功能描述: 没有属性图的搜索
输入参数: G-图数据、k-truss值、q-查询点
输出参数: 无
返 回 值: 查询的社区
调用函数: 无
被调函数: mian
修改历史:
    1.日期: 2018年10月02日
    作者: 何健
    修改: 创建函数
*************************************************************************************************/
vector<comType > EquiTrussAttribute::NCTruss(PUNGraph G, int k, int q)
{
    clock_t startTime = clock();
    vector<comType > R;
    PUNGraph subgraph = TUNGraph::New();
    set<int> NbrNodes;   // 查询q的邻居点并且边truss满足k
    //需要加一步判断邻居点中是否有k个邻居有共同的查找属性
    for (int i = 0; i < G->GetNI(q).GetDeg(); i++) {
        int w = G->GetNI(q).GetNbrNId(i);
        if (global_edgeTrussness[EdgePair(w, q)] >= k) {   //判断这条边是否满足k
            NbrNodes.insert(w);
        }
    }

    //Find all super-nodes which its k >= k containing q and put them into S(q)
    vector<int> SuperNode_S;
    for (set<int>::iterator id = vertexSNIndex[q].begin(); id != vertexSNIndex[q].end(); id++) {
        if (superNode[*id].k >= k) {
            SuperNode_S.push_back(*id);
        }
    }
    if (SuperNode_S.size() == 0) return R;

    unordered_map<int, bool> processed, VIS;        //超点是否处理，访问
    for (vector<SuperNode>::iterator it = superNode.begin() + 1; it != superNode.end(); it++) {
        processed[it->snID] = false;
        VIS[it->snID] = false;
    }

    int maxn = 0, j;
    double KTSummary_Time = 0; int KTSummaryCnt = 0;
    double MergeAndComputeNodes_Time = 0, FP_Growth_Time = 0, PsiSize = 0, NodesSize = 0;
    for (vector<int>::iterator it_S = SuperNode_S.begin(); it_S != SuperNode_S.end(); it_S++) {
        //SuperNode s = ;   去掉这种单独拿出来的做法，会浪费时间
        clock_t tim = clock();
        vector<edgePair> As = NAKTrussSummary(processed, *it_S, k, q); //As是EquiTruss返回的结果
        //if(KTSummaryCnt == 0 ) printf("Only One KTSummary Run Time:%f s\n\n", (double)(clock() - tim) / CLOCKS_PER_SEC);
        KTSummaryCnt++;
        KTSummary_Time += clock() - tim;
        if (As.size() == 0) continue;
        for (edgePair edge : As) {
            int u = edge.first, v = edge.second;
            if (!subgraph->IsNode(u)) subgraph->AddNode(u);
            if (!subgraph->IsNode(v)) subgraph->AddNode(v);
            if (!subgraph->IsEdge(u, v)) subgraph->AddEdge(u, v);
        }
    }
    FindKTrussCom(subgraph, k, q, R);
    subgraph.Clr();
    printf("实际时间(去掉传递时间) Run Time:%f s\n\n", ((double)(clock() - startTime) / CLOCKS_PER_SEC));
    return R;
}

/*************************************************************************************************
函 数 名: NAKTrussSummary
功能描述: 没有属性图寻找子图，即超点集合
输入参数: processed-超点的遍历标记、SN_id-超点id、k-truss值、q-查询点
输出参数: 无
返 回 值: 查询的社区
调用函数: 无
被调函数: NCTruss()
修改历史:
    1.日期: 2018年10月02日
      作者: 何健
      修改: 创建函数
*************************************************************************************************/
vector<edgePair> EquiTrussAttribute::NAKTrussSummary(unordered_map<int, bool> &processed, int SN_id, int k, int q)
{
    clock_t sum = clock();
    int l = 0;    //the number of communities
    vector<edgePair> As;  //As set is superNode set of including query vertex q

    if (processed[SN_id] == false) {
        processed[SN_id] = true;
        As.clear();
        queue<int> Q;    //Q可以只放int型id
        Q.push(SN_id);
        while (!Q.empty()) {
            //WhileQ_Cnt++;
            SN_id = Q.front(); Q.pop();
            clock_t tim = clock();
            vector<edgePair>::iterator iter = superNode[SN_id].SNEdgeList.begin();
            for (; iter != superNode[SN_id].SNEdgeList.end(); iter++) {
                As.push_back(*iter);
            }
            tim = clock();
            int vid = SN_id;
            for (int i = 0; i < superGraph->GetNI(vid).GetDeg(); i++) {
                int nbr = superGraph->GetNI(vid).GetNbrNId(i);
                clock_t tx = clock();
                int uid = superNode[nbr].snID;
                if (superNode[nbr].k >= k && processed[uid] == false) {
                    processed[uid] = true;
                    Q.push(nbr);
                }
            }
        }
    }
    //printf("KTS_Time Run Time:%f s\n", ((clock() - sum) / CLOCKS_PER_SEC));
    return As;
}
