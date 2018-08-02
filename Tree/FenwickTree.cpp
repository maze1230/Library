#include <bits/stdc++.h>

template<typename CommutativeMonoid>
class BIT{
public:
	using value_type = CommutativeMonoid;

private:
	std::vector<value_type> tree;
	size_t N;

public:
	BIT(){}
	BIT(const size_t& N):N(N+10), tree(N+11, value_type()){}
	BIT(const BIT& bit):tree(bit.tree), N(bit.N){}

	void update(size_t k, const value_type& diff){
		for(;k <= N;k += k & -k){
			tree[k] = tree[k]+diff;
		}
	}

	value_type query(size_t k) const { // sum of [0, k)
		value_type res = value_type();
		for(;k > 0;k -= k & -k){
			res = res + tree[k];
		}
		return res;
	}

	void updatez(size_t k, value_type v){ // 0-indexed
		update(k+1, v);
	}

	size_t search(const std::function<bool(value_type)>& f) const { // Find k :[0, k) -> True, [0, k-1) -> False
		if(f(value_type()))
			return 0;
		if(!f(query(N)))
			return N+1;
		value_type x = value_type();
		size_t k = 1, idx = 0;
		while(k <= N) k <<= 1;
		k >>= 1;
		for(;k > 0;k >>= 1){
			if((idx|k) <= N && !f(tree[idx|k]+x))
				x = x + tree[idx|=k];
		}
		return idx+1;
	}

	size_t size() const {return N;}
};

/*
	Fenwick Tree(Binary Indexed Tree)
	- typename CommutativeMonoid
		可換モノイド

		- 単位元 : デフォルトコンストラクタ
		- 演算   : operator+
*/

/*
verify: http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=3072239#1

int main(void){
	std::cin.tie(0);
	std::ios::sync_with_stdio(false);

	int_fast64_t n, q;
	scanf("%lld%lld", &n, &q);
	BIT<int_fast64_t> bit(n);
	for(int32_t i = 0;i < q;i++){
		int64_t com, x, y;
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
	BIT<int32_t> bit(200001);
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