// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/utils/type.h"

#ifdef __GNUG__
#include <cxxabi.h>

#include <cstdlib>
#include <memory>

namespace utils {

std::string demangle(const char* name) {
  int status = -4;  // some arbitrary value to eliminate the compiler warning

  // enable c++11 by passing the flag -std=c++11 to g++
  std::unique_ptr<char, void (*)(void*)> res{
      abi::__cxa_demangle(name, NULL, NULL, &status), std::free};

  return (status == 0) ? res.get() : name;
}

}  // namespace utils

#else

namespace utils {

// does nothing if not g++
std::string demangle(const char* name) { return name; }

}  // namespace utils

#endif
