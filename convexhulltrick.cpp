#include <bits/stdc++.h>
using namespace std;

#define INF_LL (int64)1e18
#define INF (int32)1e9
#define REP(i, n) for(int64 i = 0;i < (n);i++)
#define FOR(i, a, b) for(int64 i = (a);i < (b);i++)
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

template<typename T>
class ConvexHullTrick{
private:
	set<pair<T, T>> L;
	typedef set<pair<T, T>>::iterator iter;
public:
	inline bool tPrev(iter it){ return it != L.begin(); } // there is prev
	inline bool tNext(iter it){ return it != L.end() && next(it) != L.end(); }

	inline T getY(const L& l, const T& x){
		return l.first*x+l.second;
	}

	inline bool check(const L& l1, const L& l2, const L& l3){
		return (l2.first-l3.first)*(l2.second-l1.second)<=(l1.first-l2.first)*(l3.second-l2.second);
	}

	inline bool out(iter it){
		return tPrev(it) && tNext(it) && check(prev(it), it, next(it));
	}

	void add(T a, T b){
		pair<T, T> l(a, b);
		iter it = L.lower_bound(pair<T, T>(a, numeric_limits<T>::min()));
		if(it != L.end() && it->first == a){
			if(it->second <= b) return;
			it = L.erase(it);
		}

		it = L.insert(it, l);
		if(out(it)) return (void)L.erase(it);
		while(tPrev(it)&&out(prev(it))) L.erase(prev(it));
		while(tNext(it)&&out(next(it))) L.erase(next(it));

		if(out(it)) L.erase(it);
	}

	T query(const T x){
		if(L.empty()) return numeric_limits<T>::max();
	}
};

int main(void){
	cin.tie(0);
	ios::sync_with_stdio(false);
}
