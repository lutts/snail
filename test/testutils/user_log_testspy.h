// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef TEST_TESTUTILS_USER_LOG_TESTSPY_H_
#define TEST_TESTUTILS_USER_LOG_TESTSPY_H_

#include <vector>

#include "test/testutils/log_testspy.h"
#include "utils/u8string.h"

class UserLogTestSpy : public LogTestSpy {
 public:
  UserLogTestSpy();
  virtual ~UserLogTestSpy();

  static const std::vector<Message> createSingleMsgVector(
      const utils::U8String& msg) {
    std::vector<Message> msgs;
    msgs.emplace_back(LogSeverityLevel::user, msg);
    return msgs;
  }

 private:
  UserLogTestSpy(const UserLogTestSpy& other) = delete;
  UserLogTestSpy& operator=(const UserLogTestSpy& other) = delete;

  void beforeEnableLogHook() override;
};

#endif  // TEST_TESTUTILS_USER_LOG_TESTSPY_H_
