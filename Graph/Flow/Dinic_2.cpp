#include <bits/stdc++.h>

template<class CapacityType>
class Dinic {
private:
	using size_type = ::std::size_t;
	using capacity_type = CapacityType;

public:
	struct edge_info {
		size_type rev;
		capacity_type cap;
		bool is_rev;
		edge_info() {}
		edge_info(size_type rev_, capacity_type c_, bool is_rev_ = 0) : rev(rev_), cap(c_), is_rev(is_rev_) {}
	};

private:
	using Graph = DirectedGraph<edge_info>;
	using edge_type = typename Graph::edge_type;

	Graph graph;
	::std::vector<::std::int64_t> level;
	::std::vector<size_type> iter;
	capacity_type limit;

public:
	Dinic() {}
	Dinic(size_type n, capacity_type limit_) : graph(n), level(n), iter(n), limit(limit_) {}
	Dinic(const Graph& g, capacity_type limit_) : graph(g), level(g.size()), iter(g.size()), limit(limit_) {}

	void add_edge (size_type from, size_type to, capacity_type c) {
		graph.add_edge(from, to, edge_info(graph[to].size(), c));
		graph.add_edge(to, from, edge_info(graph[from].size()-1, capacity_type(0), 1));
	}

	bool bfs(size_type s, size_type t) {
		::std::fill(level.begin(), level.end(), -1);
		::std::queue<size_type> que;
		level[s] = 0;
		que.push(s);
		while (!que.empty()) {
			size_type v = que.front(); que.pop();
			for (auto &e : graph[v]) {
				if (e.info.cap > 0 && level[e.to] < 0) {
					level[e.to] = level[v] + 1;
					que.push(e.to);
				}
			}
		}
		return level[t] != -1;
	}

	capacity_type dfs(size_type v, size_type t, capacity_type flow) {
		if (v == t) return flow;
		for (size_type &i = iter[v]; i < graph[v].size(); i++) {
			edge_type &e = graph[v][i];
			if (e.info.cap > 0 && level[v] < level[e.to]) {
				capacity_type d = dfs(e.to, t, ::std::min(flow, e.info.cap));
				if (d > 0) {
					e.info.cap -= d;
					graph[e.to][e.info.rev].info.cap += d;
					return d;
				}
			}
		}
		return 0;
	}

	capacity_type max_flow(size_type s, size_type t) {
		capacity_type flow = capacity_type();
		while (bfs(s, t)) {
			::std::fill(iter.begin(), iter.end(), 0);
			capacity_type f;
			while ((f = dfs(s, t, limit)) > 0) flow += f;
		}
		return flow;
	}

	void output() {
		for (size_type i = 0; i < graph.size(); i++) {
			for (auto &e : graph[i]) {
				if (e.info.is_rev) continue;
				auto &rev_e = graph[e.to][e.info.rev];
				::std::cout << i << " -> " << e.to << " : " << rev_e.info.cap << "/" << rev_e.info.cap+e.info.cap << ::std::endl;
			}
		}
	}
};

/*

verify: http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=3428837

Require
{
	"require": [
		"../Graph.cpp",
	]
}

	Dinic

	- class CapacityType
		容量

		要求
			- default constructor
				- 0的なものにする
			- operator<(const CapacityType&)
			- operator>(const CapacityType::value_type)
			- operator+=(const CapacityType&)
			- operator-=(const CapacityType&)

			- ofstream << (output使用時)

	提供
		- constructor
			- 頂点数, Graphのいずれかと容量のINFを与えて初期化

		- add_edge(size_type u, size_type v, CapacityType c )
			- u -> vの容量cの有向辺を追加する

		- max_flow(s, t) -> capacity_type
			- sからtへの最大流を返す

		- output
			- どの辺にどれだけ流れているかを出力する

*/
