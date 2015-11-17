// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_UTILS_LOG_LOG_H_
#define SRC_UTILS_LOG_LOG_H_

// #define BOOST_LOGGER_PER_COMPILE_UNIT

enum class LogSeverityLevel {
  verbose,
  debug,
  info,
  warning,
  error,
  user,
  num_level,
};

extern void initLog();
extern void cleanupLog();
extern LogSeverityLevel getLogLevel();
extern void setLogLevel(LogSeverityLevel level);
extern void enableLog();
extern void disableLog();
extern bool isLogEnabled();

#define ALOGV ALOG_LEVEL(LogSeverityLevel::verbose)
#define ALOGD ALOG_LEVEL(LogSeverityLevel::debug)
#define ALOGI ALOG_LEVEL(LogSeverityLevel::info)
#define ALOGW ALOG_LEVEL(LogSeverityLevel::warning)
#define ALOGE ALOG_LEVEL(LogSeverityLevel::error)
#define ALOGU ALOG_LEVEL(LogSeverityLevel::user)

#define IF_ALOGV isLogLevelAllowed(LogSeverityLevel::verbose)
#define IF_ALOGD isLogLevelAllowed(LogSeverityLevel::debug)
#define IF_ALOGI isLogLevelAllowed(LogSeverityLevel::info)
#define IF_ALOGW isLogLevelAllowed(LogSeverityLevel::warning)
#define IF_ALOGE isLogLevelAllowed(LogSeverityLevel::error)
#define IF_ALOGU isLogLevelAllowed(LogSeverityLevel::user)

#define CONDITION(cond) (__builtin_expect((cond) != 0, 0))

#define ALOGV_IF(cond) \
  if (CONDITION(cond)) ALOGV

#define ALOGD_IF(cond) \
  if (CONDITION(cond)) ALOGD

#define ALOGI_IF(cond) \
  if (CONDITION(cond)) ALOGI

#define ALOGW_IF(cond) \
  if (CONDITION(cond)) ALOGW

#define ALOGE_IF(cond) \
  if (CONDITION(cond)) ALOGE

#define ALOGU_IF(cond) \
  if (CONDITION(cond)) ALOGU

/*
 * ===========================================================================
 *
 * The stuff in the rest of this file should not be used directly.
 */

#ifndef LOG_TAG
#define LOG_TAG generic
#endif

#define LOGGER_STR__(s) #s
#define LOGGER_STR_(s) LOGGER_STR__(s)
#define LOGGER_STR LOGGER_STR_(LOG_TAG)

static inline bool isLogLevelAllowed(LogSeverityLevel level) {
  return isLogEnabled() && (level >= getLogLevel());
}

#ifndef BOOST_LOGGER_PER_COMPILE_UNIT

#include "./log_global.h"

#else  // BOOST_LOGGER_PER_COMPILE_UNIT

#include "./log_per_unit.h"

#endif  // BOOST_LOGGER_PER_COMPILE_UNIT

#endif  // SRC_UTILS_LOG_LOG_H_
