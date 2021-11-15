#include <bits/stdc++.h>

template<class Converter, int NODE_NUM = 1000000>
class Trie {
public:
	using value_structure = typename Converter::value_structure;
	using size_type = std::uint64_t;
	static constexpr size_type num_of_kinds = Converter::num_of_kinds;

	struct Node {
		size_type size, end, len;
		std::array<int, num_of_kinds> ch;
		Node () : size(0), end(0), len(0) { ch.fill(-1); }
	};

private:
	std::vector<Node> node;

public:
	Trie() {
		node.reserve(NODE_NUM);
		node.push_back(Node());
	}

	void insert(const value_structure& v, size_type num = 1, size_type k = 0, size_type idx = 0) {
		node[idx].size += num;
		if (k == v.size()) {
			node[idx].end += num;
			return;
		}
		size_type nxt = Converter::convert(v[k]);
		if (node[idx].ch[nxt] == -1) {
			node.push_back(Node());
			node[idx].ch[nxt] = node.size()-1;
			node.back().len = k+1;
		}
		insert(v, num, k+1, node[idx].ch[nxt]);
	}

	size_type count_prefix(const value_structure& v, size_type k = 0, size_type idx = 0) {
		if (v.size() == k) return node[idx].size;
		size_type nxt = Converter::convert(v[k]);
		if (node[idx].ch[nxt] == -1) return 0;
		return count_prefix(v, k+1, node[idx].ch[nxt]);
	}

	size_type count(const value_structure& v, size_type k = 0, size_type idx = 0) {
		if (v.size() == k) return node[idx].end;
		size_type nxt = Converter::convert(v[k]);
		if (node[idx].ch[nxt] == -1) return 0;
		return count(v, k+1, node[idx].ch[nxt]);
	}

	template<typename F>
	void query(const value_structure& v, const F& f, size_type k = 0, size_type idx = 0) {
		if (node[idx].size > 0) f(node[idx]);
		if (v.size() == k) return;
		size_type nxt = Converter::convert(v[k]);
		if (node[idx].ch[nxt] == -1) return;
		query(v, f, k+1, node[idx].ch[nxt]);
	}
};

/*
verify: https://tenka1-2016-final-open.contest.atcoder.jp/submissions/5899740

template<class Converter>
class Trie

Converter:
	- 要求
		- value_structure: std::string, std::vectorなどの列構造
			- size, operator[] が必要

		- static constexpr std::size_t num_of_kinds
			- 現れる値の種類数

		- static convert(value_type) -> size_t
			- 値を[0, num_of_kinds)に変換する

Trie
	- 提供
		- Node
			- size
				- 部分木に単語がいくつあるか

			- end
				- その頂点で終わる単語がいくつあるか

			- len
				- その頂点までで何文字あるか
			
			- ch[num_of_kinds]
				- それぞれの子のpoolでのindex(存在しなければ-1)

		- insert(value_structure v, num, k)
			- O(|v|)
			- num個のv[k..)を挿入する

		- count_prefix(value_structure v, k)
			- O(|v|)
			- v[k..)をprefixとして含むものがいくつあるか返す

		- count(value_structure v, k)
			- O(|v|)
			- v[k..)がいくつあるか返す

		- query(value_structure v, F f, k)
			- O(|v|)
			- v[k..)のprefixそれぞれについてf(node)を呼び出す
*/