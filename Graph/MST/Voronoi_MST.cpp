#include <bits/stdc++.h>

template<class EdgeInfo>
class Graph {
protected:
	using size_type = ::std::size_t;
	struct Edge {
		size_type to;
		EdgeInfo info;
		Edge() : to(-1) {}
		Edge(size_type to_, EdgeInfo info_) : to(to_), info(info_) {}
	};
	using edge_type = Edge;

	::std::vector<::std::vector<edge_type>> edges;

public:
	Graph() {}
	Graph(size_type n) : edges(n) {}
	Graph(const Graph& g) : edges(g.edges) {}
	Graph(Graph&& g) { ::std::swap(edges, g.edges); }
	Graph& operator=(const Graph& g) { edges = g.edges; return *this;}
	Graph& operator=(Graph&& g) noexcept { ::std::swap(edges, g.edges); return *this;}

	const size_type size() { return edges.size(); }
	virtual void add_edge (size_type from, size_type to, EdgeInfo info) = 0;

	const ::std::vector<edge_type>& operator[](size_type k) { return edges[k]; }

};

template<class EdgeInfo>
class UndirectedGraph : public Graph<EdgeInfo> {
protected:
	using size_type = ::std::size_t;
public:
	using edge_type = typename Graph<EdgeInfo>::edge_type;
	UndirectedGraph() {}
	UndirectedGraph(size_type n) : Graph<EdgeInfo>(n) {}
	UndirectedGraph(const UndirectedGraph& g) : Graph<EdgeInfo>(g) {}
	UndirectedGraph(UndirectedGraph&& g) : Graph<EdgeInfo>(g) {}
	UndirectedGraph& operator=(const UndirectedGraph& g) { this->edges = g.edges; return *this;}
	UndirectedGraph& operator=(UndirectedGraph&& g) noexcept { ::std::swap(this->edges, g.edges); return *this; }

	void add_edge (size_type from, size_type to, EdgeInfo info = EdgeInfo()) {
		this->edges[from].push_back(edge_type(to, info));
		this->edges[to].push_back(edge_type(from, info));
	}
};

class UnionFind{
private:
	::std::vector<int_fast32_t> par;
	size_t n;

public:
	UnionFind(){}
	UnionFind(size_t n):n(n){
		par.resize(n, -1);
	}

	uint_fast32_t find(uint_fast32_t x){
		return par[x] < 0 ? x : par[x] = find(par[x]);
	}

	size_t size(uint_fast32_t x){
		return -par[find(x)];
	}

	bool unite(uint_fast32_t x, uint_fast32_t y){
		x = find(x);
		y = find(y);
		if(x == y) return false;
		if(size(x) < size(y)) std::swap(x, y);
		par[x] += par[y];
		par[y] = x;
		return true;
	}

	bool same(uint_fast32_t x, uint_fast32_t y){
		return find(x) == find(y);
	}
};

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

	Graph graph;
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
		Graph mst = Graph(graph.size());
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
		return graph = mst;
	}

	const weight_type& weight() { return weight_sum; }
};

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
