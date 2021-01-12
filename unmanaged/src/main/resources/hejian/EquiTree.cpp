#include "EquiTree.h"

typedef std::pair<int, TIntV> nv_type;
typedef std::pair<int, int> eid_type;
typedef int vid_type;

eid_type edge_composer(const vid_type &u, const vid_type &v)
{
    if (u < v)
        return std::make_pair(u, v);
    else
        return std::make_pair(v, u);
}

void get_low_high_deg_vertices(const PUNGraph &net, const vid_type &src, const vid_type &dst, vid_type &low, vid_type &high)
{
    if (net->GetNI(src).GetDeg() > net->GetNI(dst).GetDeg()) {
        low = dst;
        high = src;
    }
    else {
        low = src;
        high = dst;
    }
}

int compute_support(const PUNGraph &net, const PUNGraph::TObj::TEdgeI &EI)
{
    int support = 0;
    vid_type u = -1, v = -1;

    vid_type src = EI.GetSrcNId();
    vid_type dst = EI.GetDstNId();
    get_low_high_deg_vertices(net, src, dst, u, v);

    for (int i = 0; i < net->GetNI(u).GetDeg(); i++) {
        vid_type w = net->GetNI(u).GetNbrNId(i);
        if (net->IsEdge(w, v))
            support += 1;
    }

    return support;
}

/*************************************************************************************************
函 数 名:  getCC(const EquiTrussAttribute & truss)
功能描述:
输入参数:
输出参数:
返 回 值:
调用函数:
被调函数:
修改历史:
1.日期: 2019 年3月28日
作者：张倩
修改：创建函数
*************************************************************************************************/
TVec<TIntV> getCC(PUNGraph net)
{
    TVec<TIntV> out;
    TCnComV ccv;
    TSnap::GetWccs(net, ccv);
    int length = ccv.Len();
    for (int i = 0; i < length; i++)
    {
        TCnCom cc = ccv[i];
        if (!cc.Empty())
        {
            out.Add(cc.NIdV);
        }
    }
    return out;
}

TVec<TIntV> getCCfromV(const EquiTrussAttribute & truss, int i, map<int, TIntV>& truss2sid)
{
    TIntV nodev;
    for (auto it = truss2sid.begin(); it != truss2sid.end(); it++)
    {
        if (it->first >= i)
        {
            nodev.AddV(it->second);
        }
    }

    PUNGraph graph = TUNGraph::New();
    for (int i = 0; i < nodev.Len(); i++)
    {
        int src = nodev[i];
        if (!graph->IsNode(src)) graph->AddNode(src);
        for (int j = 0; j < nodev.Len(); j++)
        {
            int dst = nodev[j];
            if (!graph->IsNode(dst)) graph->AddNode(dst);
            if ((!graph->IsEdge(src, dst)) && truss.superGraph->IsEdge(src, dst)) graph->AddEdge(src, dst);
        }
    }

    return getCC(graph);
}

set<attrType> jiaoji(set<int>& attr, set<int>& tattr)
{
    set<int> temp_set;
    set_intersection(attr.begin(), attr.end(), tattr.begin(), tattr.end(), inserter(temp_set, temp_set.begin()));
    return temp_set;
}

int compute_trussness(const PUNGraph &net, map<eid_type, int> &edge_support,
    set< pair<int, eid_type> > &sorted_edge_support, map<eid_type, int>& edge_trussness) {
    int k = 2;
    while (!sorted_edge_support.empty()) {
        while (!sorted_edge_support.empty() &&
            sorted_edge_support.begin()->first <= k - 2) {
            eid_type e = sorted_edge_support.begin()->second;
            int support = sorted_edge_support.begin()->first;
            vid_type u = -1, v = -1;
            get_low_high_deg_vertices(net, e.first, e.second, u, v);
            for (int i = 0; i < net->GetNI(u).GetDeg(); i++) {
                vid_type w = net->GetNI(u).GetNbrNId(i);
                if (edge_support.find(edge_composer(u, w)) != edge_support.end() &&
                    edge_support.find(edge_composer(w, v)) != edge_support.end()) {
                    int uw_support = edge_support[edge_composer(u, w)];
                    sorted_edge_support.erase(
                        make_pair(uw_support, edge_composer(u, w)));
                    edge_support[edge_composer(u, w)] -= 1;
                    uw_support -= 1;
                    sorted_edge_support.insert(
                        make_pair(uw_support, edge_composer(u, w)));
                    int vw_support = edge_support[edge_composer(v, w)];
                    sorted_edge_support.erase(
                        make_pair(vw_support, edge_composer(v, w)));
                    edge_support[edge_composer(v, w)] -= 1;
                    vw_support -= 1;
                    sorted_edge_support.insert(
                        make_pair(vw_support, edge_composer(v, w)));
                }
            }
            edge_trussness[e] = k;
            edge_support.erase(e);
            sorted_edge_support.erase(make_pair(support, e));
        }
        k += 1;
    }
    return k - 1;
}

int ktrussness(const PUNGraph& net, map<eid_type, int>& edge_trussness) {
    set< pair<int, eid_type>> sorted_edge_support;
    map<eid_type, int> edge_support;

    for (TUNGraph::TEdgeI EI = net->BegEI(); EI < net->EndEI(); EI++) {
        int support = compute_support(net, EI);
        edge_support.insert(make_pair(edge_composer(
            EI.GetSrcNId(), EI.GetDstNId()), support));
        sorted_edge_support.insert(make_pair(support, edge_composer(
            EI.GetSrcNId(), EI.GetDstNId())));
    }
    int trussness_max = compute_trussness(net, edge_support, sorted_edge_support, edge_trussness);

    return trussness_max;
}

/*************************************************************************************************
函 数 名: EquiTreeIndex构造函数
功能描述:从EquiTrussAttribute构造树索引
输入参数:EquiTrussAttribute
输出参数:
返 回 值:
调用函数:createNode(),cMixNode()
被调函数:
修改历史:
1.日期: 2019 年3月9日
作者: 张倩
修改: 创建函数
2.日期: 2019 年3月28日
作者: 张倩
修改: 添加建立树索引的新方法
3.日期: 2019 年3月30日
作者: 张倩
修改: 添加本地索引，存储+读取；
    需要本地索引的有：

    图(树节点，树边)-通过net
    map<树节点，超点集合>-通过net
    map<truss值，树节点>-通过truss2node
    map<树节点，包含的属性值集合>-最后计算
*************************************************************************************************/
EquiTreeIndex::EquiTreeIndex(const EquiTrussAttribute & truss, const char* sNet_File, const char* sNetData_file, const char* sTreeAttr_file)
{
    //新方法
    makeSet(truss);
    //showSet();

    clock_t indextime = clock();

    Net = TNodeNet<TIntV>::New();

    ifstream isfile(sNet_File);
    if (isfile)
    {
        cout << "本地有树索引文件，加载中...." << endl;
        LoadTreeGraph(sNet_File, sNetData_file, sTreeAttr_file);
        showTree();
        return;
    }

    TVec<TIntV> cc = getCC(truss.superGraph);
    int k = 0;
    int tvalue = trussness[k];
    for (int i = 0; i < cc.Len(); i++)
    {
        TIntV temp = cc[i];
        int node = Net->AddNode();
        Net->SetNDat(node, temp);
        truss2node[tvalue].Add(node);//id为node的树节点的trussness是tvalue
    }
    while (tvalue < maxtness)
    {
        k += 1;
        tvalue = trussness[k];
        cc = getCCfromV(truss, tvalue, truss2sid);        //    此处用时较多
        for (int i = 0; i < cc.Len(); i++)
        {
            TIntV temp = cc[i]; temp.Sort();
            int node = Net->AddNode();
            Net->SetNDat(node, temp);
            truss2node[tvalue].Add(node);
            int uk = k - 1;
            for (int j = 0; j < truss2node[trussness[uk]].Len(); j++)
            {
                int upnode = truss2node[trussness[uk]][j];
                TIntV uptemp = Net->GetNDat(upnode);
                uptemp.Sort();
                if (temp.IntrsLen(uptemp) != 0)
                {
                    uptemp.Diff(temp);
                    Net->SetNDat(upnode, uptemp);
                    Net->AddEdge(upnode, node);
                    break;
                }
            }
        }
    }
    printf("生成树索引+属性Time:%f s\n\n", ((double)(clock() - indextime) / CLOCKS_PER_SEC));
    printf("Net 点：%d， 边：%d\n", Net->GetNodes(), Net->GetEdges());

    //****************************************************************************
    //输出Net 图(树节点，树边)+ map<树节点，超点集合>
    indextime = clock();
    FILE* outNet = fopen(sNet_File, "w");
    FILE* outNetData = fopen(sNetData_file, "w");
    //所有边
    //此处有问题 会重复输出data
    for (TNodeNet<TIntV>::TEdgeI i_tedge= Net->BegEI(); i_tedge != Net->EndEI(); i_tedge++)
    {
        fprintf(outNet, "%d %d\n", i_tedge.GetSrcNId(), i_tedge.GetDstNId());
    }
    //没有边的节点
    for (TNodeNet<TIntV>::TNodeI i_tnode = Net->BegNI(); i_tnode != Net->EndNI(); i_tnode++)
    {
        if (i_tnode.GetDeg() == 0)
        {
            fprintf(outNet, "%d -1\n", i_tnode.GetId());
        }
        fprintf(outNetData, "%d", i_tnode.GetId());
        TIntV v_node = i_tnode.GetDat();
        for (int i = 0; i < v_node.Len(); i++)
        {
            fprintf(outNetData, " %d", v_node[i]);
        }
        fprintf(outNetData, "\n");
    }
    fclose(outNet);
    fclose(outNetData);
    printf("输出树索引Time:%f s\n\n", ((double)(clock() - indextime) / CLOCKS_PER_SEC));


    //****************************************************************************
    //map<树节点，包含的属性值集合>
    //存储树节点包含的属性值
    indextime = clock();
    for (TNodeNet<TIntV>::TNodeI i_tnode = Net->BegNI(); i_tnode != Net->EndNI(); i_tnode++)
    {
        int tnode = i_tnode.GetId();
        TIntV v_snode = i_tnode.GetDat();
        set<Item> tattr;        //当前树节点的属性
        for (int i = 0; i < v_snode.Len(); i++)
        {
            set<Item> t_tattr;        //即将将union后的属性存入
            int snode = v_snode[i];
            set<int> sattr = truss.SNAttributes.at(snode);        //set内的元素不重复，并已经按从小到大排序
            set_union(tattr.begin(), tattr.end(), sattr.begin(), sattr.end(), inserter(t_tattr, t_tattr.begin()));
            tattr = t_tattr;
        }
        tree2attr[tnode] = tattr;
    }
    printf("生成属性索引Time:%f s\n\n", ((double)(clock() - indextime) / CLOCKS_PER_SEC));

    //****************************************************************************
    //输出树节点对应的属性map
    indextime = clock();
    FILE* outTreeAttr = fopen(sTreeAttr_file, "w");
    for (auto i = tree2attr.begin(); i != tree2attr.end(); i++)
    {
        fprintf(outTreeAttr, "%d", i->first);
        set<Item> v_tattr = i->second;
        for (auto j = v_tattr.begin(); j != v_tattr.end(); j++)
        {
            fprintf(outTreeAttr, " %d", *j);
        }
        fprintf(outTreeAttr, "\n");
    }
    printf("输出属性索引Time:%f s\n\n", ((double)(clock() - indextime) / CLOCKS_PER_SEC));

    showTree();
    return;
}

/*************************************************************************************************
函 数 名:
功能描述:增加树节点个数
输入参数:
输出参数:
返 回 值:
调用函数:
被调函数:
修改历史:
1.日期: 2019年3月9日
作者: 张倩
修改: 创建函数
*************************************************************************************************/
int EquiTreeIndex::makeNum()
{
    currentid += 1;
    return currentid;
}

bool EquiTreeIndex::LoadTreeGraph(const char * sNet_File, const char * sNetData_file, const char * sTreeAttr_file)
{
    //****************************************************************************
    //读取文件中的Net
    clock_t indextime = clock();
    FILE* inNet = fopen(sNet_File, "r");
    while (!feof(inNet))
    {
        int src, dst;
        fscanf(inNet, "%d %d\n", &src, &dst);
        if (!Net->IsNode(src)) Net->AddNode(src);
        if ((dst != -1) && (!Net->IsNode(dst))) Net->AddNode(dst);
        if ((dst != -1) && (!Net->IsEdge(src, dst))) Net->AddEdge(src, dst);
    }
    fclose(inNet);
    printf("读取树索引Time:%f s\n", ((double)(clock() - indextime) / CLOCKS_PER_SEC));
    printf("Net 点：%d， 边：%d\n", Net->GetNodes(), Net->GetEdges());

    //读取文件中nodedata
    indextime = clock();
    FILE* inNetData = fopen(sNetData_file, "r");
    char line[300000];
    TStrV cv;
    while (!feof(inNetData))
    {
        memset(line, 0, 300000);
        fgets(line, 300000, inNetData);
        if (strlen(line) == 0)    break;
        TStr(line).SplitOnAllCh(' ', cv, false);
        int node = cv[0].GetTrunc().GetInt();
        TIntV v_nattr;
        for (int i = 1; i < cv.Len(); i++)
        {
            int snode = cv[i].GetTrunc().GetInt();
            v_nattr.Add(snode);
        }
        //truss2sid[node] = v_nattr;
        //此处应该填truss2node索引
        //只要判断一个超点的truss值即可
        if (!v_nattr.Empty())
        {
            for (auto it = truss2sid.begin(); it != truss2sid.end(); it++)
            {
                if (it->second.IsIn(v_nattr[0]))
                {
                    truss2node[it->first].Add(node);
                }
            }
        }
        Net->SetNDat(node, v_nattr);//树节点对应的超点集合
    }
    fclose(inNetData);
    printf("读取nodedata Time:%f s\n\n", ((double)(clock() - indextime) / CLOCKS_PER_SEC));

    //****************************************************************************
    //读取树节点对应的属性，存入map
    indextime = clock();
    FILE* inTreeAttr = fopen(sTreeAttr_file, "r");
    int a;
    char b;
    int node = -1;
    while (!feof(inTreeAttr))
    {
        fscanf(inTreeAttr, "%d", &a);
        b = fgetc(inTreeAttr);
        if (b == ' ')
        {
            if (node == -1)
            {
                node = a;
            }
            else
            {
                tree2attr[node].insert(a);
            }
        }
        else
        {
            tree2attr[node].insert(a);
            node = -1;
        }
    }
    fclose(inTreeAttr);
    printf("读取属性索引Time:%f s\n\n", ((double)(clock() - indextime) / CLOCKS_PER_SEC));

    return true;
}

/*************************************************************************************************
函 数 名:makeSet（存在问题？）
功能描述:遍历equitruss，得到最大truss值以及得到snset
输入参数:
输出参数:
返 回 值:
调用函数:
被调函数:
修改历史:
1.日期: 2019年3月9日
作者: 张倩
修改: 创建函数
*************************************************************************************************/
void EquiTreeIndex::makeSet(const EquiTrussAttribute & truss)
{
    maxtness = 0;
    mintness = 10000000;
    for (auto iter = truss.superNode.begin()+1;
        iter != truss.superNode.end(); iter++)
    {
        trussness.AddUnique(iter->k);
        truss2sid[iter->k].Add(iter->snID);
        if (maxtness < iter->k)
            maxtness = iter->k;
        if (mintness > iter->k)
            mintness = iter->k;
    }
    printf("max %d, min %d\n", maxtness, mintness);
    trussness.Sort();
}

void EquiTreeIndex::showSet()
{
    printf("trussness，超点个数\n");
    for (auto i = truss2sid.begin(); i != truss2sid.end(); i++)
    {
        printf("%d, %d\n", i->first, i->second.Len());
    }
}

void EquiTreeIndex::showTree()
{
    printf("树索引点%d,边%d\n", Net->GetNodes(), Net->GetEdges());
    printf("树节点属性map 个数：%d\n", tree2attr.size());
    //printf("trussness，树节点个数\n");
    //for (auto it = truss2node.begin(); it != truss2node.end(); it++)
    //{
    //    printf("%d,%d\n", it->first, it->second.Len());
    //}
    
}

int Find_K_Truss(PUNGraph& net, int kt) {
    //clock_t time = clock();

    int k = kt - 1;
    for (PUNGraph::TObj::TNodeI ti = net->BegNI(); ti != net->EndNI(); ti++)
    {
        if (ti.GetDeg() <k){net->DelNode(ti.GetId());}
    }

    //printf("1:%f s\n\n", ((double)(clock() - time) / CLOCKS_PER_SEC));

    //time = clock();

    map<eid_type, int> edge_trussness;
    int kmax = ktrussness(net, edge_trussness);

    //printf("2:%f s\n\n", ((double)(clock() - time) / CLOCKS_PER_SEC));

    //time = clock();

    PUNGraph tmpnet = TUNGraph::New();
    for (PUNGraph::TObj::TEdgeI EI = net->BegEI(); EI < net->EndEI(); EI++) {
        int start = EI.GetSrcNId(); int end = EI.GetDstNId();
        if (edge_trussness[edge_composer(start, end)] >= kt) {
            if (!tmpnet->IsNode(start)) {tmpnet->AddNode(start);}
            if (!tmpnet->IsNode(end)) {tmpnet->AddNode(end);}
            tmpnet->AddEdge(start, end);
        }
    }
    //printf("3:%f s\n\n", ((double)(clock() - time) / CLOCKS_PER_SEC));

    if (tmpnet->Empty())
    {
        //printf("no subgraph.\n");
        return 0;
    }
    else
    {
        TCnComV ccv;
        TSnap::GetWccs(tmpnet, ccv);
        //printf("%d个\n", ccv.Len());
        return ccv.Len();
    }
}

/*************************************************************************************************
函 数 名:  findSubTree(int k)
功能描述:在树索引中找到大于等于K的子图
输入参数:int k
输出参数:
返 回 值:
调用函数:
被调函数:
修改历史:
1.日期: 2019 年3月28日
作者：张倩
修改：创建函数
*************************************************************************************************/
void EquiTreeIndex::findSubTree(int k, set<int>& attr, const EquiTrussAttribute & truss, int num)
{
    //clock_t findtreetine = clock();
    clock_t begin = clock();

    TIntV v_node = truss2node[k];
    TVec<PNGraph> v_subgraph;
    for (int i = 0; i < v_node.Len(); i++)
    {
        int t_node = v_node[i];
        PNGraph t_graph = TNGraph::New();
        t_graph    = TSnap::GetBfsTree(Net, t_node, true, false);        //广度优先搜索生成树 t_graph里的节点id与原树索引的节点id相同
        v_subgraph.Add(t_graph);
    }
    int length = v_subgraph.Len();
    printf("子树个数：%d\n", length);
    //printf("找到子树的时间:%f s\n\n", ((double)(clock() - findtreetine) / CLOCKS_PER_SEC));

    //findtreetine = clock();
    //找到包含给定属性集attr的树节点
    TVec<TIntV> snvv;
    for (int i = 0; i < length; i++)
    {
        PNGraph t_graph = v_subgraph[i];
        TIntV snv;
        for (TNGraph::TNodeI i_node = t_graph->BegNI(); i_node != t_graph->EndNI(); i_node++)
        {
            int tnode = i_node.GetId();
            set<int> tattr = tree2attr[tnode];
            set<int> temp_set;
            set_intersection(attr.begin(), attr.end(), tattr.begin(), tattr.end(), inserter(temp_set, temp_set.begin()));
            if ((int)temp_set.size() >= num)
            {
                snv.AddVMerged(Net->GetNDat(tnode));
            }
        }
        snvv.Add(snv);
    }
    //printf("对应的超点个数：%d\n", snvv.Len());
    //printf("找到对应的超点时间:%f s\n\n", ((double)(clock() - findtreetine) / CLOCKS_PER_SEC));

    unordered_map<int, set<attrType> > attOfVer = truss.attributesOfVertex_set;//节点的属性索引
    unordered_map<int, set<attrType> > attOfSNode = truss.SNAttributes;//超点的属性索引

    //findtreetine = clock();
    
    int cnt = 0;

    for (int i = 0; i < snvv.Len(); i++)
    {
        TIntV snv = snvv[i];    //子树
        PUNGraph graph = TUNGraph::New();
        //printf("%d\n", snv.Len());
        for (int j = 0; j < snv.Len(); j++)
        {
            int snode = snv[j];
            set<int> temp_set = jiaoji(attr, attOfSNode[snode]);
            if (temp_set.size() >= num)
            {
                vector<edgePair> sedgev = truss.superNode[snode].SNEdgeList;//超点snode对应的边的集合
                for (int m = 0; m < sedgev.size(); m++)
                {
                    int src = sedgev[m].first;
                    int dst = sedgev[m].second;
                    if ((int)jiaoji(attr, attOfVer[src]).size() >= num && (int)jiaoji(attr, attOfVer[dst]).size() >= num)
                    {
                        if (!graph->IsNode(src)) { graph->AddNode(src); }
                        if (!graph->IsNode(dst)) { graph->AddNode(dst); }
                        if (!graph->IsEdge(src, dst)) { graph->AddEdge(src, dst); }
                    }
                }
            }
        }
        if (!graph->Empty())
        {
            //printf("true");
            cnt = cnt + Find_K_Truss(graph, k);
        }
    }
    printf("%d\n", cnt);
    printf("搜索时间:%f s\n\n", ((double)(clock() - begin) / CLOCKS_PER_SEC));
}

