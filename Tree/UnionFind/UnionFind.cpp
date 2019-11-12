#include <bits/stdc++.h>

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

int main(void){
	std::cin.tie(0);
	std::ios::sync_with_stdio(false);
}

/*

class UnionFind

UnionFind
	- 提供
		- find(uint x) -> uint
			- xの属する木の根を返す

		- size(uint x) -> size_t
			- xの属する木の頂点数を返す

		- unite(uint x, uint y) -> bool
			- xの属する木とyの属する木を併合する
			- 既に同じ木に属しているときはfalseを返す

		- same(uint x, uint y) -> bool
			- xとyが同じ木に属しているかを返す

*/
