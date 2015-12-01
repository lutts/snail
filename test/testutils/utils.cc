// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#include <cstdlib>
#include <ctime>
#include <string>
#include <utility>  // std::move
#include <stdexcept>

#include "utils/u8string.h"
#include "test/testutils/utils.h"

namespace xtestutils {

int randomInt() {
  static bool rand_seeded = false;
  if (!rand_seeded) {
    std::srand(std::time(0));
    rand_seeded = true;
  }

  return std::rand();
}

int randomIntInRange(int min, int max) {
  if (min > max)
    throw std::invalid_argument("randomIntInRange: min > max is wrong");
  return min + randomInt() % (max - min + 1);
}

utils::U8String genRandomString(const int len) {
  std::string str;
  str.reserve(len);

  static const char alphanum[] =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";

  for (int i = 0; i < len; ++i) {
    str.push_back(alphanum[randomInt() % (sizeof(alphanum) - 1)]);
  }

  return utils::U8String{std::move(str)};
}

utils::U8String genRandomDifferentString(const utils::U8String& refStr,
                                         const int len) {
  utils::U8String newStr = genRandomString(len);

  while (refStr == newStr) {
    newStr = genRandomString(len);
  }

  return newStr;
}

bool randomBool() {
  auto a = randomInt();
  auto b = randomInt();
  return a < b;
}

}  // namespace xtestutils
