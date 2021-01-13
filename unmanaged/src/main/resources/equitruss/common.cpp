#include "common.h"

using namespace std;

edgePair EdgePair(int u, int v)
{
	if (u < v) return make_pair(u, v);
	return make_pair(v, u);
}

bool cmp(node a, node b) {
	return a.comEdgeCnt < b.comEdgeCnt;
};

// Split a string into an int array
void SplitInt(const string& s, vector<int>& v, const string& c) // str, vec, ","
{
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (string::npos != pos2) {
		int num = stoi(s.substr(pos1, pos2 - pos1), nullptr, 10); // string to int
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

// Find all the k-truss communities containing query vertex q
void FindKTrussCom(PUNGraph G, int k, int q, vector<Community>& C)
{
	unordered_map<edgePair, int, pair_hash> edgeTrussness;
	unordered_map<int, int> nodeTrussness;
	int kMax = TrussDecomposition(G, nodeTrussness, edgeTrussness);	

	unordered_map<edgePair, bool, pair_hash> visited;
	for (TUNGraph::TEdgeI EI = G->BegEI(); EI < G->EndEI(); EI++) {
		edgePair edge = EdgePair(EI.GetSrcNId(), EI.GetDstNId());
		visited[edge] = false;
	}

	for (int i = 0; i < G->GetNI(q).GetDeg(); i++) {
		int nq = G->GetNI(q).GetNbrNId(i);
		edgePair edge = EdgePair(nq, q);
		if (edgeTrussness[edge] >= k && visited[edge] == false) {
			Community temp;

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
					int w = G->GetNI(u).GetNbrNId(j);	//the same neighbor of u and v
					if (!G->IsEdge(w, v))
					{
						continue;
					}
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

			if (int(temp.size()) != 0) {
				C.push_back(temp);
				temp.clear();
			}
		}
	}
	edgeTrussness.clear(), nodeTrussness.clear();
	return;
}

// Compute the support of every edge in graph
void EdgeSupport(PUNGraph graph, unordered_map<edgePair, int, pair_hash>& sup, set<pair<int, edgePair>>& nonDecSup)
{
	for (TUNGraph::TEdgeI EI = graph->BegEI(); EI < graph->EndEI(); EI++) {
		int src = EI.GetSrcNId(), dst = EI.GetDstNId();
		int temp;
		if (graph->GetNI(src).GetDeg() > graph->GetNI(dst).GetDeg()) {
			temp = src, src = dst, dst = temp;
		}

		int support = 0;
		for (int i = 0; i < graph->GetNI(src).GetDeg(); i++) {
			int nbr = graph->GetNI(src).GetNbrNId(i);
			if (graph->IsEdge(nbr, dst)) support++;
		}

		edgePair edge = EdgePair(src, dst);
		sup[edge] = support;
		nonDecSup.insert(make_pair(support, edge));
	}
	return;
}

// Do the truss decomposition
int TrussDecomposition(PUNGraph G, unordered_map<int, int>& nodeTrussness, unordered_map<edgePair, int, pair_hash>& edgeTrussness)
{
	TIntV newGraph;
	G->GetNIdV(newGraph);
	PUNGraph graph = TSnap::GetSubGraph(G, newGraph);
	unordered_map<edgePair, int, pair_hash> sup;
	set<pair<int, edgePair>> nonDecSup;
	EdgeSupport(graph, sup, nonDecSup);

	int k = 2;
	while (!nonDecSup.empty()) {
		while (!nonDecSup.empty() && nonDecSup.begin()->first <= k - 2) {
			int s = nonDecSup.begin()->first;
			edgePair e = nonDecSup.begin()->second;

			int u = e.first;
			int v = e.second;	// u < v			
								// d(u) < d(v)
			if (graph->GetNI(u).GetDeg() > graph->GetNI(v).GetDeg()) swap(u, v);

			for (int i = 0; i < graph->GetNI(u).GetDeg(); i++) {
				int nbr = graph->GetNI(u).GetNbrNId(i);
				if (!graph->IsEdge(nbr, v))
				{
					continue;
				}
				if (sup[EdgePair(nbr, v)] != -2 && sup[EdgePair(nbr, u)] != -2) {
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
			nodeTrussness[e.first] = k;
			nodeTrussness[e.second] = k;
			edgeTrussness[e] = k;
			sup[e] = -2;
			nonDecSup.erase(make_pair(s, e));
			graph->DelEdge(e.first, e.second);
		}
		k += 1;
	}
	return k - 1;
}

// Output the size and the shared attributes of every community
string ShowCommunitySize(vector<pair<Community, set<int> > > R, FILE* file)
{
    string communities_str = "";
    if (R.size() != 0)
	{
	    // R: communities
		fprintf(file, "Community Count = %d\n", R.size());
		for (int i = 0; i < R.size(); i++) {
			// nodes
		    set<int> nodes; // nodes
			for (vector<edgePair>::iterator it = R[i].first.begin(); it != R[i].first.end(); it++)
			{
				nodes.insert(it->first);
				nodes.insert(it->second);
			}
            string nodes_str;
            for(auto node : nodes){
                nodes_str.append(to_string(node));
                nodes_str.append(",");
            }
            nodes_str.pop_back();

			fprintf(file, "\tCommunity %d: Node Count(%d) Nodes %s Edge Count(%d) Attributes(", i + 1, nodes.size(), nodes_str.data(),  R[i].first.size());

            nodes_str.append("#"); // split node and attr
            // attr
			for (attrType str :	R[i].second)
			{
                nodes_str.append(to_string(str));
                nodes_str.append(",");
                fprintf(file, "%d ", str);
			}
            nodes_str.pop_back();
			fprintf(file, ")\n");
			communities_str.append(nodes_str);
			communities_str.append("@");
			nodes.clear();
		}
		communities_str.pop_back();
	}
    return communities_str;
}
