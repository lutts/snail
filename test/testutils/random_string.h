// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef TEST_TESTUTILS_RANDOM_STRING_H_
#define TEST_TESTUTILS_RANDOM_STRING_H_

#include <QString>
#include "test/testutils/utils.h"

namespace xtestutils {

class RandomString {
 public:
  RandomString()
      : ustr_(genRandomString())
      , qstr_(U8StringToQString(ustr_)) { }

  const utils::U8String& ustr() const {
    return ustr_;
  }

  const QString& qstr() const {
    return qstr_;
  }

 private:
  utils::U8String ustr_;
  QString qstr_;
};

}  // namespace xtestutils

#endif  // TEST_TESTUTILS_RANDOM_STRING_H_
