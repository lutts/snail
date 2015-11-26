// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef TEST_TESTUTILS_FIXTURE_FACTORY_H_
#define TEST_TESTUTILS_FIXTURE_FACTORY_H_

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
 *
 * Concrete FixtureFactory use SourceFixtureType and TargetFixtureType to
 * support copy/move between different state fixtures
 */
template <typename TargetFixtureType, typename TestCase,
          typename SourceFixtureType = TargetFixtureType>
class CopyConstructFixtureFactory;

template <typename TargetFixtureType, typename TestCase,
          typename SourceFixtureType = TargetFixtureType>
class CopyAssignmentFixtureFactory;

template <typename TargetFixtureType, typename TestCase,
          typename SourceFixtureType = TargetFixtureType>
class MoveConstructorFixtureFactory;

template <typename TargetFixtureType, typename TestCase,
          typename SourceFixtureType = TargetFixtureType>
class MoveAssignmentFixtureFactory;

template <typename FixtureType, typename TestCase>
class FixtureFactory {
 public:
  virtual ~FixtureFactory() = default;

  virtual FixtureType* createFixture(const utils::U8String& name,
                                     TestCase* test_case) {
    auto fixture = new FixtureType(name, test_case);
    fixture->setup();
    return fixture;
  }

  static FixtureFactory<FixtureType, TestCase>* getInstance() {
    static FixtureFactory<FixtureType, TestCase> fixture_factory_;
    return &fixture_factory_;
  }

  template <typename SourceFixtureType = FixtureType>
  static FixtureFactory<FixtureType, TestCase>* getCopyConstructFactory() {
    static CopyConstructFixtureFactory<FixtureType, TestCase, SourceFixtureType>
        copy_ctor_factory_;
    return &copy_ctor_factory_;
  }

  template <typename SourceFixtureType = FixtureType>
  static FixtureFactory<FixtureType, TestCase>* getCopyAssignmentFactory() {
    static CopyAssignmentFixtureFactory<FixtureType, TestCase,
                                        SourceFixtureType> copy_assign_factory_;
    return &copy_assign_factory_;
  }

  template <typename SourceFixtureType = FixtureType>
  static FixtureFactory<FixtureType, TestCase>* getMoveConstructFactory() {
    static MoveConstructorFixtureFactory<FixtureType, TestCase,
                                         SourceFixtureType> move_ctor_factory_;
    return &move_ctor_factory_;
  }

  template <typename SourceFixtureType = FixtureType>
  static FixtureFactory<FixtureType, TestCase>* getMoveAssignmentFactory() {
    static MoveAssignmentFixtureFactory<FixtureType, TestCase,
                                        SourceFixtureType> move_assign_factory_;
    return &move_assign_factory_;
  }
};

template <typename TargetFixtureType, typename TestCase,
          typename SourceFixtureType>
class CopyConstructFixtureFactory
    : public FixtureFactory<TargetFixtureType, TestCase> {
 public:
  TargetFixtureType* createFixture(const utils::U8String& name,
                                   TestCase* test_case) override {
    SourceFixtureType base_fixture{name, test_case};
    base_fixture.setup();

    auto fixture = new TargetFixtureType(base_fixture);
    fixture->setup();
    return fixture;
  }
};

template <typename TargetFixtureType, typename TestCase,
          typename SourceFixtureType>
class CopyAssignmentFixtureFactory
    : public FixtureFactory<TargetFixtureType, TestCase> {
 public:
  TargetFixtureType* createFixture(const utils::U8String& name,
                                   TestCase* test_case) override {
    SourceFixtureType base_fixture{name, test_case};
    base_fixture.setup();

    auto fixture = new TargetFixtureType{name, test_case};
    fixture->setup();

    *fixture = base_fixture;
    fixture->checkSetup();

    return fixture;
  }
};

template <typename TargetFixtureType, typename TestCase,
          typename SourceFixtureType>
class MoveConstructorFixtureFactory
    : public FixtureFactory<TargetFixtureType, TestCase> {
 public:
  TargetFixtureType* createFixture(const utils::U8String& name,
                                   TestCase* test_case) override {
    SourceFixtureType base_fixture{name, test_case};
    base_fixture.setup();

    auto fixture = new TargetFixtureType(std::move(base_fixture));
    fixture->setup();
    return fixture;
  }
};

template <typename TargetFixtureType, typename TestCase,
          typename SourceFixtureType>
class MoveAssignmentFixtureFactory
    : public FixtureFactory<TargetFixtureType, TestCase> {
 public:
  TargetFixtureType* createFixture(const utils::U8String& name,
                                   TestCase* test_case) override {
    SourceFixtureType base_fixture{name, test_case};
    base_fixture.setup();

    auto fixture = new TargetFixtureType("", test_case);
    fixture->setup();

    *fixture = std::move(base_fixture);
    fixture->checkSetup();

    return fixture;
  }
};

template <typename F, typename T>
class GlobalFixtureLoader {
 public:
  explicit GlobalFixtureLoader(T* test_case) {
    const ::testing::TestInfo* const test_info =
        ::testing::UnitTest::GetInstance()->current_test_info();
    if (test_info->value_param()) {
      auto fixture_factory = test_case->GetParam();
      fixture_.reset(
          fixture_factory->createFixture("GlobalFixture", test_case));
    }
  }

  F* operator->() { return fixture_.get(); }

  std::unique_ptr<F> fixture_;
};

#endif  // TEST_TESTUTILS_FIXTURE_FACTORY_H_
