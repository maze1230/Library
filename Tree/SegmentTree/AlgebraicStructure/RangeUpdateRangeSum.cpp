namespace range_update_range_sum {
  class v_monoid {
  public:
    using value_type = PLL;
    static const value_type identity() { return PLL(0, 1); }
    static const value_type initializer() { return PLL(0, 1); }
    static value_type operation(const value_type& a, const value_type& b) {
      return PLL(a.fs + b.fs, a.sc + b.sc);
    }
  };

  class o_monoid {
  public:
    using value_type = int64;
    static const value_type identity() { return -1; }
    static value_type operation(const value_type& a, const value_type& b) {
      if (b == -1) return a;
      return b;
    }
  };

  class modifier {
  public:
    using value_type = v_monoid::value_type;
    using op_type = o_monoid::value_type;
    static value_type operation(const value_type& a, const op_type& b) {
      if (b == -1) return a;
      return PLL(b * a.sc, a.sc);
    }
  };
}