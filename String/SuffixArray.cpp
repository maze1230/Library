#include <bits/stdc++.h>

template<::std::size_t maximum, ::std::size_t max_digit,
	class RandomAccessIterator, class F>
void LSD_RadixSort(RandomAccessIterator first, RandomAccessIterator last,
		const F &get_digit){
	using value_type = typename ::std::iterator_traits<RandomAccessIterator>::value_type;
	using size_type = ::std::size_t;

	const ::std::vector<value_type> cpy(first, last);
	const size_type size = cpy.size();
	::std::array<size_type, maximum+2> count;
	::std::vector<size_type> cur(size), tmp(size);

	::std::iota(cur.begin(), cur.end(), 0);

	for(size_type digit = 0;digit < max_digit;++digit){
		::std::fill(count.begin(), count.end(), 0);
		for(const auto i : cur){
			++count[get_digit(cpy[i], digit) + 1];
		}

		for(size_type i = 1;i < maximum;++i){
			count[i+1] += count[i];
		}

		for(const auto i : cur){
			tmp[count[get_digit(cpy[i], digit)]++] = i;
		}
		::std::swap(cur, tmp);
	}

	for(size_type i = 0;i < size;++i){
		*(first + i) = cpy[cur[i]];
	}
}

template<typename T=::std::string>
class SuffixArray{
private:
	static const int MAX_LENGTH = 500050;

	::std::size_t size;
	T s;
	::std::vector<int> rank, sa, _lcp;
	::std::vector<::std::vector<int>> table; // sparse table to calculate _lcp

	struct log2{
		int v[MAX_LENGTH];
		log2(){
			v[0] = v[1] = 0;
			for(int i = 2;i < MAX_LENGTH;i++){
				v[i] = v[i >> 1]+1;
			}
		}

		int f(int idx){
			return v[idx];
		}
	};

	int k; // for doubling

public:
	SuffixArray(){}
	SuffixArray(T s):s(s), size(s.size()), sa(size+1), rank(size+1), _lcp(size+1, -1){
		build_sa();
	}

	::std::vector<int> build_sa(){
		::std::vector<int> tmp(size+1);
		for(int i = 0;i <= size;i++){
			sa[i] = i;
			rank[i] = i < size ? s[i] : 0;
		}

		auto compare = [&](int i, int j){
			if(rank[i] != rank[j]) return rank[i] < rank[j];
			int rank_i = i + k <= size ? rank[i+k] : -1;
			int rank_j = j + k <= size ? rank[j+k] : -1;
			return rank_i < rank_j;
		};

		for(k = 1;k <= size;k *= 2){
			 // ::std::sort(sa.begin(), sa.end(), compare);
			 LSD_RadixSort<200000, 1>(sa.begin(), sa.end(),
					 [&](int x, int d){return x+k < rank.size() ? rank[x+k] : 0;});
			 LSD_RadixSort<200000, 1>(sa.begin(), sa.end(),
					 [&](int x, int d){return rank[x];});
			
			tmp[sa[0]] = 0;
			for(int i = 1;i <= size;i++){
				tmp[sa[i]] = tmp[sa[i-1]] + (compare(sa[i-1], sa[i]) ? 1 : 0);
			}
			::std::swap(rank, tmp);
		}
		return sa;
	}

	::std::vector<int> build_lcp(){
		int h = 0;

		for(int i = 0;i < size+1;i++){
			if(rank[i] == size){
				h = 0;
				continue;
			}
			for(int j = sa[rank[i]+1]; ::std::max(i, j)+h <= size && s[i+h] == s[j+h] ; h++);
			_lcp[rank[i]] = h;
			if(h > 0) h--;
		}

		table = ::std::vector<::std::vector<int>>(20, ::std::vector<int>(size+1));
		for(int i = 0;i <= size;i++){
			table[0][i] = _lcp[i];
		}

		for(int k = 1;(1 << k) <= size;k++){
			for(int i = 0;i + (1 << k) <= size;i++){
				table[k][i] = ::std::min(table[k-1][i], table[k-1][i+(1 << (k-1))]);
			}
		}

		return _lcp;
	}

	int lcp(int i, int j){ // Longest Common Prefix of s[i..] and s[j..]
		static log2 log2_table;
		i = rank[i];
		j = rank[j];
		if(i > j) ::std::swap(i, j);
		
		int k = log2_table.v[j-i];
		return ::std::min(table[k][i], table[k][j-(1 << k)]);
	}

	int operator[](int k) const{
		return sa[k];
	}

	bool is_upper(T& t, int si){ // True if t > sa[si]
		int ti = 0; si = sa[si];
		while(si < s.size() && ti < t.size()){
			if(t[ti] > s[si]) return true;
			if(t[ti] < s[si]) return false;
			ti++; si++;
		}
		assert(ti >= t.size() || si >= s.size());
		return (ti < t.size() && si >= s.size());
	}

	int lower_bound(T& t){
		int l = -1, r = size+1, m;
		while(r-l > 1){
			m = (l+r) >> 1;
			if(is_upper(t, m))
				l = m;
			else
				r = m;
		}
		return r;
	}

	int upper_bound(T& t){
		int l = -1, r = size+1, m;
		t.back()++;
		while(r-l > 1){
			m = (l+r) >> 1;
			if(is_upper(t, m))
				l = m;
			else
				r = m;
		}
		t.back()--;
		return r;
	}

	::std::pair<int, int> bounds(T& t){
		return ::std::pair<int, int>(lower_bound(t), upper_bound(t));
	}
};

/*

	SuffixArray<Container>(s)
	- SuffixArrayの計算をする
		- 普通のソート
			- O(N log^2 N)
		- 基数ソート
			- O(N log N) 
			- 普通のソートより遅い

	Container v
	- operator[], size(), back()を持つ
	- Container::value_type
		- int型にcastできる必要がある

	
	Member functions
	- SuffixArray<T>(T s)
		- sのSuffixArrayを構築する

	- vector<int> build_lcp()
		- Longest Common Prefixを構築
		- 任意のi, jに対しs[i..)とs[j..)のLCPを求められるようにするためSparse Tableを構築

	- int lcp(int i, int j)
		- s[i..)とs[j..)のLCPを返す

	- int opeartor[](i)
		- 辞書順でi番目となる接尾辞s[k..)のkを返す

	- bool is_upper(T t, si)
		- s[si..) < tのときtrueを返す

	- int lower_bound(T& t)
		- LCP(s[k-1..), t) < |t| ∧ LCP(s[k..), t) == |t|となるkを返す

	- int upper_bound(T& t)
		- LCP(s[k-1..), t) == |t| ∧ LCP(s[k..), t) < |t|となるkを返す

	- pair<int, int> bounds(T& t)
		- (lower_bound(t), upper_bound(t))を返す
*/

/*

verify:http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=3136120#1

template<typename T, typename E>
class SegTree {
private:
    using F = std::function<T(T, T)>;
    using G = std::function<T(T, E)>;
    int n;
    F f;
    G g;
    T ti; // e0:F
    std::vector<T> node;
public:
    SegTree() {}
    SegTree(int _n, F f, G g, T ti) :f(f), g(g), ti(ti) {
        init(_n);
    }
    SegTree(std::vector<T> v, F f, G g, T ti) :f(f), g(g), ti(ti) {
        init(v.size());
        for (int i = 0;i < v.size();i++) node[i + n - 1] = v[i];
        for (int i = n - 2;i >= 0;i--) node[i] = merge(node[i * 2 + 1], node[i * 2 + 2]);
    }
 
    inline void init(int _n) {
        n = 1;
        while (n < _n) n *= 2;
        node.resize(2 * n - 1, ti);
    }
 
    inline T merge(T lhs, T rhs) {
        if (lhs == ti) return rhs;
        else if (rhs == ti) return lhs;
        return f(lhs, rhs);
    }
 
    void update(int k, E x) {
        k += n - 1;
        node[k] = g(node[k], x);
        while (k) {
            k = (k - 1) / 2;
            node[k] = merge(node[k * 2 + 1], node[k * 2 + 2]);
        }
    }
 
    T query(int a, int b, int k = 0, int l = 0, int r = -1) {
        if (r < 0) r = n;
        if (b <= l || r <= a) return ti;
        if (a <= l && r <= b) return node[k];
        return merge(query(a, b, k * 2 + 1, l, (l + r) / 2), query(a, b, k * 2 + 2, (l + r) / 2, r));
    }
};
 
int main(void) {
    using namespace std;
    string s[2];
    cin >> s[0];
    s[1] = s[0];
    reverse(s[1].begin(), s[1].end());
 
    SuffixArray<> SA[2];
    vector<int> sa[2];
    vector<int> rank[2];
    SegTree<int, int> sg[2];
 
    auto F = [](int a, int b) {return min(a, b);};
    auto G = [](int a, int b) {return b;};
 
    for (int i = 0;i < 2;i++) {
        SA[i] = SuffixArray<>(s[i]);
        sa[i] = SA[i].build_sa(); SA[i].build_lcp();
        rank[i].resize(sa[i].size());
 
        for (int j = 0;j < sa[i].size();j++) {
            rank[i][sa[i][j]] = j;
        }
        sg[i] = SegTree<int, int>(sa[i], F, G, numeric_limits<int>::max());
    }
 
    int m;
    cin >> m;
    for (int i = 0;i < m;i++) {
        string a, b;
        cin >> a >> b;
        reverse(b.begin(), b.end());
 
        pair<int, int> x = SA[0].bounds(a), y = SA[1].bounds(b);
        int l = sg[0].query(x.first, x.second);
        int r = s[0].size() - sg[1].query(y.first, y.second) - 1;
        if (x.first < x.second && y.first < y.second && l + a.size() <= r + b.size())
            cout << r - l + 1 << endl;
        else
            cout << 0 << endl;
    }
}
*/
