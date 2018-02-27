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

class Bucket{
private:
	int n, bs, bn;
	vector<ll> data;
	vector<ll> bucket, lazy;
	vector<bool> lazyFlag;
public:
	Bucket(vector<ll> v, int sz){
		data = v;
		n = v.size();
		bs = sz;
		bn = (n + bs - 1) / bs;
		data.resize(bn*bs, INF_LL);
		bucket.assign(bn, INF_LL);
		lazy.assign(bn, 0);
		lazyFlag.assign(bn, 0);
		REP(i, bn){
			ll minx = INF_LL;
			REP(j, bs){
				chmin(minx, data[i*bs+j]);
			}
			bucket[i] = minx;
		}
	}

	void eval(int k){
		if(lazyFlag[k]){
			lazyFlag[k] = false;
			FOR(i, bs*k, bs*(k+1)){
				data[i] = lazy[k];
			}
			lazy[k] = 0;
		}
	}

	void update(int s, int t, int x){
		REP(k, bn){
			int l = k*bs, r = (k+1)*bs;
			if(r <= s || t <= l) continue;
			if(s <= l && r <= t){
				lazyFlag[k] = true;
				bucket[k] = x;
				lazy[k] = x;
			}else{
				eval(k);
				FOR(i, max(s, l), min(t, r)){
					data[i] = x;
				}
				bucket[k] = INF_LL;
				FOR(i, l, r){
					chmin(bucket[k], data[i]);
				}
			}
		}
	}

	ll query(int s, int t){
		ll res = INF_LL;
		REP(k, bn){
			int l = k*bs, r = (k+1)*bs;
			if(r <= s || t <= l) continue;
			if(s <= l && r <= t){
				chmin(res, bucket[k]);
			}else{
				eval(k);
				FOR(i, max(s, l), min(t, r)){
					chmin(res, data[i]);
				}
			}
		}
		return res;
	}
};

int main(void){
	cin.tie(0);
	ios::sync_with_stdio(false);
}
