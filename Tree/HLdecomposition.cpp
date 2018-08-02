#include <bits/stdc++.h>
using namespace std;

#define INF_LL (int64)1e18
#define INF (int32)1e9
#define REP(i, n) for(int64 i = 0;i < (n);i++)
#define FOR(i, a, b) for(int64 i = (a);i < (b);i++)
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

class HLDecomposition{
private:
	vector<vector<int32>> G;
	vector<int32> vid, inv, dep, par, typ, sub, head, in, out;
	int32 n, pos;
public:
	HLDecomposition(){}
	HLDecomposition(int32 n):
		n(n), pos(0), G(n), vid(n, -1), inv(n), dep(n, -1),
		par(n), typ(n), sub(n, 1), head(n), in(n, -1), out(n, -1){}

	void add_edge(int32 u, int32 v){
		G[u].push_back(v);
		G[v].push_back(u);
	}

	void build(){
		int32 type = 0;
		for(int32 i = 0;i < n;i++){
			if(dep[i] == -1){
				dfs(i);
				dfs2(i, type++, i);
			}
		}
	}

	void dfs(int32 v){
		using T = pair<int32, int32>;
		dep[v] = 0;
		par[v] = -1;
		stack<T> st;
		st.emplace(v, 0);
		while(st.size()){
			v = st.top().first;
			int32 &i = st.top().second;
			if(i<G[v].size()){
				int32 u = G[v][i++];
				if(u == par[v]) continue;
				par[u] = v;
				dep[u] = dep[v]+1;
				st.emplace(u, 0);
			}else{
				st.pop();
				for(int32 &u : G[v]){
					if(u == par[v]) continue;
					sub[v] += sub[u];
					if(G[v][0] == par[v] || sub[u] > sub[G[v][0]]){
						swap(u, G[v][0]);
					}
				}
			}
		}
	}

	void dfs2(int32 v, int32 t, int32 h){
		typ[v] = t;
		in[v] = pos;
		out[v] = in[v]+sub[v]-1;
		vid[v] = pos++;
		inv[vid[v]] = v;
		head[v] = h;
		for(int32 u : G[v]){
			if(u == par[v]) continue;
			if(u == G[v][0])
				dfs2(u, t, h);
			else
				dfs2(u, t, u);
		}
	}

	void for_each(int32 u, int32 v, const function<void(int32, int32)>& f){
		while(1){
			if(vid[u] > vid[v]) swap(u, v);
			f(max(vid[head[v]], vid[u]), vid[v]);
			if(head[u] != head[v]) v=par[head[v]];
			else break;
		}
	}

	void for_each_edge(int32 u, int32 v, const function<void(int32, int32)>& f){
		while(1){
			if(vid[u] > vid[v]) swap(u, v);
			if(head[u] != head[v]){
				f(vid[head[v]], vid[v]);
				v = par[head[v]];
			}else{
				if(u!=v) f(vid[u]+1, vid[v]);
				break;
			}
		}
	}

	void for_subtree(int32 u, const function<void(int32, int32)>& f){
		if(in[u] == -1){
			cout << "Invalid vertex." << endl;
			return;
		}
		f(in[u], out[u]);
	}

	int32 lca(int32 u, int32 v){
		while(1){
			if(vid[u] > vid[v]) swap(u, v);
			if(head[u] != head[v]) v = par[head[v]];
			else return u;
		}
	}

	int32 distance(int32 u, int32 v){
		return dep[u]+dep[v]-2*dep[lca(u, v)];
	}

	int32 getvid(int32 v){
		return vid[v];
	}
};

int main(void){
	cin.tie(0);
	ios::sync_with_stdio(false);
}
