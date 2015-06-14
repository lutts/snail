// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_UTILS_LOG_LOG_GLOBAL_H_
#define SRC_UTILS_LOG_LOG_GLOBAL_H_

#include "src/utils/u8string.h"

#define MYLOGGER_PRIV_DATA_SIZE 512

class LoggerClient {
 public:
  LoggerClient(const char* tag, LogSeverityLevel sev);
  virtual ~LoggerClient();

  LoggerClient& operator<< (char c);
  LoggerClient& operator<< (const char* p);
  LoggerClient& operator<< (bool value);
  LoggerClient& operator<< (signed char value);
  LoggerClient& operator<< (unsigned char value);
  LoggerClient& operator<< (short value);  // NOLINT
  LoggerClient& operator<< (unsigned short value);  // NOLINT
  LoggerClient& operator<< (int value);
  LoggerClient& operator<< (unsigned int value);
  LoggerClient& operator<< (long value);  // NOLINT
  LoggerClient& operator<< (unsigned long value);  // NOLINT
  LoggerClient& operator<< (float value);
  LoggerClient& operator<< (double value);
  LoggerClient& operator<< (long double value);
  LoggerClient& operator<< (const void* value);
  LoggerClient& operator<< (std::string const& str);  // NOLINT
  // LoggerClient& operator<< (utils::U8String const& str);
  LoggerClient& operator<< (std::ios_base& (*pf)(std::ios_base&));

 private:
  LoggerClient(const LoggerClient& other) = delete;
  LoggerClient& operator=(const LoggerClient& other) = delete;

  class LoggerClientPrivate;
  LoggerClientPrivate* priv;

  unsigned char priv_data[MYLOGGER_PRIV_DATA_SIZE];
};

#define ALOG_LEVEL(lvl)                         \
  if (isLogLevelAllowed(lvl))                   \
    LoggerClient(LOGGER_STR, lvl)

#endif  // SRC_UTILS_LOG_LOG_GLOBAL_H_
