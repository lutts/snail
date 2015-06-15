// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#include <string>
#include <utility>  // std::move

#include "utils/u8string.h"
#include "test/testutils/utils.h"

namespace xtestutils {

utils::U8String genRandomString(const int len) {
  std::string str;
  str.reserve(len);

  static const char alphanum[] =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";

  for (int i = 0; i < len; ++i) {
    str.push_back(alphanum[rand() % (sizeof(alphanum) - 1)]);
  }

  return utils::U8String { std::move(str) };
}

utils::U8String genRandomDifferentString(const utils::U8String& refStr,
                                        const int len) {
  utils::U8String newStr = genRandomString(len);

  while (refStr == newStr) {
    newStr = genRandomString(len);
  }

  return newStr;
}

}  // namespace xtestutils
