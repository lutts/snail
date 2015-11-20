// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef FIXTURE_FACTORY_H_
#define FIXTURE_FACTORY_H_

template <typename F, typename T>
class CopyConstructFixtureFactory;

template <typename F, typename T>
class CopyAssignmentFixtureFactory;

template <typename F, typename T>
class MoveConstructorFixtureFactory;

template <typename F, typename T>
class MoveAssignmentFixtureFactory;

template <typename F, typename T>
class FixtureFactory {
 public:
  virtual ~FixtureFactory() = default;

  using FixtureType = F;
  using TestCaseType = T;

  virtual F* createFixture(const utils::U8String& name, T* test_case) {
    auto fixture = new F(name, test_case);
    fixture->setup();
    return fixture;
  }

  static FixtureFactory<F, T>* getInstance() {
    static FixtureFactory<F, T> fixture_factory_;
    return &fixture_factory_;
  }

  static FixtureFactory<F, T>* getCopyConstructFactory() {
    static CopyConstructFixtureFactory<F, T> copy_ctor_factory_;
    return &copy_ctor_factory_;
  }

  static FixtureFactory<F, T>* getCopyAssignmentFactory() {
    static CopyAssignmentFixtureFactory<F, T> copy_assign_factory_;
    return &copy_assign_factory_;
  }

  static FixtureFactory<F, T>* getMoveConstructFactory() {
    static MoveConstructorFixtureFactory<F, T> move_ctor_factory_;
    return &move_ctor_factory_;
  }

  static FixtureFactory<F, T>* getMoveAssignmentFactory() {
    static MoveAssignmentFixtureFactory<F, T> move_assign_factory_;
    return &move_assign_factory_;
  }
};

template <typename F, typename T>
class CopyConstructFixtureFactory : public FixtureFactory<F, T> {
 public:
  F* createFixture(const utils::U8String& name, T* test_case) override {
    F base_fixture{name, test_case};
    base_fixture.setup();

    auto fixture = new F(base_fixture);
    fixture->setup();
    return fixture;
  }
};

template <typename F, typename T>
class CopyAssignmentFixtureFactory : public FixtureFactory<F, T> {
 public:
  F* createFixture(const utils::U8String& name, T* test_case) override {
    F base_fixture{name, test_case};
    base_fixture.setup();

    auto fixture = new F{name, test_case};
    fixture->setup();

    *fixture = base_fixture;
    fixture->checkSetup();

    return fixture;
  }
};

template <typename F, typename T>
class MoveConstructorFixtureFactory : public FixtureFactory<F, T> {
 public:
  F* createFixture(const utils::U8String& name, T* test_case) override {
    F base_fixture{name, test_case};
    base_fixture.setup();

    auto fixture = new F(std::move(base_fixture));
    fixture->setup();
    return fixture;
  }
};

template <typename F, typename T>
class MoveAssignmentFixtureFactory : public FixtureFactory<F, T> {
 public:
  F* createFixture(const utils::U8String& name, T* test_case) override {
    F base_fixture{name, test_case};
    base_fixture.setup();

    auto fixture = new F("", test_case);
    fixture->setup();

    *fixture = std::move(base_fixture);
    fixture->checkSetup();

    return fixture;
  }
};

#endif  // FIXTURE_FACTORY_H_
