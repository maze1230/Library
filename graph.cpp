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

int main(void){
	cin.tie(0);
	ios::sync_with_stdio(false);
}
