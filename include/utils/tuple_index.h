// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_UTILS_TUPLE_INDEX_H_
#define INCLUDE_UTILS_TUPLE_INDEX_H_

#include <tuple>

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

#endif  // INCLUDE_UTILS_TUPLE_INDEX_H_
