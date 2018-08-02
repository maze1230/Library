#include <bits/stdc++.h>

class HLDecomposition{
private:
	std::vector<std::vector<int_fast32_t>> G;
	std::vector<int_fast32_t> vid, inv, dep, par, typ, sub, head, in, out;
	size_t n, pos;
public:
	HLDecomposition(){}
	HLDecomposition(size_t n):
		n(n), pos(0), G(n), vid(n, -1), inv(n), dep(n, -1),
		par(n), typ(n), sub(n, 1), head(n), in(n, -1), out(n, -1){}

	void add_edge(uint_fast32_t u, uint_fast32_t v){
		G[u].push_back(v);
		G[v].push_back(u);
	}

	void build(){
		uint_fast32_t type = 0;
		for(int_fast32_t i = 0;i < n;i++){
			if(dep[i] == -1){
				dfs(i);
				dfs2(i, type++, i);
			}
		}
	}

	void dfs(uint_fast32_t v){
		using T = std::pair<uint_fast32_t, uint_fast32_t>;
		dep[v] = 0;
		par[v] = -1;
		std::stack<T> st;
		st.emplace(v, 0);
		while(st.size()){
			v = st.top().first;
			uint_fast32_t &i = st.top().second;
			if(i<G[v].size()){
				uint_fast32_t u = G[v][i++];
				if(u == par[v]) continue;
				par[u] = v;
				dep[u] = dep[v]+1;
				st.emplace(u, 0);
			}else{
				st.pop();
				for(int_fast32_t &u : G[v]){
					if(u == par[v]) continue;
					sub[v] += sub[u];
					if(G[v][0] == par[v] || sub[u] > sub[G[v][0]]){
						std::swap(u, G[v][0]);
					}
				}
			}
		}
	}

	void dfs2(uint_fast32_t v, uint_fast32_t t, uint_fast32_t h){
		typ[v] = t;
		in[v] = pos;
		out[v] = in[v]+sub[v]-1;
		vid[v] = pos++;
		inv[vid[v]] = v;
		head[v] = h;
		for(uint_fast32_t u : G[v]){
			if(u == par[v]) continue;
			if(u == G[v][0])
				dfs2(u, t, h);
			else
				dfs2(u, t, u);
		}
	}

	void for_each(uint_fast32_t u, uint_fast32_t v, const std::function<void(uint_fast32_t, uint_fast32_t)>& f){
		while(1){
			if(vid[u] > vid[v]) std::swap(u, v);
			f(std::max(vid[head[v]], vid[u]), vid[v]);
			if(head[u] != head[v]) v=par[head[v]];
			else break;
		}
	}

	void for_each_edge(uint_fast32_t u, uint_fast32_t v, const std::function<void(uint_fast32_t, uint_fast32_t)>& f){
		while(1){
			if(vid[u] > vid[v]) std::swap(u, v);
			if(head[u] != head[v]){
				f(vid[head[v]], vid[v]);
				v = par[head[v]];
			}else{
				if(u!=v) f(vid[u]+1, vid[v]);
				break;
			}
		}
	}

	void for_subtree(uint_fast32_t u, const std::function<void(uint_fast32_t, uint_fast32_t)>& f){
		if(in[u] == -1){
			std::cout << "Invalid vertex." << std::endl;
			return;
		}
		f(in[u], out[u]);
	}

	uint_fast32_t lca(uint_fast32_t u, uint_fast32_t v){
		while(1){
			if(vid[u] > vid[v]) std::swap(u, v);
			if(head[u] != head[v]) v = par[head[v]];
			else return u;
		}
	}

	uint_fast32_t distance(uint_fast32_t u, uint_fast32_t v){
		uint_fast32_t ret = dep[u]+dep[v]-2*dep[lca(u, v)];
		assert(ret >= 0);
		return ret;
	}

	uint_fast32_t getvid(uint_fast32_t v){
		return vid[v];
	}
};

/*
	Heavy-Light Decomposition
	- 木を列に分解する

	- for_each
		- u->vのパス上の頂点への処理
		- f(u, v) -> [u, v]
	
	- for_each_edge
		- u->vのパス上の辺への処理
		- 重みなどは根から遠い方の頂点に持たせる
		- f(u, v) -> [u, v]

	- for_subtree
		- uの部分木への処理
		- 頂点の場合はそのまま f(u, v) -> [u, v]
		- 辺の場合は呼び出されたときにu+1 f(u, v) -> [u+1, v]
*/

int main(void){
	std::cin.tie(0);
	std::ios::sync_with_stdio(false);
}
