// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_UTILS_BASIC_UTILS_H_
#define SRC_UTILS_BASIC_UTILS_H_

#include <memory>

#define V_UNUSED(v) (void)(v)

#define LINE_TRACE                              \
  do {                                          \
    std::cout << "enter "                       \
              << __PRETTY_FUNCTION__ << ":"     \
              << __LINE__                       \
              << std::endl;                     \
  } while (0)

namespace utils {
// default deleter version
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <typename T, int N>
constexpr int sizeof_array(const T (&)[N] ) {
  return N;
}

}  // namespace utils

#endif  // SRC_UTILS_BASIC_UTILS_H_
