#include "stdafx.h"
#include "EquiTrussAttribute.h"
#include "DataProcess.h"
#include "DataProcess.cpp"
#include <omp.h>
//#include <direct.h>
//#include <io.h>
using namespace std;


/************************************************************************************************
                                            变量说明
*************************************************************************************************/

/************************************************************************************************
                                            全局变量
*************************************************************************************************/
map<string, int> keyword_in;
double attr_rate = 1;            // 属性伸缩性，占20个数的百分比
int attrNumMax = 20;            // 点的属性数量值，默认是20，测试用的是5
double vertex_rate = 1;            // 顶点伸缩性，占300个数的百分比
int k_value = 6;
#define OK true
#define ERROR false

/*************************************************************************************************
函 数 名: PrintG()
功能描述: 输出图点边信息
输入参数: 无
输出参数: 无
返 回 值: 无
调用函数: 无
被调函数: mian()
修改历史:
    1.日期: 2019年03月24日
      作者: l
      修改: 创建函数
*************************************************************************************************/
#define MAXD 10000000
#define MAXK 10000000
#define MAXS 1000000
vector<int> doc[MAXD];
vector<int> key[MAXK];
char line[MAXS];
int nd, nk;   // 最大值
void read_in(string path, vector<int> nodes) {
    nd = 0; nk = 0;
    int nl = 0;
    FILE *fin = fopen(path.c_str(), "r");
    while (fgets(line, MAXS, fin)) {
        //if (++nl % 100000 == 0) printf("%d lines finished!\n", nl);
        int len = strlen(line);
        for (int i = 0; i < len; ++i)
            if (!isdigit(line[i])) line[i] = '\0';
        int d, k;
        sscanf(line, "%d", &d);   // 遇到空格就结束吧
        if (nodes[d] == 0) continue;
        for (int i = 1; i < len; ++i)
            if (isdigit(line[i]) && !isdigit(line[i - 1])) {
                sscanf(line + i, "%d", &k);
                doc[d].push_back(k);   // 点中有哪些属性
                key[k].push_back(d);   // 属性中有哪些点
                nd = max(nd, d + 1);  // +1的原因不会取到
                nk = max(nk, k + 1);
            }
    }
    for (int i = 0; i < nd; ++i) sort(doc[i].begin(), doc[i].end());
    for (int i = 0; i < nk; ++i) sort(key[i].begin(), key[i].end());
    printf("nd=%d, nk=%d\n", nd, nk);
    fclose(fin);
}

/*************************************************************************************************
函 数 名: PrintG()
功能描述: 输出图点边信息
输入参数: 无
输出参数: 无
返 回 值: 无
调用函数: 无
被调函数: mian()
修改历史:
    1.日期: 2017年12月24日
      作者: 何健
      修改: 创建函数
*************************************************************************************************/
void PrintG(PUNGraph G)
{
    cout << G->GetNodes() << endl;
    cout << G->GetEdges() << endl;
    return;
}

/*************************************************************************************************
函 数 名: Test
功能描述: 测试
输入参数: 无
输出参数: 无
返 回 值: 无
调用函数: 无
被调函数: main()
修改历史:
    1.日期: 2018年9月20日
      作者: 何健
      修改: 创建函数
*************************************************************************************************/
void Test()
{
    PUNGraph G = TSnap::LoadEdgeList<PUNGraph>("../DataSets/dblp_edges.txt", 0, 1);  //加载图数据
    EquiTrussAttribute ojbk;        //对象
    vector<comType> C;
    ojbk.FindKTrussCom(G, 4, 1, C);
    for (comType vec : C) {
        for (edgePair edge : vec) {
            cout << edge.first << ' ' << edge.second << endl;
        }
    }
    return;
}

/*************************************************************************************************
函 数 名: DBLP_Process()
功能描述: 真实的dblp数据集提取
输入参数: 无
输出参数: 无
返 回 值: 无
调用函数: DblpGenerate()
被调函数: main()
修改历史:
    1.日期: 2017年12月24日
      作者: 何健
      修改: 创建函数
*************************************************************************************************/
void DBLP_Process()
{
    clock_t generate_time = clock();
    printf("开始解析处理DBLP数据集....\n");
    DblpGenerate("../DataSets/dblp-2018-04-01.xml", "../Results/dblp_ungraph.txt", "../Results/dblp_vertex.txt", "../Results/dblp_attr_original.txt");
    printf("Generate Time:%f s\n\n", ((double)(clock() - generate_time) / CLOCKS_PER_SEC));
    return;
}

int PictureCnt = 1;
/*************************************************************************************************
函 数 名: Draw
功能描述: 对社区进行作图
输入参数: .png文件
输出参数:
返 回 值:
调用函数:
被调函数:
修改历史:
    1.日期: 2017年12月25日
      作者: 何健
      修改: 创建函数
*************************************************************************************************/
void Draw(comType R, set<int> nodes)
{
    PUNGraph G = TUNGraph::New();  //你说你，多少次这个错误
    TIntStrH Name;
    for (int node : nodes) {
        G->AddNode(node);
        Name.AddDat(node) = to_string(node).data();
    }
    for (edgePair edge : R) G->AddEdge(edge.first, edge.second);
    string str = to_string(PictureCnt) + ".png";
    PictureCnt++;
    const char *fileName = str.data();
    TSnap::DrawGViz<PUNGraph>(G, gvlDot, fileName, "", Name);
    return;
}

/*************************************************************************************************
函 数 名: CommunitySize
功能描述: 计算社区大小
输入参数:
输出参数: 返回查询点所有社区的顶点数量之和和边数量之和
返 回 值:
调用函数:
被调函数:
修改历史:
    1.日期: 2017年12月25日
      作者: 何健
      修改: 创建函数
*************************************************************************************************/
pair<int, int> CommunitySize(vector<comType > R, vector<pair<int, int> > &comSize)
{
    int node_sum = 0, edge_sum = 0;
    if (R.size() != 0) {
        printf("Output the communities's size = %d\n", R.size());
        for (int i = 0; i < R.size(); i++) {
            set<int> nodes;
            //const char* sOut = "NewGraph.txt";
            //FILE* fout = fopen(sOut, "w");

            for (vector<edgePair>::iterator it = R[i].begin(); it != R[i].end(); it++) {
                //cout << "(" << it->first << "," << it->second << ")";
                nodes.insert(it->first);
                nodes.insert(it->second);
                //fprintf(fout, "%d\t%d\n", it->first, it->second);
            }
            //fclose(fout);
            //输出社区顶点
            //for (int node : nodes) {
            //    cout << node << ' ';
            //}
            //cout << endl;

            printf("Community %d: Nodes(%d) Edges(%d)\n", i + 1, nodes.size(), R[i].size());
            node_sum += nodes.size();
            edge_sum += R[i].size();
            comSize.push_back(make_pair(nodes.size(), R[i].size()));

            //Draw(R[i], nodes); //visualize
            nodes.clear();
        }
    }
    printf("\n");
    return make_pair(node_sum, edge_sum);  //这里返回的分别是社区点数量之和和边数量之和
}


/*************************************************************************************************
函 数 名: Init
功能描述: 初始化数据集，早应该想到这种方法了。一些设置的变量最好显示出来，并且应该是全局常变量。不要写void函数
输入参数:
输出参数:
返 回 值:
调用函数:
被调函数:
修改历史:
    1.日期: 2018年9月17日
      作者: 何健
      修改: 创建函数
*************************************************************************************************/
string Init()
{
    printf("\n\t\t<<紧密子图查询(基于k-truss的属性网络社区发现和搜索)>>\n\n");
    int tmp;
    printf("###选择数据集###:\n 1.dblp\n 2.dbpedia\n 3.yago\n 4.tencent\n 5.test \n\n请输入: ");
    string datasetName;
    tmp = 4;    // 使用nohup命令，但是无法进行输入。修改了两处。但问题不大就是初始化了
    cin >> tmp;
     switch (tmp) {
    case 1: datasetName = "dblp"; break;
    case 2: datasetName = "dbpedia"; break;
    case 3: datasetName = "yago"; break;
    case 4: datasetName = "tencent"; break;
    case 5: datasetName = "test"; break;
    default: break;
    }
    
    attr_rate = 1;                        // 属性伸缩性，占20个数的百分比
    attrNumMax = 5;                        // 默认是20，测试用的是5
    vertex_rate = 0.2;                    // 顶点伸缩性，占300个数的百分比
    k_value = 6;                        // truss值
    
    /*
    bool flag = true;
    while (1) {
        printf("*************预处理*************\n");
        printf("1、计算数据集的顶点平均度\n");
        printf("2、随机生成数据集的查询点\n");
        printf("3、随机生成数据集的顶点伸缩性\n");
        printf("4、随机生成数据集的属性伸缩性\n");
        printf("5、查看图数据信息\n");
        printf("6、其他(跳过)\n请输入: ");
        tmp = 6;
        cin >> tmp;
        string str0, str1, str2;
        stringstream ss;
        int i;
        switch (tmp) {
        case 1:Calc_Degree(); break;
        case 2:
            str0 = string("../DataSets/") + datasetName + string("_edges.txt");
            Generate_Rand_300_Data(str0.data()); break;  //真是菜，属性没有加载进去，一直死循环。后来又出现段错误，原因是文件夹名大小写错误
        case 3:
            printf("请输入伸缩率(0-1): ");
            cin >> vertex_rate;
            str1 = string("../DataSets/") + datasetName + string("_edges.txt");      // 边数据
            i = vertex_rate * 100;
            ss << i;
            str2 = string("../Results/") + datasetName + ss.str() + string("%_vertices.txt");   // 结果
            VertexScalability(str1.data(), str2.data(), vertex_rate); break;
        case 4: break;
        case 5: break;
        default: flag = false; break;
        }
        if (!flag) break;
    }
    */
    
    
   
    printf("请输入关键词属性伸缩率(0.2/0.4/0.6/0.8/1，默认1): ");
    cin >> attr_rate;
    printf("请输入顶点伸缩率(0.2/0.4/0.6/0.8/1，默认0.2): ");
    cin >> vertex_rate;
    printf("请输入关键词数量(1/3/5/7/9，默认5): ");
    cin >> attrNumMax;
    printf("请输入truss值(4/5/6/7/8，默认6): ");
    cin >> k_value;
    printf("aRate = %.2f   vRate = %.2f   attrNumMax = %d   kValue = %d \n\n", attr_rate, vertex_rate, attrNumMax, k_value);
    return datasetName;
}

/*************************************************************************************************
函 数 名: AttributeCohesiceness() SaveSearchedCommunity()
功能描述: 计算   保存搜索到的社区到本地硬盘
输入参数:
输出参数: file1-统计计算时间结果、file2-详细的社区共有属性
返 回 值: 无
调用函数: obj对象
被调函数: 无
修改历史:
    1.日期: 2018年10月10日
      作者: 何健
      修改: 创建函数
*************************************************************************************************/
bool AttributeCohesiceness(FILE* file1, FILE* file2, vector<int> &v, vector<comType> &R, vector<string> &comS, double spendTime, double cmf, double cpj)
{
    printf("cmf = %f\t cpj = %f\n", cmf, cpj);
    time_t tim = clock();
    vector<edgePair> comSize;  //每个结果社区的大小
    edgePair comSizeSum = CommunitySize(R, comSize);
    int nodeSum = comSizeSum.first, edgeSum = comSizeSum.second;
    int comCnt = R.size();
    int q = v[0], k = v[1], nodeTrussness = v[2], selectACnt = v[3], nodeACnt = v[4];
    fprintf(file2, "\n%d\t%d\t%d\n", q, k, comCnt);
    if (R.size() != 0) {   // 找到社区
        /*
        for (int i = 1; i <= comCnt; i++) {
            set<int> nodes;
            for (edgePair edge : R[i - 1]) {
                nodes.insert(edge.first);
                nodes.insert(edge.second);
            }

            if (comS.size() == comCnt) fprintf(file2, "%d\t%d\t%d\t%s\n", i, nodes.size(), R[i-1].size(), comS[i - 1].data());   // 如果社区有共有属性即有查询结果
            else fprintf(file2, "%d\t%d\t%d\tnull\n", i, nodes.size(), R[i - 1].size());  // 无，给NCTruss的方便之门

            for (edgePair edge : R[i - 1]) {
                fprintf(file2, "%d\t%d\n", edge.first, edge.second);
            }
        }
        */
        fprintf(file1, "%d\t%d\t%d\t%d/%d\t%f\t%d\tnode_sum=%d edge_sum=%d\t%f\t%f\n", \
            q, k, nodeTrussness, selectACnt, nodeACnt, spendTime, comCnt, nodeSum, edgeSum, cmf, cpj);
    }
    else {                    // 没有找到社区
        fprintf(file1, "%d\t%d\t%d\t%d/%d\t%f\tnot found\t-\t0\t0\n", \
            q, k, nodeTrussness, selectACnt, nodeACnt, spendTime);
    }
    printf("CMF_CPJ时间:%f s\n", ((double)(clock() - tim) / CLOCKS_PER_SEC));
    return OK;
}

/*************************************************************************************************
函 数 名: StructureCohesuveness()
功能描述: 程序入口
输入参数:
输出参数: file1-统计计算时间结果、file2-详细的社区共有属性
返 回 值: 无
调用函数: obj对象
被调函数: 无
修改历史:
    1.日期: 2019年03月16日
      作者: 何健
      修改: 创建函数
*************************************************************************************************/
void StructureCohesuveness(FILE* out_file, vector<int> &v, vector<comType> &R, double spendTime)
{
    time_t T = clock();
    vector<edgePair> comSize;  //每个结果社区的大小
    edgePair comSizeSum = CommunitySize(R, comSize);
    int nodeSum = comSizeSum.first, edgeSum = comSizeSum.second;
    int comCnt = R.size();
    int q = v[0], k = v[1], nodeTrussness = v[2], selectACnt = v[3], nodeACnt = v[4];

    double averageDegree = 0, averageTrussness = 0;

    for (int i = 0; i < R.size(); i++) {
        set<int> nodes;
        for (edgePair edge : R[i]) {
            nodes.insert(edge.first);
            nodes.insert(edge.second);
        }
        PUNGraph G = TUNGraph::New();
        for (int node : nodes) G->AddNode(node);
        for (edgePair edge : R[i]) G->AddEdge(edge.first, edge.second);
        int degreeSum = 0, trussSum = 0;
        for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
            int degree = NI.GetDeg();
            degreeSum += degree;
        }
        double aveD = degreeSum * 1.0 / nodes.size();

        clock_t startTime = clock();
        EquiTrussAttribute obj;        //对象
        obj.EdgeSupport(G);
        printf("Run Time:%f s\n\n", ((double)(clock() - startTime) / CLOCKS_PER_SEC));

        edgeMap edgeTrussness;
        unordered_map<int, int> nodeTrussness;
        int kMax = obj.TrussDecomposition(G, edgeTrussness, nodeTrussness);    //返回边的最大trussness
        printf("kMax = %d\n", kMax);
        printf("Run Time:%f s\n\n", ((double)(clock() - startTime) / CLOCKS_PER_SEC));

        for (TUNGraph::TEdgeI EI = G->BegEI(); EI < G->EndEI(); EI++) {
            int u = EI.GetDstNId(), v = EI.GetSrcNId();
            if (u > v) swap(u, v);
            int truss = edgeTrussness[make_pair(u, v)];
            trussSum += truss;
        }
        double aveT = trussSum * 1.0 / R[i].size();

        averageDegree += aveD;
        averageTrussness += aveT;
        G.Clr();
    }
    averageDegree /= R.size(); //顶点的平均trussness？
    averageTrussness /= R.size(); //边的平均trussness？
    printf("averageDegree = %f\t averageTrussness = %f\n", averageDegree, averageTrussness);

    if (R.size() != 0) {   // 找到社区
        fprintf(out_file, "%d\t%d\t%d\t%d/%d\t%f\t%d\tnode_sum=%d edge_sum=%d\t%f\t%f\n", \
            q, k, nodeTrussness, selectACnt, nodeACnt, spendTime, comCnt, nodeSum, edgeSum, averageDegree, averageTrussness);
    }
    else {                    // 没有找到社区
        fprintf(out_file, "%d\t%d\t%d\t%d/%d\t%f\tnot found\t-\t0\t0\n", \
            q, k, nodeTrussness, selectACnt, nodeACnt, spendTime);
    }
    printf("StructureCohesuveness时间:%f s\n", ((double)(clock() - T) / CLOCKS_PER_SEC));
    return;
}


/*************************************************************************************************
函 数 名: CMF
功能描述: 计算社区中属性的紧密程度
输入参数: q-查询点、obj-类对象、com-计算的社区
输出参数: 无
返 回 值: 返回社区的CMF值
调用函数: 无
被调函数: main()
修改历史:
    1.日期: 2018年10月08日
     作者: 何健
     修改: 创建函数
*************************************************************************************************/
double CMF(int q, EquiTrussAttribute &obj, vector<comType> &com)
{
    time_t tim = clock();
    double cmf = 0;     // 返回cmf值
    int W = obj.attributesOfVertex_set[q].size();   // q的属性数量
    int comCnt = com.size();    // 社区数量

    for (int i = 0; i < comCnt; i++) {
        set<int> nodes;
        for (edgePair edge : com[i]) {
            nodes.insert(edge.first);
            nodes.insert(edge.second);
        }
        int C = nodes.size();        // 社区中顶点数量
        if (C == 0) continue;
        for (attrType attr : obj.attributesOfVertex_set[q]) {
            set<int> temp;
            if (obj.attr_truss[attr].size() == 0) continue;
            set_intersection(obj.attr_truss[attr].begin(), obj.attr_truss[attr].end(), nodes.begin(), nodes.end(), inserter(temp, temp.begin()));
            int f = temp.size();     // 社区中包含当前属性的顶点数量
            cmf += (f * 1.0 / C);
            temp.clear();
        }
        nodes.clear();
    }
    if (comCnt && W) cmf = cmf / comCnt / W;   // 防止除以0
    printf("CMF时间:%f s\n", ((double)(clock() - tim) / CLOCKS_PER_SEC));
    return cmf;
}

/*************************************************************************************************
函 数 名: CPJ
功能描述: 计算社区中属性的紧密程度，使用Jaccard系数。社区中每个点进行组合计算属性交集个数除以并集个数的商然后求平均值。
输入参数: obj-类对象、com-计算的社区
输出参数: 无
返 回 值: 返回社区的CPJ值
调用函数: 无
被调函数: main()
修改历史:
    1.日期: 2018年10月10日
      作者: 何健
      修改: 创建函数
    2.日期：2018年10月30日
      作者：何健
      修改：更新计算
*************************************************************************************************/
double CPJ(int q, vector<comType> &com, string attributes, int n_threads)
{
    double t = omp_get_wtime();
    omp_set_num_threads(n_threads);
    time_t tim = clock();
    double cpj_value = 0;     // 返回cpj值
    int comCnt = com.size();    // 社区数量

    for (int i = 0; i < comCnt; i++) {
        set<int> nodes;
        for (edgePair edge : com[i]) {
            nodes.insert(edge.first);
            nodes.insert(edge.second);
        }
        /*
        优化：计算u和v的时候只计算一次，相同的点对u和u直接算1
        */
        vector<int> vNodes;
        vector<int> is_meet(max_memory);
        is_meet.assign(max_memory, 0);
        for (int node : nodes) {
            vNodes.push_back(node);
            is_meet[node] = 1;
        }
        double sum = 0;     // 当前社区的平均CPJ
        int C = nodes.size();
        if (C == 0) continue;

        // 考虑社区点太多  如何优化这里是关键？？？？？？
        cout << "社区中顶点数量 = " << C << endl;
        cout << "社区中边的数量 = " << com[i].size() << endl;
        //if (C > 100007) return 0;   // 删除这个查询点结果
        //C = min(C, 100007);   // 某个社区太多可以不计算这个查询点

        // 20190327修改为多线程，但有很大的bug，注意顶点和属性不要混淆都是int
        clock_t T = clock();
        for (int j = 0; j < MAXD; j++) {
            doc[j].clear();
            key[j].clear();
        }
        read_in(attributes, is_meet);

        long long n_pairs = 0;

        #pragma omp parallel
        {
            int *cnt = new int[nd];
            memset(cnt, 0, sizeof(int) * nd);
            vector<int> v_doc;

            #pragma omp for schedule(dynamic)
            for (int j = 0; j < C; ++j) {   // 第一个点
                int u = vNodes[j];
                if (doc[u].size() == 0) continue;  // 当前点无属性


                v_doc.clear();
                for (int k = 0; k < (int)doc[u].size(); ++k) {
                    int tmp = doc[u][k];   // 当前点的每一个属性
                    for (int p = key[tmp].size() - 1; p >= 0; --p) {
                        int v = key[tmp][p];  // 属性所在的每一个顶点
                        if (v <= u) break;
                        if (cnt[v] == 0) v_doc.push_back(v);  // 存储的第二、三。。。个点
                        ++cnt[v];
                    }
                }
                //sort(v_doc.begin(), v_doc.end());

                #pragma omp critical
                {
                    n_pairs += v_doc.size();  // 存储有多少对点有共有属性
                    for (int v : v_doc) {   // 这里输出应该是当前点和其他点的匹配对
                        int w2 = cnt[v];
                        if (w2 == 0) continue;

                        set<int> all;
                        for (int tmp : doc[u]) all.insert(tmp);   // doc存储就是点的属性关键字集合
                        for (int tmp : doc[v]) all.insert(tmp);
                        //    printf("%d %d共有属性个数=%d\n", u, v, cnt[v]);
                        int w1 = all.size();
                        if (w1) {
                            //printf("CPJ----(%d,%d) w2=%d\t w1=%d\n", u, v, w2, w1);
                            sum += (w2 * 1.0 / w1) * 2;  // 乘以2是相互
                        }
                    }
                    for (int k = 0; k < (int)v_doc.size(); ++k) {
                        //printf( "sim(%d,%d)=%d\n", i, v_doc[j], cnt[v_doc[j]] );
                        cnt[v_doc[k]] = 0;
                    }
                }

                if (j % 10000*n_threads == 0) {
                    printf("%d docs finished, %lld pairs generated!\n", j, n_pairs);
                    printf("Processing time = %0.3lf sec\n", omp_get_wtime() - t);
                    //cout << clock() - T << endl;
                }
            }
            delete[] cnt;
        }
        printf("Total number of pairs = %lld, Processing time = %0.3lf sec\n", n_pairs, omp_get_wtime() - t);
        cout << "T1 = " << (double)(clock() - T) / CLOCKS_PER_SEC << endl;
        sum += C;   // 加上相同的点对和为C，即它本身
        sum /= (C * C);
        cpj_value += sum;
    }
    if (comCnt) cpj_value /= comCnt;
    printf("CPJ时间:%f s\n", ((double)(clock() - tim) / CLOCKS_PER_SEC));
    return cpj_value;
}

/*备份*/
double CPJ_backup(int q, EquiTrussAttribute &obj, vector<comType> &com)
{
    time_t tim = clock();
    double cpj = 0;     // 返回cpj值
    int comCnt = com.size();    // 社区数量

    for (int i = 0; i < comCnt; i++) {
        set<int> nodes;
        for (edgePair edge : com[i]) {
            nodes.insert(edge.first);
            nodes.insert(edge.second);

        }
        //优化：计算u和v的时候只计算一次，相同的点对u和u直接算1
        vector<int> vNodes;
        for (int node : nodes) vNodes.push_back(node);
        double tmp = 0;
        int C = nodes.size();
        if (C == 0) continue;

        // 考虑社区点太多  如何优化这里是关键？？？？？？
        cout << "社区中顶点数量 = " << C << endl;
        cout << "社区中边的数量 = " << com[i].size() << endl;
        //if (C > 100007) return 0;   // 删除这个查询点结果
                  //C = min(C, 100007);   // 某个社区太多可以不计算这个查询点

        double T2 = 0;
        clock_t T1 = clock();
        for (int j = 0; j < C; j++) {
            if (j % 10000 == 0) cout << "j = " << j << endl;
            for (int k = j + 1; k < C; k++) {
                int u = vNodes[j], v = vNodes[k];
                // 这里应该先用交集过滤一下
                clock_t T = clock();
                set<attrType> intersectionS;  //交集
                set_intersection(obj.attributesOfVertex_set[u].begin(), obj.attributesOfVertex_set[u].end(), obj.attributesOfVertex_set[v].begin(), obj.attributesOfVertex_set[v].end(), inserter(intersectionS, intersectionS.begin()));
                int w2 = intersectionS.size();
                if (w2 == 0) {
                    T2 += T - clock();
                    continue;

                }

                set<attrType> unionS;  //并集
                set_union(obj.attributesOfVertex_set[u].begin(), obj.attributesOfVertex_set[u].end(), obj.attributesOfVertex_set[v].begin(), obj.attributesOfVertex_set[v].end(), inserter(unionS, unionS.begin()));
                int w1 = unionS.size();
                if (w1) {
                    //printf("CPJ_backup----(%d,%d) w2=%d\t w1=%d\n", u, v, w2, w1);
                    tmp += (w2 * 1.0 / w1) * 2;
                }
                T2 += clock() - T;

            }

        }
        cout << "T1 = " << (double)(clock() - T1) / CLOCKS_PER_SEC << endl;
        cout << "T2 = " << (double)(T2) / CLOCKS_PER_SEC << endl;
        tmp += C;   // 加上相同的点对和为C
        tmp /= (C * C);
        cpj += tmp;

    }
    if (comCnt) cpj /= comCnt;
    printf("CPJ时间:%f s\n", ((double)(clock() - tim) / CLOCKS_PER_SEC));
    return cpj;

}


void DeleteRepeat()
{
    char* input = "../DataSets/tencent_edges.txt";
    char* output = "../Results/yago_edges.txt";
    FILE* fin = fopen(input, "r");
    FILE* fout = fopen(output, "w");

    cout << "input = " << input << endl;
    cout << "output = " << output << endl;
    cout << "Process Data...." << endl << endl;

    set<pair<int, int> > edges;
    int maxn_vertex = -1;
    int u, v, cnt = 0;
    while (!feof(fin)) {
        cnt++;
        fscanf(fin, "%d\t%d\n", &u, &v);
        if (u > v) swap(u, v);
        if (u != v) {                       // 防止自环
            edges.insert(make_pair(u, v));  // 防止重边
            if (v > maxn_vertex) maxn_vertex = v;
        }
        if (cnt % 1000000 == 0) cout << "已完成" << cnt << endl;
    }
    //for (auto edge : edges) {
    //    fprintf(fout, "%d\t%d\n", edge.first, edge.second);
    //}
    cout << "原来图边的数量 = " << cnt << endl;
    cout << "处理后边的数量 = " << edges.size() << endl;
    cout << "最大的顶点值 = " << maxn_vertex << endl;
    fclose(fin);
    fclose(fout);
    return;
}

void FUNC(int n) {
    for (int i = 0; i < 10000; ++i) {
        //do nothing, just waste time
    }
    printf("%d, ", n);
}
void TestOpenmp()
{
    // 可以看出VS自带openmp并行
    #pragma omp parallel for
    for (int i = 0; i < 10; ++i) FUNC(i);
    return;
}


double CMF_offline(int q, string attributes, vector<comType> &com)
{
    time_t tim = clock();
    double cmf = 0;     // 返回cmf值
    int comCnt = com.size();    // 社区数量

    set<int> nods;
    for (int i = 0; i < comCnt; i++) {
        for (edgePair edge : com[i]) {
            nods.insert(edge.first);
            nods.insert(edge.second);
        }
    }
    vector<int> is_meet(max_memory);
    is_meet.assign(max_memory, 0);
    for (int node : nods) {
        is_meet[node] = 1;
    }
    for (int j = 0; j < MAXD; j++) {
        doc[j].clear();
        key[j].clear();
    }
    read_in(attributes, is_meet);
    nods.clear();
    int W = doc[q].size();   // q的属性数量

    for (int i = 0; i < comCnt; i++) {
        set<int> nodes;
        for (edgePair edge : com[i]) {
            nodes.insert(edge.first);
            nodes.insert(edge.second);
        }
        int C = nodes.size();        // 社区中顶点数量
        if (C == 0) continue;
        for (attrType attr : doc[q]) {
            set<int> temp;
            set<int> y;
            for (int a : key[attr]) y.insert(a);
            set_intersection(y.begin(), y.end(), nodes.begin(), nodes.end(), inserter(temp, temp.begin()));
            int f = temp.size();     // 社区中包含当前属性的顶点数量
            cmf += (f * 1.0 / C);
            temp.clear();
        }
        nodes.clear();
    }
    if (comCnt && W) cmf = cmf / comCnt / W;   // 防止除以0
    printf("CMF时间:%f s\n", ((double)(clock() - tim) / CLOCKS_PER_SEC));
    return cmf;
}

void Split(const string str, vector<int>& v)
{
    int len = str.size();
    int value = 0;
    bool flag = false;
    for (int i = 0; i < len; i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            value = value * 10 + str[i] - '0';
            flag = true;
        }
        else {
            if (flag) {
                v.push_back(value);
                value = 0;
            }
        }
    }
    return;
}
void ForDBLP()
{
    FILE *file = fopen("T1117ACCore_detail_dblp100%_S5_K6.txt", "r");
    string attributes = "../DataSets/Attribute/dblp_attributes_int.txt";
    FILE *CMFCPJ = fopen("Improve_CMFCPJ.txt", "w");
    fprintf(CMFCPJ, "q\tk\tnodeTrussness\tattrCnt\tspendTime\tcomCnt\tcomSum\tCMF\tCPJ\n");

    FILE* STRUCTURE = fopen("Improve_STRUCTURE.txt", "w");
    fprintf(STRUCTURE, "q\tk\tnodeTrussness\tattrCnt\tspendTime\tcomCnt\tcomSum\taveDegree\taveTruss\n");

    FILE *temp = fopen("temp.txt", "w");
    
    int comCnt = 0;
    int index = 0;
    int qCnt = 0, q;        // 查询点的数量=300
    int nodeCnt = 0, edgeCnt = 0;
    bool flag = false;  // 是否开始计数边
    int n_threads = 30;

    vector<comType> R;
    comType com;

    while (!feof(file)) {
        index++;
        char str[1005];
        fgets(str, 1005, (FILE*)file);// 包括换行符!!!重点
        vector<int> v;
        Split(str, v);
        if (v.size() == 3) {
            if (flag) {
                printf("edgeCnt = %d\n", edgeCnt);
                //printf("q = %d\n", q);
                R.push_back(com);
                vector<int> tmpV(5);
                tmpV[0] = q;
                tmpV[1] = k_value;
                tmpV[2] = 5;
                tmpV[3] = 5;
                tmpV[4] = 5;
                vector<string> comSS;
                double cmf = CMF_offline(q, attributes, R);
                double cpj = CPJ(q, R, attributes, n_threads);
                AttributeCohesiceness(CMFCPJ, temp, tmpV, R, comSS, 56, cmf, cpj);
                StructureCohesuveness(STRUCTURE, tmpV, R, 56);

                printf("R size = %d\n", R.size());
                for (int i = 0; i < R.size(); i++) {
                    printf("edge size = %d\n", R[i].size());
                }
                R.clear();
                com.clear();
            }
            printf("q = %d\t k = %d\t index = %d\t comCnt = %d\n", v[0], v[1], index, v[2]);
            qCnt++;
            q = v[0];
            flag = false;
            continue;
        }
        if (v.size() > 3) {
            if (flag) {
                printf("edgeCnt = %d\n", edgeCnt);
                //printf("q = %d\n", q);
                R.push_back(com);
                com.clear();
            }
            flag = true;   // 这里是开始按钮
            edgeCnt = 0;
            continue;
        }
        if (flag) {
            edgeCnt++;
            com.push_back(make_pair(v[0], v[1]));
        }
    }
    if (flag) {
        printf("edgeCnt = %d\n", edgeCnt);
        //printf("q = %d\n", q);
        R.push_back(com);
        vector<int> tmpV(5);
        tmpV[0] = q;
        tmpV[1] = k_value;
        tmpV[2] = 5;
        tmpV[3] = 5;
        tmpV[4] = 5;
        vector<string> comSS;
        double cmf = CMF_offline(q, attributes, R);
        double cpj = CPJ(q, R, attributes, n_threads);
        AttributeCohesiceness(temp, CMFCPJ, tmpV, R, comSS, 56, cmf, cpj);
        StructureCohesuveness(STRUCTURE, tmpV, R, 56);

        printf("R size = %d\n", R.size());
        for (int i = 0; i < R.size(); i++) {
            printf("edge size = %d\n", R[i].size());
        }
        R.clear();
        com.clear();
    }
    cout << "qCnt = " << qCnt << endl;
    fclose(file);
    fclose(STRUCTURE);
    fclose(CMFCPJ);
    fclose(temp);
    return;
}

// 20190621
void Compute_Core_Truss(vector<comType> &com, int &minDegree, int &minTrussness)
{
    for (comType C : com) {
        PUNGraph G = TUNGraph::New();
        for (edgePair edge : C) {
            int u = edge.first, v = edge.second;
            if (!G->IsNode(u)) G->AddNode(u);
            if (!G->IsNode(v)) G->AddNode(v);
            G->AddEdge(u, v);
        }
        clock_t tim = clock();
        cout << "1、Compute Edge Support......" << endl;
        EquiTrussAttribute obj;        //对象
        obj.EdgeSupport(G);
        printf("Run Time:%f s\n\n", ((double)(clock() - tim) / CLOCKS_PER_SEC));

        tim = clock();
        cout << "2、Compute Edge Truss......" << endl;
        edgeMap edgeTrussness; //边的trussness
        unordered_map<int, int> nodeTrussness;    //点的trussness
        int kMax = obj.TrussDecomposition(G, edgeTrussness, nodeTrussness);    //返回边的最大trussness
        printf("Run Time:%f s\n\n", ((double)(clock() - tim) / CLOCKS_PER_SEC));

        for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
            int degree = NI.GetDeg(), node = NI.GetId();
            if (degree < minDegree) minDegree = degree;
            if (nodeTrussness[node] < minTrussness) minTrussness = nodeTrussness[node];
        }
        G.Clr();
    }
    return;
}

int main(int argc, char *argv[])
{
    //TestOpenmp();return 0;
    //TestCPJ();
    //ForDBLP(); return 0;
    //DBLP_Process(); return 0;   // 2019年01月07日重新处理dblp数据集
    Env = TEnv(argc, argv, TNotify::StdNotify);       // 程序计时   Snap自带的计时器
    Env.PrepArgs(TStr::Fmt("HanKin.build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));

    /*******************************************加载文件名字管理****************************************/
    string dataset_name = Init();   // 初始化配置
    clock_t startTime = clock();
    /* 改成输入是多么明智的选择啊 */

    stringstream ss;
    int vertexRange = 100;
    //printf("请输入点占的范围比例(0-100):");
    //cin >> vertexRange;
    ss << vertexRange;
    time_t seed = time(NULL);
    struct tm *nowTime = localtime(&seed);
    string skr = "";
    skr += to_string(nowTime->tm_mon + 1) + to_string(nowTime->tm_mday);
    //cout << skr << endl;

    string str1 = string("../DataSets/Graph/") + dataset_name + ss.str() + string("%_vertices.txt");      // 边数据文件
    const char* ungraph = str1.data();
    //string str2 = string("../DataSets/") + dataset_name + string("_attributes.txt");                // 加载属性数据文件
    string str2 = string("../DataSets/Attribute/") + dataset_name + string("_attributes_int.txt");                // 加载属性数据文件
    const char* attributes = str2.data();

    string str6 = string("../DataSets/Attribute/") + dataset_name + string("_String2Int.txt");                // 加载属性匹配数据文件
    const char* attr_int2str_file = str6.data();

    string str3 = string("../Query_Vertices/vertices_rand300_6truss_") + dataset_name + string(".txt");  // 查询点数据文件
    //string str3 = string("../Query_Vertices/vertices_rand300_6truss_tencent.txt");
    const char* query_file = str3.data();
    string str4 = string("../Results/T") + skr + dataset_name + ss.str() + string("%_ar") + to_string(attr_rate) + string("_vr") + to_string(vertex_rate) + string("_s") + to_string(attrNumMax) + string("_k") + to_string(k_value) + string(".txt");   // 结果文件：点范围-属性-truss-时间
    const char* result_file = str4.data();
    string str5 = string("../DataSets/AttrEquiTruss/") + dataset_name + ss.str() + string("_attribute_equitruss.txt");   // 属性equiTruss文件
    const char* load_file = str5.data();

    // 准备图文件、属性文件、查询点文件。索引生成需要新建文件夹：AttrEquiTruss、SuperGraph
    const char *folder1 = "../DataSets/AttrEquiTruss", *folder2 = "../DataSets/SuperGraph";
    //if (0 != _access(folder1, 0)) {    // 糟心，Linux下没有io.h文件
    //    mkdir(folder1);
    //}
    //if (0 != _access(folder2, 0)) {
    //    mkdir(folder1);
    //}

    string SEdge = string("../DataSets/SuperGraph/") + dataset_name + ss.str() + string("_SEdge.txt");
    string SNode = string("../DataSets/SuperGraph/") + dataset_name + ss.str() + string("_SNode.txt");
    string SEdgeTruss = string("../DataSets/SuperGraph/") + dataset_name + ss.str() + string("_SEdgeTruss.txt");
    string SNodeTruss = string("../DataSets/SuperGraph/") + dataset_name + ss.str() + string("_SNodeTruss.txt");
    const char* SEdge_c = SEdge.data();
    const char* SNode_c = SNode.data();
    const char* SEdgeTruss_c = SEdgeTruss.data();
    const char* SNodeTruss_c = SNodeTruss.data();
    /*****************************************************************************************************/

    printf("\n==========1.Read Local Data File（建图）==========\n");
    printf("DataSet: %s\n", ungraph);
    PUNGraph graph = TSnap::LoadEdgeList<PUNGraph>(ungraph, 0, 1);  //加载图数据
    printf("nodes = %d\n", graph->GetNodes());
    printf("edges = %d\n", graph->GetEdges());
    printf("Run Time:%f s\n\n", ((double)(clock() - startTime) / CLOCKS_PER_SEC));

    EquiTrussAttribute obj;        //对象
    printf("==========Start EquiTrussIndex==========\n");
    clock_t indexTime = clock();
    obj.superGraph = obj.EquiTrussIndex(graph, SNode_c, SEdge_c, SNodeTruss_c, SEdgeTruss_c);            //创建索引，主要是这里花费时间，可以存储本地

    printf("EquiTruss索引创建时间:%f s\n\n", ((double)(clock() - indexTime) / CLOCKS_PER_SEC));
    //return 0;   // 测试查询结果的社区是否满足条件

    obj.vertexSNIndex = obj.VertexSNIndex();            //每个顶点包含在那些超点里面
                                                        // 这个只能叫做映射，不能称为索引
    printf("顶点包含在超点映射时间:%f s\n\n", ((double)(clock() - indexTime) / CLOCKS_PER_SEC));

    printf("==========5.Community Search Based on AttributeSearch（准备搜索）==========\n");
    srand(seed);
    graph = TSnap::LoadEdgeList<PUNGraph>(ungraph, 0, 1);

    printf("==========5-1.Read Attribute Dataset File（读取属性到内存）==========\n");
    printf("DataSet: %s\n", attributes);
    obj.Vertex_Attribute(attributes, attr_int2str_file, graph);   //读取属性数据集，存储到内存中
    printf("Run Time:%f s\n\n", ((double)(clock() - startTime) / CLOCKS_PER_SEC));


    cout << endl << "========5-2start find attribute truss（建立属性索引）...=====" << endl;
    indexTime = clock();

    obj.Find_Attribute_Truss(obj.attr_int2str.size(), graph, load_file);  // 非最大的属性id
    printf("属性索引创建时间:%f s\n\n", ((double)(clock() - indexTime) / CLOCKS_PER_SEC));
    //return 0;


    printf("==========5-3.GetAttributes（建立超点映射）==========\n");
    indexTime = clock();
    obj.GetSNAttributes();   //计算超点中的属性集
    printf("Run Time:%f s\n\n", ((double)(clock() - indexTime) / CLOCKS_PER_SEC));

    printf("==========5-4.Start Searching Vertex（开始查询）==========\n");
    int number = 0;
    double allTime = 0;
    list<pair<int, int> > result;
    
    /*
    string str[6];
    FILE* files[6];
    string fileNames[9] = { "Improve_", "Improve_detail_", "ACCore_", "ACCore_detail_",
        "NCTruss_", "NCTruss_detail_", "ImproveStructure_", "ACCoreStructure_", "NCTrussStructure_"};
    for (int i = 0; i < 6; i++) {
        str[i] = string("../Results/T") + skr + fileNames[i] + dataset_name + ss.str() + string("%_ar") + attr_rate + string("_vr") + vertex_rate + string("_s") + attrNumMax + string("_k") + k_value + string(".txt");
        files[i] = fopen(str[i].data(), "w");
        // 20190322修改中文“点”为vertex，要进行区分前后社区
        if (i & 1) fprintf(files[i], "vertex\t k\t comCnt\ncomIndex\t nodeSize\t edgeSize\t commonAttribute\n");
        else fprintf(files[i], "q\tk\tnodeTrussness\tattrCnt\tspendTime\tcomCnt\tcomSum\tCMF\tCPJ\n");
    }
    FILE* structures[3];
    string struc[3];
    FILE* caseStudy = fopen("../Results/caseStudy.txt", "w");
    fprintf(caseStudy, "q_vertex\tk_value\tnode_truss\tsum_com1\tsum_com2\tspendT1\tspendT2\tminCore1\tminTruss1\tminCore2\tminTruss2\n");

    for (int i = 0; i < 3; i++) {
        struc[i] = string("../Results/T") + skr + fileNames[i+6] + dataset_name + ss.str() + string("%_ar") + attr_rate + string("_vr") + vertex_rate + string("_s") + attrNumMax + string("_k") + k_value + string(".txt");
        structures[i] = fopen(struc[i].data(), "w");
        fprintf(structures[i], "q\tk\tnodeTrussness\tattrCnt\tspendTime\tcomCnt\tcomSum\taveDegree\taveTruss\n");
    }
    */

    FILE* TestDataFile = fopen(query_file, "r");
    FILE* TestData_Result = fopen(result_file, "w");
    fprintf(TestData_Result, "number\tq_vertex\tk_value\ttruss_val\tattr_cnt\tATCBasic\tATCBasic_1\tATCIndex\tATCIndex_1\tATCImprove\tKIndexDec\tKIndexInc\tcommunities\tsum_com\n");
    while (!feof(TestDataFile)) {
        int q_vertex, truss_value, attr_cnt;  //q_vertex查询点，k_value输入的k值，truss_value查询点本身的truss值
        fscanf(TestDataFile, "%d\t%d", &q_vertex, &truss_value);
        //cout << q_vertex << ' ' << truss_value << endl;
        //continue;
        //cout << "Input q_vertex = "; cin >> q_vertex;
        //cout << "Input k_value = "; cin >> k_value;                                                    /*******修改1*******/
        //k_value = 4; // 降低k值试试，测试Tencent数据集
        for (int i = 0; i < 1; i++) {        //同一组数据多次测试
            clock_t t = clock();

            //查询点q_vertex的属性读取
            vector<attrType> SAttr = obj.attributesOfVertex[q_vertex];

            int maxn_attr = SAttr.size();
            maxn_attr = min(maxn_attr, attrNumMax);          //属性数量不能太多，最大为20
            attr_cnt = floor(maxn_attr * attr_rate + 0.5);  //这里对属性的个数限制                                /********修改2*******/
            attr_cnt = max(1, attr_cnt);   //防止属性为0，上面是进行四舍五入取整

            set<int> check;
            set<attrType> SATR;    // 查询点的属性关键字
            for (int cnt = 0; cnt < attr_cnt && cnt < maxn_attr; cnt++) {  //随机选取属性
                int tex = 0;
                while (1) {
                    tex = (rand() + 3) % maxn_attr;
                    int pre = check.size();
                    check.insert(tex);
                    if (int(check.size()) > pre) break;
                }
                SATR.insert(SAttr[tex]);
            }
            printf("%s 查询第%d次: q = %d  k = %d  truss = %d  attr_cnt = %d\n", ungraph, ++number, q_vertex, k_value, truss_value, attr_cnt);
            printf("All(%d) the attribute S(%d) = {", SAttr.size(), SATR.size());
            bool flag = true;
            for (attrType attr : SATR) {
                if (flag) {
                    flag = false;
                    cout << attr;
                }
                else cout << ',' << attr;
            }
            cout << '}' << endl << endl;
            double basic_time, basic_1_time, index_time, index_1_time, improve_time, kindexDec_time, kindexInc_time;
            vector<pair<int, int> > comSize[10];
            pair<int, int> comSizeSum[10];
            vector<comType > R[10];
            vector<int> tmpV(5);
            tmpV[0] = q_vertex;
            tmpV[1] = k_value;
            tmpV[2] = truss_value;
            tmpV[3] = attr_cnt;
            tmpV[4] = SAttr.size();
            
            /*****************************************************************************************************/
            printf("==========5-4-1.Start ATCBasic==========\n");
            vector<string> comS;
            clock_t tmp = clock();
            R[0] = obj.ATCBasic(graph, k_value, q_vertex, SATR, comS);
            printf("Basic查询时间:%f s\n", ((double)(clock() - tmp) / CLOCKS_PER_SEC));
            basic_time = (double)(clock() - tmp) / CLOCKS_PER_SEC;
            comSizeSum[0] = CommunitySize(R[0], comSize[0]);
            
            /*****************************************************************************************************/
            printf("==========5-4-2.Start ATCBasic_1==========\n");
            tmp = clock();
            R[1] = obj.ATCBasic_1(graph, k_value, q_vertex, SATR, comS);
            printf("Basic_1查询时间:%f s\n", ((double)(clock() - tmp) / CLOCKS_PER_SEC));
            basic_1_time = (double)(clock() - tmp) / CLOCKS_PER_SEC;
            comSizeSum[1] = CommunitySize(R[1], comSize[1]);
            
            /*****************************************************************************************************/
            printf("==========5-4-3.Start ATCIndex==========\n");
            tmp = clock();
            R[2] = obj.ATCIndex(graph, k_value, q_vertex, SATR);
            printf("Index查询时间:%f s\n", ((double)(clock() - tmp) / CLOCKS_PER_SEC));
            index_time = (double)(clock() - tmp) / CLOCKS_PER_SEC;
            comSizeSum[2] = CommunitySize(R[2], comSize[2]);
            
            /*****************************************************************************************************/
            printf("==========5-4-4.Start ATCIndex_1==========\n");
            tmp = clock();
            R[3] = obj.ATCIndex_1(graph, k_value, q_vertex, SATR);
            printf("Index查询时间:%f s\n", ((double)(clock() - tmp) / CLOCKS_PER_SEC));
            index_1_time = (double)(clock() - tmp) / CLOCKS_PER_SEC;
            comSizeSum[3] = CommunitySize(R[3], comSize[3]);

            /*****************************************************************************************************/
            printf("==========5-4-5.Start ATCImprove==========\n");
            tmp = clock();
            R[4] = obj.ATCImprove(graph, k_value, q_vertex, SATR);
            printf("Improve查询时间:%f s\n", ((double)(clock() - tmp) / CLOCKS_PER_SEC));
            improve_time = (double)(clock() - tmp) / CLOCKS_PER_SEC;
            comSizeSum[4] = CommunitySize(R[4], comSize[4]);

            /*****************************************************************************************************/
            printf("==========5-4-6.Start KindexDec==========\n");
            clock_t tim = clock();
            R[5] = obj.KIndexDec(graph, k_value, q_vertex, SATR, load_file);
            printf("KindexDec(自上而下)查询时间:%f s\n", ((double)(clock() - tim - obj.extra_time) / CLOCKS_PER_SEC));
            kindexDec_time = (double)(clock() - tim - obj.extra_time) / CLOCKS_PER_SEC;
            comSizeSum[5] = CommunitySize(R[5], comSize[5]);
            //cout << comSizeSum[5].first << ' ' << comSizeSum[5].second << endl;

            /*****************************************************************************************************/
            printf("==========5-4-7.Start KindexInc==========\n");
            tim = clock();
            R[6] = obj.KIndexInc(graph, k_value, q_vertex, SATR, load_file);
            printf("KindexInc(自下而上)查询时间:%f s\n", ((double)(clock() - tim - obj.extra_time) / CLOCKS_PER_SEC));
            kindexInc_time = (double)(clock() - tim - obj.extra_time) / CLOCKS_PER_SEC;
            comSizeSum[6] = CommunitySize(R[6], comSize[6]);
            //cout << comSizeSum[6].first << ' ' << comSizeSum[6].second << endl;
            /*****************************************************************************************************/

            set<int> isEqualNode, isEqualEdge, isEqualCom;
            for (int cnt = 0; cnt < 7; cnt++) {
                isEqualNode.insert(comSizeSum[cnt].first);
                isEqualEdge.insert(comSizeSum[cnt].second);
                isEqualCom.insert(R[i].size());
            }
            if (isEqualNode.size() == 1 && isEqualEdge.size() == 1 && isEqualCom.size() == 1) {
                //fprintf(resImproveFile1, "%d\t%d\t%d\n", q_vertex, k_value, comSize[0].size());

                if (comSize[0].size() != 0) {   // 找到社区
                    //for (int cnt = 0; cnt < int(comsize[0].size()); cnt++) {
                    //    fprintf(resimprovefile1, "%d\t%s\n", cnt + 1, coms[i]);
                    //    for (edgepair edge : r[0][i]) {
                    //        fprintf(resimprovefile1, "%d\t%d\n", edge.first, edge.second);
                    //    }
                    //}
                    result.push_back(make_pair(k_value, q_vertex));
                    fprintf(TestData_Result, "%d\t%d\t%d\t%d\t%d/%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%d\tnode_sum=%d edge_sum=%d\n", \
                        number, q_vertex, k_value, truss_value, attr_cnt, SAttr.size(),  basic_time, basic_1_time, index_time, index_1_time, improve_time, kindexDec_time, kindexInc_time, R[0].size(), comSizeSum[0].first, comSizeSum[0].second);
                }
                else {                    // 没有找到社区
                    fprintf(TestData_Result, "%d\t%d\t%d\t%d\t%d/%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\tnot found\t-\n", \
                        number, q_vertex, k_value, truss_value, attr_cnt, SAttr.size(), basic_time, basic_1_time, index_time, index_1_time, improve_time, kindexDec_time, kindexInc_time);
                }
            }
            else {                        // 算法之间有问题
                cout << "something the matter!!!!!!!!!!!" << endl;
                fprintf(TestData_Result, "%d\t%d\t%d\t%d\t%d/%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\tat question\t-\n", \
                    number, q_vertex, k_value, truss_value, attr_cnt, SAttr.size(), basic_time, basic_1_time, index_time, index_1_time, improve_time, kindexDec_time, kindexInc_time);
            }
            allTime += ((double)(clock() - t) / CLOCKS_PER_SEC);
            cout << "************************************************************" << endl;
            int real_attr_cnt = min(attr_cnt, int(SAttr.size()));
            printf("q = %d, k = %d %d/%d: 本次总的查询时间:%f s\n", q_vertex, k_value, real_attr_cnt, SAttr.size(), ((double)(clock() - t) / CLOCKS_PER_SEC));
            cout << "************************************************************" << endl << endl;
        }
    }
    fclose(TestData_Result);
    fclose(TestDataFile);
    //for (int i = 0; i < 6; i++) fclose(files[i]);
    //for (int i = 0; i < 3; i++) fclose(structures[i]);

    cout << "All Community Search Time(Alone):  " << allTime << 's' << endl;
    printf("Run Time:%f s\n\n", ((double)(clock() - startTime) / CLOCKS_PER_SEC));
    Env.PrepArgs(TStr::Fmt("HanKin.build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));

    if (result.size() == 0) {
        cout << "没有查询到一个顶点GG!!!" << endl;
        return 0;
    }
    for (pair<int, int> ans : result) {
        cout << ans.first << ' ' << ans.second << endl;
    }
    return 0;
}
