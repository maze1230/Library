#include <bits/stdc++.h>

class manage_intervals {
private:
  using Interval = std::pair<int64_t, int64_t>;

  std::set<Interval> ivals;

  static Interval merge(Interval a, Interval b) {
    assert(a.first <= a.second);
    assert(b.first <= b.second);

    if (!intersect(a, b)) return Interval(-1, 0);
    return Interval(std::min(a.first, b.first), std::max(a.second, b.second));
  }

  static bool intersect(Interval a, Interval b) {
    assert(a.first <= a.second);
    assert(b.first <= b.second);

    // [1, 2)と[2, 5)は交わっているとする
    if (a.first > b.first) swap(a, b);
    return b.first <= a.second;
  }

  static bool _contains(Interval a, int64_t pos, bool right=false) {
    // right==trueであれば、[1, 5)は5を含むことになる
    if (a.first <= pos && pos < a.second) return true;
    else if (a.second == pos && right) return true;
    else return false;
  }

  static bool _contains(Interval a, Interval b) {
    return a.first <= b.first && b.second <= a.second;
  }
public:
  manage_intervals() {
    ivals.insert(Interval(std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::min()));
    ivals.insert(Interval(std::numeric_limits<int64_t>::min() + 1, std::numeric_limits<int64_t>::min() + 1));
    ivals.insert(Interval(std::numeric_limits<int64_t>::max(), std::numeric_limits<int64_t>::max()));
  }

  void insert(int64_t l, int64_t r) {
    assert(l <= r);
    Interval ival(l, r);

    auto itr = ivals.lower_bound(Interval(l, l));
    while (intersect(ival, *itr)) {
      ival = merge(ival, *itr);
      itr = ivals.erase(itr);
    }
    itr = prev(itr);
    while (intersect(ival, *itr)) {
      ival = merge(ival, *itr);
      itr = prev(ivals.erase(itr));
    }
    ivals.insert(ival);
  }

  void insert(const Interval& ival) {
    insert(ival.first, ival.second);
  }

  void erase(int64_t l, int64_t r) {
    // すべて半開区間で扱うので1点のみ取り除くことはできない
    assert(l < r);
    Interval ival(l, r);

    auto itr = ivals.lower_bound(Interval(l, l));
    while (intersect(ival, *itr)) {
      Interval ival2 = *itr;
      itr = ivals.erase(itr);
      if (!_contains(ival, ival2)) {
        insert(ival.second, ival2.second);
      }
    }
    itr = prev(ivals.lower_bound(Interval(l, l)));
    while (intersect(ival, *itr)) {
      Interval ival2 = *itr;
      itr = prev(ivals.erase(itr));
      if (!_contains(ival, ival2)) {
        insert(ival2.first, ival.first);
        if (ival.second < ival2.second) {
          insert(ival.second, ival2.second);
        }
      }
    }
  }

  void erase(const Interval& ival) {
    erase(ival.first, ival.second);
  }

  bool contains(int64_t pos) {
    auto itr = ivals.lower_bound(Interval(pos, pos));
    return _contains(*itr, pos, true) || _contains(*prev(itr), pos, true);
  }

  void dump() {
    std::cout << "ivals = {" << std::endl;
    for (auto &ival : ivals) {
      std::cout << "  [" << ival.first << ", " << ival.second << ") " << std::endl;
    }
    std::cout << "}" << std::endl;
  }
};

/*
verify:

class manage_intervals

Features:
 区間の追加、削除を行うことができる
 [1, 5), [5, 10)と追加されたら[1, 10]にまとめる
 [1, 10)から[2, 5)が削除されたら[1, 2), [5, 10)に分割する

manage_intervals
	- 提供
		- insert(l, r) or insert(Interval)
			- 償却計算量 O(log N)
			- [l, r)を追加する

    - erase(l, r) or erase(Interval)
			- 償却計算量 O(log N)
			- [l, r)に重なっている部分を削除する

		- contains(pos)
			- 計算量 O(log N)
			- posを内包する区間があるかを返す

		* 未実装
		- search(function f) -> size_type
			- 計算量 O(log N)?
			- f([0, k)) is true and f([0, k+1)) is falseとなるkを返す
*/

int main(void) {
  manage_intervals ivals;

  ivals.dump();
  ivals.insert(10, 20);
  ivals.dump();
  ivals.insert(21, 22);
  ivals.dump();
  ivals.insert(20, 21);
  ivals.dump();
  ivals.insert(0, 5);
  ivals.dump();
  ivals.insert(-10, 11);
  ivals.dump();
  ivals.erase(-2, 3);
  ivals.dump();
  ivals.erase(5, 10);
  ivals.dump();
  ivals.erase(-8, 13);
  ivals.dump();
  std::cout << ivals.contains(-12) << std::endl;
  std::cout << ivals.contains(-5) << std::endl;
  std::cout << ivals.contains(-8) << std::endl;
  std::cout << ivals.contains(12) << std::endl;
}