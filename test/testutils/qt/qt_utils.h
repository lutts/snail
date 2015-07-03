// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef TEST_TESTUTILS_QT_QT_UTILS_H_
#define TEST_TESTUTILS_QT_QT_UTILS_H_

#include <QString>
#include "test/testutils/utils.h"

namespace xtestutils {

QString genRandomQString(const int len = 16);

}  // namespace xtestutils

extern void PrintTo(const QString& str, ::std::ostream* os);
extern void PrintTo(const QChar& chr, ::std::ostream* os);

#endif  // TEST_TESTUTILS_QT_QT_UTILS_H_
