// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_UTILS_TYPE_H_
#define SRC_UTILS_TYPE_H_

#include <string>
#include <typeinfo>

// from
// http://stackoverflow.com/questions/281818/
// unmangling-the-result-of-stdtype-infoname

namespace utils {

std::string demangle(const char* name);

template <class T>
std::string type(const T& t) {
  return demangle(typeid(t).name());
}

}  // namespace utils

#endif  // SRC_UTILS_TYPE_H_
