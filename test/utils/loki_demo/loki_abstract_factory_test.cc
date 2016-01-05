//-*- TestCaseName: LokiAbstractFactoryTest;-*-
// Copyright (c) 2016
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"

#include <memory>
#include <typeinfo>
#include "loki/AbstractFactory.h"

class Soldier {
 public:
  virtual ~Soldier() {}
};
class Monster {
 public:
  virtual ~Monster() {}
};
class SuperMonster {
 public:
  virtual ~SuperMonster() {}
};

class SillySoldier : public Soldier {};
class SillyMonster : public Monster {};
class SillySuperMonster : public SuperMonster {};

class BadSoldier : public Soldier {};
class BadMonster : public Monster {};
class BadSuperMonster : public SuperMonster {};

#ifndef LOKI_DISABLE_TYPELIST_MACROS

typedef Loki::AbstractFactory<LOKI_TYPELIST_3(Soldier, Monster, SuperMonster)>
    AbstractEnemyFactory;

typedef Loki::ConcreteFactory<AbstractEnemyFactory, Loki::OpNewFactoryUnit,
                              LOKI_TYPELIST_3(SillySoldier, SillyMonster,
                                              SillySuperMonster)>
    EasyLevelEnemyFactory;

typedef Loki::ConcreteFactory<AbstractEnemyFactory, Loki::OpNewFactoryUnit,
                              LOKI_TYPELIST_3(BadSoldier, BadMonster,
                                              BadSuperMonster)>
    HardLevelEnemyFactory;

#else  // LOKI_DISABLE_TYPELIST_MACROS

typedef Loki::AbstractFactory<Seq<Soldier, Monster, SuperMonster>::Type>
    AbstractEnemyFactory;

typedef Loki::ConcreteFactory<
    AbstractEnemyFactory, Loki::OpNewFactoryUnit,
    Seq<SillySoldier, SillyMonster, SillySuperMonster>::Type>
    EasyLevelEnemyFactory;

typedef Loki::ConcreteFactory<
    AbstractEnemyFactory, Loki::OpNewFactoryUnit,
    Seq<BadSoldier, BadMonster, BadSuperMonster>::Type> HardLevelEnemyFactory;

#endif

using namespace Loki;

class LokiAbstractFactoryTest : public ::testing::Test {
 public:
  LokiAbstractFactoryTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~LokiAbstractFactoryTest() { }
  void SetUp() override {}
  // void TearDown() override { }
};

TEST_F(LokiAbstractFactoryTest, should_construct_properly) {  // NOLINT
  std::unique_ptr<AbstractEnemyFactory> easyFactory(new EasyLevelEnemyFactory);

  Soldier* s;

  s = easyFactory->Create<Soldier>();
  ASSERT_TRUE(!!(typeid(*s) == typeid(SillySoldier)));
  delete s;

  std::unique_ptr<AbstractEnemyFactory> hardFactory(new HardLevelEnemyFactory);

  s = hardFactory->Create<Soldier>();
  ASSERT_TRUE(!!(typeid(*s) == typeid(BadSoldier)));
  delete s;
}
