// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef TEST_TESTUTILS_UTILS_H_
#define TEST_TESTUTILS_UTILS_H_

#include <cstdlib>
#include <ctime>

#include "utils/u8string.h"

#define CUSTOM_ASSERT(...)                      \
  do {                                          \
    SCOPED_TRACE("");                           \
    __VA_ARGS__;                                \
    if (::testing::Test::HasFatalFailure())     \
      return;                                   \
  } while (0)

namespace xtestutils {

utils::U8String genRandomString(const int len = 16);
utils::U8String genRandomDifferentString(const utils::U8String& refStr,
                                         const int len = 16);

template <typename T>
T* genDummyPointer() {
  static bool seeded = false;
  if (!seeded) {
    std::srand(std::time(0));
    seeded = true;
  }
  return reinterpret_cast<T*>(std::rand());
}

}  // namespace xtestutils


#endif  // TEST_TESTUTILS_UTILS_H_
