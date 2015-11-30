// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#include <boost/format.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/attributes/current_process_id.hpp>
#include <boost/log/attributes/current_thread_id.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/support/date_time.hpp>

#include <cstddef>
#include <string>
#include <iostream>  // NOLINT

#include "src/utils/log/log.h"

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

#ifndef BOOST_LOGGER_PER_COMPILE_UNIT

typedef boost::log::sources::severity_channel_logger_mt<
    LogSeverityLevel,  // the type of the severity level
    std::string        // the type of the channel name
    > my_logger_mt;

BOOST_LOG_INLINE_GLOBAL_LOGGER_INIT(my_logger, my_logger_mt) {
  return my_logger_mt(boost::log::keywords::channel = "generic");
}

class LoggerClient::LoggerClientPrivate {
 public:
  LoggerClientPrivate(LogSeverityLevel sev, const std::string& channel_name)
      : lrec(my_logger::get().open_record(
            (keywords::channel = channel_name, keywords::severity = sev))) {
    static_assert(sizeof(*this) < MYLOGGER_PRIV_DATA_SIZE,
                  "LoggerClientPrivate's size is larger that reserved space, "
                  "please increase MYLOGGER_PRIV_DATE_SIZE");

    if (!!lrec) strm.attach_record(lrec);
  }
  ~LoggerClientPrivate() {}

  logging::record lrec;
  logging::record_ostream strm;
};

LoggerClient::LoggerClient(const char* tag, LogSeverityLevel sev)
    : priv(new (priv_data) LoggerClientPrivate(sev, tag)) {
  static_assert(sizeof(LoggerClientPrivate) <= MYLOGGER_PRIV_DATA_SIZE,
                "LoggerClientPrivate is large than expect");
}

LoggerClient::~LoggerClient() {
  if (!!priv->lrec) {
    try {
      priv->strm.flush();
    } catch (...) {
    }

    try {
      my_logger::get().push_record(boost::move(priv->lrec));
    } catch (...) {
    }

    priv->~LoggerClientPrivate();
  }
}

#define BYPASS_TO_BOOST_LOG_STREAM(Type)               \
  LoggerClient& LoggerClient::operator<<(Type value) { \
    if (!!priv->lrec) {                                \
      priv->strm << value;                             \
    }                                                  \
                                                       \
    return *this;                                      \
  }

BYPASS_TO_BOOST_LOG_STREAM(char)
BYPASS_TO_BOOST_LOG_STREAM(const char*)
BYPASS_TO_BOOST_LOG_STREAM(bool)
BYPASS_TO_BOOST_LOG_STREAM(signed char)
BYPASS_TO_BOOST_LOG_STREAM(unsigned char)
BYPASS_TO_BOOST_LOG_STREAM(short)           // NOLINT
BYPASS_TO_BOOST_LOG_STREAM(unsigned short)  // NOLINT
BYPASS_TO_BOOST_LOG_STREAM(int)
BYPASS_TO_BOOST_LOG_STREAM(unsigned int)
BYPASS_TO_BOOST_LOG_STREAM(long)           // NOLINT
BYPASS_TO_BOOST_LOG_STREAM(unsigned long)  // NOLINT
BYPASS_TO_BOOST_LOG_STREAM(float)
BYPASS_TO_BOOST_LOG_STREAM(double)
BYPASS_TO_BOOST_LOG_STREAM(long double)
BYPASS_TO_BOOST_LOG_STREAM(const void*)
BYPASS_TO_BOOST_LOG_STREAM(std::string const&)

LoggerClient& LoggerClient::operator<<(std::ios_base& (*pf)(std::ios_base&)) {
  if (!!priv->lrec) priv->strm << pf;
  return *this;
}

#endif  // BOOST_LOGGER_PER_COMPILE_UNIT

std::ostream& operator<<(std::ostream& strm, LogSeverityLevel level) {
  const int maxlevel = static_cast<int>(LogSeverityLevel::num_level);
  static const char logChar[maxlevel] = {'V', 'D', 'I', 'W', 'E', 'U'};

  int ilevel = static_cast<int>(level);

  if (level < LogSeverityLevel::num_level)
    strm.put(logChar[ilevel]);
  else
    strm << ilevel;

  return strm;
}

// Define the attribute keywords
// BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", LogSeverityLevel)
BOOST_LOG_ATTRIBUTE_KEYWORD(channel, "Channel", std::string)
BOOST_LOG_ATTRIBUTE_KEYWORD(process_id, "ProcessID",
                            attrs::current_process_id::value_type)
BOOST_LOG_ATTRIBUTE_KEYWORD(thread_id, "ThreadID",
                            attrs::current_thread_id::value_type)
// BOOST_LOG_ATTRIBUTE_KEYWORD(linux_process_id, "LinuxProcessID", int)

namespace {

using console_sink_type =
    boost::shared_ptr<sinks::synchronous_sink<sinks::text_ostream_backend> >;

console_sink_type console_sink;

using file_sink_type =
    boost::shared_ptr<sinks::synchronous_sink<sinks::text_file_backend> >;

file_sink_type debug_file_sink;
file_sink_type user_file_sink;

void addDebugSinks() {
  // Setup the common formatter for all sinks
  logging::formatter debug_fmt =
      expr::stream << expr::format_date_time<boost::posix_time::ptime>(
                          "TimeStamp", "%m-%d %H:%M:%S") << " " << process_id
                   << " " << thread_id << " " << severity << " " << channel
                   << ": " << expr::smessage;

  console_sink = logging::add_console_log(std::clog);
  console_sink->set_formatter(debug_fmt);

  debug_file_sink = logging::add_file_log(
      keywords::target = "logs/debug_logs",
      keywords::file_name = "snail_debug_%m-%d_%H-%M-%S_%N.log",
      keywords::rotation_size = 1 * 1024 * 1024,
      keywords::max_size = 16 * 1024 * 1024,
      keywords::open_mode = (std::ios::out | std::ios::app));

  debug_file_sink->set_formatter(debug_fmt);
}

void addUserSinks() {
  logging::formatter user_fmt =
      expr::stream << expr::format_date_time<boost::posix_time::ptime>(
                          "TimeStamp", "%m-%d %H:%M:%S") << ": "
                   << expr::smessage;

  user_file_sink = logging::add_file_log(
      keywords::target = "logs/user_logs",
      keywords::filter = expr::attr<LogSeverityLevel>("Severity") ==
                         LogSeverityLevel::user,  // NOLINT
      keywords::file_name = "snail_user_%N.log",
      keywords::max_size = 3 * 1024 * 1024,
      keywords::open_mode = (std::ios::out | std::ios::app));

  user_file_sink->set_formatter(user_fmt);
}
}  // namespace

// TODO(lutts): read current_level from user settings
static LogSeverityLevel current_level = LogSeverityLevel::verbose;
static bool loggingEnabled = false;  // default to false before initLog called

void initLog() {
  addDebugSinks();
  addUserSinks();

#if 0
  logging::core::get()->add_global_attribute(
      "LinuxProcessId",
      attrs::constant<int>(static_cast<int>(getpid())));
#endif
  logging::add_common_attributes();

  // TODO(lutts): read from user settings
  loggingEnabled = true;
}

void cleanupLog() {
  loggingEnabled = false;
  logging::core::get()->remove_all_sinks();
  user_file_sink.reset();
  debug_file_sink.reset();
  console_sink.reset();
}

// set log level to verbose, debug, info, warning, error, user, or disable
//
// do not set to num_level, set to num_level will do nothing.
//
// level can retrieved by call getLogLevel()
void setLogLevel(LogSeverityLevel level) {
  if (level == LogSeverityLevel::num_level) return;
  current_level = level;
}

// return the current log level
LogSeverityLevel getLogLevel() { return current_level; }

void enableLogging() { logging::core::get()->set_logging_enabled(true); }

void disableLogging() { logging::core::get()->set_logging_enabled(false); }

void enableLog() {
  enableLogging();
  loggingEnabled = true;
}

void disableLog() {
  disableLogging();
  loggingEnabled = false;
}

bool isLogEnabled() { return loggingEnabled; }
