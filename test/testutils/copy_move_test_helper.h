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

template <typename FixtureType, typename... Factories>
class CopyMoveFixtureHelperGenerator {
 public:
  template <typename BaseFactory>
  class BasicFixtureHelper : public TestFixtureHelper<FixtureType> {
   public:
    FixtureType* createTestFixture() override {
      return BaseFactory::createFixture();
    }

    static TestFixtureHelper<FixtureType>* getInstance() {
      static BasicFixtureHelper<BaseFactory> helper;
      return &helper;
    }
  };

  template <template <typename, typename, typename> class DerivedT,
            typename BaseFactory, typename TargetFactory,
            typename ConcreteFixtureType>
  class GenericCustomFixtureHelper : public TestFixtureHelper<FixtureType> {
   public:
    FixtureType* createTestFixture() override {
      std::unique_ptr<ConcreteFixtureType> base_fixture{
          BaseFactory::createFixture()};

      auto fixture = TargetFactory::createFixture();

      doCustomOperation(fixture, base_fixture.get());

      fixture->checkSetup();
      return fixture;
    }

    static constexpr bool shouldTestForEachTargetFactory() { return true; }

    static TestFixtureHelper<FixtureType>* getInstance() {
      static DerivedT<BaseFactory, TargetFactory, ConcreteFixtureType> helper;
      return &helper;
    }

   private:
    virtual void doCustomOperation(ConcreteFixtureType* target_fixture,
                                   ConcreteFixtureType* base_fixture) = 0;
  };

  template <typename BaseFactory, typename unusedT,
            typename ConcreteFixtureType>
  class CopyConstructFixtureHelper : public TestFixtureHelper<FixtureType> {
   public:
    FixtureType* createTestFixture() override {
      std::cout << "test copy construct......" << std::endl;

      std::unique_ptr<ConcreteFixtureType> base_fixture{
          BaseFactory::createFixture()};

      auto fixture = new ConcreteFixtureType(*base_fixture);
      fixture->setup();

      return fixture;
    }

    static constexpr bool shouldTestForEachTargetFactory() { return false; }

    static TestFixtureHelper<FixtureType>* getInstance() {
      static CopyConstructFixtureHelper<BaseFactory, unusedT,
                                        ConcreteFixtureType> helper;
      return &helper;
    }
  };

  template <typename BaseFactory, typename CopyTargetFactory,
            typename ConcreteFixtureType>
  class CopyAssignmentFixtureHelper
      : public GenericCustomFixtureHelper<CopyAssignmentFixtureHelper,
                                          BaseFactory, CopyTargetFactory,
                                          ConcreteFixtureType> {
   private:
    void doCustomOperation(ConcreteFixtureType* target_fixture,
                           ConcreteFixtureType* base_fixture) {
      std::cout << "test copy assignment......" << std::endl;
      *target_fixture = *base_fixture;
    }
  };

  template <typename BaseFactory, typename unusedT,
            typename ConcreteFixtureType>
  class MoveConstructFixtureHelper : public TestFixtureHelper<FixtureType> {
   public:
    FixtureType* createTestFixture() override {
      std::cout << "test move construct......" << std::endl;

      std::unique_ptr<ConcreteFixtureType> base_fixture{
          BaseFactory::createFixture()};

      auto fixture = new ConcreteFixtureType(std::move(*base_fixture));
      fixture->setup();

      return fixture;
    }

    static constexpr bool shouldTestForEachTargetFactory() { return false; }

    static TestFixtureHelper<FixtureType>* getInstance() {
      static MoveConstructFixtureHelper<BaseFactory, unusedT,
                                        ConcreteFixtureType> helper;
      return &helper;
    }
  };

  template <typename BaseFactory, typename MoveTargetFactory,
            typename ConcreteFixtureType>
  class MoveAssignmentFixtureHelper
      : public GenericCustomFixtureHelper<MoveAssignmentFixtureHelper,
                                          BaseFactory, MoveTargetFactory,
                                          ConcreteFixtureType> {
   private:
    void doCustomOperation(ConcreteFixtureType* target_fixture,
                           ConcreteFixtureType* base_fixture) {
      std::cout << "test move assignment......" << std::endl;

      *target_fixture = std::move(*base_fixture);
    }
  };

  template <typename BaseFactory, typename ConcreteFixtureType = FixtureType,
            template <typename, typename, typename> class... Helpers>
  static std::vector<TestFixtureHelper<FixtureType>*> fixtureHelpers() {
    std::vector<TestFixtureHelper<FixtureType>*> helpers;

    constexpr int num_base_helpers = 1;
    constexpr int num_helpers = sizeof...(Helpers) * sizeof...(Factories);
    helpers.reserve(num_base_helpers + num_helpers);

    helpers.push_back(BasicFixtureHelper<BaseFactory>::getInstance());

    getHelpers<BaseFactory, ConcreteFixtureType, Helpers...>(&helpers);

    return helpers;
  }

 private:
  template <typename BaseFactory, typename ConcreteFixtureType>
  static void getHelpers(
      std::vector<TestFixtureHelper<FixtureType>*>* helpers) {
    (void)helpers;
  }

  template <typename BaseFactory, typename ConcreteFixtureType,
            template <typename, typename, typename> class Helper,
            template <typename, typename, typename> class... OtherHelpers>
  static void getHelpers(
      std::vector<TestFixtureHelper<FixtureType>*>* helpers) {
    getHelpersImpl<BaseFactory, ConcreteFixtureType, Helper, Factories...>(
        helpers);

    getHelpers<BaseFactory, ConcreteFixtureType, OtherHelpers...>(helpers);
  }

  template <typename BaseFactory, typename ConcreteFixtureType,
            template <typename, typename, typename> class Helper,
            typename TargetFactory, typename... OtherF>
  static void getHelpersImpl(
      std::vector<TestFixtureHelper<FixtureType>*>* helpers) {
    using HelperType = Helper<BaseFactory, TargetFactory, ConcreteFixtureType>;
    helpers->push_back(HelperType::getInstance());
    if (HelperType::shouldTestForEachTargetFactory()) {
      getHelpersImpl<BaseFactory, ConcreteFixtureType, Helper, OtherF...>(
          helpers);
    }
  }

  template <typename BaseFactory, typename ConcreteFixtureType,
            template <typename, typename, typename> class Helper>
  static void getHelpersImpl(
      std::vector<TestFixtureHelper<FixtureType>*>* helpers) {
    (void)helpers;
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
