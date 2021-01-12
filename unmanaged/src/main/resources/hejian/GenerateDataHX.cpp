/* 
GenerateDataHX.cpp : 定义控制台应用程序的入口点。
time: 2017.12.01
author : HanKin
university : WhuHan University
language : C++
other : Snap
*/
#pragma once
#include "stdafx.h"
#include <stdio.h>
#include <time.h>
#include <vector>
#include <map>
#include <list>
#include <string>
#include <cstdio>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <Snap.h>
#include <set>
#define DEBUG
#define NSIZE  8
using namespace std;

/***********二进制文件转文本文件**********/
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

	map<int, set<string>> attrOfNodes;	//使用set防止相同属性
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

