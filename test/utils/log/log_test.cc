//-*- TestCaseName: LogTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <vector>
#include <ios>

#include "utils/utils.h"
#include "utils/u8string.h"

#include "test/testutils/log_testspy.h"
#include "test/testutils/user_log_testspy.h"
#include "src/utils/log/log.h"

#include "test/testutils/utils.h"

using ::testing::TestWithParam;
using ::testing::GMOCK_FLAG(verbose);
using ::testing::internal::kErrorVerbosity;
using ::testing::internal::kInfoVerbosity;
using ::testing::internal::kWarningVerbosity;

namespace utils {
namespace tests {

// NOTE: log file is intentional not tested,
// use human eyes to see if the log file is what we want!
class LogTest : public ::testing::TestWithParam<LogSeverityLevel> {
 protected:
  LogTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  ~LogTest() {}
  virtual void SetUp() {}
  virtual void TearDown() {}

  const U8String kVerboseMsg{u8"a verbose message"};
  const U8String kDebugMsg{u8"a debug message"};
  const U8String kInfoMsg{u8"an info message"};
  const U8String kWarningMsg{u8"a warning message"};
  const U8String kErrorMsg{u8"a error message"};
  const U8String kUserMsg{u8"a user message"};

  void emitAllPossibleLevelLogs();

  const std::vector<LogTestSpy::Message> buildExpectMessagesByLevel(
      LogSeverityLevel lvl);
};

static auto level_range =
    enum_class_range<LogSeverityLevel, LogSeverityLevel::verbose,
                     LogSeverityLevel::num_level>();

INSTANTIATE_TEST_CASE_P(AllLogLevels, LogTest,
                        ::testing::ValuesIn(level_range.begin(),
                                            level_range.end()));

TEST_F(LogTest, logShouldDefaultDisabledBeforeStartupLogFunc) {  // NOLINT
  // Verify results
  ASSERT_FALSE(isLogEnabled());
}

TEST_F(LogTest, logShouldEnabledAfterInitLogCalled) {  // NOLINT
  // Exercise system
  initLog();

  // Verify results
  ASSERT_TRUE(isLogEnabled());

  // Teardown fixture
  cleanupLog();
}

TEST_F(LogTest, logShouldDisabledAfterCleanupLog) {  // NOLINT
  // Setup fixture
  initLog();

  // Exercise system
  cleanupLog();

  // Verify results
  ASSERT_FALSE(isLogEnabled());
}

TEST_F(LogTest, logShouldEnabledAfterEnableLogCalled) {  // NOLINT
  // Exercise system
  enableLog();

  // Verify results
  ASSERT_TRUE(isLogEnabled());

  // Teardown fixture
  disableLog();
}

TEST_F(LogTest, logShouldDisabledAfterDisableLogCalled) {  // NOLINT
  // Setup fixture
  enableLog();

  // Exercise system
  disableLog();

  // Verify results
  ASSERT_FALSE(isLogEnabled());
}

TEST_F(LogTest, nothingWillOutputWhenLogDisabled) {  // NOLINT
  // Setup fixture
  LogTestSpy logSpy;
  logSpy.start();
  disableLog();

  std::vector<LogTestSpy::Message> empty_messages;

  // Exercise system
  emitAllPossibleLevelLogs();

  // Verify results
  ASSERT_EQ(empty_messages, logSpy.actual_messages);
}

#define ALOGX_TEST(logger, lvl, msg)                   \
  TEST_F(LogTest, logger##_ShouldEmit##msg) {          \
    /* Setup fixture */                                \
    LogTestSpy logSpy;                                 \
    logSpy.start();                                    \
                                                       \
    /* expectations */                                 \
    std::vector<LogTestSpy::Message> expect_message;   \
    expect_message.emplace_back(lvl, msg);             \
                                                       \
    /* Exercise system */                              \
    logger << msg;                                     \
                                                       \
    /* Verify results */                               \
    ASSERT_EQ(expect_message, logSpy.actual_messages); \
  }

ALOGX_TEST(ALOGV, LogSeverityLevel::verbose, kVerboseMsg)
ALOGX_TEST(ALOGD, LogSeverityLevel::debug, kDebugMsg)
ALOGX_TEST(ALOGI, LogSeverityLevel::info, kInfoMsg)
ALOGX_TEST(ALOGW, LogSeverityLevel::warning, kWarningMsg)
ALOGX_TEST(ALOGE, LogSeverityLevel::error, kErrorMsg)
ALOGX_TEST(ALOGU, LogSeverityLevel::user, kUserMsg)

void LogTest::emitAllPossibleLevelLogs() {
  ALOGV << kVerboseMsg;
  ALOGD << kDebugMsg;
  ALOGI << kInfoMsg;
  ALOGW << kWarningMsg;
  ALOGE << kErrorMsg;
  ALOGU << kUserMsg;
}

const std::vector<LogTestSpy::Message> LogTest::buildExpectMessagesByLevel(
    LogSeverityLevel lvl) {
  std::vector<LogTestSpy::Message> expect_messages;

  if (!isLogEnabled()) return expect_messages;  // return empty vector

  if (lvl == LogSeverityLevel::num_level) lvl = getLogLevel();

  switch (lvl) {
    case LogSeverityLevel::verbose:
      expect_messages.emplace_back(LogSeverityLevel::verbose, kVerboseMsg);
    // fall through
    case LogSeverityLevel::debug:
      expect_messages.emplace_back(LogSeverityLevel::debug, kDebugMsg);
    // fall through
    case LogSeverityLevel::info:
      expect_messages.emplace_back(LogSeverityLevel::info, kInfoMsg);
    // fall through
    case LogSeverityLevel::warning:
      expect_messages.emplace_back(LogSeverityLevel::warning, kWarningMsg);
    // fall through
    case LogSeverityLevel::error:
      expect_messages.emplace_back(LogSeverityLevel::error, kErrorMsg);
    // fall through
    case LogSeverityLevel::user:
      expect_messages.emplace_back(LogSeverityLevel::user, kUserMsg);
    // fall through
    default:
      break;
  }

  return expect_messages;
}

TEST_P(LogTest, setLogLevelShouldBlockUnderLevelMessages) {
  // Setup fixture
  LogTestSpy logSpy;
  logSpy.start();

  LogSeverityLevel saved_lvl = getLogLevel();

  // expectations
  const std::vector<LogTestSpy::Message> expect_messages =
      buildExpectMessagesByLevel(GetParam());

  // Exercise system
  setLogLevel(GetParam());
  emitAllPossibleLevelLogs();

  // Verify results
  ASSERT_EQ(expect_messages, logSpy.actual_messages);

  // Teardown fixture
  setLogLevel(saved_lvl);
}

#define IF_ALOGX_TEST(logger, lvl)                                            \
  TEST_F(LogTest, logger##_ShouldReturnTrueWhenLevelAllowed) {                \
    /* Setup fixture */                                                       \
    LogTestSpy logSpy;                                                        \
    logSpy.start();                                                           \
                                                                              \
    LogSeverityLevel saved_lvl = getLogLevel();                               \
    LogSeverityLevel test_lvl = lvl;                                          \
                                                                              \
    /* expectations */                                                        \
    const std::vector<LogTestSpy::Message> expect_messages =                  \
        buildExpectMessagesByLevel(test_lvl);                                 \
                                                                              \
    /* Exercise system */                                                     \
    setLogLevel(test_lvl);                                                    \
    if (logger) {                                                             \
      emitAllPossibleLevelLogs();                                             \
    }                                                                         \
                                                                              \
    /* Verify results */                                                      \
    ASSERT_EQ(expect_messages, logSpy.actual_messages);                       \
                                                                              \
    /* Teardown fixture */                                                    \
    setLogLevel(saved_lvl);                                                   \
  }                                                                           \
                                                                              \
  TEST_F(LogTest, logger##_ShouldReturnFalseWhenLevelNotAllowed) {            \
    /* Setup fixture */                                                       \
    LogTestSpy logSpy;                                                        \
    logSpy.start();                                                           \
                                                                              \
    LogSeverityLevel saved_lvl = getLogLevel();                               \
    LogSeverityLevel test_lvl = lvl;                                          \
                                                                              \
    /* expectations */                                                        \
    const std::vector<LogTestSpy::Message> empty_messages;                    \
                                                                              \
    /* Exercise system */                                                     \
    using LevelUnderlyingType = std::underlying_type<LogSeverityLevel>::type; \
    LogSeverityLevel higherLevel =                                            \
        static_cast<LogSeverityLevel>((LevelUnderlyingType)test_lvl + 1);     \
    setLogLevel(higherLevel);                                                 \
    if (higherLevel == LogSeverityLevel::num_level) disableLog();             \
    if (logger) {                                                             \
      emitAllPossibleLevelLogs();                                             \
    }                                                                         \
                                                                              \
    /* Verify results */                                                      \
    ASSERT_EQ(empty_messages, logSpy.actual_messages);                        \
                                                                              \
    /* Teardown fixture */                                                    \
    setLogLevel(saved_lvl);                                                   \
  }

IF_ALOGX_TEST(IF_ALOGV, LogSeverityLevel::verbose)
IF_ALOGX_TEST(IF_ALOGD, LogSeverityLevel::debug)
IF_ALOGX_TEST(IF_ALOGI, LogSeverityLevel::info)
IF_ALOGX_TEST(IF_ALOGW, LogSeverityLevel::warning)
IF_ALOGX_TEST(IF_ALOGE, LogSeverityLevel::error)
IF_ALOGX_TEST(IF_ALOGU, LogSeverityLevel::user)

#define ALOGX_IF_TEST(logger, lvl, msg)                          \
  TEST_F(LogTest, logger##_ShouldEmit##msg##WhenConditionTrue) { \
    /* Setup fixture */                                          \
    LogTestSpy logSpy;                                           \
    logSpy.start();                                              \
                                                                 \
    LogSeverityLevel saved_lvl = getLogLevel();                  \
    setLogLevel(LogSeverityLevel::verbose);                      \
                                                                 \
    /* expectations */                                           \
    std::vector<LogTestSpy::Message> expect_messages;            \
    expect_messages.emplace_back(lvl, msg);                      \
                                                                 \
    /* Exercise system */                                        \
    logger(true) << msg;                                         \
                                                                 \
    /* Verify results */                                         \
    ASSERT_EQ(expect_messages, logSpy.actual_messages);          \
                                                                 \
    /* Teardown fixture */                                       \
    setLogLevel(saved_lvl);                                      \
  }                                                              \
                                                                 \
  TEST_F(LogTest, logger##_ShouldNotAnyMsgWhenConditionFalse) {  \
    /* Setup fixture */                                          \
    LogTestSpy logSpy;                                           \
    logSpy.start();                                              \
                                                                 \
    LogSeverityLevel saved_lvl = getLogLevel();                  \
    setLogLevel(LogSeverityLevel::verbose);                      \
                                                                 \
    /* expectations */                                           \
    const std::vector<LogTestSpy::Message> empty_messages;       \
                                                                 \
    /* Exercise system */                                        \
    logger(false) << msg;                                        \
                                                                 \
    /* Verify results */                                         \
    ASSERT_EQ(empty_messages, logSpy.actual_messages);           \
                                                                 \
    /* Teardown fixture */                                       \
    setLogLevel(saved_lvl);                                      \
  }

ALOGX_IF_TEST(ALOGV_IF, LogSeverityLevel::verbose, kVerboseMsg)
ALOGX_IF_TEST(ALOGD_IF, LogSeverityLevel::debug, kDebugMsg)
ALOGX_IF_TEST(ALOGI_IF, LogSeverityLevel::info, kInfoMsg)
ALOGX_IF_TEST(ALOGW_IF, LogSeverityLevel::warning, kWarningMsg)
ALOGX_IF_TEST(ALOGE_IF, LogSeverityLevel::error, kErrorMsg)
ALOGX_IF_TEST(ALOGU_IF, LogSeverityLevel::user, kUserMsg)

TEST_F(LogTest,
       UserLogTestSpyShouldOnlySnifferUserLevelMessagesOnly) {  // NOLINT
  // Setup fixture
  UserLogTestSpy logSpy;
  logSpy.start();

  // expectations
  std::vector<LogTestSpy::Message> expect_messages;
  expect_messages.emplace_back(LogSeverityLevel::user, kUserMsg);

  // Exercise system
  emitAllPossibleLevelLogs();

  // Verify results
  ASSERT_EQ(expect_messages, logSpy.actual_messages);
}

}  // namespace tests
}  // namespace utils
