#include <bits/stdc++.h>

template<int DIGITS = 30, int NODE_NUM = 1000000>
class BinaryTrie {
public:
  using value_type = std::uint64_t;
  using size_type = std::uint64_t;

  struct Node {
    bool v;
    size_type ch[2], size;
    Node () : v(0), size(0) {
      ch[0] = ch[1] = -1;
    }
  };
private:
  std::vector<Node> node;
  size_type size_;;

public:
  BinaryTrie() {
    node.reserve(NODE_NUM);
    node.push_back(Node());
  }

  inline static size_type child_key(const value_type& v, size_type k) {
    return (v >> (DIGITS - k - 1)) & 1;
  }

  void insert(const value_type& v, size_type num = 1, size_type k = 0, size_type idx = 0) {
    node[idx].size += num;
    if (k == DIGITS) { size_ += num; return; }
    size_type nxt = child_key(v, k);
    if (node[idx].ch[nxt] == -1) {
      node[idx].ch[nxt] = node.size();
      node.push_back(Node());
    }
    insert(v, num, k+1, node[idx].ch[nxt]);
  }

  bool erase(const value_type& v, size_type num = 1, size_type k = 0, size_type idx = 0) {
    if (k == DIGITS) {
      node[idx].size() -= num;
      size_ -= num;
      return true;
    }
    size_type nxt = child_key(v, k);
    if (node[idx].ch[nxt] == -1) {
      return false;
    } else if (erase(v, num, k+1, node[idx].ch[nxt])) {
      node[idx].size() -= num;
    }
  }

  const size_type size() { return size_; }

  size_type count(const value_type& v, size_type k = 0, size_type idx = 0) {
    if (k == DIGITS) return node[idx].size;
    size_type nxt = (v >> (DIGITS - k - 1)) & 1;
    if (node[idx].ch[nxt] == -1) return 0;
    return count(v, k+1, node[idx].ch[nxt]);
  }
  
  value_type min_element(const value_type& x = 0, value_type now = 0, size_type k = 0, size_type idx = 0) { // v xor xが最小となるようなv xor xを返す
    if (k == DIGITS)  return now;
    size_type nxt = child_key(x, k);
    if (node[idx].ch[nxt] != -1) {
      return min_element(x, now, k+1, node[idx].ch[nxt]);
    } else if (node[idx].ch[nxt ^ 1] != -1) {
      return min_element(x, now | (1 << (DIGITS - k - 1)), k+1, node[idx].ch[nxt ^ 1]);
    } else {
      return -1;
    }
  }

  value_type max_element(const value_type& x = 0) { // v xor vが最大となるようなv xor xを返す

  }
};