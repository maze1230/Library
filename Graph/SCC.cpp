#include <bits/stdc++.h>
using namespace std;

#define INF_LL (int64)1e18
#define INF (int32)1e9
#define REP(i, n) for(int i = 0;i < (n);i++)
#define FOR(i, a, b) for(int i = (a);i < (b);i++)
#define all(x) x.begin(),x.end()
#define fs first
#define sc second
using int32 = int_fast32_t;
using uint32 = uint_fast32_t;
using int64 = int_fast64_t;
using uint64 = uint_fast64_t;
using PII = pair<int32, int32>;
using PLL = pair<int64, int64>;

const double eps = 1e-10;

template<typename A, typename B>inline void chmin(A &a, B b){if(a > b) a = b;}
template<typename A, typename B>inline void chmax(A &a, B b){if(a < b) a = b;}

const int32 DIRECTED = 0;
const int32 UNDIRECTED = 1;

template<int32 isUNDIRECTED=0>
class Graph{
	struct Edge{
		int32 u, v, id;
		int64 c;
		Edge(int32 u, int32 v, int64 c=0, int32 id=0):u(u), v(v), c(c), id(id){}
	};

	int32 V, E;
	vector<vector<Edge>> G;
	vector<Edge> Es;
public:
	Graph(){}
	Graph(int32 V):V(V){G.resize(V);}
	Graph(const Graph<isUNDIRECTED>& g):V(g.V), E(g.E), G(g.G), Es(g.Es){}

	void add_edge(int32 u, int32 v, int64 c=0, int32 id=0){
		G[u].emplace_back(u, v, c, id);
		if(isUNDIRECTED) G[v].emplace_back(v, u, c, id);
		Es.emplace_back(u, v, c, id);
		E++;
	}

	const vector<Edge>& operator[](int32 k){
		return G[k];
	}
};

class SCC{
private:
	Graph<DIRECTED> orgG, revG, newG;
	vector<int32> ord, comp;
	vector<bool> used;
	vector<vector<int32>> vs;

	int32 V, nV;
public:
	SCC(){}
	SCC(int32 V):orgG(V), revG(V), comp(V, -1), used(V, 0), V(V){}

	void add_edge(int32 u, int32 v){
		orgG.add_edge(u, v);
		revG.add_edge(v, u);
	}

	void dfs(int32 v){
		used[v] = true;
		for(auto e : orgG[v]){
			if(!used[e.v]) dfs(e.v);
		}
		ord.push_back(v);
	}

	void rdfs(int32 v, int32 k){
		used[v] = true;
		comp[v] = k;
		for(auto e : revG[v]){
			if(!used[e.v]) rdfs(e.v, k);
		}
	}

	int32 build(){
		for(int32 i = 0;i < V;i++){
			if(!used[i]) dfs(i);
		}
		fill(used.begin(), used.end(), 0);
		int32 k = 0;
		for(int32 i = ord.size()-1;i >= 0;i--){
			if(!used[ord[i]]) rdfs(ord[i], k++);
		}
		nV = k;

		vs.resize(k, vector<int32>());
		for(int32 i = 0;i < V;i++)
			vs[comp[i]].push_back(i);

		newG = Graph<DIRECTED>(k);
		for(int32 i = 0;i < V;i++){
			for(auto e : orgG[i]){
				if(comp[i] != comp[e.v])
					newG.add_edge(comp[i], comp[e.v], e.c);
			}
		}
		return k;
	}

	int32 size(){
		return nV;
	}

	const Graph<DIRECTED>& graph(){
		return newG;
	}

	const vector<int32>& vertices(int32 v){
		return vs[v];
	}

	int32 operator[](int32 k){
		return comp[k];
	}
};

class TwoSAT{
private:
	SCC scc;
	int32 n;
	vector<bool> truth;
public:
	TwoSAT(){}
	TwoSAT(int32 n):n(n), scc(2*n){}

	void add_state(int32 a, bool truth1, int32 b, bool truth2){
		int32 na = truth1 ? a+n : a, nb = truth2 ? b+n : b;
		a = truth1 ? a : a+n; b = truth2 ? b : b+n;
		scc.add_edge(na, b);
		scc.add_edge(nb, a);
	}

	bool build(){
		scc.build();
		for(int32 i = 0;i < n;i++){
			if(scc[i] == scc[i+n]) return false;
			if(scc[i] > scc[i+n]) truth.push_back(true);
			else truth.push_back(false);
		}
		return true;
	}

	const vector<bool>& result(){
		return truth;
	}

	bool operator[](int32 k){
		return truth[k];
	}
};

int main(void){
	cin.tie(0);
	ios::sync_with_stdio(false);
}
