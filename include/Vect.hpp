#ifndef VECT_HPP
# define VECT_HPP

# include <utility>
# include <type_traits>
# include <math.h>

template <std::size_t dim, class T>
class Vect
{
private:
  T data[dim];

  template<class V, size_t... indices>
  constexpr Vect(V const (&other)[dim], std::index_sequence<indices...>)
    : Vect(other[indices]...)
  {
  }

  template<size_t... indices>
  constexpr Vect(std::index_sequence<indices...>)
    : Vect(((void)indices, T{})...)
  {
  }

public:
  template<class V>
  constexpr Vect(V const (&other)[dim])
    : Vect(other, std::make_index_sequence<dim>{})
  {
  }

  template<class V>
  constexpr Vect(Vect<dim, V> const &other)
    : Vect(other.data)
  {
  }

  constexpr Vect()
  : Vect(std::make_index_sequence<dim>{})
  {
  }

  template<class... U, typename std::enable_if<sizeof...(U) == dim>::type * = nullptr>
  constexpr Vect(U &&... ts) : data{std::forward<U>(ts)...}
  {
  }

  constexpr T *begin()
  {
    return data;
  }

  constexpr T *end()
  {
    return data + dim;
  }

  constexpr T const *begin() const
  {
    return data;
  }

  constexpr T const *end() const
  {
    return data + dim;
  }

  template<class Operation>
  void applyOnSelf(Operation op)
  {
    for (unsigned int i(0); i < dim; ++i)
      data[i] = op(i);
  }

  constexpr T &operator[](unsigned int index)
  {
    return (data[index]);
  }

  constexpr T const &operator[](unsigned int index) const
  {
    return (data[index]);
  }

#define VECT_OPERATOR_DEF(OP)						\
  template<class U>							\
  constexpr Vect<dim, T>& operator OP##=(Vect<dim, U> const &other)	\
  {									\
    for (size_t i(0u); i != dim; ++i)					\
      data[i] OP##= other[i];						\
    return (*this);							\
  };									\
									\
  template<class U>							\
  constexpr auto operator OP(Vect<dim, U> const &other) const		\
  {									\
    Vect<dim, decltype(data[0] OP other[0])> result{*this};		\
									\
    result OP##= other;							\
    return result;							\
  };									\
  									\
  template<class U>							\
  constexpr Vect<dim, T>& operator OP##=(U const &other)		\
  {									\
    for (auto &elem : *this)						\
      elem OP##= other;							\
    return (*this);							\
  };									\
									\
  template<class U>							\
  constexpr auto operator OP(U const &other) const			\
  {									\
    Vect<dim, decltype(data[0] OP other)> result{*this};		\
									\
    result OP##= other;							\
    return result;							\
  }

  VECT_OPERATOR_DEF(+);
  VECT_OPERATOR_DEF(-);
  VECT_OPERATOR_DEF(*);
  VECT_OPERATOR_DEF(/);
  VECT_OPERATOR_DEF(%);
  VECT_OPERATOR_DEF(^);
  VECT_OPERATOR_DEF(|);
  VECT_OPERATOR_DEF(&);

  // TODO optimize ?
  constexpr bool equals(Vect<dim, T> const& other) const
  {
    unsigned int i(0);

    for (; i != dim && data[i] == other.data[i]; ++i);
    return i == dim;
  }

private:
  template<class FUNC, size_t... indices>
  constexpr Vect<dim, T> map_impl(FUNC &&func, std::index_sequence<indices...>) const
  {
    return {func(data[indices])...};
  }


public:
  template<class FUNC>
  constexpr Vect<dim, T> map(FUNC &&func) const
  {
    return (map_impl(func, std::make_index_sequence<dim>{}));
  }

#define VECT_UNARY_OP_DEF(OP)				\
  constexpr Vect<dim, T> operator OP(void) const	\
  {							\
    struct Mapper					\
    {							\
      T operator()(T const &t)				\
      {							\
	return OP t;					\
      }							\
    };							\
    return map(Mapper{});				\
  }							\

  VECT_UNARY_OP_DEF(-);
  VECT_UNARY_OP_DEF(+);
  VECT_UNARY_OP_DEF(~);
  VECT_UNARY_OP_DEF(!);

public:
  constexpr T sum(void) const
  {
    T result{0u};

    for (auto const &t : *this)
      result += t;
    return (result);
  }

  constexpr T scalar(Vect<dim, T> const &other) const
  {
    return ((*this * other).sum());
  }

  constexpr T length2() const
  {
    return ((*this * *this).sum());
  }

  Vect<dim, T> normalized() const
  {
    return length2() > 0 ? ((*this) / sqrt(length2())) : *this;
  }

#define VECT_NAMED_COMPONENT(NAME, INDEX)				\
  template<unsigned int _dim = dim, typename std::enable_if<(_dim > 0)>::type* = nullptr> \
  T NAME() const							\
  {									\
    return (data[INDEX]);						\
  }									\
									\
  template<unsigned int _dim = dim, typename std::enable_if<(_dim > 0)>::type* = nullptr> \
  T &NAME()								\
  {									\
    return (data[INDEX]);						\
  }									\

  VECT_NAMED_COMPONENT(x, 0);
  VECT_NAMED_COMPONENT(y, 1);
  VECT_NAMED_COMPONENT(z, 2);
  VECT_NAMED_COMPONENT(w, 3);

  constexpr bool all() const
  {
    unsigned int i(0);

    while (i != dim && data[i])
      i = i + 1;
    return (i == dim);
  }

  constexpr operator decltype(data) & ()
  {
    return (data);
  }

  constexpr operator decltype(data) const & () const
  {
    return (data);
  }

  template<class L>
  static void forEach(Vect<dim, T> begin, Vect<dim, T> end, L l)
  {
    Vect<dim, T> index(begin);

    for (index[0] = begin[0]; index[0] < end[0]; ++index[0])
      for (index[1] = begin[1]; index[1] < end[1]; ++index[1])
	l(index);
  }
};

#endif // VECT_HPP_
