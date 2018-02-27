#include <bits/stdc++.h>
using namespace std;

#define INF_LL (ll)1e18
#define INF (int)1e9
#define REP(i, n) for(int i = 0;i < (n);i++)
#define FOR(i, a, b) for(int i = (a);i < (b);i++)
#define all(x) x.begin(),x.end()
#define fs first
#define sc second
using ll = long long;
using PII = pair<int, int>;
using PLL = pair<ll, ll>;

const double eps = 1e-10;

template<typename A, typename B>inline void chmin(A &a, B b){if(a > b) a = b;}
template<typename A, typename B>inline void chmax(A &a, B b){if(a < b) a = b;}

class Union_find{
private:
	vector<int> par;
	int n;

public:
	Union_find(int n):n(n){
		par.resize(n, -1);
	}

	int find(int x){
		return par[x] < 0 ? x : par[x] = find(par[x]);
	}

	int size(int x){
		return -par[find(x)];
	}

	void unite(int x, int y){
		x = find(x);
		y = find(y);
		if(x == y) return;
		if(size(x) < size(y)) swap(x, y);
		par[x] += par[y];
		par[y] = x;
	}

	bool same(int x, int y){
		return find(x) == find(y);
	}
};

int main(void){
	cin.tie(0);
	ios::sync_with_stdio(false);
}
