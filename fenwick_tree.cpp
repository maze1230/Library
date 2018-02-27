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

class BIT{
private:
	vector<ll> dat;
	int n;
public:
	BIT(){}
	BIT(int _n){
		n = _n+1;
		dat.resize(n, 0);
	}

	void add(int i, ll v){
		while(i <= n){
			dat[i] += v;
			i += i & -i;
		}
	}

	ll sum(int i){
		ll res = 0;
		while(i > 0){
			res += dat[i];
			i -= i & -i;
		}
		return res;
	}

	void addz(int i, ll v){
		i++;
		add(i, v);
	}

	ll sumz(int i){
		return sum(i+1);
	}
};

int main(void){
	cin.tie(0);
	ios::sync_with_stdio(false);
}
