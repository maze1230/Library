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
};

int main(void){
	cin.tie(0);
	ios::sync_with_stdio(false);
}
