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

}  // namespace utils
