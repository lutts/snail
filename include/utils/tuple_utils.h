// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_UTILS_TUPLE_UTILS_H_
#define INCLUDE_UTILS_TUPLE_UTILS_H_

#include <tuple>

namespace utils {

template <class T, class Tuple>
struct tuple_index;

template <class T, class... Types>
struct tuple_index<T, std::tuple<T, Types...>> {
  static constexpr std::size_t value = 0;
};

template <class T, class U, class... Types>
struct tuple_index<T, std::tuple<U, Types...>> {
  static constexpr std::size_t value =
      1 + tuple_index<T, std::tuple<Types...>>::value;
};

template <std::size_t>
struct TupleElementPos {};

// tuple_for_each
template <typename Tuple, size_t Pos, typename Func>
void tuple_for_each(Tuple& t, TupleElementPos<Pos>, Func f) {  // NOLINT
  f(std::get<std::tuple_size<Tuple>::value - Pos>(t));
  tuple_for_each(t, TupleElementPos<Pos - 1>(), f);
}

template <typename Tuple, typename Func>
void tuple_for_each(Tuple& t, TupleElementPos<1>, Func f) {  // NOLINT
  f(std::get<std::tuple_size<Tuple>::value - 1>(t));
}

template <typename Tuple, typename Func>
void tuple_for_each(Tuple& t, Func f) {  // NOLINT
  tuple_for_each(t, TupleElementPos<std::tuple_size<Tuple>::value>(), f);
}

// tuple_for_each_pair
template <typename Tuple, size_t Pos, typename Func>
void tuple_for_each_pair(Tuple& t1, Tuple& t2, TupleElementPos<Pos>,
                         Func f) {  // NOLINT
  f(std::get<std::tuple_size<Tuple>::value - Pos>(t1),
    std::get<std::tuple_size<Tuple>::value - Pos>(t2));
  tuple_for_each_pair(t1, t2, TupleElementPos<Pos - 1>(), f);
}

template <typename Tuple, typename Func>
void tuple_for_each_pair(Tuple& t1, Tuple& t2, TupleElementPos<1>,
                         Func f) {  // NOLINT
  f(std::get<std::tuple_size<Tuple>::value - 1>(t1),
    std::get<std::tuple_size<Tuple>::value - 1>(t2));
}

template <typename Tuple, typename Func>
void tuple_for_each_pair(Tuple& t1, Tuple& t2, Func f) {  // NOLINT
  tuple_for_each_pair(t1, t2, TupleElementPos<std::tuple_size<Tuple>::value>(),
                      f);
}

}  // namespace utils

#endif  // INCLUDE_UTILS_TUPLE_UTILS_H_
