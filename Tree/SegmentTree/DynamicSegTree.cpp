#include <bits/stdc++.h>

template<class ValueMonoid>
class DynamicSegTree{
private:
	using value_structure = ValueMonoid;
	using value_type = typename value_structure::value_type;
	using size_type = ::std::size_t;
	struct Node;
	using pointer = ::std::unique_ptr<Node>;
	using pointer_value = ::std::pair<pointer, value_type>;

	struct Node{
		value_type v;
		pointer lch, rch;
		Node(){}
		Node(value_type a) : v(a) {}
	};
	pointer root;
	size_type n;
public:
	DynamicSegTree(){}
	DynamicSegTree(size_type n_) : n(n_) {
		root.reset(new Node(value_structure::identity()));
	}

	template<class F>
	void update(size_type k, const F &f){
		root = update(k, f, ::std::move(root));
	}

	template<class F>
	pointer update(size_type k, const F &f, pointer now, size_type l = 0, size_type r = -1){
		if (r < 0) { r = n; }
		if (r - l == 1) {
			now->v = f(::std::move(now->v));
			return ::std::move(now);
		}

		size_type m = (l + r) >> 1;
		if (k < m) {
			if (!now->lch) now->lch.reset(new Node(value_structure::identity()));
			now->lch = update(k, f, ::std::move(now->lch), l, m);
		} else {
			if (!now->rch) now->rch.reset(new Node(value_structure::identity()));
			now->rch = update(k, f, ::std::move(now->rch), m, r);
		}
		value_type lv = now->lch ? now->lch->v : value_structure::identity();
		value_type rv = now->rch ? now->rch->v : value_structure::identity();
		now->v = value_structure::operation(lv, rv);
		return ::std::move(now);
	}

	value_type query(size_type a, size_type b){
		value_type res;
		tie(root, res) = query(a, b, ::std::move(root));
		return res;
	}

	pointer_value query(size_type a, size_type b, pointer now, size_type l = 0, size_type r = -1){
		if (r < 0) { r = n; }
		if (a <= l && r <= b) return ::std::make_pair(::std::move(now), now->v);
		if (r <= a || b <= l) return ::std::make_pair(::std::move(now), value_structure::identity());

		size_type m = (l + r) >> 1;

		value_type lv = value_structure::identity(), rv = value_structure::identity();
		if (now->lch)
			tie(now->lch, lv) = query(a, b, ::std::move(now->lch), l, m);
		if (now->rch)
			tie(now->rch, rv) = query(a, b, ::std::move(now->rch), m, r);

		return ::std::make_pair(::std::move(now), value_structure::operation(lv, rv));
	}
};

/*

verify: https://arc008.contest.atcoder.jp/submissions/4145634

template<ValueMonoid>
class DynamicSegTree

ValueMonoid
	-	要求
		- value_type
		- identity() -> value_type : 単位元を返す
		- operation(value_type, value_type) -> value_type : 演算結果を返す

SegTree
	- 提供
		- query(size_type l, size_type r) -> value_type
			- 計算量 O(log N)
			- [l, r)までの計算結果

		- update(size_type k, function f)
			- 計算量 O(log N)
			- kth elementをfを適用した結果に変更する

*/


int main(void){
}
