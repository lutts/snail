// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/attributes/value_extraction.hpp>

#include <vector>
#include <iostream>  // NOLINT

#include "test/testutils/log_testspy.h"
#include "src/utils/log/log.h"

#include "utils/u8string.h"

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;

BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", LogSeverityLevel)

namespace xtestutils {

LogTestSpy::LogTestSpy() {}

void LogTestSpy::start() {
  typedef sinks::synchronous_sink<sinks::text_ostream_backend> text_sink;
  boost::shared_ptr<text_sink> sink = boost::make_shared<text_sink>();

  sink->locked_backend()->add_stream(boost::make_shared<std::stringstream>());

  sink->set_formatter(
      [this](logging::record_view const &rec, logging::formatting_ostream &) {
        actual_messages.emplace_back(
            rec[severity].get(), utils::U8String(rec[expr::smessage].get()));
      });

  logging::core::get()->add_sink(sink);

  beforeEnableLogHook();

  enableLog();
}

LogTestSpy::~LogTestSpy() {
  disableLog();

  logging::core::get()->remove_all_sinks();
}

std::ostream &operator<<(std::ostream &s, const LogTestSpy::Message &msg) {
  s << "<" << (std::underlying_type<LogSeverityLevel>::type)(msg.level) << ", "
    << msg.message << ">";

  return s;
}

}  // namespace xtestutils
