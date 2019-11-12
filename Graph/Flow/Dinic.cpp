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
protected:
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

class Dinic : public Graph<DIRECTED>{
	vector<int32> level;
	vector<int32> iter;
public:
	Dinic(){}
	Dinic(int32 V):Graph<DIRECTED>(V){
		level.resize(V);
		iter.resize(V);
	}
	Dinic(const Graph<DIRECTED>& g):Graph<DIRECTED>(g){
		level.resize(V);
		iter.resize(V);
	}

	void add_edge(int32 from, int32 to, int32 cap){
		G[from].push_back(Edge(G[to].size(), to, cap));
		G[to].push_back(Edge(G[from].size()-1, from, 0));
	}

	void bfs(int32 s){
		fill(level.begin(), level.end(), -1);
		queue<int32> que;
		level[s] = 0;
		que.push(s);
		while(!que.empty()){
			int32 v = que.front(); que.pop();
			for(int32 i = 0;i < G[v].size();i++){
				Edge &e = G[v][i];
				if(e.c > 0 && level[e.v] < 0){
					level[e.v] = level[v]+1;
					que.push(e.v);
				}
			}
		}
	}

	int64 dfs(int32 v, int32 t, int64 f){
		if(v == t) return f;
		for(int32 &i = iter[v];i < G[v].size();i++){
			Edge &e = G[v][i];
			if(e.c > 0 && level[v] < level[e.v]){
				int32 d = dfs(e.v, t, min(f, e.c));
				if(d > 0){
					e.c -= d;
					G[e.v][e.u].c += d;
					return d;
				}
			}
		}
		return 0;
	}

	int64 max_flow(int32 s, int32 t){
		int64 flow = 0;
		for(;;){
			bfs(s);
			if(level[t] < 0) return flow;
			fill(iter.begin(), iter.end(), 0);
			int64 f;
			while((f = dfs(s, t, INF)) > 0)
				flow += f;
		}
		return flow;
	}
};

int main(void){
	cin.tie(0);
	ios::sync_with_stdio(false);
}
