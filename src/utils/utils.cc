// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#include "src/utils/utils.h"

namespace utils {

void init_locale() {
  boost::locale::generator gen;
  // Make system default locale global
  std::locale loc = gen("");
  std::locale::global(loc);
  std::cout.imbue(loc);

  volatile boost::locale::format fmt("");
  (void)fmt;
}

#if 0
// TODO(lutts): this function is not for use,
// just for import boost::locale::format
template<typename... Args>
U8String formatString2(const char* format, Args const&... args) {
  boost::locale::format fmt(format);
  int dummy[sizeof...(Args)] = { ( fmt % args, 0)... };
  (void)dummy;
  return fmt.str();
}
#endif

}  // namespace utils
