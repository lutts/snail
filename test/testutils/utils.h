// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef TEST_TESTUTILS_UTILS_H_
#define TEST_TESTUTILS_UTILS_H_

#include "utils/u8string.h"

#define CUSTOM_ASSERT(...)                          \
  do {                                              \
    SCOPED_TRACE("");                               \
    __VA_ARGS__;                                    \
    if (::testing::Test::HasFatalFailure()) return; \
  } while (0)

namespace xtestutils {

int randomInt();
int randomIntInRange(int min, int max);

utils::U8String genRandomString(const int len = 16);
utils::U8String genRandomDifferentString(const utils::U8String& refStr,
                                         const int len = 16);
bool randomBool();

template <typename T>
T* genDummyPointer() {
  return reinterpret_cast<T*>(randomInt());
}

template <typename T>
T* genDifferentDummyPointer(T* ref_ptr) {
  T* ptr = nullptr;

  while (ptr == ref_ptr) {
    ptr = genDummyPointer<T>();
  }

  return ptr;
}

}  // namespace xtestutils

#endif  // TEST_TESTUTILS_UTILS_H_
