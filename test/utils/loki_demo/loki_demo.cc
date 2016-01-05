// Copyright (c) 2016
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#include <memory>

#include "loki_demo_factory.h"
#include "loki_demo_lib.h"

int main() {
  std::unique_ptr<AbstractEnemyFactory> easyFactory(getSillyFactory());

  Soldier* s;

  s = easyFactory->Create<Soldier>();
  s->printName();
  delete s;

  std::unique_ptr<AbstractEnemyFactory> hardFactory(getBadFactory());

  s = hardFactory->Create<Soldier>();
  s->printName();
  delete s;
}
