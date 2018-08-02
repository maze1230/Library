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

template<typename T, typename E>
class Treap{
private:

	using F = function<T(T, T)>;
	using G = function<T(T, E)>;
	using H = function<E(E, E)>;
	using P = function<E(E, int32)>;
	F f;
	G g;
	H h;
	P p;
	T ti;
	E ei;
	struct Node{
		T val, dat;
		E lazy;
		Node* lch, rch;
		int32 cnt;
		double pri;

		Node(T v, double p):val(v), pri(p), cnt(1), dat(v){
			lch = rch = nullptr;
		}
	};

	const int32 LIM = 1e6;
	vector<Node> pool;
	int32 ptr;

public:
	Treap(){}
	Treap(F f, G g, H h, T ti, E ei, P p=[](E a, int32 b){return a;}):
		f(f), g(g), h(h), ti(ti), ei(ei), p(p), pool(LIM), ptr(0){}
	int32 count(Node *t){ return t == nullptr ? 0 : t->cnt; }
	T query(Node *t){ return t == nullptr ? ti : t->dat; }

	Node* eval(Node *t){
		if(t->lazy == ei) return t;
		t->val = g(t->val, p(t->lazy, 1));
		if(t->lch != nullptr){
			t->lch->lazy = h(t->lch->lazy, t->lazy);
			t->lch->val = g(t->lch->val, p(t->lch->lazy, count(t->lch)));
		}
		if(t->rch != nullptr){
			t->rch->lazy = h(t->rch->lazy, t->lazy);
			t->rch->val = g(t->rch->val, p(t->rch->lazy, count(t->rch)));
		}
		t->lazy = ei;
		return update(t);
	}

	Node *update(Node *t){
		t->cnt = count(t->lch)+count(t->rch)+1;
		t->dat = f(f(query(t->lch), query(t->rch)), t->val);
		return t;
	}

	Node *merge(Node *l, Node *r){
		if(l == nullptr || r == nullptr) return l == nullptr ? r : l;

		if(l->pri > r->pri){
			eval(l);
			l->rch = merge(l->rch, r);
			return update(l);
		}else{
			eval(r);
			r->lch = merge(r->lch, l);
			return update(r);
		}
	}

	pair<Node*, Node*> split(Node *t, int32 k){
		if(t == nullptr) return make_pair(nullptr, nullptr);
		eval(t);
		if(k <= count(t->lch)){
			pair<Node*, Node*> s = split(t->lch, k);
			t->lch = s.second;
			return make_pair(s.first, update(t));
		}else{
			pair<Node*, Node*> s = split(t->rch, k-count(t->lch)-1);
			t->rch = s.first;
			return make_pair(update(t), s.second);
		}
	}
	
	Node *insert(Node *t, int32 k, Node *n){
		
	}
};

int main(void){
	cin.tie(0);
	ios::sync_with_stdio(false);
}
