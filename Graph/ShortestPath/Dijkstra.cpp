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
	Graph& operator=(const Graph& g) : edges(g.edges) {}
	Graph& operator=(Graph&& g) { ::std::swap(edges, g.edges); }

	const size_type size() { return edges.size(); }
	virtual void add_edge (size_type from, size_type to, EdgeInfo info) = 0;

	const ::std::vector<edge_type>& operator[](size_type k) { return edges[k]; }

};

template<class EdgeInfo>
class DirectedGraph : public Graph<EdgeInfo> {
protected:
	using size_type = ::std::size_t;
public:
	using edge_type = typename Graph<EdgeInfo>::edge_type;
	DirectedGraph() {}
	DirectedGraph(size_type n) : Graph<EdgeInfo>(n) {}
	DirectedGraph(const DirectedGraph& g) : Graph<EdgeInfo>(g) {}
	DirectedGraph(DirectedGraph&& g) : Graph<EdgeInfo>(g) {}
	DirectedGraph& operator=(const DirectedGraph& g) { this->edges = g.edges; return *this;}
	DirectedGraph& operator=(DirectedGraph&& g) noexcept { ::std::swap(this->edges, g.edges); return *this; }

	void add_edge (size_type from, size_type to, EdgeInfo info = EdgeInfo()) {
		this->edges[from].push_back(edge_type(to, info));
	}
};

template<class DistanceType>
class Dijkstra {
private:
	using size_type = ::std::size_t;
	using distance_type = DistanceType;
	using Graph = DirectedGraph<distance_type>;

	Graph graph;
public:
	Dijkstra() {}
	Dijkstra(size_type n) : graph(n) {}
	Dijkstra(const Graph& g) : graph(g) {}

	void add_edge (size_type from, size_type to, distance_type d) {
		graph.add_edge(from, to, d);
	}

	::std::vector<distance_type> build(size_type start) {
		::std::vector<distance_type> dis(graph.size(), invalid_value());

		using P = ::std::pair<distance_type, size_type>;
		::std::priority_queue<P, ::std::vector<P>, ::std::greater<P>> pq;
		dis[start] = 0;
		pq.push(P(dis[start], start));
		while (pq.size()) {
			P p = pq.top(); pq.pop();
			if (p.first > dis[p.second]) continue;
			for (auto e : graph[p.second]) {
				if (dis[e.to] > dis[p.second] + e.info) {
					dis[e.to] = dis[p.second] + e.info;
					pq.push(P(dis[e.to], e.to));
				}
			}
		}

		return dis;
	}

	static const distance_type invalid_value () {
		static const distance_type inf = ::std::numeric_limits<distance_type>::max() / 2 - 1;
		return inf;
	}
};
 
/*

	Dijkstra
	
	- class DistanceType
		距離の型であり，数値型である必要がある

	提供

	- constructor(size_type n)
		- n頂点からなるグラフを準備する
	- constructor(Graph g)
		- グラフgをコピーする
	
	- add_edge(size_type u, size_type v, DistanceType d)
		- u -> vに距離dの辺をつなぐ

	- build (size_type v) -> vector<distance_type>
		- O((N + M) log M) : 頂点数N, 辺数M
		- vを始点としたときの拡張点への最短距離を求める
			- 到達不可能であるときはinvalid_value()が入っている

	- invalid_value () -> distance_type
		- 処理上のinf(numeric_limits<distance_type>::max()/2-1を返す

*/
