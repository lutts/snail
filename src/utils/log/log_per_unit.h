// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_UTILS_LOG_LOG_PER_UNIT_H_
#define SRC_UTILS_LOG_LOG_PER_UNIT_H_

// TODO(lutts): per TAG log level support

#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/record_ostream.hpp>

#include <string>

typedef boost::log::sources::severity_channel_logger_mt<
    LogSeverityLevel,  // the type of the severity level
    std::string        // the type of the channel name
    > my_logger_mt;

#define LOGGER_NAME_(x) x##_logger
#define LOGGER_NAME(x) LOGGER_NAME_(x)

BOOST_LOG_INLINE_GLOBAL_LOGGER_INIT(LOGGER_NAME(LOG_TAG), my_logger_mt) {
  return my_logger_mt(boost::log::keywords::channel = LOGGER_STR);
}

#define ALOG_LEVEL(lvl) \
  if (isLogLevelAllowed(lvl)) BOOST_LOG_SEV(LOGGER_NAME(LOG_TAG)::get(), lvl)

#endif  // SRC_UTILS_LOG_LOG_PER_UNIT_H_
