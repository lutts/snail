// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef TEST_TESTUTILS_LOG_TESTSPY_H_
#define TEST_TESTUTILS_LOG_TESTSPY_H_

#include <vector>
#include "src/utils/log/log.h"
#include "utils/u8string.h"

enum class LogSeverityLevel;

namespace xtestutils {

class LogTestSpy {
 public:
  struct Message {
    Message(LogSeverityLevel lvl, const utils::U8String& msg)
        : level(lvl), message(msg) {}
    bool operator==(const Message& other) const {
      return (level == other.level) && (message == other.message);
    }

    LogSeverityLevel level;
    utils::U8String message;
  };

  LogTestSpy();
  virtual ~LogTestSpy();

  void start();

  std::vector<Message> actual_messages;

 private:
  LogTestSpy(const LogTestSpy& other) = delete;
  LogTestSpy& operator=(const LogTestSpy& other) = delete;

  virtual void beforeEnableLogHook() {}
};

std::ostream& operator<<(std::ostream& s, const LogTestSpy::Message& msg);

}  // namespace xtestutils

#endif  // TEST_TESTUTILS_LOG_TESTSPY_H_
