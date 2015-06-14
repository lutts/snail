// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>

#include "test/testutils/user_log_testspy.h"
#include "src/utils/log/log.h"  // LogSeverityLevel

namespace logging = boost::log;
namespace expr = boost::log::expressions;

UserLogTestSpy::UserLogTestSpy() { }

void UserLogTestSpy::beforeEnableLogHook() {
  logging::core::get()->set_filter(
      expr::attr<LogSeverityLevel>("Severity") == LogSeverityLevel::user);
}

UserLogTestSpy::~UserLogTestSpy() {
  logging::core::get()->reset_filter();
}
