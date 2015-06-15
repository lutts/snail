// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_UTILS_UTILS_H_
#define SRC_UTILS_UTILS_H_

#include <boost/locale.hpp>
#include "utils/u8string.h"

namespace utils {

void init_locale();

template<typename... Args>
U8String formatString(const char* format, Args const&... args) {
  boost::locale::format fmt(format);
  int dummy[sizeof...(Args)] = { ( fmt % args, 0)... };
  (void)dummy;
  return fmt.str();
}

}  // namespace utils

#endif  // SRC_UTILS_UTILS_H_
