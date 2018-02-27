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

class RMQ{
private:
	int n;
	vector<ll> node, lazy;
	vector<bool> lazyFlag;
public:
	RMQ(vector<ll> v){
		int sz = v.size();
		n = 1; while(n < sz) n *= 2;
		node.resize(2*n-1, INF_LL);
		lazy.resize(2*n-1, 0);
		lazyFlag.resize(2*n-1, false);
		REP(i, sz) node[i+n-1] = v[i];
		for(int i = n-2;i >= 0;i--) node[i] = min(node[2*i+1], node[2*i+2]);
	}

	void eval(int k, int l, int r){
		if(lazyFlag[k]){
			node[k] = lazy[k];
			if(r-l > 1){
				lazy[2*k+1] = lazy[k];
				lazy[2*k+2] = lazy[k];
				lazyFlag[2*k+1] = lazyFlag[2*k+2] = true;
			}
			lazy[k] = 0;
			lazyFlag[k] = false;
		}
	}

	void update(int a, int b, ll x, int k=0, int l=0, int r=-1){
		if(r < 0) r = n;
		eval(k, l, r);
		if(b <= l || r <= a) return;

		if(a <= l && r <= b){
			lazy[k] = x;
			lazyFlag[k] = true;
			eval(k, l, r);
		}else{
			update(a, b, x, k*2+1, l, (l+r)/2);
			update(a, b, x, k*2+2, (l+r)/2, r);
			node[k] = min(node[k*2+1], node[k*2+2]);
		}
	}

	ll query(int a, int b, int k=0, int l=0, int r=-1){
		if(r < 0) r = n;
		eval(k, l, r);

		if(b <= l || r <= a) return INF_LL;
		if(a <= l && r <= b) return node[k];

		return min(query(a, b, k*2+1, l, (l+r)/2), query(a, b, k*2+2, (l+r)/2, r));
	}
};

class LazySegTree{
private:
	int n;
	vector<ll> node, lazy;
	vector<bool> lazyFlag;
public:
	LazySegTree(const vector<ll>& v){
		int sz = v.size();
		n = 1; while(n < sz) n *= 2;
		node.resize(2*n-1, 0);
		lazy.resize(2*n-1, 0);
		lazyFlag.resize(2*n-1, false);
		REP(i, sz) node[i+n-1] = v[i];
		for(int i = n-2;i >= 0;i--) node[i] = node[2*i+1]+node[2*i+2];
	}

	void eval(int k, int l, int r){
		if(lazyFlag[k]){
			node[k] += lazy[k] * (r-l);
			if(r-l > 1){
				lazy[2*k+1] += lazy[k];
				lazy[2*k+2] += lazy[k];
				lazyFlag[2*k+1] = lazyFlag[2*k+2] = true;
			}
			lazy[k] = 0;
			lazyFlag[k] = false;
		}
	}

	void add(int a, int b, ll x, int k=0, int l=0, int r=-1){
		if(r < 0) r = n;
		eval(k, l, r);
		if(b <= l || r <= a) return;

		if(a <= l && r <= b){
			lazy[k] += x;
			lazyFlag[k] = true;
			eval(k, l, r);
		}else{
			add(a, b, x, k*2+1, l, (l+r)/2);
			add(a, b, x, k*2+2, (l+r)/2, r);
			node[k] = node[k*2+1] + node[k*2+2];
		}
	}

	ll query(int a, int b, int k=0, int l=0, int r=-1){
		if(r < 0) r = n;
		eval(k, l, r);

		if(b <= l || r <= a) return 0;
		if(a <= l && r <= b) return node[k];

		return query(a, b, k*2+1, l, (l+r)/2) + query(a, b, k*2+2, (l+r)/2, r);
	}
};

int main(void){
	cin.tie(0);
	ios::sync_with_stdio(false);
}
