#include <bits/stdc++.h>

template<class Abel>
class WeightedUnionFind{
private:
	::std::vector<int_fast32_t> par;
	::std::vector<Abel> diff_weight;
	size_t n;

public:
	WeightedUnionFind(){}
	WeightedUnionFind(size_t n, Abel identity = 0):n(n){
		par.resize(n, -1);
		diff_weight.resize(n, identity);
	}

	uint_fast32_t find(uint_fast32_t x){
		if (par[x] < 0) {
			return x;
		} else {
			uint_fast32_t root = find(par[x]);
			diff_weight[x] += diff_weight[par[x]];
			return par[x] = root;
		}
	}

	Abel weight(uint_fast32_t x){
		find(x);
		return diff_weight[x];
	}

	size_t size(uint_fast32_t x){
		return -par[find(x)];
	}

	bool unite(uint_fast32_t x, uint_fast32_t y, Abel w){
		w += weight(x); w -= weight(y);
		x = find(x); y = find(y);
		if(x == y) return false;
		if(size(x) < size(y)) std::swap(x, y), w = -w;
		par[x] += par[y];
		par[y] = x;
		diff_weight[y] = w;
		return true;
	}

	Abel diff(uint_fast32_t x, uint_fast32_t y){
		return weight(y) - weight(x);
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

verify: https://atcoder.jp/contests/abc087/submissions/4032803

template<Abel>
class WeightedUnionFind

TODO: Abelとしているが実数の+-演算以外に何を載せられるか考える

WeightedUnionFind
	- Constructor
		- WeightedUnionFind(size_t n, Abel identity)
			- 全頂点数nと各頂点の最初の重みidentityを渡す

	- 提供
		- find(uint x) -> uint
			- xの属する木の根を返す

		- weight(uint x) -> Abel
			- xの属する木の根との相対的距離を返す

		- size(uint x) -> size_t
			- xの属する木の頂点数を返す

		- unite(uint x, uint y, Abel w) -> bool
			- xの属する木とyの属する木を併合する
			- weight(y) - weight(x) = wとなるようにする
			- 既に同じ木に属しているときはfalseを返す

		- diff(uint x, uint y) -> Abel
			- weight(y) - weight(x)を返す

		- same(uint x, uint y) -> bool
			- xとyが同じ木に属しているかを返す

*/
