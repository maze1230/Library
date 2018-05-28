#include <bits/stdc++.h>
using namespace std;

#define INF_LL (int64)1e18
#define INF (int32)1e9
#define REP(i, n) for(int i = 0;i < (n);i++)
#define FOR(i, a, b) for(int i = (a);i < (b);i++)
#define all(x) x.begin(),x.end()
#define fs first
#define sc second
using int32 = int_fast32_t;
using uint32 = uint_fast32_t;
using int64 = int_fast64_t;
using uint64 = uint_fast64_t;
using PII = pair<int32, int32>;
using PLL = pair<int64, int64>;

const double eps = 1e-10;

template<typename A, typename B>inline void chmin(A &a, B b){if(a > b) a = b;}
template<typename A, typename B>inline void chmax(A &a, B b){if(a < b) a = b;}

class UnionFind{
private:
	vector<int32> par;
	int32 n;

public:
	UnionFind(){}
	UnionFind(int32 n):n(n){
		par.resize(n, -1);
	}

	int32 find(int32 x){
		return par[x] < 0 ? x : par[x] = find(par[x]);
	}

	int32 size(int32 x){
		return -par[find(x)];
	}

	void unite(int32 x, int32 y){
		x = find(x);
		y = find(y);
		if(x == y) return;
		if(size(x) < size(y)) swap(x, y);
		par[x] += par[y];
		par[y] = x;
	}

	bool same(int32 x, int32 y){
		return find(x) == find(y);
	}
};

int main(void){
	cin.tie(0);
	ios::sync_with_stdio(false);
}
