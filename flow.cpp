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
		long long c; // cost
		Edge(int u, int v, long long c=0, int id=0):u(u), v(v), c(c), id(id){}
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

class Dinic : public Graph<DIRECTED>{
	vector<int> level;
	vector<int> iter;
public:
	Dinic(){}
	Dinic(int V):Graph<DIRECTED>(V){
		level.resize(V);
		iter.resize(V);
	}
	Dinic(const Graph<DIRECTED>& g):Graph<DIRECTED>(g){
		level.resize(V);
		iter.resize(V);
	}

	void add_edge(int from, int to, int cap){
		G[from].push_back(Edge(G[to].size(), to, cap));
		G[to].push_back(Edge(G[from].size()-1, from, 0));
	}

	void bfs(int s){
		fill(level.begin(), level.end(), -1);
		queue<int> que;
		level[s] = 0;
		que.push(s);
		while(!que.empty()){
			int v = que.front(); que.pop();
			for(int i = 0;i < G[v].size();i++){
				Edge &e = G[v][i];
				if(e.c > 0 && level[e.v] < 0){
					level[e.v] = level[v]+1;
					que.push(e.v);
				}
			}
		}
	}

	long long dfs(int v, int t, long long f){
		if(v == t) return f;
		for(int &i = iter[v];i < G[v].size();i++){
			Edge &e = G[v][i];
			if(e.c > 0 && level[v] < level[e.v]){
				int d = dfs(e.v, t, min(f, e.c));
				if(d > 0){
					e.c -= d;
					G[e.v][e.u].c += d;
					return d;
				}
			}
		}
		return 0;
	}

	long long max_flow(int s, int t){
		long long flow = 0;
		for(;;){
			bfs(s);
			if(level[t] < 0) return flow;
			fill(iter.begin(), iter.end(), 0);
			long long f;
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
