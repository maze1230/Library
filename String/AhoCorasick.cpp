#include <bits/stdc++.h>

template<class Converter, class SuffixInfo, int NODE_NUM = 1000000>
class AhoCorasick {
public:
  using value_structure = typename Converter::value_structure;
  using size_type = std::uint64_t;
  static constexpr size_type num_of_kinds = Converter::num_of_kinds;

  using value_type = typename SuffixInfo::value_type;
  using merged_value_type = typename SuffixInfo::merged_value_type;

  struct Node {
    size_type size, end, len, faillink;
    value_type val;
    merged_value_type suf_info;
    std::array<int, num_of_kinds> ch;
    Node () : size(0), end(0), len(0), val(SuffixInfo::identity()),
              suf_info(SuffixInfo::midentity()), faillink(0) { ch.fill(-1); }
  };

private:
  std::vector<Node> node;

public:
  AhoCorasick() {
    node.reserve(NODE_NUM);
    node.push_back(Node());
  }

  void insert(const value_structure& v, size_type num = 1, value_type val = value_type(), size_type k = 0, size_type idx = 0) {
    node[idx].size += num;
    if (k == v.size()) {
      node[idx].end += num;
      node[idx].val = SuffixInfo::operation(node[idx].val, val);
      return;
    }
    size_type nxt = Converter::convert(v[k]);
    if (node[idx].ch[nxt] == -1) {
      node.push_back(Node());
      node[idx].ch[nxt] = node.size()-1;
      node.back().len = k+1;
    }
    insert(v, num, val, k+1, node[idx].ch[nxt]);
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

  size_type proceed(size_type k, size_type c, bool need_convert = 1) {
    if (need_convert) c = Converter::convert(c);
    while (node[k].ch[c] == -1)  k = node[k].faillink;
    return node[k].ch[c];
  }

  void build() {
    std::queue<size_type> que;
    for (int i = 0; i < num_of_kinds; i++) {
      if (node[0].ch[i] == -1) node[0].ch[i] = 0;
      else {
        que.push(node[0].ch[i]);
        SuffixInfo::merge(node[node[0].ch[i]].suf_info, node[node[0].ch[i]].val);
      }
    }
    while (que.size()) {
      int k = que.front();
      que.pop();
      for (int i = 0; i < num_of_kinds; i++) {
        if (node[k].ch[i] == -1) continue;
        size_type nx = node[k].ch[i];
        node[nx].faillink = proceed(node[k].faillink, i, false);

        SuffixInfo::merge(node[nx].suf_info, node[nx].val);
        SuffixInfo::merge(node[nx].suf_info, node[node[nx].faillink].suf_info);
        que.push(nx);
      }
    }
  }

  const Node& operator[](size_type k) {
    return node[k];
  }
};

class Converter {
public:
  using value_structure = std::string;
  using value_type = typename value_structure::value_type;
  static constexpr std::size_t num_of_kinds = 26;
  static std::size_t convert(const value_type& v) {
  }
};

class SuffixInfo {
public:
  using value_type = int;
  using merged_value_type = int;
  static value_type identity() { return 0; }
  static merged_value_type midentity() { return 0; }
  static value_type operation(const value_type& a, const value_type& b) {
    return a | b;
  }
  static void merge(merged_value_type& a, const merged_value_type& b) {
    a |= b;
  }
//  static void merge(merged_value_type& a, const value_type& b) {
//    a |= b;
//  }
};

/*
verify: https://tenka1-2016-final-open.contest.atcoder.jp/submissions/8400970
        http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=3986517#1

template<class Converter, class SuffixInfo>
class AhoCorasick

Converter:
	- 要求
		- value_structure: std::string, std::vectorなどの列構造
			- size, operator[] が必要

		- static constexpr std::size_t num_of_kinds
			- 現れる値の種類数

		- static convert(value_type) -> size_t
			- 値を[0, num_of_kinds)に変換する

SuffixInfo:
  - 要求
    - value_type: それぞれの文字列が持つ値の型

    - merged_value_type: suffixが一致する要素の情報をまとめたもの

    - identity -> value_type
      - value_typeの単位元（初期値）

    - midentity -> merged_value_type
      - merged_value_typeの初期値（単位元とは？？？）

    - operation(const value_type&, const value_type&)

    - merge(merged_value_type&, const value_type&)
    - merge(merged_value_type&, const merged_value_type&)


AhoCorasick
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

    - build

    - proceed(k, c)
      - 今いる頂点がkのとき、次の文字がcのときに進む頂点を返す
 */
