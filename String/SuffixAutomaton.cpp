#include <bits/stdc++.h>

template<int NODE_NUM=1000000>
class SuffixAutomaton {
  using value_structure = std::string;
  using value_type = char;
  using size_type = std::int32_t;

  struct Node {
    size_type link, len, pre, original;
    value_type key;
    std::map<value_type, size_type> next;
    Node(value_type key=-1, size_type len=0, size_type link=-1, size_type original=-1)
      : key(key), link(link), len(len), pre(1), original(original) {}

    void dump() {
      std::cout << "(key, len, link, pre cnt): (" << key << ", " << len << ", " << link << ", " << pre << ")" << std::endl;
      for (auto&& u : next) {
        std::cout << "(char, id): (" << u.first << ", " << u.second << ")" << std::endl;
      }
    }
  };

private:
  size_type last, len;

public:
  std::vector<Node> node;

  SuffixAutomaton(): last(0) {
    node.reserve(NODE_NUM);
    node.push_back(Node());
  }

  SuffixAutomaton(const value_structure& s): SuffixAutomaton() {
    extend_by_string(s);
  }

  void extend(const value_type c, size_type base=-1) {
    if (base >= 0) last = base;
    size_type cur, p = last;
    if (node[last].next.count(c) == 0) {
      node.push_back(Node(c, node[last].len + 1));
      cur = node.size() - 1;
    } else {
      cur = node[last].next[c];
    }
    last = cur;

    for (; p != -1 && node[p].next.count(c) == 0; p = node[p].link) node[p].next[c] = cur;

    if (p == -1) {
      node[cur].link = 0;
    } else {
      size_type q = node[p].next[c];
      if (node[p].len + 1 == node[q].len) {
        if (q != cur)
          node[cur].link = q;
        else
          node[cur].pre++;
      } else {
        size_type clone = node.size();
        node.push_back(node[q]);
        node[clone].len = node[p].len + 1;
        node[clone].original = q;

        node[clone].pre = (q == cur); // 新しい文字列を追加する際にcloneが必要になることがあるが、このときはprefixになっている
        if (q == cur) last = clone;

        for (; p != -1 && node[p].next[c] == q; p = node[p].link) node[p].next[c] = clone;
        node[q].link = node[cur].link = clone;
      }
    }
  }

  void extend_by_string(const value_structure& s, size_type base=-1) {
    for (int i = 0; i < s.size(); i++) {
      extend(s[i], (i == 0 ? base : -1));
    }
  }

  void topological_sort() {
    std::vector<size_type> indeg(node.size(), 0), ord(node.size());
    std::vector<Node> node2(node.size());
    size_type p = 1;

    for (auto&& v : node) {
      for (auto&& u : v.next) {
        indeg[u.second]++;
      }
    }
    node2[0] = node[0]; ord[0] = 0;
    for (int i = 0; i < node.size(); i++) {
      for (auto &&u : node2[i].next) {
        if (--indeg[u.second] == 0) {
          ord[u.second] = p;
          node2[p++] = node[u.second];
        }
      }
    }

    for (auto &&v : node2) {
      v.link = v.link == -1 ? -1 : ord[v.link];
      v.original = ord[v.original];
      for (auto &&u : v.next) {
        u.second = ord[u.second];
      }
    }
    swap(node, node2);
  }

  Node& operator[](size_type k) {
    return node[k];
  }

  const size_type size() {
    return node.size();
  }

  void dump() {
    std::cout << "---------------------------------------------------------" << std::endl;
    for (int i = 0; i < node.size(); i++) {
      std::cout << "id: " << i << std::endl;
      node[i].dump();
    }
    std::cout << "---------------------------------------------------------" << std::endl;
  }
};

/*
verify: https://codeforces.com/contest/616/submission/89088308

template<NODE_NUM>
class SuffixAutomaton

SuffixAutomaton
  - 文字の種類をCとする

	- 提供
		- Node
			- len
				- その頂点に属する文字列でもっとも長いものの長さ

			- link
				- SuffixLinkが指す頂点の番号(頂点0は-1を指している)

			- key
				- その頂点へ遷移する文字

			- next
				- map<key, int>で、ある文字で遷移する頂点の番号

      - pre
        - そのノードにprefixが受理される文字列がいくつあるか

      - dump()
        - 頂点の情報を標準出力へ出力する

    - len
      - SuffixAutomatonに追加した文字列の長さ
      - sizeとは違い、頂点数ではない

		- extend(value_type c, base=-1)
		  - O(log C)
			- 現在のSuffixAutomatonの末尾(baseが-1なら末尾、その他ならその番号の頂点)にcを追加する

		- extend_by_string(value_structure s, base=-1)
			- O(|s| log C)
			- 現在のSuffixAutomatonの末尾(baseが-1なら末尾、その他ならその番号の頂点)にsを追加する
			- extendを1文字ずつやるのがめんどくさいとき用

		- topological_sort()
			- O(|v| log C)
			- 頂点の番号がトポロジカルになるように並べる
			- DFSしなくても小さい順に見るだけで良くなる

    - size() -> size_type
      - 頂点の数を返す

    - dump()
      - 現在の状態を出力する
 */
