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
