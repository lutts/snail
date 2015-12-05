// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef TEST_TESTUTILS_COPY_MOVE_TEST_HELPER_H_
#define TEST_TESTUTILS_COPY_MOVE_TEST_HELPER_H_

namespace xtestutils {

/*
 * The Meaning of Copy
 *   * Equivalence
 *   * Independence
 *
 * Notes about Equivalence Test:
 *   * if the cloned object and source object pass the same test cases, we
 * think the cloned object is equal to the source object
 *     * NOTE: if your object has operator==, you must test it by yourself
 *
 * Notes about Independence Test:
 *   * after copy or move, the source fixture is destroyed, if the cloned object
 *     can act independently, we think it is independent with the source object.
 *   * It is hard to test whether after x=y, operations on x should not
 *     implicitly change the state of y
 *
 * Notes about object state after move operations
 *  *. move means we will not use the source object after move, so, if the
 *     source object can be successfully destroyed, and there's no resource
 *     leak, we can be sure the move is ok, and so we do not need to explicitly
 *     test the moved source object after move operation
 *  *. if you want to use the source object after move, may be what you are
 *     really need an copy operation
 *  *. if what you want is exchange the state of two object, may be what you are
 *     really need is swap operation
 *  *. currently we can only catch mock objects leak, because if a mock object
 *     is not freed, gmock will give an error.
 *  *. dynamic allocated non-mock objects' leak should be used carefully,
 *     because we can NOT tell whether it's freed. A guideline is never new any-
 *     thing in logic code, `new`s should only be allowed in factories.
 *
 * Notes about objects who has many states which different in essence:
 *  * you may use "Testcase Class per Fixture" pattern
 *  * we can support copy/move between all these states, but may be lead to many
 * many tests, and if that makes the test slow, choose typical ones instead of
 * all
 *
 * Notes about why we use an external factory to create fixture instead of new
 * the fixture directly:
 *  * if the class under test has many states, and you want to test copy/move
 * between these states, you have two choice:
 *    1. use different fixture to represents different states
 *    2. use the same fixture, then use an external State object to represent
 * all states
 *
 *    choice 1 is a bad choice, because we must implement copy/move methods for
 * every other fixture in every fixture, so if we have 5 states, we will end
 * up with 20 copy/move methods in every fixture, and it also violate Open-Close
 * Principle, because if we want to add a new state, we must add copy/move
 * methods in every other fixtures.
 *
 *   choice 2 is a good choice, the fixture and state can implement copy/move
 * only once. but this choice is not free. we must pay for an extra factory
 * class, and we must pay for an extra new operation of the state object. but
 * these are cheap operations which are acceptable.
 */

#include <memory>
#include <vector>
#include <type_traits>
#include <iostream>  // NOLINT

template <typename FixtureType>
class TestFixtureHelper {
 public:
  virtual FixtureType* createTestFixture() = 0;
};

template <typename FixtureType, typename BaseFactory>
class CopyMoveFixtureHelper : public TestFixtureHelper<FixtureType> {
 public:
  virtual FixtureType* createTestFixture() {
    return BaseFactory::createFixture();
  }
};

template <typename FixtureType, typename BaseFactory>
class CopyConstructFixtureHelper
    : public CopyMoveFixtureHelper<FixtureType, BaseFactory> {
 public:
  FixtureType* createTestFixture() override {
    std::cout << "test copy construct......" << std::endl;

    std::unique_ptr<FixtureType> base_fixture{
        CopyMoveFixtureHelper<FixtureType, BaseFactory>::createTestFixture()};

    auto fixture = new FixtureType(*base_fixture);
    fixture->setup();

    return fixture;
  }
};

template <typename FixtureType, typename BaseFactory,
          typename CopyTargetFactory = BaseFactory>
class CopyAssignmentFixtureHelper
    : public CopyMoveFixtureHelper<FixtureType, BaseFactory> {
 public:
  FixtureType* createTestFixture() override {
    std::cout << "test copy assignment......" << std::endl;

    std::unique_ptr<FixtureType> base_fixture{
        CopyMoveFixtureHelper<FixtureType, BaseFactory>::createTestFixture()};

    auto fixture = CopyTargetFactory::createFixture();
    *fixture = *base_fixture;
    fixture->checkSetup();
    return fixture;
  }
};

template <typename FixtureType, typename BaseFactory>
class MoveConstructFixtureHelper
    : public CopyMoveFixtureHelper<FixtureType, BaseFactory> {
 public:
  FixtureType* createTestFixture() override {
    std::cout << "test move construct......" << std::endl;

    std::unique_ptr<FixtureType> base_fixture{
        CopyMoveFixtureHelper<FixtureType, BaseFactory>::createTestFixture()};

    auto fixture = new FixtureType(std::move(*base_fixture));
    fixture->setup();

    return fixture;
  }
};

template <typename FixtureType, typename BaseFactory,
          typename MoveTargetFactory = BaseFactory>
class MoveAssignmentFixtureHelper
    : public CopyMoveFixtureHelper<FixtureType, BaseFactory> {
 public:
  FixtureType* createTestFixture() override {
    std::cout << "test move assignment......" << std::endl;

    std::unique_ptr<FixtureType> base_fixture{
        CopyMoveFixtureHelper<FixtureType, BaseFactory>::createTestFixture()};

    auto fixture = MoveTargetFactory::createFixture();
    *fixture = std::move(*base_fixture);
    fixture->checkSetup();
    return fixture;
  }
};

#define TEST_ENABLE_COPY_CONSTRUCT_TEST (1 << 0)
#define TEST_ENABLE_COPY_ASSIGNMENT_TEST (1 << 1)
#define TEST_ENABLE_MOVE_CONSTRUCT_TEST (1 << 2)
#define TEST_ENABLE_MOVE_ASSIGNMENT_TEST (1 << 3)

template <typename FixtureType, typename... Factories>
class CopyMoveFixtureHelperGenerator {
 public:
  template <typename BaseFactory>
  static std::vector<TestFixtureHelper<FixtureType>*> fixtureHelpers(
      unsigned int enabled_tests = 0u) {
    std::vector<TestFixtureHelper<FixtureType>*> helpers;
    helpers.reserve(1 + 1 + 1 + 2 * sizeof...(Factories));

    helpers.push_back(getBaseHelper<BaseFactory>());

    if (enabled_tests & TEST_ENABLE_COPY_CONSTRUCT_TEST) {
      helpers.push_back(getCopyConstructHelper<FixtureType, BaseFactory>());
    }

    if (enabled_tests & TEST_ENABLE_COPY_ASSIGNMENT_TEST) {
      auto copy_assign_helpers =
          getCopyAssignHelpers<FixtureType, BaseFactory>();
      helpers.insert(helpers.end(), copy_assign_helpers.begin(),
                     copy_assign_helpers.end());
    }

    if (enabled_tests & TEST_ENABLE_MOVE_CONSTRUCT_TEST) {
      helpers.push_back(getMoveConstructHelper<FixtureType, BaseFactory>());
    }

    if (enabled_tests & TEST_ENABLE_MOVE_ASSIGNMENT_TEST) {
      auto move_assign_helpers =
          getMoveAssignHelpers<FixtureType, BaseFactory>();
      helpers.insert(helpers.end(), move_assign_helpers.begin(),
                     move_assign_helpers.end());
    }

    return helpers;
  }

  template <typename BaseFactory>
  static CopyMoveFixtureHelper<FixtureType, BaseFactory>* getBaseHelper() {
    static CopyMoveFixtureHelper<FixtureType, BaseFactory> base_helper;
    return &base_helper;
  }

  // copy construct
  template <typename F, typename BaseFactory>
  static CopyMoveFixtureHelper<
      typename std::enable_if<std::is_copy_constructible<F>::value, F>::type,
      BaseFactory>*
  getCopyConstructHelper() {
    static CopyConstructFixtureHelper<F, BaseFactory> copy_helper;
    return &copy_helper;
  }

  template <typename F, typename BaseFactory>
  static CopyMoveFixtureHelper<
      typename std::enable_if<!std::is_copy_constructible<F>::value, F>::type,
      BaseFactory>*
  getCopyConstructHelper() {
    throw std::logic_error(
        "copy construct test enabled, but fixture does not allow copy "
        "construct");
    return nullptr;
  }

  // move construct
  template <typename F, typename BaseFactory>
  static CopyMoveFixtureHelper<
      typename std::enable_if<std::is_move_constructible<F>::value, F>::type,
      BaseFactory>*
  getMoveConstructHelper() {
    static MoveConstructFixtureHelper<F, BaseFactory> move_helper;
    return &move_helper;
  }

  template <typename F, typename BaseFactory>
  static CopyMoveFixtureHelper<
      typename std::enable_if<!std::is_move_constructible<F>::value, F>::type,
      BaseFactory>*
  getMoveConstructHelper() {
    throw std::logic_error(
        "move construct test enabled, but fixture does not allow move "
        "construct");
    return nullptr;
  }

  // copy assign
  template <typename F, typename BaseFactory>
  static std::vector<CopyMoveFixtureHelper<
      typename std::enable_if<std::is_copy_assignable<F>::value, F>::type,
      BaseFactory>*>
  getCopyAssignHelpers() {
    return getCopyMoveAssignHelpers<CopyAssignmentFixtureHelper, BaseFactory,
                                    Factories...>();
  }

  template <typename F, typename BaseFactory>
  static std::vector<CopyMoveFixtureHelper<
      typename std::enable_if<!std::is_copy_assignable<F>::value, F>::type,
      BaseFactory>*>
  getCopyAssignHelpers() {
    throw std::logic_error(
        "copy assign test enabled, but fixture does not allow copy assign");
    return std::vector<CopyMoveFixtureHelper<F, BaseFactory>*>();
  }

  // move assign
  template <typename F, typename BaseFactory>
  static std::vector<CopyMoveFixtureHelper<
      typename std::enable_if<std::is_move_assignable<F>::value, F>::type,
      BaseFactory>*>
  getMoveAssignHelpers() {
    return getCopyMoveAssignHelpers<MoveAssignmentFixtureHelper, BaseFactory,
                                    Factories...>();
  }

  template <typename F, typename BaseFactory>
  static std::vector<CopyMoveFixtureHelper<
      typename std::enable_if<!std::is_move_assignable<F>::value, F>::type,
      BaseFactory>*>
  getMoveAssignHelpers() {
    throw std::logic_error(
        "move assign test enabled, but fixture does not allow move assign");
    return std::vector<CopyMoveFixtureHelper<F, BaseFactory>*>();
  }

 private:
  template <template <typename, typename, typename> class H,
            typename BaseFactory, typename F, typename... OtherF>
  static std::vector<CopyMoveFixtureHelper<FixtureType, BaseFactory>*>
  getCopyMoveAssignHelpers() {
    static H<FixtureType, BaseFactory, F> helper;

    std::vector<CopyMoveFixtureHelper<FixtureType, BaseFactory>*> helpers;
    helpers.push_back(&helper);

    auto other_helpers = getCopyMoveAssignHelpers<H, BaseFactory, OtherF...>();
    helpers.insert(helpers.end(), other_helpers.begin(), other_helpers.end());

    return helpers;
  }

  template <template <typename, typename, typename> class H,
            typename BaseFactory>
  static std::vector<CopyMoveFixtureHelper<FixtureType, BaseFactory>*>
  getCopyMoveAssignHelpers() {
    return std::vector<CopyMoveFixtureHelper<FixtureType, BaseFactory>*>();
  }
};

template <typename F, typename T>
class FixtureLoaderFromHelper {
 public:
  explicit FixtureLoaderFromHelper(T* test_case) {
    const ::testing::TestInfo* const test_info =
        ::testing::UnitTest::GetInstance()->current_test_info();
    if (test_info->value_param()) {
      auto fixture_factory = test_case->GetParam();
      fixture_.reset(fixture_factory->createTestFixture());
    }
  }

  F* get() { return fixture_.get(); }
  F* release() { return fixture_.release(); }
  F* operator->() { return fixture_.get(); }

 private:
  std::unique_ptr<F> fixture_;
};

}  // namespace xtestutils

#endif  // TEST_TESTUTILS_COPY_MOVE_TEST_HELPER_H_
