#include <bits/stdc++.h>


template<class EdgeWeight>
class VoronoiMST {
private:
	using size_type = ::std::size_t;
	using weight_type = EdgeWeight;

	using edge_info = typename Kruskal<weight_type>::edge_info;
	using Graph = UndirectedGraph<edge_info>;

	Graph graph;
	weight_type weight_sum;
public:
	VoronoiMST() {}
	VoronoiMST(size_type n) : graph(n) {}
	VoronoiMST(const Graph& g) : graph(g) {}
	VoronoiMST(Graph&& g) : graph(g) {}

	void add_edge(size_type from, size_type to, const weight_type w) {
		graph.add_edge(from, to, edge_info(from, w));
	}

	const Graph& build(const ::std::vector<::std::int64_t>& base) {
		::std::vector<size_type> base_(base.begin(), base.end());
		return build(base_);
	}

	const Graph& build(const ::std::vector<size_type>& base) {
		::std::vector<weight_type> ws(graph.size());
		::std::vector<size_type> visit(graph.size(), -1);

		for (auto x : base) {
			assert(0 <= x && x < base.size());
		}
		Kruskal<weight_type> kr(base.size());

		using P = ::std::tuple<weight_type, size_type, size_type>;
		::std::priority_queue<P, ::std::vector<P>, ::std::greater<P>> pq;

		for (auto x : base) {
			pq.push(P(0, x, x));
		}

		while (pq.size()) {
			weight_type w;
			size_type u, src;
			::std::tie(w, u, src) = pq.top(); pq.pop();
			if (visit[u] == -1) {
				visit[u] = src;
				ws[u] = w;
				for (auto &v : graph[u]) {
					pq.push(P(w + v.info.w, v.to, src));
				}
			} else if (visit[u] < src) {
				kr.add_edge(src, visit[u], w + ws[u]);
			}
		}
		graph = kr.build();
		weight_sum = kr.weight();

		return graph;
	}

	const weight_type& weight() { return weight_sum; }
};


/*

Require
{
	"require": [
		"../Graph.cpp",
		"../../Tree/UnionFind/UnionFind.cpp",
		"Kruskal.cpp",
	]
}


	Voronoi MST

	- class EdgeWeight
		辺の重み
		
		要求
			- operator<
			- operator+
			- operator=

	提供
		- constructor
			- 頂点数，Graphのいずれかで初期化できる

		- add_edge(size_type u, size_type v, EdgeWeight w )
			- u <-> vと結ぶ重みwの無向辺を追加する

		- build(vector<size_type> v) -> Graph
			- 現在のGraphの頂点でvに含まれるものを頂点とする最小全域森を構築し，返す

		- weight -> EdgeWeight
			- 最後に構築した最小全域森の重みを返す
*/
