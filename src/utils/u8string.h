// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <lutts.cao@gmail.com>
//
// [Desc]

#ifndef SRC_UTILS_U8STRING_H_
#define SRC_UTILS_U8STRING_H_

#include <string>

/*!
 * \relates utils::U8String
 *
 * Converts a QString to a utils::U8String without a requirement to link to Qt.
 */
#define QStringToU8String(s) s.toStdString()

/*!
 * \relates utils::U8String
 *
 * Converts a utils::U8String to a QString without a requirement to link to Qt.
 */
#define U8StringToQString(s) QString::fromStdString(s)

namespace utils {

using U8String = ::std::string;

}  // namespace utils

#endif  // SRC_UTILS_U8STRING_H_
