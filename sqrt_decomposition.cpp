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

class Bucket{
private:
	int32 n, bs, bn;
	vector<int64> data;
	vector<int64> bucket, lazy;
	vector<bool> lazyFlag;
public:
	Bucket(vector<int64> v, int32 sz){
		data = v;
		n = v.size();
		bs = sz;
		bn = (n + bs - 1) / bs;
		data.resize(bn*bs, INF_LL);
		bucket.assign(bn, INF_LL);
		lazy.assign(bn, 0);
		lazyFlag.assign(bn, 0);
		for(int32 i = 0;i < bn;i++){
			int64 minx = INF_LL;
			for(int32 j = 0;j < bs;j++){
				chmin(minx, data[i*bs+j]);
			}
			bucket[i] = minx;
		}
	}

	void eval(int32 k){
		if(lazyFlag[k]){
			lazyFlag[k] = false;
			for(int32 i = bs*k;i < bs*(k+1);i++){
				data[i] = lazy[k];
			}
			lazy[k] = 0;
		}
	}

	void update(int32 s, int32 t, int32 x){
		for(int32 k = 0;k < bn;k++){
			int32 l = k*bs, r = (k+1)*bs;
			if(r <= s || t <= l) continue;
			if(s <= l && r <= t){
				lazyFlag[k] = true;
				bucket[k] = x;
				lazy[k] = x;
			}else{
				eval(k);
				for(int32 i = max(s, l);i < min(t, r);i++){
					data[i] = x;
				}
				bucket[k] = INF_LL;
				for(int32 i = l;i < r;i++){
					chmin(bucket[k], data[i]);
				}
			}
		}
	}

	int64 query(int32 s, int32 t){
		int64 res = INF_LL;
		for(int32 k = 0;k < bn;k++){
			int32 l = k*bs, r = (k+1)*bs;
			if(r <= s || t <= l) continue;
			if(s <= l && r <= t){
				chmin(res, bucket[k]);
			}else{
				eval(k);
				for(int32 i = max(s, l);i < min(t, r);i++){
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
