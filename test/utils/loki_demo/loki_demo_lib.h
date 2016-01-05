// Copyright (c) 2016
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef LOKI_DEMO_LIB_H_
#define LOKI_DEMO_LIB_H_

class Soldier {
 public:
  virtual ~Soldier() {}

  virtual void printName() = 0;
};

class Monster {
 public:
  virtual ~Monster() {}

  virtual void printName() = 0;
};
class SuperMonster {
 public:
  virtual ~SuperMonster() {}

  virtual void printName() = 0;
};

#endif  // LOKI_DEMO_LIB_H_
