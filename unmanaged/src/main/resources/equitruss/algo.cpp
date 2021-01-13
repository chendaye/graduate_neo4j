#include "algo.h"
using namespace std;

void search(EquiTrussAttribute obj, int q_vertex,  FILE* resultFile, FILE* detailFile, int k_value = 3, int attr_count = 3, int selection = 1){
    int attr_cnt;
    int attrNumMax = 20; // max number attr
    // Generate the query attribute set SATR, based on the attribute set of the query vertex
    set<attrType> SAttr;
    for (auto attr : obj.attributesOfVertex[q_vertex])
    {
        SAttr.insert(attr);
    }
    int maxn_attr = SAttr.size();
    maxn_attr = min(maxn_attr, attrNumMax);
    attr_cnt = attr_count > maxn_attr ? maxn_attr : attr_count;
    set<attrType> SATR;
    int cnt = 0;
    for (auto attr : SAttr)
    {
        if (cnt >= attr_cnt)
        {
            break;
        }
        SATR.insert(attr);
        cnt++;
    }

    double time = -1;
    int resultSize = -1;
    clock_t startTime = clock();
    string communities_str;
    auto communities = selection == 2 ? obj.CAC_MTIndexD(k_value, q_vertex, SATR) : obj.CAC_MTIndexI(k_value, q_vertex, SATR);
    time = (double)(clock() - startTime) / CLOCKS_PER_SEC;
    if (communities.size() > 0)
    {
        fprintf(detailFile, "Query vertex: %d\n", q_vertex);
        communities_str = ShowCommunitySize(communities, detailFile);
        fprintf(detailFile, "\n");
    }
    resultSize = communities.size();


    // Output the query attribute set
    string attrStr;
    for (auto attr: SATR)
    {
        attrStr.append(to_string(attr));
        attrStr.append(",");
    }
    attrStr.pop_back();
    fprintf(resultFile, "%d\t%d\t%lf\t%d\t%s\t%s\n", q_vertex, k_value, time, resultSize, attrStr.data(), communities_str.data());
}

string searchNeo4j(EquiTrussAttribute obj, int q_vertex,  int k_value = 3, int attr_count = 3, int selection = 1){
    int attr_cnt;
    int attrNumMax = 20; // max number attr
    set<attrType> SAttr;
    for (auto attr : obj.attributesOfVertex[q_vertex])
    {
        SAttr.insert(attr);
    }
    int maxn_attr = SAttr.size();
    maxn_attr = min(maxn_attr, attrNumMax);
    attr_cnt = attr_count > maxn_attr ? maxn_attr : attr_count;
    set<attrType> SATR;
    int cnt = 0;
    for (auto attr : SAttr)
    {
        if (cnt >= attr_cnt)
        {
            break;
        }
        SATR.insert(attr);
        cnt++;
    }

    double time = -1;
    int resultSize = -1;
    clock_t startTime = clock();
    string communities_str;
    auto communities = selection == 2 ? obj.CAC_MTIndexD(k_value, q_vertex, SATR) : obj.CAC_MTIndexI(k_value, q_vertex, SATR);
    time = (double)(clock() - startTime) / CLOCKS_PER_SEC;
    if (communities.size() > 0)
    {
        communities_str = ShowCommunitySizeNeo4j(communities);
    }
    resultSize = communities.size();


    // Output the query attribute set
    string attrStr;
    for (auto attr: SATR)
    {
        attrStr.append(to_string(attr));
        attrStr.append(",");
    }
    attrStr.pop_back();
    string res;
    res.append(to_string(q_vertex)+":");
    res.append(to_string(k_value)+":");
    res.append(to_string(time)+":");
    res.append(to_string(resultSize)+":");
    res.append(attrStr+":");
    res.append(communities_str);
    return res;
}

void clearResult(char *path){
    string str4 = string(path);
    // create result path
    string cmd1 = "rm -rf  " + str4;
    system(cmd1.data());
    string cmd2 = "mkdir -p -m 777 " + str4;
    system(cmd2.data());
}

void algorithm_queries(char *argv1, char *argv2, char *argv3, char *argv4, int argv5, int argv6, int argv7){
    clock_t startTime = clock();
    string str1 = string(argv1); // graph data file
    const char* ungraph = str1.data(); // 1445	15978

    string str2 = string(argv2); // attribute data file
    const char* attributes = str2.data(); // 1607439	1,2,3,4,5,6,7,8,9,10,11,12,13,14 ; index: home	1

    string str3 = string(argv3); // query vertices file
    const char* query = str3.data(); // 1114924	7

    /*******************************write******************************************/
    // result path
    clearResult(argv4);

    string str4 = string(argv4);
    // time result file； write
    string query_result = str4 + string("/query_result.txt");
    const char* result = query_result.data();

    // result detail file； write
    string community_result = str4 + string("/community_result.txt");
    const char* detail = community_result.data();

    // Build MTIndex attribute equitruss index file； write
    string mt_index = str4 + string("/mt_index.txt");
    const char* attrTruss = mt_index.data();

    // Build the equitruss index the path where to save equitruss index
    // superNode_file
    string SEdge = str4 + string("/SEdge.txt");
    const char* SEdge_c = SEdge.data();

    // superEdge_file
    string SNode = str4 + string("/SNode.txt");
    const char* SNode_c = SNode.data();

    // nodeTruss_file
    string SEdgeTruss = str4 + string("/SEdgeTruss.txt");
    const char* SEdgeTruss_c = SEdgeTruss.data();

    // edgeTruss_file
    string SNodeTruss = str4 + string("/SNodeTruss.txt");
    const char* SNodeTruss_c = SNodeTruss.data();

    EquiTrussAttribute obj;

    printf("==========Read graph data file==========\n");
    obj.G = TSnap::LoadEdgeList<PUNGraph>(ungraph, 0, 1);
    printf("nodes = %d\n", obj.G->GetNodes());
    printf("edges = %d\n", obj.G->GetEdges());
    printf("Run Time: %f s\n\n", ((double)(clock() - startTime) / CLOCKS_PER_SEC));


    printf("==========Start building EquiTruss==========\n");
    clock_t indexTime = clock();
    // const char* superNode_file, const char* superEdge_file, const char* nodeTruss_file, const char* edgeTruss_file
    obj.BuildEquiTruss(SNode_c, SEdge_c, SNodeTruss_c, SEdgeTruss_c); // Build the equitruss index, and save the index to files
    printf("EquiTruss Build Time: %f s\n\n", ((double)(clock() - indexTime) / CLOCKS_PER_SEC));

    printf("==========Read attribute data file==========\n");
    startTime = clock();
    obj.LoadVertexAttribute(attributes);
    printf("Run Time: %f s\n\n", ((double)(clock() - startTime) / CLOCKS_PER_SEC));


    printf("==========Start finding attribute truss==========\n");
    indexTime = clock();
    obj.BuildAttrTruss(attrTruss);  // Build MTIndex
    obj.GetSNAttributes();
    printf("Attribute EquiTruss Build Time: %f s\n\n", ((double)(clock() - indexTime) / CLOCKS_PER_SEC));

    printf("==========Start querying==========\n");

    int k_value = argv5;
    int attr_count = argv6;
    int selection = argv7;

    FILE* TestDataFile = fopen(query, "r"); // query vertex
    FILE* resultFile = fopen(result, "w"); // attr community
    FILE* detailFile = fopen(detail, "w"); // all community
    fprintf(resultFile, "vertex\tquery_k\ttime\tresult_community_count\tquery_attribute_set\tcommunities_set\n");
    while (!feof(TestDataFile))
    {
        int q_vertex;
        fscanf(TestDataFile, "%d", &q_vertex);
        if (!obj.G->IsNode(q_vertex))
            continue;
        // search community
        search(obj, q_vertex, resultFile, detailFile, k_value, attr_count, selection);
    }
    printf("==========Finish querying==========\n");
}


string algorithm_neo4j(char *argv1, char *argv2, int argv3, char *argv4, int argv5, int argv6, int argv7){
    clock_t startTime = clock();
    string str1 = string(argv1); // graph data file
    const char* ungraph = str1.data(); // 1445	15978

    string str2 = string(argv2); // attribute data file
    const char* attributes = str2.data(); // 1607439	1,2,3,4,5,6,7,8,9,10,11,12,13,14 ; index: home	1

    /*******************************write******************************************/
    // result path
    clearResult(argv4);

    // Build MTIndex attribute equitruss index file； write
    string str4 = string(argv4);
    string mt_index = str4 + string("/mt_index.txt");
    const char* attrTruss = mt_index.data();

    // Build the equitruss index the path where to save equitruss index
    // superNode_file
    string SEdge = str4 + string("/SEdge.txt");
    const char* SEdge_c = SEdge.data();

    // superEdge_file
    string SNode = str4 + string("/SNode.txt");
    const char* SNode_c = SNode.data();

    // nodeTruss_file
    string SEdgeTruss = str4 + string("/SEdgeTruss.txt");
    const char* SEdgeTruss_c = SEdgeTruss.data();

    // edgeTruss_file
    string SNodeTruss = str4 + string("/SNodeTruss.txt");
    const char* SNodeTruss_c = SNodeTruss.data();

    EquiTrussAttribute obj;
    // Read graph data file
    obj.G = TSnap::LoadEdgeList<PUNGraph>(ungraph, 0, 1);
    // Start building EquiTruss
    // const char* superNode_file, const char* superEdge_file, const char* nodeTruss_file, const char* edgeTruss_file
    obj.BuildEquiTruss(SNode_c, SEdge_c, SNodeTruss_c, SEdgeTruss_c); // Build the equitruss index, and save the index to files
    // Read attribute data file
    obj.LoadVertexAttribute(attributes);

    // Start finding attribute truss
    obj.BuildAttrTruss(attrTruss);  // Build MTIndex
    obj.GetSNAttributes();

    // Start querying
    int q_vertex = argv3;
    int k_value = argv5;
    int attr_count = argv6;
    int selection = argv7;

    // fprintf(resultFile, "vertex\tquery_k\ttime\tresult_community_count\tquery_attribute_set\tcommunities_set\n");
    if (!obj.G->IsNode(q_vertex))
        return NULL;
    // search community
    return searchNeo4j(obj, q_vertex, k_value, attr_count, selection);
}


