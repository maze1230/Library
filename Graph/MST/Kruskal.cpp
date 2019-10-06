#include <bits/stdc++.h>


template<class EdgeWeight>
class Kruskal {
private:
	using size_type = ::std::size_t;
	using weight_type = EdgeWeight;

public:
	struct edge_info {
		size_type from;
		weight_type w;
		edge_info() {}
		edge_info(size_type f_, weight_type w_) : from(f_), w(w_) {}
	};

private:
	using Graph = UndirectedGraph<edge_info>;
	using edge_type = typename Graph::edge_type;

	Graph graph, mst;
	weight_type weight_sum;
public:
	Kruskal() {}
	Kruskal(size_type n) : graph(n) {}
	Kruskal(const Graph& g) : graph(g) {}
	Kruskal(Graph&& g) : graph(g) {}

	void add_edge(size_type from, size_type to, weight_type w) {
		graph.add_edge(from, to, edge_info(from, w));
	}

	const Graph& build() {
		UnionFind uf(graph.size());
		mst = Graph(graph.size());
		weight_sum = weight_type();
		::std::vector<edge_type> edges;

		for (size_type i = 0; i < graph.size(); i++) {
			for (auto edge : graph[i]) {
				edges.push_back(edge);
			}
		}

		::std::sort(edges.begin(), edges.end(),
			[](const edge_type& lhs, const edge_type& rhs) {
				return lhs.info.w < rhs.info.w;
			}
		);

		for (auto edge : edges) {
			if (uf.same(edge.info.from, edge.to)) continue;
			uf.unite(edge.info.from, edge.to);
			mst.add_edge(edge.info.from, edge.to, edge.info);
			weight_sum = weight_sum + edge.info.w;
		}
		return mst;
	}

	const weight_type& weight() { return weight_sum; }
};

/*

Require
{
	"require": [
		"../Graph.cpp",
		"../../Tree/UnionFind/UnionFind.cpp",
	]
}

	 Kruskal

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

		- build -> Graph
			- 現在のGraphから最小全域森を構築し，返す

		- weight -> EdgeWeight
			- 最後に構築した最小全域森の重みを返す
*/
