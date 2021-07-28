namespace range_add_range_sum {
  class v_monoid {
  public:
    using value_type = int64_t;
    static const value_type identity() { return 0; }
    static const value_type initializer() { return 0; }
    static value_type operation(const value_type& a, const value_type& b) {
      return a + b;
    }
  };

  class o_monoid {
  public:
    using value_type = int64_t;
    static const value_type identity() { return 0; }
    static value_type operation(const value_type& a, const value_type& b) {
      return a + b;
    }
  };

  class modifier {
  public:
    using value_type = v_monoid::value_type;
    using op_type = o_monoid::value_type;

    static value_type operation(const value_type& a, const op_type& b) {
      return a + b;
    }
  };
}
