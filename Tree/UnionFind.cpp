#include <bits/stdc++.h>

class UnionFind{
private:
	std::vector<int_fast32_t> par;
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

	void unite(uint_fast32_t x, uint_fast32_t y){
		x = find(x);
		y = find(y);
		if(x == y) return;
		if(size(x) < size(y)) std::swap(x, y);
		par[x] += par[y];
		par[y] = x;
	}

	bool same(uint_fast32_t x, uint_fast32_t y){
		return find(x) == find(y);
	}
};

int main(void){
	std::cin.tie(0);
	std::ios::sync_with_stdio(false);
}
