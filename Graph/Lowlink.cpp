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

template<int isUNDIRECTED=1>
class Lowlink : public Graph<isUNDIRECTED>{
	vector<int32> ord, low;
	vector<int32> articulation;
	vector<pair<int32, int32>> bridge;
public:
	Lowlink(){}
	Lowlink(int32 V):Graph<isUNDIRECTED>(V){}
	Lowlink(const Graph<isUNDIRECTED>& g):Graph<isUNDIRECTED>(g){}

	void build(){
		ord.resize(this->V, -1);
		low.resize(this->V, -1);
		int32 k = 0;

		function<void(int32, int32)> dfs = [&, this](int32 v, int32 p){
			ord[v] = k++;
			low[v] = ord[v];
			bool isArt = 0;
			int32 cnt = 0;

			for(auto e : this->G[v]){
				if(e.v == p) continue;
				if(ord[e.v] == -1){
					cnt++;
					dfs(e.v, v);
					low[v] = min(low[v], low[e.v]);
					if(ord[v] < low[e.v]) bridge.emplace_back(e.u, e.v);
					if(~p && ord[v] <= low[e.v]) isArt = 1;
				}else{
					low[v] = min(low[v], ord[e.v]);
				}
			}
			if(p==-1 && cnt > 1) isArt = 1;
			if(isArt) articulation.push_back(v);
		};
		for(int32 i = 0;i < this->V;i++)
			if(ord[i] == -1) dfs(i, -1);
	}

	const vector<pair<int32, int32>>& getBridge(){
		return bridge;
	}

	const vector<int32>& getArticulation(){
		return articulation;
	}

	bool isBridge(int32 u, int32 v) const{
		if(ord[u] > ord[v]) swap(u, v);
		return ord[u] < low[v];
	}
};

template<int isUNDIRECTED=1>
class BECC{
private:
	Lowlink<isUNDIRECTED> orgG;
	Graph<isUNDIRECTED> G;
	vector<int32> belongs;
	vector<vector<int32>> comps;

public:
	BECC(){}
	BECC(const Graph<isUNDIRECTED>& g):orgG(g){
		orgG.build();
		belongs.resize(orgG.size(), -1);
	}

	void build(){
		vector<bool> used(orgG.size(), 0);
		int32 k = 0;
		function<void(int32, int32)> dfs = [&](int32 v, int32 p){
			belongs[v] = k;
			if(p == -1)
				comps.push_back(vector<int32>());
			comps[k].push_back(v);
			for(auto e : orgG[v]){
				if(e.v == p) continue;
				if(orgG.isBridge(e.u, e.v)) continue;
				if(belongs[e.v] >= 0) continue;

				dfs(e.v, v);
			}
		};

		for(int32 i = 0;i < orgG.size();i++){
			if(belongs[i] >= 0) continue;
			dfs(i, -1);
			k++;
		}

		G = Graph<isUNDIRECTED>(k);
		for(auto e : orgG.getBridge()){
			G.add_edge(belongs[e.u], belongs[e.v]);
		}
	}

	int32 belong(int32 v){
		return belongs[v];
	}

	const Graph<isUNDIRECTED> getGraph(){
		return G;
	}
};

int main(void){
	cin.tie(0);
	ios::sync_with_stdio(false);
}
