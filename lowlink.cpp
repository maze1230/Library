#include <bits/stdc++.h>
using namespace std;

#define INF_LL (ll)1e18
#define INF (int)1e9
#define REP(i, n) for(int i = 0;i < (n);i++)
#define FOR(i, a, b) for(int i = (a);i < (b);i++)
#define all(x) x.begin(),x.end()
#define fs first
#define sc second
using ll = long long;
using PII = pair<int, int>;
using PLL = pair<ll, ll>;

const double eps = 1e-10;

template<typename A, typename B>inline void chmin(A &a, B b){if(a > b) a = b;}
template<typename A, typename B>inline void chmax(A &a, B b){if(a < b) a = b;}

const int DIRECTED = 0;
const int UNDIRECTED = 1;

template<int isUNDIRECTED=0>
class Graph{
protected:
	struct Edge{
		int u, v, id; // from, to, 何番目の辺か
		ll c; // cost
		Edge(int u, int v, ll c=0, int id=0):u(u), v(v), c(c), id(id){}
	};

	int V, E; // 頂点数、辺数
	vector<vector<Edge>> G;
	vector<Edge> Es;

public:
	Graph(){}
	Graph(int V):V(V){G.resize(V);}
	Graph(const Graph<isUNDIRECTED>& g):V(g.V), E(g.E), G(g.G), Es(g.Es){}

	void add_edge(int u, int v, ll c=0, int id=0){
		G[u].emplace_back(u, v, c, id);
		if(isUNDIRECTED) G[v].emplace_back(v, u, c, id);
		Es.emplace_back(u, v, c, id);
		E++;
	}

	const vector<Edge>& operator[](int k){
		return G[k];
	}

	unsigned long long size(){
		return V;
	}
};

template<int isUNDIRECTED>
class Lowlink : public Graph<isUNDIRECTED>{
	vector<int> ord, low;
	vector<int> articulation;
	vector<pair<int, int>> bridge;
public:
	Lowlink(){}
	Lowlink(int V):Graph<isUNDIRECTED>(V){}
	Lowlink(const Graph<isUNDIRECTED>& g):Graph<isUNDIRECTED>(g){}

	void build(){
		ord.resize(this->V, -1);
		low.resize(this->V, -1);
		int k = 0;
		
		function<void(int, int)> dfs = [&, this](int v, int p){
			ord[v] = k++;
			low[v] = ord[v];
			bool isArt = 0; // 関節点か
			int cnt = 0;

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
		for(int i = 0;i < this->V;i++)
			if(ord[i] == -1) dfs(i, -1);
	}

	const vector<pair<int, int>>& getBridge(){
		return bridge;
	}
	
	const vector<int>& getArticulation(){
		return articulation;
	}

	bool isBridge(int u, int v) const{
		if(ord[u] > ord[v]) swap(u, v);
		return ord[u] < low[v];
	}
};

template<int isUNDIRECTED>
class BECC{
private:
	Lowlink<isUNDIRECTED> orgG;
	Graph<isUNDIRECTED> G;
	vector<int> belongs;
	vector<vector<int>> comps;

public:
	BECC(){}
	BECC(const Graph<isUNDIRECTED>& g):orgG(g){
		orgG.build();
		belongs.resize(orgG.size(), -1);
	}

	void build(){
		vector<bool> used(orgG.size(), 0);
		int k = 0;
		function<void(int, int)> dfs = [&](int v, int p){
			belongs[v] = k;
			if(p == -1)
				comps.push_back(vector<int>());
			comps[k].push_back(v);
			for(auto e : orgG[v]){
				if(e.v == p) continue;
				if(orgG.isBridge(e.u, e.v)) continue;
				if(belongs[e.v] >= 0) continue;

				dfs(e.v, v);
			}
		};

		for(int i = 0;i < orgG.size();i++){
			if(belongs[i] >= 0) continue;
			dfs(i, -1);
			k++;
		}

		G = Graph<isUNDIRECTED>(k);
		for(auto e : orgG.getBridge()){
			G.add_edge(belongs[e.u], belongs[e.v]);
		}
	}

	int belong(int v){
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
