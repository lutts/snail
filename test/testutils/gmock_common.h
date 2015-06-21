// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef TEST_TESTUTILS_GMOCK_COMMON_H_
#define TEST_TESTUTILS_GMOCK_COMMON_H_

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <vector>
#include <iostream>  // NOLINT
#include <string>

#if 0
using ::testing::GMOCK_FLAG(verbose);
using ::testing::internal::kErrorVerbosity;
using ::testing::internal::kInfoVerbosity;
using ::testing::internal::kWarningVerbosity;
using ::testing::StrictMock;
using ::testing::SaveArg;
using ::testing::_;
#endif

using namespace testing;  // NOLINT
using namespace testing::internal;  // NOLINT

using CheckPointType = MockFunction<void(std::string check_point_name)>;

class MockObjectRecorder {
 public:
  MockObjectRecorder() = default;
  ~MockObjectRecorder() = default;

  void addMockObj(void* mock_obj) {
    auto iter = std::find(mock_objs.begin(), mock_objs.end(), mock_obj);
    if (iter != mock_objs.end())
      return;

    mock_objs.push_back(mock_obj);
  }

  void verify() {
    for (auto mock_obj : mock_objs) {
      Mock::VerifyAndClear(mock_obj);
    }

    clear();
  }

  void clear() {
    mock_objs.clear();
  }

 private:
  MockObjectRecorder(const MockObjectRecorder& other) = delete;
  MockObjectRecorder& operator=(const MockObjectRecorder& other) = delete;

  std::vector<void*> mock_objs;
};

#define RECORD_USED_MOCK_OBJECTS_SETUP MockObjectRecorder mock_obj_recorder
#define VERIFY_RECORDED_MOCK_OBJECTS mock_obj_recorder.verify()
#define CLEAR_RECORDED_MOCK_OBJECTS mock_obj_recorder.clear()

#define R_EXPECT_CALL(obj, ...)                 \
  mock_obj_recorder.addMockObj(&(obj));         \
  EXPECT_CALL(obj, __VA_ARGS__)

#define R_ON_CALL(obj, ...)                     \
  mock_obj_recorder.addMockObj(&(obj));         \
  EXPECT_CALL(obj, __VA_ARGS__)

// #define OPTIMIZE_SINGLE_RUN_SETUP_EXPECTATIONS

// NOTE: single run setup expections is not that easy to implement,
// this is because we are not always want to setup default return value on
// mock object functions, without default return value or ON_CALLs, some
// function calls on mock object will fail

#ifdef OPTIMIZE_SINGLE_RUN_SETUP_EXPECTATIONS

#define SINGLE_RUN_TEST_SETUP_EXPECTATIONS_BEGIN        \
  static bool setup_mock_checked = false;               \
  if (!setup_mock_checked) {                            \
  setup_mock_checked = true;                            \
  do { } while (0)

#define SINGLE_RUN_TEST_SETUP_EXPECTATIONS_END } do { } while (0)

#else  // !OPTIMIZE_SINGLE_RUN_SETUP_EXPECTATIONS

#define SINGLE_RUN_TEST_SETUP_EXPECTATIONS_BEGIN do { } while (0)
#define SINGLE_RUN_TEST_SETUP_EXPECTATIONS_END   do { } while (0)

#endif  // OPTIMIZE_SINGLE_RUN_SETUP_EXPECTATIONS

#endif  // TEST_TESTUTILS_GMOCK_COMMON_H_
