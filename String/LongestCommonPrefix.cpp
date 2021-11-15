#include <bits/stdc++.h>

class SA_IS {
private:
  std::string str;
  std::vector<int> sa; 
  bool built = false;

  static inline bool is_lms(const std::vector<int>& is_l, size_t idx) {
    return idx > 0 && is_l[idx-1] && !is_l[idx];
  }

  void create_bin(const std::vector<int>& v, std::vector<int>& bin) {
    std::fill(bin.begin(), bin.end(), 0);
    for (auto& x : v) bin[x+1]++;
    for (int i = 1; i < bin.size(); i++) bin[i] += bin[i-1];
  }

  void set_lms(const std::vector<int>& v, std::vector<int>& sa, std::vector<int>& bin, const std::vector<int>& is_l) {
    for (int i = 0; i < v.size(); i++) {
      if (is_lms(is_l, i)) { sa[--bin[v[i]+1]] = i;}
    }
    for (int i = 0; i < v.size(); i++) {
      if (is_lms(is_l, i)) { bin[v[i]+1]++; }
    }
  }

  void induced_sort(const std::vector<int>& v, std::vector<int>& sa, size_t kind_num, std::vector<int>& bin, const std::vector<int>& is_l) {
    std::vector<int> cnt(kind_num+2, 0);
    for (int i = 0; i < v.size(); i++) {
      if (sa[i] <= 0 || !is_l[sa[i]-1]) continue;
      int kind = v[sa[i]-1];
      sa[bin[kind] + cnt[kind]++] = sa[i]-1;
    }
  }

  void reverse_induced_sort(const std::vector<int>& v, std::vector<int>& sa, std::vector<int>& bin, const std::vector<int>& is_l) {
    for (int i = v.size()-1; i > 0; i--) {
      if (sa[i] <= 0 || is_l[sa[i]-1]) continue;
      int kind = v[sa[i]-1];
      sa[--bin[kind+1]] = sa[i]-1;
    }
  }
public:
  SA_IS() : built(false) {}
  SA_IS(const std::string &str_) : str(str_) { build(); }
  SA_IS(std::string &&str_) : str(str_) { build(); }

  std::vector<int> build(const std::vector<int>& v, int kinds) {
    if (v.size() == 1) return std::vector<int>(1, 0);
    std::vector<int> bin(kinds+2, 0), is_l(v.size(), 0);
    std::vector<int> sa(v.size(), -1);

    for (int i = v.size()-2; i >= 0; i--) {
      is_l[i] = v[i] == v[i+1] ? is_l[i+1] : v[i] > v[i+1];
    }

    create_bin(v, bin);
    set_lms(v, sa, bin, is_l);
    induced_sort(v, sa, kinds, bin, is_l);
    reverse_induced_sort(v, sa, bin, is_l);

    std::vector<int> order(v.size());
    int cur = 0, prev = -1;
    for (int i = 0; i < v.size(); i++) if (is_lms(is_l, i)) order[i] = cur++;

    std::vector<int> next_v(cur);
    cur = -1;
    for (int i = 0; i < v.size(); i++) {
      if (!is_lms(is_l, sa[i])) continue;
      bool diff = false;
      for (int d = 0; d < v.size(); d++) {
        if (prev == -1 || v[sa[i]+d] != v[prev+d] || is_l[sa[i]+d] != is_l[prev+d]) {
          diff = true;
          break;
        }
        if (d > 0 && is_lms(is_l, sa[i]+d)) break;
      }
      if (diff) { cur++; prev = sa[i]; }
      next_v[order[sa[i]]] = cur;
    }

    std::vector<int> re_order(next_v.size());
    std::vector<int> next_sa = build(next_v, cur);
    std::fill(sa.begin(), sa.end(), -1);
    for (int i = 0; i < v.size(); i++) if (is_lms(is_l, i)) re_order[order[i]] = i;
    create_bin(v, bin);
    for (int i = next_sa.size()-1; i >= 0; i--) sa[--bin[v[re_order[next_sa[i]]]+1]] = re_order[next_sa[i]];
    create_bin(v, bin);
    induced_sort(v, sa, kinds, bin, is_l);
    reverse_induced_sort(v, sa, bin, is_l);

    return sa;
  }

  void build() {
    str += '$';
    std::vector<int> v(str.size());
    for (int i = 0; i < str.size(); i++) v[i] = str[i];
    sa = build(v, *std::max_element(v.begin(), v.end()));
    sa = std::vector<int>(sa.begin()+1, sa.end());
    built = true;
    str = str.substr(0, str.size()-1);
  }

  const std::vector<int>& get_sa() {
    assert(built);
    return sa;
  }

  bool is_upper(const std::string& t, int s_idx) { // t > sa[si]
    int idx = 0; s_idx = sa[s_idx];
    while (s_idx+idx < sa.size() && idx < t.size()) {
      if (t[idx] > str[s_idx+idx]) return true;
      if (t[idx] < str[s_idx+idx]) return false;
      idx++;
    }
    return (idx < t.size() && s_idx + idx >= str.size());
  }

  int lower_bound(const std::string& t) {
    int l = -1, r = sa.size(), m;
    while (r - l > 1) {
      m = (l + r) >> 1;
      if (is_upper(t, m)) l = m;
      else r = m;
    }
    return r;
  }

  ::std::pair<int, int> bounds(std::string& t) {
    int l = lower_bound(t); t.back()++;
    int r = lower_bound(t); t.back()--;
    return ::std::pair<int, int>(l, r);
  }

  const int& operator[](const size_t& k) {
    return sa[k];
  }

  const size_t size() { return str.size()-1; }
};

template<class MinDataStructure = std::vector<int>>
class LCP {
private:
  std::vector<int> lcp_, rank, sa;
  std::string str;
  MinDataStructure ds;

public:
  LCP() {}
  LCP(const std::string& str_, const std::vector<int>& sa_) : str(str_), sa(sa_) {
    build();
  }

  void build() {
    rank = std::vector<int>(str.size());
    lcp_ = std::vector<int>(sa.size());

    for (int i = 0; i < str.size(); i++) rank[sa[i]] = i;
    int h = 0;
    for (int i = 0; i < str.size(); i++) {
      if (rank[i] == sa.size()-1) continue;

      for (int j = sa[rank[i]+1]; i < str.size() && j < str.size() && str[i+h] == str[j+h]; h++);
      lcp_[rank[i]] = h;
      if (h > 0) h--;
    }

    ds = MinDataStructure(lcp_);
  }

  std::vector<int> get_lcp_vector() {
    return lcp_;
  }

  int lcp(int idx1, int idx2) { // lcp of A[idx1..] and A[idx2..]
    assert(0 <= idx1 && idx1 < rank.size() && 0 <= idx2 && idx2 < rank.size());
    idx1 = rank[idx1]; idx2 = rank[idx2];
    if (idx1 > idx2) std::swap(idx1, idx2);
    return ds.query(idx1, idx2);
  }

  bool is_upper(const std::string& t, int s_idx, int& k) { // t > A[s_idx..]
    s_idx = sa[s_idx];
    while (k < t.size() && s_idx+k < str.size() && t[k] == str[s_idx+k]) k++;
    if (t.size() <= k) return false;
    if (str.size() <= s_idx+k) return true;
    return t[k] > str[s_idx+k];
  }

  int lower_bound(const std::string& t) {
    int l = -1, r = lcp_.size(), m_prev = (l + r) >> 1, m;
    int k = 0;
    (is_upper(t, m_prev, k) ? l : r) = m_prev;
    while (r - l > 1) {
      m = (l + r) >> 1;
      int k2 = lcp(sa[m_prev], sa[m]);
      if (k != k2) {
        if ((m_prev < m) ^ (k < k2)) r = m;
        else l = m;
      } else {
        if (is_upper(t, m, k)) l = m;
        else r = m;
        m_prev = m;
      }
    }
    return r;
  }

  std::pair<int, int> bounds(std::string& t) {
    int l = lower_bound(t); t.back()++;
    int r = lower_bound(t); t.back()--;
    return std::make_pair(l, r);
  }

  const int operator[](int k) {
    return lcp_[k];
  }
};

/*

MinDataStructure
  - 要求
    - constructor(std::vector<int>)

    - query(size_t l, size_t r) -> int
      - [l, r)の最小値

LongestCommonPrefix
  - 提供
    - constructor(string, suffix array)
      - stringとそのsuffix arrayからLCPを計算する
      - buildを呼び出す

    - build()
      - constructorで呼び出される
      - LCPを計算する

    - get_lcp_vector() -> std::vector<int>
      - 計算したLCPのvectorを返す

    - lcp(size_type l, size_type r) -> int
      - 計算量: MinDataStructureのqueryと同じ
      - str[l..]とstr[r..]のLCPを計算する

    - bounds(std::string t) -> pair<int, int>
      - 計算量: O(|t| log |s|)
      - SA[l, r)のprefixにstringが含まれるような(l, r)を返す
*/