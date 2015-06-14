// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#include <QChar>
#include <QString>
#include <ostream>  // NOLINT

#include "src/utils/u8string.h"
#include "test/testutils/qt/qt_utils.h"

namespace xtestutils {

QString genRandomQString(const int len) {
  return U8StringToQString(genRandomString(len));
}

}  // namespace xtestutils

void PrintTo(const QString& str, ::std::ostream* os) {
  *os << "QString(" << QStringToU8String(str) << ")";
}

void PrintTo(const QChar& chr, ::std::ostream* os) {
  *os << "QChar(" << chr.toLatin1() << ")";
}
