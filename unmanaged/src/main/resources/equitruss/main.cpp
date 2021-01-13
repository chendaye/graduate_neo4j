#include "EquiTrussAttribute.h"
#include "common.h"

using namespace std;

/**
 * todo: input:
 *  - graph data file : argv[1]
 *  - attribute data file : argv[2]
 *  - query vertices file : argv[3]
 *  - query k value: k_value
 *  - the amount of query attribute: attr_count
 *  - 1 for CAC-MTIndexI, and 2 for CAC-MTIndexD: selection
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char **argv) 
{
	clock_t startTime = clock();
	string str1 = string(argv[1]); // graph data file
	const char* ungraph = str1.data(); // 1445	15978

	string str2 = string(argv[2]); // attribute data file               
	const char* attributes = str2.data(); // 1607439	1,2,3,4,5,6,7,8,9,10,11,12,13,14 ; index: home	1

	string str3 = string(argv[3]); // query vertices file 
	const char* query = str3.data(); // 1114924	7

	/*******************************write******************************************/
    // result path
    string str4 = string(argv[4]);

    // create result path
    string cmd1 = "rm -rf  " + str4;
    system(cmd1.data());
    string cmd2 = "mkdir -p -m 777 " + str4;
    system(cmd2.data());



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



	/*****************************************************************************************************/

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



	int attrNumMax = 20, number = 0, attr_count, k_value, selection;

    k_value = stoi(argv[5]);
//	cout << "Please specify the query k value: ";
//	cin >> k_value;

    attr_count = stoi(argv[6]);
//	cout << "Please specify the amount of query attribute: ";
//	cin >> attr_count;

    selection = stoi(argv[7]);
//	cout << "Enter 1 for CAC-MTIndexI, and 2 for CAC-MTIndexD: ";
//	cin >> selection;

    /*****************************************************************************************************/

	FILE* TestDataFile = fopen(query, "r"); // query vertex
	FILE* resultFile = fopen(result, "w"); // attr community
	FILE* detailFile = fopen(detail, "w"); // all community
	fprintf(resultFile, "index\tvertex\tquery_k\ttime\tresult_community_count\tquery_attribute_set\n");
	while (!feof(TestDataFile))
	{
		int q_vertex, attr_cnt;
		fscanf(TestDataFile, "%d", &q_vertex);
		
		if (!obj.G->IsNode(q_vertex))
		{
			continue;
		}
		
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
		if (selection == 2)
		{
			auto communities = obj.CAC_MTIndexD(k_value, q_vertex, SATR);
			time = (double)(clock() - startTime) / CLOCKS_PER_SEC;
			if (communities.size() > 0)
			{
				fprintf(detailFile, "Query vertex: %d\n", q_vertex);
				ShowCommunitySize(communities, detailFile);
				fprintf(detailFile, "\n");
			}
			resultSize = communities.size();
		}
		else if (selection == 1)
		{
			auto communities = obj.CAC_MTIndexI(k_value, q_vertex, SATR);
			time = (double)(clock() - startTime) / CLOCKS_PER_SEC;
			if (communities.size() > 0)
			{
				fprintf(detailFile, "Query vertex: %d\n", q_vertex);
				ShowCommunitySize(communities, detailFile);
				fprintf(detailFile, "\n");
			}
			resultSize = communities.size();
		}
		
		// Output the query attribute set
		string attrStr;
		for (auto attr: SATR)
		{
			attrStr.append(to_string(attr));
			attrStr.append(",");
		}
		attrStr.pop_back();
		fprintf(resultFile, "%d\t%d\t%d\t%lf\t%d\t%s\n", ++number, q_vertex, k_value, time, resultSize, attrStr.data());
	}
	printf("==========Finish querying==========\n");
	return 0;
}
