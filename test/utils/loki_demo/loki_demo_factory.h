// Copyright (c) 2016
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef LOKI_DEMO_FACTORY_H_
#define LOKI_DEMO_FACTORY_H_

#include "loki/AbstractFactory.h"

class Soldier;
class Monster;
class SuperMonster;

#ifndef LOKI_DISABLE_TYPELIST_MACROS

typedef Loki::AbstractFactory<LOKI_TYPELIST_3(Soldier, Monster, SuperMonster)>
    AbstractEnemyFactory;

#else  // LOKI_DISABLE_TYPELIST_MACROS

typedef Loki::AbstractFactory<Seq<Soldier, Monster, SuperMonster>::Type>
    AbstractEnemyFactory;

#endif

extern AbstractEnemyFactory* getSillyFactory();
extern AbstractEnemyFactory* getBadFactory();

#endif  // LOKI_DEMO_FACTORY_H_
