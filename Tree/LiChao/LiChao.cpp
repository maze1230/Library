#include <bits/stdc++.h>

template<typename T = ::std::int64_t, class Compare = ::std::less<T>>
class LiChaoTree{
private:
	static Compare comp;
	using size_type = ::std::size_t;

	struct Line{
		T a, b;
		bool used;
		Line() : a(0), b(0), used(false) {}
		Line(const T &a_, const T &b_) : a(a_), b(b_), used(true) {}
		T operator()(const T &x) { return a*x+b; }
	};

	size_type n, cnt;
	::std::vector<Line> node;
	::std::vector<T> pos;

	void add(size_type l, size_type r, const Line &line){
		cnt++;
		size_type l0 = l, r0 = r;
		size_type sz = 1;
		for (l += n, r += n; l < r; l >>= 1, r >>= 1, sz <<= 1) {
			if (l & 1) add(l, l0, l0+sz, line), l0 += sz, l++;
			if (r & 1) --r, r0 -= sz, add(r, r0, r0+sz, line);
		}
	}

	void add(size_type k, size_type l, size_type r, Line line){
		if (!node[k].used) {
			node[k] = line;
			return;
		}
		T ly = line(pos[l]), ry = line(pos[r-1]);
		T nly = node[k](pos[l]), nry = node[k](pos[r-1]);
		if (comp(nly, ly) && comp(nry, ry)) return;
		if (comp(ly, nly) && comp(ry, nry)) {
			node[k] = line;
			return;
		}
		if (r - l == 1) return;
		size_type m = (l + r) >> 1;
		if (comp(nly, ly)) swap(node[k], line);
		if (comp(line(pos[m]), node[k](pos[m]))) {
			swap(node[k], line);
			add((k << 1) | 1, m, r, line);
		} else {
			add(k << 1, l, m, line);
		}
	}
public:
	LiChaoTree(){}
	LiChaoTree(const ::std::vector<T> &v) : pos(v), cnt(0) {
		n = 1;
		while (n < pos.size()) n <<= 1;
		sort(pos.begin(), pos.end());
		pos.erase(unique(pos.begin(), pos.end()), pos.end());
		pos.resize(n, pos.back());
		node.resize(n << 1);
	}

	void add(T a, T b){
		add(0, n, Line(a, b));
	}

	void add(T a, T b, T lx, T rx){
		size_type l = lower_bound(pos.begin(), pos.end(), lx) - pos.begin();
		size_type r = lower_bound(pos.begin(), pos.end(), rx) - pos.begin();
		add(l, r, Line(a, b));
	}

	::std::pair<bool, T> query(T x) {
		Line l = get(x);
		return ::std::make_pair(l.used, l(x));
	}

	Line get(T x){
		size_type i = lower_bound(pos.begin(), pos.end(), x) - pos.begin() + n;
		size_type res = -1;
		for(; i > 0; i >>= 1){
			if (node[i].used)
				if (res == -1 || comp(node[i](x), node[res](x))) res = i;
		}
		return res == -1 ? Line() : node[res];
	}

	size_type size() { return cnt; } // the number of lines
};

template<class T, class Compare>
Compare LiChaoTree<T, Compare>::comp;
/*
verify: https://csacademy.com/submission/2150043/
				https://atcoder.jp/contests/dp/submissions/4254496

template<T = int64_t, Compare = less<T>>
class LiChaoTree

T
	-	要求
		- T(T) : copy constructor
		- operator+
		- operator*
		- operator< : (Compareがgreaterの時>が追加で必要)

LiChaoTree
	- 提供
		- Constructor(vector<T> pos)
			- queryの引数として取り得る値のvectorを渡す

		- query(T a) -> pair<bool, T>
			- 計算量 O(log N)
			- 最小のf(a)を返す(Compare = less<T>のとき)
				- aに直線，線分のいずれも存在しない時，pair.firstがfalseになりsecondは未定義

		- add(T a, T b)
			- 計算量 O(log N)
			- f(x) = ax+bを追加する

		- add(T a, T b, T lx, T rx)
			- 計算量 O(log^2 N)
			- f(x) = ax+b [lx, rx) を追加する
			
		- size() -> size_type
			- 計算量 O(1)
			- 現在存在する直線の数，線分も含む
*/

