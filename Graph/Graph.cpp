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
	Graph& operator=(Graph&& g) { ::std::swap(edges, g.edges); return *this;}

	const size_type size() { return edges.size(); }
	virtual void add_edge (size_type from, size_type to, EdgeInfo info) = 0;

	const ::std::vector<edge_type>& operator[](size_type k) const { return edges[k]; }
	::std::vector<edge_type>& operator[](size_type k) { return edges[k]; }

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
