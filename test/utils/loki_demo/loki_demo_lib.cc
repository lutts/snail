// Copyright (c) 2016
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#include "loki_demo_factory.h"
#include "loki_demo_lib.h"

#include <iostream>

class SillySoldier : public Soldier {
  void printName() override { std::cout << "silly soldier" << std::endl; }
};
class SillyMonster : public Monster {
  void printName() override { std::cout << "silly monster" << std::endl; }
};
class SillySuperMonster : public SuperMonster {
  void printName() override {
    std::cout << "silly supper monster" << std::endl;
  }
};

class BadSoldier : public Soldier {
  void printName() override { std::cout << "bad soldier" << std::endl; }
};
class BadMonster : public Monster {
  void printName() override { std::cout << "bad monster" << std::endl; }
};
class BadSuperMonster : public SuperMonster {
  void printName() override { std::cout << "bad super monster" << std::endl; }
};

typedef Loki::ConcreteFactory<AbstractEnemyFactory, Loki::OpNewFactoryUnit,
                              LOKI_TYPELIST_3(SillySoldier, SillyMonster,
                                              SillySuperMonster)>
    EasyLevelEnemyFactory;

typedef Loki::ConcreteFactory<AbstractEnemyFactory, Loki::OpNewFactoryUnit,
                              LOKI_TYPELIST_3(BadSoldier, BadMonster,
                                              BadSuperMonster)>
    HardLevelEnemyFactory;

AbstractEnemyFactory* getSillyFactory() { return new EasyLevelEnemyFactory; }

AbstractEnemyFactory* getBadFactory() { return new HardLevelEnemyFactory; }
