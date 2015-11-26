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
#include <stdexcept>
#include <algorithm>

#include "utils/basic_utils.h"  // make_unique, <memory>
#include "test/testutils/utils.h"
#include "test/testutils/simple_mock_listener.h"
#include "test/testutils/slot_catcher.h"
#include "test/testutils/mock_object_generator.h"
#include "test/testutils/model_view_mock_generator.h"
#include "test/testutils/fixture_factory.h"

#if 0
using ::testing::GMOCK_FLAG(verbose);
using ::testing::internal::kErrorVerbosity;
using ::testing::internal::kInfoVerbosity;
using ::testing::internal::kWarningVerbosity;
using ::testing::StrictMock;
using ::testing::SaveArg;
using ::testing::_;
#endif

using namespace testing;            // NOLINT
using namespace testing::internal;  // NOLINT

class ErrorVerbosityTest : public ::testing::Test {
 public:
  ErrorVerbosityTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }

  virtual ~ErrorVerbosityTest() = default;
};

template <typename T>
class ErrorVerbosityTestWithParam : public ::testing::TestWithParam<T> {
 public:
  ErrorVerbosityTestWithParam() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }

  virtual ~ErrorVerbosityTestWithParam() = default;
};

using CheckPointType = MockFunction<void(std::string check_point_name)>;

// gmock will generate default return value for simple data types, such as int,
// or pointer, ComplexReturnValue is used when StrictMock is inappropriate and
// you want ensure the method is not called unless you set an expection on it
class ComplexReturnValue {
 public:
  explicit ComplexReturnValue(int dummy) : dummy_{dummy} {}

 private:
  int dummy_;
};

class MockObjectRecorder {
 public:
  MockObjectRecorder() = default;
  // WARNING: use the following copy & move operation carefully
  // copy is not allowed, but move is ok
  MockObjectRecorder(MockObjectRecorder&& rhs)
      : mock_objs(std::move(rhs.mock_objs)) {}
  MockObjectRecorder& operator=(MockObjectRecorder&& rhs) {
    swap(rhs);
    return *this;
  }
  void swap(MockObjectRecorder& rhs) { std::swap(mock_objs, rhs.mock_objs); }
  friend inline void swap(MockObjectRecorder& v1, MockObjectRecorder& v2) {
    v1.swap(v2);
  }

  ~MockObjectRecorder() = default;

  void addMockObj(void* mock_obj) {
    auto iter = std::find(mock_objs.begin(), mock_objs.end(), mock_obj);
    if (iter != mock_objs.end()) return;

    mock_objs.push_back(mock_obj);
  }

  bool verify() {
    bool result = true;
    for (auto mock_obj : mock_objs) {
      result &= Mock::VerifyAndClear(mock_obj);
    }

    clear();

    return result;
  }

  void clear() { mock_objs.clear(); }

 private:
  MockObjectRecorder(const MockObjectRecorder& other) = delete;
  MockObjectRecorder& operator=(const MockObjectRecorder& other) = delete;

  std::vector<void*> mock_objs;
};

#define RECORD_USED_MOCK_OBJECTS_SETUP MockObjectRecorder mock_obj_recorder
#define VERIFY_RECORDED_MOCK_OBJECTS mock_obj_recorder.verify()
#define CLEAR_RECORDED_MOCK_OBJECTS mock_obj_recorder.clear()

#define R_EXPECT_CALL(obj, ...)         \
  mock_obj_recorder.addMockObj(&(obj)); \
  EXPECT_CALL(obj, __VA_ARGS__)

#define R_ON_CALL(obj, ...)             \
  mock_obj_recorder.addMockObj(&(obj)); \
  ON_CALL(obj, __VA_ARGS__)

// #define OPTIMIZE_SINGLE_RUN_SETUP_EXPECTATIONS

// NOTE: single run setup expections is not that easy to implement,
// this is because we are not always want to setup default return value on
// mock object functions, without default return value or ON_CALLs, some
// function calls on mock object will fail

#ifdef OPTIMIZE_SINGLE_RUN_SETUP_EXPECTATIONS

#define SINGLE_RUN_TEST_SETUP_EXPECTATIONS_BEGIN \
  static bool setup_mock_checked = false;        \
  if (!setup_mock_checked) {                     \
    setup_mock_checked = true;                   \
    do {                                         \
  } while (0)

#define SINGLE_RUN_TEST_SETUP_EXPECTATIONS_END \
  }                                            \
  do {                                         \
  } while (0)

#else  // !OPTIMIZE_SINGLE_RUN_SETUP_EXPECTATIONS

#define SINGLE_RUN_TEST_SETUP_EXPECTATIONS_BEGIN \
  do {                                           \
  } while (0)
#define SINGLE_RUN_TEST_SETUP_EXPECTATIONS_END \
  do {                                         \
  } while (0)

#endif  // OPTIMIZE_SINGLE_RUN_SETUP_EXPECTATIONS

/**
 * TestFixture is a small closure to setup environment for a test task/object.
 *
 * When to Use TestFixture?
 *   if you find yourself copy the same sequence of code from once test to
 * another, you have two choice:
 *   1. extract the sequence of code to SetUp, or if inappropriate
 *   2. extract the sequence of code to TestFixture
 *
 * TestFixture and Creation Method pattern
 *   In his book xTest Test Patterns, Gerard Meszaros's examples are java
 * examples, in C++, when you create a test object, and the test object depends
 * on other helper test objects, you must store these helper test objects some-
 * where, and you must remember to free these object when test finished. Java
 * programs do not care these problems, because java object which has reference
 * will not be freed, and when there's no reference, it will garbage collected.
 *
 *   C++ is not that lucky, most of time, Creation Method is not sufficient,
 * what we need is an Creation Class, Creation class wrap the helper test
 * objects in a closure, create them on demand, free them when those object are
 * not needed.
 */
class TestFixture {
 public:
  TestFixture() : TestFixture{"GlobalFixture"} {}
  explicit TestFixture(const utils::U8String& name) : fixture_name_(name) {}
  // WARNING: use the following copy & move operation carefully
  // mock object recorder are not copied
  TestFixture(const TestFixture& rhs)
      : fixture_name_(rhs.fixture_name_), mock_obj_recorder{} {}
  TestFixture& operator=(const TestFixture& rhs) {
    fixture_name_ = rhs.fixture_name_;
    // mock_obj_recorder not changed
    return *this;
  }
  TestFixture(TestFixture&& rhs)
      : fixture_name_{std::move(rhs.fixture_name_)},
        mock_obj_recorder{std::move(rhs.mock_obj_recorder)} {}
  TestFixture& operator=(TestFixture&& rhs) {
    swap(rhs);
    return *this;
  }

  void swap(TestFixture& rhs) {
    std::swap(fixture_name_, rhs.fixture_name_);
    std::swap(mock_obj_recorder, rhs.mock_obj_recorder);
  }

  friend inline void swap(TestFixture& v1, TestFixture& v2) { v1.swap(v2); }

  virtual ~TestFixture() { verify(); }

  utils::U8String fixtureName() { return fixture_name_; }

  /** setup fixture
   */
  void setup() {
    doSetup();
    checkSetup();
    abortIfFailure();
  }

  /** check if setup() did the right things
   *
   * we need checkSetup() because HasFatalFailure() will return true only when
   * failure is raised in a subroutine before HasFatalFailure() call
   */
  virtual void checkSetup() {}

  void verify() {
    if (!mock_obj_recorder.verify()) {
      std::cerr << fixture_name_ << ": fixture verify failed." << std::endl;
    }
  }

 protected:
  void abortIfFailure() {
    if (::testing::Test::HasFatalFailure())
      throw std::logic_error(fixture_name_);
  }

 private:
  virtual void doSetup() {}

  utils::U8String fixture_name_;

 protected:
  MockObjectRecorder mock_obj_recorder{};
};

#define LINE_NUMBER_STR_(x) #x
#define LINE_NUMBER_STR(x) LINE_NUMBER_STR_(x)
#define FIXTURE_LOCATION __FILE__ ":" LINE_NUMBER_STR(__LINE__)

#define FixtureHelper(FixtureType, var)    \
  FixtureType var{FIXTURE_LOCATION, this}; \
  var.setup();

template <typename F>
class TestFixtureLoader {
 public:
  template <typename... Args>
  TestFixtureLoader(Args&&... args)
      : fixture_{std::forward<Args>(args)...} {
    setup();
  }

  F* operator->() { return &fixture_; }

 private:
  void setup() { fixture_.setup(); }

  F fixture_;
};

#endif  // TEST_TESTUTILS_GMOCK_COMMON_H_
