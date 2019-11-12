#include <bits/stdc++.h>

template<class ValueMonoid, class OperatorMonoid, class Modifier>
class DynamicLazySegTree{
private:
	using value_structure = ValueMonoid;
	using value_type = typename value_structure::value_type;
	using operator_structure = OperatorMonoid;
	using operator_type = typename operator_structure::value_type;
	using modifier = Modifier;
	using size_type = ::std::size_t;

	struct Node;
	using pointer = ::std::unique_ptr<Node>;
	using pointer_value = ::std::pair<pointer, value_type>;

	struct Node{
		value_type v;
		operator_type o;
		pointer lch, rch;
		Node(){}
		Node(value_type a, operator_type b) : v(a), o(b) {}
	};
	pointer root;
	size_type n;

	inline static operator_type apply(const operator_type &a, const operator_type &b) {
		return operator_structure::operation(a, b);
	}

	inline static value_type reflect(const value_type &a, const operator_type &b){
		if (b == operator_structure::identity()) return a;
		return modifier::operation(a, b);
	}

	inline static pointer push(pointer node, size_type k){
		if (node->o == operator_structure::identity()) return ::std::move(node);
		node->v = reflect(node->v, node->o);
		if (k > 1) {
			if (!node->lch) node->lch = ::std::make_unique<Node>(Node(value_structure::identity(), operator_structure::identity()));
			if (!node->rch) node->rch = ::std::make_unique<Node>(Node(value_structure::identity(), operator_structure::identity()));
			node->lch->o = apply(node->lch->o, node->o);
			node->rch->o = apply(node->rch->o, node->o);
		}
		node->o = operator_structure::identity();

		return ::std::move(node);
	}

public:
	DynamicLazySegTree(){}
	DynamicLazySegTree(size_type n_) : n(n_) {
		root = ::std::make_unique<Node>(value_structure::identity(), operator_structure::identity());
	}

	template<class F>
	void update(size_type k, const F &f){
		root = update(k, f, ::std::move(root), 0, n);
	}

	template<class F>
	pointer update(size_type k, const F &f, pointer now, size_type l = 0, size_type r = -1){
		now = push(::std::move(now), r-l);

		if (r - l == 1) {
			now->v = f(::std::move(now->v));
			return ::std::move(now);
		}

		size_type m = (l + r) >> 1;
		if (k < m) {
			if (!now->lch) now->lch = ::std::make_unique<Node>(value_structure::identity(), operator_structure::identity());;
			now->lch = update(k, f, ::std::move(now->lch), l, m);
		} else {
			if (!now->rch) now->rch = ::std::make_unique<Node>(value_structure::identity(), operator_structure::identity()); now->rch = update(k, f, ::std::move(now->rch), m, r);
		}
		value_type lv = now->lch ? now->lch->v : value_structure::identity();
		value_type rv = now->rch ? now->rch->v : value_structure::identity();
		now->v = value_structure::operation(lv, rv);
		return ::std::move(now);
	}

	void update(size_type a, size_type b, const operator_type &x){
		root = update(a, b, x, ::std::move(root), 0, n);
	}

	pointer update(size_type a, size_type b, const operator_type &x, pointer now, size_type l = 0, size_type r = -1){
		now = push(::std::move(now), r-l);

		if (a <= l && r <= b) {
			now->o = apply(now->o, x);
			now = push(::std::move(now), r-l);
			return ::std::move(now);
		}
		if (b <= l || r <= a) return ::std::move(now);

		size_type m = (l + r) >> 1;
		if (!now->lch) now->lch = ::std::make_unique<Node>(value_structure::identity(), operator_structure::identity());
		if (!now->rch) now->rch = ::std::make_unique<Node>(value_structure::identity(), operator_structure::identity());

		now->lch = update(a, b, x, ::std::move(now->lch), l, m);
		now->rch = update(a, b, x, ::std::move(now->rch), m, r);

		now->v = value_structure::operation(now->lch->v, now->rch->v);

		return ::std::move(now);
	}

	value_type query(size_type a, size_type b){
		value_type res;
		tie(root, res) = query(a, b, ::std::move(root), 0, n);
		return res;
	}

	pointer_value query(size_type a, size_type b, pointer now, size_type l = 0, size_type r = -1){
		now = push(::std::move(now), r-l);

		if (a <= l && r <= b) return ::std::make_pair(::std::move(now), now->v);
		if (r <= a || b <= l) return ::std::make_pair(::std::move(now), value_structure::identity());

		size_type m = (l + r) >> 1;

		value_type lv = value_structure::identity(), rv = value_structure::identity();
		if (now->lch) tie(now->lch, lv) = query(a, b, ::std::move(now->lch), l, m);
		if (now->rch) tie(now->rch, rv) = query(a, b, ::std::move(now->rch), m, r);

		return ::std::make_pair(::std::move(now), value_structure::operation(lv, rv));
	}
};
