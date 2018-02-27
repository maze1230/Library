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

class BIT{
private:
	vector<int64> dat;
	int32 n;
public:
	BIT(){}
	BIT(int32 _n){
		n = _n+1;
		dat.resize(n, 0);
	}
	BIT(const BIT& bit):dat(bit.dat), n(bit.n){}

	void add(int32 i, int64 v){
		while(i <= n){
			dat[i] += v;
			i += i & -i;
		}
	}

	int64 sum(int32 i){
		int64 res = 0;
		while(i > 0){
			res += dat[i];
			i -= i & -i;
		}
		return res;
	}

	void addz(int32 i, int64 v){
		i++;
		add(i, v);
	}

	int64 sumz(int32 i){
		return sum(i+1);
	}
};

int main(void){
	cin.tie(0);
	ios::sync_with_stdio(false);
}
