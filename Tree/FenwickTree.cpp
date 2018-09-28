#include <bits/stdc++.h>

template<typename CommutativeMonoid>
class FenwickTree{
public:
	using value_structure = CommutativeMonoid;
	using value_type = typename CommutativeMonoid::value_type;
	using size_type = ::std::size_t;

private:
	::std::vector<value_type> tree;
	size_type N;
	
	static value_type calc(const value_type& a, const value_type& b){
		return value_structure::operation(a, b);
	}

public:
	FenwickTree(){}
	FenwickTree(const size_type& N):N(N+10), tree(N+11, value_structure::identity()){}
	FenwickTree(const FenwickTree& bit):tree(bit.tree), N(bit.N){}

	void update(size_type k, const value_type& diff){
		for(;k <= N;k += k & -k){
			tree[k] = calc(tree[k], diff);
		}
	}

	value_type query(size_type k) const { // sum of [0, k)
		value_type res = value_structure::identity();
		for(;k > 0;k -= k & -k){
			res = calc(res, tree[k]);
		}
		return res;
	}

	void updatez(size_type k, value_type v){ // 0-indexed
		update(k+1, v);
	}

	size_type search(const std::function<bool(value_type)>& f) const { // Find k :[0, k) -> True, [0, k-1) -> False
		if(f(value_structure::identity()))
			return 0;
		if(!f(query(N)))
			return N+1;
		value_type x = value_structure::identity();
		size_type k = 1, idx = 0;
		while(k <= N) k <<= 1;
		k >>= 1;
		for(;k > 0;k >>= 1){
			if((idx|k) <= N && !f(tree[idx|k]+x))
				x = calc(x, tree[idx |= x]);
		}
		return idx+1;
	}

	size_type size() const {return N;}
};

/*
	Fenwick Tree(Binary Indexed Tree)
	- typename CommutativeMonoid
		可換モノイド

		要求
		- value_type
		- static constexpr value_type identity -> 単位元を返す
		- static value_type operation()
			- (const value_type& -> const value_type&) -> value_type
*/

/*

// verify: http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=3159984#1

class PlusMonoid{
public:
	using value_type = int_fast64_t;
	static constexpr value_type identity(){return 0;}
	
	static value_type operation(const value_type& a, const value_type& b){
		return a+b;
	}

};

int main(void){
	int_fast64_t n, q;
	scanf("%lld%lld", &n, &q);
	FenwickTree<PlusMonoid> bit(n);
	while(q--){
		int_fast64_t com, x, y;
		scanf("%lld%lld%lld", &com, &x, &y);
		if(com == 0){
			bit.update(x, y);
		}else{
			printf("%lld\n", bit.query(y)-bit.query(x-1));
		}
	}
}
*/

/*
verify: https://beta.atcoder.jp/contests/arc033/submissions/2936185
int main(void){
	int64_t Q;
	FenwickTree<int32_t> bit(200001);
	scanf("%lld", &Q);
	for(int32_t i = 0;i < Q;i++){
		int32_t T, X;
		scanf("%d%d", &T, &X);
		if(T == 1){
			bit.update(X, 1);
		}else{
			int32_t k = bit.search([&](int32_t x){return x >= X;});
			printf("%d\n", k);
			bit.update(k, -1);
		}
	}
}
*/
