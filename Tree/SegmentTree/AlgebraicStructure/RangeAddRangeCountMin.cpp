namespace range_add_range_count_min {
  class v_monoid {
  public:
    using value_type = pair<int64_t, int>; // min, cnt
    static const value_type identity() { return value_type(INF, 0); }
    static const value_type initializer() { return value_type(0, 1); }
    static const value_type operation(const value_type& a, const value_type& b) {
      if (a.first == b.first) {
        return value_type(a.first, a.second + b.second);
      } else {
        return min(a, b);
      }
    }
  };

  class o_monoid {
  public:
    using value_type = int64_t;
    static const value_type identity() { return 0; }
    static const value_type operation(const value_type& a, const value_type& b) {
      return a + b;
    }
  };

  class modifier {
  public:
    using value_type = v_monoid::value_type;
    using op_type = o_monoid::value_type;

    static value_type operation(const value_type& a, const op_type& b) {
      return value_type(a.first + b, a.second);
    }
  };
}
