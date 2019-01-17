#include <bits/stdc++.h>
using namespace std;

template<typename T, typename E>
class SegTree{
private:
	using F = function<T(T, T)>;
	using G = function<T(T, E)>;
	int n;
	F f;
	G g;
	T ti; // e0:F
	vector<T> node;
public:
	SegTree(){}
	SegTree(int _n, F f, G g, T ti):f(f), g(g), ti(ti){
		init(_n);
	}
	SegTree(vector<T> v, F f, G g, T ti):f(f), g(g), ti(ti){
		init(v.size());
		for(int i = 0;i < v.size();i++) node[i+n-1] = v[i];
		for(int i = n-2;i >= 0;i--) node[i] = merge(node[i*2+1], node[i*2+2]);
	}

	inline void init(int _n){
		n = 1;
		while(n < _n) n *= 2;
		node.resize(2*n-1, ti);
	}

	inline T merge(T lhs, T rhs){
		if(lhs == ti) return rhs;
		else if(rhs == ti) return lhs;
		return f(lhs, rhs);
	}

	void update(int k, E x){
		k += n-1;
		node[k] = g(node[k], x);
		while(k){
			k = (k-1)/2;
			node[k] = merge(node[k*2+1], node[k*2+2]);
		}
	}

	T query(int a, int b, int k=0, int l=0, int r=-1){
		if(r < 0) r = n;
		if(b <= l || r <= a) return ti;
		if(a <= l && r <= b) return node[k];
		return merge(query(a, b, k*2+1, l, (l+r)/2), query(a, b, k*2+2, (l+r)/2, r));
	}
};

int main(void){
}
