#include <bits/stdc++.h>

template<class OperatorMonoid, template<class...> class Container=::std::vector>
class DualSegTree{
public:
  using operator_structure = OperatorMonoid;
  using operator_type = typename value_structure::operator_type;
  using const_reference = const value_type &;
  using container_type = Container<value_type>;
  using size_type = typename container_type::size_type;

private:
  ::std::vector<operator_type> tree;
  size_type size_, height;

  static size_type getsize(const size_type x){
    size_type ret = 1;
    while(ret < x)
      ret <<= 1;
    return ret;
  }

  static size_type getheight(const size_type x){
    size_type ret = 0;
    while((static_cast<size_type>(1) << ret) < x){
      ret++;
    }
    return ret;
  }

  void propagate(const size_type index){
    for(size_type shift = height; shift ; --shift){
      push(index >> shift);
    }
  }

  inline value_type calc(const operator_type& a, const operator_type& b){
    return operator_structure::operation(a, b);
  }

  inline void calc_node(const size_type index){
    if(tree.size() <= (index << 1 | 1)) return;
    tree[index] = value_structure::operation(tree[index<<1], tree[index<<1 | 1]);
  }
public:
  SegTree() : size_(0), tree(){}
  SegTree(const size_type size)
    : size_(size), height(getheight(size)), tree(size << 1, value_structure::identity()){}
  template<class InputIterator>
  SegTree(InputIterator first, InputIterator last)
    : size_(::std::distance(first, last)){
    height = getheight(size_);
    tree = container_type(size_, value_structure::identity());
    tree.insert(tree.end(), first, last);
    for(size_type i = size_;i > 0;i--){
      calc_node(i);
    }
  }

  size_type size() const { return size_; }
  const_reference operator[](const size_type k) const {
    assert(k < size_);
    return tree[k+size_];
  }

  value_type query(size_type l, size_type r){
    assert(l <= r);
    assert(0 <= l && l < size_);
    assert(0 <= r && r <= size_);
    value_type retl = value_structure::identity(), retr = value_structure::identity();
    for(l += size_, r += size_; l < r ; l >>= 1, r >>= 1){
      if(l&1) retl = calc(retl, tree[l++]);
      if(r&1) retr = calc(tree[--r], retr);
    }
    return calc(retl, retr);
  }

  template<class F>
  void update(size_type index, const F& f){
    assert(0 <= index && index < size());
    index += size_;
    tree[index] = f(::std::move(tree[index]));
    while(index >>= 1)
      calc_node(index);
  }

  /*
  template<class F>
  size_type search(const F& f) const { // [0, result) is True and [0, result-1) is not.
    if(f(value_structure::identity()))
      return 0;
    if(!f(tree[1]))
      return size_+1;
    value_type acc = value_structure::identity();
    size_type i = 1;
    while(i <
  }
  */
};
