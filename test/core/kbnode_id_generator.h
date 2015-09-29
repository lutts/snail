// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef TEST_CORE_KBNODE_ID_GENERATOR_H_
#define TEST_CORE_KBNODE_ID_GENERATOR_H_

#include <cstdlib>

#include "snail/kb_global.h"

namespace snailcore {
namespace tests {

class KbNodeIdGenerator {
 public:
  static KbNodeIdType gen() {
    static KbNodeIdType last_id = 0;
    return ++last_id;
  }
};


}  // namespace tests
}  // namespace snailcore

#endif  // TEST_CORE_KBNODE_ID_GENERATOR_H_
