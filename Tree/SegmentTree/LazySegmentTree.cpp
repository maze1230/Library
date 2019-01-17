#include <bits/stdc++.h>

template<typename T, typename E>
class LazySegTree{
private:
	using F = function<T(T, T)>;
	using G = function<T(T, E)>;
	using H = function<E(E, E)>;
	using P = function<E(E, int64)>;
	int32 n;
	vector<T> node;
	vector<E> lazy;
	F f;
	G g;
	H h;
	P p;
	T ti;
	E ei;
public:
	LazySegTree(){}
	LazySegTree(int32 _n, F f, G g, H h, T ti, E ei, P p = [](E a, int32 b){return a;}):f(f), g(g), h(h), p(p), ti(ti), ei(ei){
		init(_n);
	}

	LazySegTree(vector<T> v, F f, G g, H h, T ti, E ei, P p = [](E a, int32 b){return a;}):f(f), g(g), h(h), p(p), ti(ti), ei(ei){
		init(v.size());
		for(int32 i = 0;i < v.size();i++) node[i+n-1] = v[i];
		for(int32 i = n-2;i >= 0;i--) node[i] = merge(node[i*2+1], node[i*2+2]);
	}

	void init(int32 _n){
		n = 1;
		while(n < _n) n*=2;
		node.resize(2*n-1, ti);
		lazy.resize(2*n-1, ei);
	}

	inline T merge(T lhs, T rhs){
		if(lhs == ti) return rhs;
		else if(rhs == ti) return lhs;
		return f(lhs, rhs);
	}

	inline void eval(int32 k, int32 l, int32 r){
		if(lazy[k] == ei) return;
		node[k] = g(node[k], p(lazy[k], r-l));
		if(r-l > 1){
			lazy[k*2+1] = h(lazy[k*2+1], lazy[k]);
			lazy[k*2+2] = h(lazy[k*2+2], lazy[k]);
		}
		lazy[k] = ei;
	}

	T update(int32 a, int32 b, E x, int32 k=0, int32 l=0, int32 r=-1){
		if(r<0) r = n;
		eval(k, l, r);
		if(b <= l || r <= a) return node[k];
		if(a <= l && r <= b){
			lazy[k] = h(lazy[k], x);
			return g(node[k], p(lazy[k], r-l));
	}
		return node[k] = merge(update(a, b, x, k*2+1, l, (l+r)/2), update(a, b, x, k*2+2, (l+r)/2, r));
	}

	T query(int32 a, int32 b, int32 k=0, int32 l=0, int32 r=-1){
		if(r<0) r = n;
		eval(k, l, r);
		if(b <= l || r <= a) return ti;
		if(a <= l && r <= b) return node[k];
		return merge(query(a, b, k*2+1, l, (l+r)/2), query(a, b, k*2+2, (l+r)/2, r));
	}
};

