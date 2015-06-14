// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_UTILS_UTILS_H_
#define SRC_UTILS_UTILS_H_

#include <boost/locale.hpp>

#include <type_traits>
#include <iterator>  // std::iterator, std::input_iterator_tag
#include <utility>   // std::pair

#include "src/utils/u8string.h"

namespace utils {

void init_locale();

template<typename... Args>
U8String formatString(const char* format, Args const&... args) {
  boost::locale::format fmt(format);
  int dummy[sizeof...(Args)] = { ( fmt % args, 0)... };
  (void)dummy;
  return fmt.str();
}


// Wrapper class allowing enumerations to be used in range-based for loops
// =======================================================================

template<typename T,
         T first = (T) 0,
         T last = T::count>
struct enum_class_range {
  struct iter : public std::iterator<std::input_iterator_tag, T> {
   public:
    explicit iter(T v) : v_(v) { }
    constexpr T operator* () const {
      return v_;
    }
    constexpr bool operator!= (iter other) const {
      return v_ != other.v_;
    }
    void operator++ () {
      v_ = (T) ((typename std::underlying_type<T>::type)(v_) + 1);
    }

   private:
    T v_;
  };

  static constexpr iter begin()
  { return iter{first}; }
  static constexpr iter end()
  { return iter{last}; }
};

// like equal_range pair, but can be used with C++11 ranged-for
//
// usage1:
//   auto rng = make_range(somevec.begin(), somevec.end());
//   for (auto item : rng) {
//     // do something with item
//   }
//
// usage2:
//   std::multimap<char, int> mymm;
//   ...
//   auto rng = make_range(mymm.equal_range(ch));
//   for (auto item : rng) {
//     // do something with item
//   }
template <typename IterType>
class range {
 public:
  range(IterType begin, IterType end)
      : begin_(begin), end_(end) { }

  IterType begin() const { return begin_; }
  IterType end() const { return end_; }
  typename std::iterator_traits<IterType>::difference_type
  distance() const { return std::distance(begin_, end_); }

 private:
  IterType begin_;
  IterType end_;
};

template <typename IterType>
range<IterType> make_range(IterType&& begin, IterType&& end) {  // NOLINT
  return range<IterType>(std::forward<IterType>(begin),
                         std::forward<IterType>(end));
}

template <typename IterType>
range<IterType> make_range(std::pair<IterType, IterType> const& equal_rng) {
  return range<IterType>(equal_rng.first, equal_rng.second);
}

}  // namespace utils

#endif  // SRC_UTILS_UTILS_H_
