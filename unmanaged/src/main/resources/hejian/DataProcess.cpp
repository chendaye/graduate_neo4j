/***********************************************************************************************
								版权所有(C), 2017-2018, HanKin
************************************************************************************************
文 件 名: DataPrecess.h
版    本: 1.1
作    者: 何健
创建时间: 2018年12月01日
语    言: C++
最后更改: 无
功能描述: 数据处理，比如随机生成300个数据，顶点伸缩性
功能列表:
修改历史:
	  1.日期: 2017年12月01日
		作者: 何健
		修改: 创建文件
      2.日期: 2018年08月05日
		作者: 何健
		修改: 将文件中的函数整理封装并规范化
*************************************************************************************************/

#pragma once
#include "stdafx.h"
#include "DataProcess.h"
#include "EquiTrussAttribute.h"

/************************************************************************************************
								变量说明
*************************************************************************************************/
typedef pair<string, int> PAIR;
struct CmpByValue {
	bool operator()(const PAIR& lhs, const PAIR& rhs) {
		return lhs.second > rhs.second;
	}
};

/*************************************************************************************************
函 数 名: 二进制文件转文本文件
功能描述:
输入参数:
输出参数:
返 回 值:
调用函数:
被调函数:
修改历史:
	1.日期:
	  作者:
	  修改:
*************************************************************************************************/
inline void Bin2Text(const char* sIn, const char* sOut) {
	FILE* fin = fopen(sIn, "r");
	FILE* fout = fopen(sOut, "w");
	while (!feof(fin)) {
		char str[505];
		fgets(str, 505, fin);
		//fscanf(fin, "%s\n", &str);
		//cout << strlen(str) << endl;
		//int cnt = 0;
		//for (int i = 0; i < strlen(str); i++) {
		//	if (str[i] != ' ') cnt++;
		//}
		//cout << cnt << endl;
		int  ibina[NSIZE];
		char cRead[NSIZE + 1];
		int tag = 0, dex = 0;
		while (str[tag] != ' ') tag++;
		tag++;
		while (str[tag] != '\0') {
			dex = 0;
			for (int i = tag; i < tag + NSIZE * 2; i++) {
				cRead[dex++] = str[i];
				i++;
			}
			cRead[NSIZE] = '\0';
			//cout << cRead << endl;
			for (int i = 0; i < NSIZE; i++) {
				ibina[i] = (cRead[i] - '0');
			}
			int iTemp = 1, cChar = 0;
			for (int i = 7; i >= 0; i--)
			{
				cChar += ibina[i] * iTemp;
				iTemp *= 2;
			}
			printf("%c", cChar);
			fprintf(fout, "%c", cChar);
			tag = tag + 16;
		}
	}
	fclose(fin);
	fclose(fout);
	return;
}

/*************************************************************************************************
函 数 名: FacebookFeature
功能描述: facebook数据集特征提取
输入参数:
输出参数:
返 回 值:
调用函数:
被调函数:
修改历史:
	1.日期:
	  作者:
	  修改:
*************************************************************************************************/
inline void GenerateData(const char* fileName, const char* sIn, const char* sOut)
{
	PUNGraph graph = TSnap::LoadEdgeList<PUNGraph>(fileName, 0, 1);
	int nodesCnt = graph->GetNodes();
	//得到属性的数量，属性用数字表示，即1 - numberOfAttr
	int seed = time(0);
	srand(seed);
	FILE* fin = fopen(sIn, "r");
	FILE* fout = fopen(sOut, "w");

	int numberOfAttr = nodesCnt * 0.005;
	cout << "|A| = " << numberOfAttr << endl;
	if (numberOfAttr < 1) numberOfAttr = 1;

	int allCount = 0, cnt = 0;
	set<int> nodes;
	list<int> allNodes;

	map<int, set<int> > attrOfNodes;	//使用set防止相同属性
	while (!feof(fin)) {
		char str[500005];
		cnt++;
		fgets(str, 500005, fin);	//一行就是一个community,总共应该有5000个。
									//fscanf(fin, "%s", &str);
		vector<int> community;
		//cout << str << endl;
		int node = 0;
		for (int i = 0; i < strlen(str) - 1; i++) {
			if (str[i] == '\t') {
				community.push_back(node);
				nodes.insert(node);
				allNodes.push_back(node);
				node = 0;
				continue;
			}
			node = node * 10 + (str[i] - '0');
		}
		community.push_back(node);
		nodes.insert(node); allNodes.push_back(node);

		int numberOfNodes = community.size();

		//int numberOfAttr = numberOfNodes * 0.005;   //错了，正确的姿势应该是最先在所有点的数量乘以0.005
		//if (numberOfAttr < 1) numberOfAttr = 1;
		for (int j = 0; j < 3; j++) {	//随机选择三个属性
			int attribute = rand() % numberOfAttr + 1;
			list<int> vis;
			for (int k = 0; k < numberOfNodes * 0.8; k++) {	//属性随机分给社区中80%的点
				int node = community[rand() % numberOfNodes];
				list<int>::iterator it = find(vis.begin(), vis.end(), node);
				while (it != vis.end()) {
					node = community[rand() % numberOfNodes];
					it = find(vis.begin(), vis.end(), node);
				}
				vis.push_back(node);
				attrOfNodes[node].insert(attribute);
			}
			vis.clear();
		}
		community.clear();
	}

	for (int i : nodes) {
		int temp = rand() % 5 + 1;
		while (temp--) {
			int attribute = rand() % numberOfAttr + 1;
			attrOfNodes[i].insert(attribute);
		}
	}

	map<int, set<int> >::iterator it = attrOfNodes.begin();
	for (; it != attrOfNodes.end(); it++) {
		fprintf(fout, "%d", it->first);
		fprintf(fout, "\t");
		set<int>::iterator attribute = it->second.begin();
		fprintf(fout, "%d", *attribute);
		attribute++;
		for (; attribute != it->second.end(); attribute++) {
			fprintf(fout, ",%d", *attribute);
			allCount++;
		}
		fprintf(fout, "\n");
	}
	cout << "attr(V) = " << allCount << endl;
	cout << "cnt = " << cnt - 1 << endl;
	cout << "nodes = " << nodes.size() << endl;
	cout << "allNodes = " << allNodes.size() << endl;

	fclose(fin);
	fclose(fout);
	return;
}

/*************************************************************************************************
函 数 名: FacebookFeature
功能描述: facebook数据集特征提取
输入参数:
输出参数:
返 回 值:
调用函数:
被调函数:
修改历史:
	1.日期:
	  作者:
      修改:
*************************************************************************************************/
inline void FacebookFeature(const char* sIn1, const char* sIn2, const char* sOut)
{
	FILE* fin1 = fopen(sIn1, "r");
	FILE* fin2 = fopen(sIn2, "r");
	FILE* fout = fopen(sOut, "w");

	map<int, string> featnames;
	while (!feof(fin2)) {
		char str[5005];
		fgets(str, 5005, fin2);
		int id = 0, i;
		for (i = 0; i < strlen(str) - 1; i++) {
			if (str[i] == ' ') break;
			id = id * 10 + (str[i] - '0');
		}
		char s[5005];
		strncpy(s, str + i + 1, strlen(str) - i);
		s[strlen(str) - i - 1] = '\0';
		//cout << s; //自带换行符
		featnames[id] = s;
	}

	map<int, set<string> > attrOfNodes;	//使用set防止相同属性
	while (!feof(fin1)) {
		char str[5005];
		fgets(str, 5005, fin1);
		int node = 0, i;
		for (i = 0; i < strlen(str) - 1; i++) {
			if (str[i] == ' ') {
				break;
			}
			node = node * 10 + (str[i] - '0');
		}
		int tag = 0;
		for (i = i + 1; i < strlen(str) - 1; i++) {
			if (str[i] == '1') {
				attrOfNodes[node].insert(featnames[tag]);
			}
			i++, tag++;
		}
	}
	map<int, set<string> >::iterator it = attrOfNodes.begin();
	for (; it != attrOfNodes.end(); it++) {
		fprintf(fout, "%d", it->first);
		fprintf(fout, "\t");
		for (string attribute : it->second) {
			//cout << attribute << endl;
			char *temp = &attribute[0];
			fputs(temp, fout);
			fprintf(fout, "\t");
		}
		fprintf(fout, "\n");
	}
	fclose(fin1);
	fclose(fin2);
	fclose(fout);
	return;
}

/*************************************************************************************************
函 数 名: SplitStr
功能描述: 将字符串按指定分隔符进行分割
输入参数:
输出参数:
返 回 值:
调用函数:
被调函数:
修改历史:
	1.日期:
	  作者:
      修改:
*************************************************************************************************/
inline void SplitStr(string& s, vector<string>& v, const string& c)
{
	string title;
	transform(s.begin(), s.end(), back_inserter(title), ::tolower);
	s = "";
	for (int i = 0; i < title.size(); i++) {
		if ((title[i] >= 'a' && title[i] <= 'z') || title[i] == ' ') {
			s += title[i];
		}
	}
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (string::npos != pos2) {
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length()) {
		v.push_back(s.substr(pos1));
	}
	return;
}

extern map<string, int> keyword_in;

inline void InvalidKeyword()
{
	string stopwords[] = { "i", "me", "my", "myself", "we", "our", "ours", "ourselves", "you", "you’re",\
		"you've", "you'll", "you'd", "your", "yours", "yourself", "yourselves", "he", "him", "his", "himself",\
		"she", "she's", "her", "hers", "herself", "it", "it's", "its", "itself", "they", "them", "their",\
		"theirs", "themselves", "what", "which", "who", "whom", "this", "that", "that'll", "these",\
		"those", "am", "is", "are", "was", "were", "be", "been", "being", "have", "has", "had", "having",\
		"do", "does", "did", "doing", "a", "an", "the", "and", "but", "if", "or", "because", "as", "until",\
		"while", "of", "at", "by", "for", "with", "about", "against", "between", "into", "through", "during",\
		"before", "after", "above", "below", "to", "from", "up", "down", "in", "out", "on", "off", "over",\
		"under", "again", "further", "then", "once", "here", "there", "when", "where", "why", "how", "all",\
		"any", "both", "each", "few", "more", "most", "other", "some", "such", "no", "nor", "not", "only", "own",\
		"same", "so", "than", "too", "very", "s", "t", "can", "will", "just", "don", "don't", "should", "should've",\
		"now", "d", "ll", "m", "o", "re", "ve", "y", "ain", "aren", "aren't", "couldn", "couldn't", "didn",\
		"didn't", "doesn", "doesn't", "hadn", "hadn't", "hasn", "hasn't", "haven", "haven't", "isn", "isn't",\
		"ma", "mightn", "mightn't", "mustn", "mustn't", "needn", "needn't", "shan", "shan't", "shouldn", "shouldn't",\
		"wasn", "wasn't", "weren", "weren't", "won", "won't", "wouldn", "wouldn't", "without" };
	int len = sizeof(stopwords) / sizeof(string);
	for (int i = 0; i < len; i++) {
		keyword_in[stopwords[i]] = 1;
	}
	return;
}

inline bool Check(string keyword)
{
	if (keyword_in[keyword] == 1) return false;
	return true;
}

inline void DblpGenerate(char* file_in, char* ungraph_out, char* vertex_out, char* original_out)
{
	/*
	PWgtNet Net = TWgtNet::New();
	TWgtNet DBL;
	const TStr db = "../DataSets/dblp.xml";
	DBL.LoadDblpCoAuth("ds");
	DBL.LoadDblpCoAuth(db);
	TDblpLoader *dblp = new TDblpLoader(TStr("../DataSets/dblp.xml"));
	*/
	cout << "Start Process dblp Dataset....." << endl;
	TDblpLoader dblp(file_in);
	dblp.GetFPosStr();
	int authorCount = 1;
	map<string, int> author_vertex;  //方便hash查找，从1开始，默认是0.  存储作者对应的顶点
	map<string, vector<string> > author_attributes;  //存储作者的全部属性

	int cnt = 0;
	InvalidKeyword();
	FILE* fout_ungraph = fopen(ungraph_out, "w");
	vector<string> keywords;
	vector<int> nodes;
	PUNGraph graph = PUNGraph::New();  //添加图是为了去重边
	while (dblp.Next()) {
		//if (cnt++ == 30) break;
		//cout << dblp.Year << endl;
		string titleName = dblp.Title.CStr();
		titleName[titleName.size() - 1] = '\0';  //去掉末尾的句号
												 //cout << titleName << endl;
		keywords.clear();
		SplitStr(titleName, keywords, " ");

		nodes.clear();
		for (int i = 0; i < dblp.AuthorV.Len(); i++) {    //当前文章的所有作者
			string authorName = dblp.AuthorV[i].CStr();
			//cout << authorName << endl;
			if (author_vertex[authorName] == 0) {  //如果当前作者第一次出现，给他分配一个顶点值
				graph->AddNode(authorCount);
				author_vertex[authorName] = authorCount++;
			}
			nodes.push_back(author_vertex[authorName]);
			for (string keyword : keywords) {
				if (keyword != " " && keyword != "") {
					if (Check(keyword)) {
						//cout << keyword << endl;
						author_attributes[authorName].push_back(keyword);
					}
				}
			}
		}
		if (nodes.size() > 1) {
			for (int i : nodes) {
				for (int j : nodes) {
					if (i != j) {
						if (!graph->IsEdge(i, j)) {
							fprintf(fout_ungraph, "%d\t%d\n", i, j);
							graph->AddEdge(i, j);
						}
					}
				}
			}
		}
	}
	graph.Clr();
	cout << "作者数量 = " << author_vertex.size() << endl;
	cout << author_attributes.size() << endl;
	cout << "dblp_author_ungraph.txt over!" << endl;

	FILE* fout = fopen(vertex_out, "w");
	map<string, int>::iterator it = author_vertex.begin();
	for (; it != author_vertex.end(); it++) {
		fprintf(fout, "%s", it->first.data());
		fprintf(fout, "\t");
		fprintf(fout, "%d\n", it->second);
	}
	cout << "dblp_author_vertex.txt over!" << endl;
	/*
	map<string, int> topAttributes;
	map<string, vector<string> > author_attrs;  //最好筛选出来的前20属性数据集
	for (map<string, vector<string> >::iterator it = author_attributes.begin(); it != author_attributes.end(); it++) {
	//if (it->second.size() <= 20) {   //可能里面有重复的
	//	author_attrs[it->first].assign(it->second.begin(), it->second.end());
	//	continue;
	//}
	topAttributes.clear();
	for (string attr : it->second) {
	topAttributes[attr]++;
	}
	//把map中元素转存到vector中
	vector<PAIR> topAttributes_vec(topAttributes.begin(), topAttributes.end());

	//对vector排序
	sort(topAttributes_vec.begin(), topAttributes_vec.end(), CmpByValue());
	for (int i = 0; i < topAttributes_vec.size() && i < 20; i++) {  //限制条件前20条，问题在于没有做词形还原，有点无意义
	author_attrs[it->first].push_back(topAttributes_vec[i].first);
	}
	topAttributes_vec.clear();
	}
	*/
	//进行修改，先做词形还原，然后进行频繁排序，所以会把所有的属性存储下来，会很大
	FILE* fout_attrs = fopen(original_out, "w");
	//map<string, vector<string> >::iterator itor = author_attrs.begin();
	map<string, vector<string> >::iterator itor = author_attributes.begin();
	for (; itor != author_attributes.end(); itor++) {
		string authorName = itor->first;
		fprintf(fout_attrs, "%d", author_vertex[authorName]);  // %-20s字符串左对齐占20个字符位置
		fprintf(fout_attrs, "\t");
		bool flag = false;
		for (string str : itor->second) {
			//if (str == "" || str == " ") continue;
			if (!flag) {
				flag = true;
				fprintf(fout_attrs, "%s", str.data());
			}
			else fprintf(fout_attrs, ",%s", str.data());
		}
		fprintf(fout_attrs, "\n");
	}
	cout << "dblp_author_attr_original.txt over!" << endl;
	fclose(fout);
	fclose(fout_ungraph);
	fclose(fout_attrs);
	return;
}
inline void HuangXinGenerate()
{
	/*
	*数据处理
	*/
	if (1) {
		clock_t tim = clock();
		//GenerateData(ungraphName, cmtyName, attributeName);

		/*
		*10 ego-networks indicated by its ego-user X.
		*nodeId.edges : The edges in the ego network for the node 'nodeId'.边数据集
		*nodeId.circles : The set of circles for the ego node. Each line contains one circle。没有用
		*nodeId.feat : The features for each of the nodes that appears in the edge file.
		第一个是顶点，后面234个二进制位属性Features are '1' if the user has this property in their profile, and '0' otherwise.
		*nodeId.egofeat : The features for the ego user.应该是ego网络中所有属性的并集
		*nodeId.featnames : The names of each of the feature dimensions. 每位对应的属性名称
		*10个文件应该统一起来，形成一个属性文件
		*/
		int facebookCmty[10] = { 0, 107, 348, 414, 686, 698, 1684, 1912, 3437, 3890 };
		string folderName = "../DataSets/facebook/facebook/";
		for (int i = 0; i < 1; i++) {
			string str = folderName + to_string(facebookCmty[i]) + ".feat";
			char* feat = new char[str.size() + 5]; strcpy(feat, str.data());
			str = folderName + to_string(facebookCmty[i]) + ".featnames";
			char* featnames = new char[str.size() + 5]; strcpy(featnames, str.data());
			char* facebookAttribute = "../DataSets/facebook/facebook.attribute.txt";
			FacebookFeature(feat, featnames, facebookAttribute);
		}
		printf("\nGenerateData TIME: %f s\n", ((double)(clock() - tim) / CLOCKS_PER_SEC));
	}
}

/*************************************************************************************************
函 数 名: Generate_Rand_300_Data
功能描述: 对300个顶点随机生成
输入参数:
输出参数:
返 回 值:
调用函数:
被调函数:
修改历史:
	1.日期: 2018年08月05日
	  作者: 何健
	  修改: 创建函数
*************************************************************************************************/
inline void Generate_Rand_300_Data(const char* dataName)
{
	//生成随机300个测试顶点
	EquiTrussAttribute ojbk;		//对象

	PUNGraph tencent = TSnap::LoadEdgeList<PUNGraph>("../DataSets/tencent_edges.txt", 0, 1);  //加载图数据
	ojbk.Rand_300_Vertives(tencent, "../Query_Vertices/vertices_rand300_6truss_tencent.txt", 2421057);
	return;  //不要影响以前的数据集

	PUNGraph dblp = TSnap::LoadEdgeList<PUNGraph>("../DataSets/dblp_edges.txt", 0, 1);  //加载图数据
	ojbk.Rand_300_Vertives(dblp, "../Query_Vertices/vertices_rand300_6truss_dblp.txt", 2101223);

	PUNGraph yago = TSnap::LoadEdgeList<PUNGraph>("../DataSets/yago_edges.txt", 0, 1);  //加载图数据
	ojbk.Rand_300_Vertives(yago, "../Query_Vertices/vertices_rand300_6truss_yago.txt", 2638095);

	PUNGraph dbpedia = TSnap::LoadEdgeList<PUNGraph>("../DataSets/dbpedia_edges.txt", 0, 1);  //加载图数据
	ojbk.Rand_300_Vertives(dbpedia, "../Query_Vertices/vertices_rand300_6truss_dbpedia.txt", 6081599);
	return;
}

/*************************************************************************************************
函 数 名: VertexScalability
功能描述: 对整个图随机选取20%、40%、60%、80%四个子图数据集构成新图，还要选取新的300个查询点数据
输入参数:
输出参数:
返 回 值:
调用函数:
被调函数:
修改历史:
	1.日期: 2018年08月05日
	  作者: 何健
	  修改: 创建函数
*************************************************************************************************/
inline int VertexScalability(const char *inFileName, const char *outFileName, double vertex_rate)
{
	PUNGraph graph = TSnap::LoadEdgeList<PUNGraph>(inFileName, 0, 1);  //加载图数据
	printf("nodes = %d\n", graph->GetNodes());
	printf("edges = %d\n", graph->GetEdges());

	FILE* outFile = fopen(outFileName, "w");  // 输出图文件
	int nodeNum = graph->GetNodes();          // 顶点数量不是最大id咋办?应该不会有这么蠢，但真的不是同一个值
	int selectNum = nodeNum * vertex_rate;    // 选择数量

	int maxNode = nodeNum;
	for (TUNGraph::TNodeI NI = graph->BegNI(); NI < graph->EndNI(); NI++) {
		if (NI.GetId() > maxNode) maxNode = NI.GetId();
	}
	printf("nodeNum = %d  maxNode = %d  selectNum = %d\n\n", nodeNum, maxNode, selectNum);

	set<int> nodes;
	int seed = time(0); srand(seed);
	mt19937 mt_time(seed);
	uniform_int_distribution<int> dist(1, maxNode);  //[1, maxNode]间的随机整数
	while (1) {
		int node = dist(mt_time);
		if (graph->IsNode(node)) {
			nodes.insert(node);
			if (nodes.size() == selectNum) break;
		}
	}
	unordered_map<int, bool> hashTable;
	for (int node : nodes) hashTable[node] = true;
	// 愚蠢，遍历边啊
	for (TUNGraph::TEdgeI EI = graph->BegEI(); EI < graph->EndEI(); EI++) {
		int u = EI.GetDstNId(), v = EI.GetSrcNId();
		if (hashTable[u] && hashTable[v]) {
			if (u > v) swap(u, v);
			fprintf(outFile, "%d\t%d\n", u, v);
		}
	}
	hashTable.clear();
	fclose(outFile);
	graph.Clr();
	return 0;
}


/*************************************************************************************************
函 数 名: Calc_Degree
功能描述: 计算社交网络图中的平均顶点度
输入参数: 无
输出参数: 平均度
返 回 值: 无
调用函数:
被调函数:
修改历史:
	1.日期: 2018年08月05日
	  作者: 何健
	  修改: 创建函数
*************************************************************************************************/
inline void Calc_Degree()
{
	PUNGraph dblp = TSnap::LoadEdgeList<PUNGraph>("../DataSets/dblp_edges.txt", 0, 1);  //加载图数据
	long long sum = 0;
	for (TUNGraph::TNodeI NI = dblp->BegNI(); NI < dblp->EndNI(); NI++) {
		int tmp = NI.GetDeg();
		sum += tmp;
	}
	int node_sum = dblp->GetNodes();
	double ans = sum * 1.0 / node_sum;
	cout << "dblp = " << ans << endl;

	PUNGraph dbpedia = TSnap::LoadEdgeList<PUNGraph>("../DataSets/dbpedia_edges.txt", 0, 1);  //加载图数据
	sum = 0;
	for (TUNGraph::TNodeI NI = dbpedia->BegNI(); NI < dbpedia->EndNI(); NI++) {
		int tmp = NI.GetDeg();
		sum += tmp;
	}
	node_sum = dbpedia->GetNodes();
	ans = sum * 1.0 / node_sum;
	cout << "dbpedia = " << ans << endl;

	PUNGraph yago = TSnap::LoadEdgeList<PUNGraph>("../DataSets/yago_edges.txt", 0, 1);  //加载图数据
	sum = 0;
	for (TUNGraph::TNodeI NI = yago->BegNI(); NI < yago->EndNI(); NI++) {
		int tmp = NI.GetDeg();
		sum += tmp;
	}
	node_sum = yago->GetNodes();
	ans = sum * 1.0 / node_sum;
	cout << "yago = " << ans << endl;

	PUNGraph tencent = TSnap::LoadEdgeList<PUNGraph>("../DataSets/tencent_edges.txt", 0, 1);  //加载图数据
	sum = 0;
	for (TUNGraph::TNodeI NI = tencent->BegNI(); NI < tencent->EndNI(); NI++) {
		int tmp = NI.GetDeg();
		sum += tmp;
	}
	node_sum = tencent->GetNodes();
	ans = sum * 1.0 / node_sum;
	cout << "tencent = " << ans << endl;
	return;
}


/*************************************************************************************************
函 数 名: main
功能描述: 测试主函数
输入参数:
输出参数:
返 回 值: 无
调用函数:
被调函数:
修改历史:
	1.日期: 2018年08月05日
	  作者: 何健
	  修改: 创建函数
*************************************************************************************************/
#ifdef DEBUGr
int main(void)
{
	clock_t t = clock();
	//Bin2Text("GenerateData.txt", "data.txt");
	GenerateData("../DataSets/dblp/com-dblp.all.cmty.txt", "../DataSets/GenerateDataXH.txt");
	//FacebookFeature("facebook/0.feat", "facebook/0.featnames","ret.txt");
	printf("\nTIME: %f s\n", ((double)(clock() - t) / CLOCKS_PER_SEC));
	return 0;
}
#endif // DEBUG

