// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef COPY_MOVE_HELPER_H_
#define COPY_MOVE_HELPER_H_

/*
 * The Meaning of Copy
 *   * Equivalence
 *   * Independence
 *
 * Notes about Equivalence:
 *   * if the cloned object can pass all the tests, we think the cloned object
 *     is equal to the source object
 *       * NOTE: if your object has operator==, you must test it by yourself
 *
 * Notes about Independence:
 *   * after copy or move, the source fixture is destroyed, if the cloned object
 *     can act independently, we think it is independent with the source object.
 *   * It is hard to test whether after x=y, operations on x should not
 *     implicitly change the state of y
 *
 * Notes about object state after move operations
 *  1. move means we will not use the source object after move, so, if the
 *     source object can be successfully destroyed, and there's no resource
 *     leak, we can be sure the move is ok, and so we do not need to explicitly
 *     test the moved source object after move operation
 *  2. if you want to use the source object after move, may be what you are
 *     really need an copy operation
 *  3. if what you want is exchange the state of two object, may be what you are
 *     really need is swap operation
 *  3. currently we can only catch mock objects leak, because if a mock object
 *     is not freed, gmock will give an error.
 *  4. dynamic allocated non-mock objects' leak should be used carefully,
 *     because we can NOT tell whether it's freed. A guideline is never new any-
 *     thing in logic code, `new`s should only be allowed in factories.
 *
 * Note on state choosen
 *   SUT may have many states which different in essence, so, which state do we
 * use to test copy/move semantics?
 *   The short answer is: choose a typical one
 *   The long answer is: if you have time and energy, test every state
 */

#include <memory>

template <typename FixtureType, typename BaseFactory>
class CopyMoveFixtureHelper {
 public:
  virtual FixtureType* createFixture() { return BaseFactory::createFixture(); }
};

template <typename FixtureType, typename BaseFactory>
class CopyConstructFixtureHelper
    : public CopyMoveFixtureHelper<FixtureType, BaseFactory> {
 public:
  FixtureType* createFixture() override {
    std::unique_ptr<FixtureType> base_fixture{
        CopyMoveFixtureHelper<FixtureType, BaseFactory>::createFixture()};

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
  FixtureType* createFixture() override {
    std::unique_ptr<FixtureType> base_fixture{
        CopyMoveFixtureHelper<FixtureType, BaseFactory>::createFixture()};

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
  FixtureType* createFixture() override {
    std::unique_ptr<FixtureType> base_fixture{
        CopyMoveFixtureHelper<FixtureType, BaseFactory>::createFixture()};

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
  FixtureType* createFixture() override {
    std::unique_ptr<FixtureType> base_fixture{
        CopyMoveFixtureHelper<FixtureType, BaseFactory>::createFixture()};

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
  static std::vector<CopyMoveFixtureHelper<FixtureType, BaseFactory>*>
  fixtureHelpers(unsigned int enabled_tests = 0) {
    std::vector<CopyMoveFixtureHelper<FixtureType, BaseFactory>*> helpers;
    helpers.reserve(1 + 1 + 1 + 2 * sizeof...(Factories));

    helpers.push_back(getBaseHelper<BaseFactory>());

    if (enabled_tests & TEST_ENABLE_COPY_CONSTRUCT_TEST) {
      helpers.push_back(getCopyConstructHelper<BaseFactory>());
    }

    if (enabled_tests & TEST_ENABLE_COPY_ASSIGNMENT_TEST) {
      auto copy_assign_helpers = getCopyAssignHelpers<BaseFactory>();
      helpers.insert(helpers.end(), copy_assign_helpers.begin(),
                     copy_assign_helpers.end());
    }

    if (enabled_tests & TEST_ENABLE_MOVE_CONSTRUCT_TEST) {
      helpers.push_back(getMoveConstructHelper<BaseFactory>());
    }

    if (enabled_tests & TEST_ENABLE_MOVE_ASSIGNMENT_TEST) {
      auto move_assign_helpers = getMoveAssignHelpers<BaseFactory>();
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

  template <typename BaseFactory>
  static CopyMoveFixtureHelper<FixtureType, BaseFactory>*
  getCopyConstructHelper() {
    static CopyConstructFixtureHelper<FixtureType, BaseFactory> copy_helper;
    return &copy_helper;
  }

  template <typename BaseFactory>
  static CopyMoveFixtureHelper<FixtureType, BaseFactory>*
  getMoveConstructHelper() {
    static MoveConstructFixtureHelper<FixtureType, BaseFactory> move_helper;
    return &move_helper;
  }

  template <typename BaseFactory>
  static std::vector<CopyMoveFixtureHelper<FixtureType, BaseFactory>*>
  getCopyAssignHelpers() {
    return getCopyMoveAssignHelpers<CopyAssignmentFixtureHelper, BaseFactory,
                                    Factories...>();
  }

  template <typename BaseFactory>
  static std::vector<CopyMoveFixtureHelper<FixtureType, BaseFactory>*>
  getMoveAssignHelpers() {
    return getCopyMoveAssignHelpers<MoveAssignmentFixtureHelper, BaseFactory,
                                    Factories...>();
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
      fixture_.reset(fixture_factory->createFixture());
    }
  }

  F* operator->() { return fixture_.get(); }

  std::unique_ptr<F> fixture_;
};

#endif  // COPY_MOVE_HELPER_H_
