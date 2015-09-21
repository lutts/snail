// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef KBNODE_ID_GENERATOR_H_
#define KBNODE_ID_GENERATOR_H_

#include <cstdlib>

#include "core/kb_global.h"

namespace snailcore {
namespace tests {

class KbNodeIdGenerator {
 public:
  static KbNodeIdType gen() {
    return std::rand();
  }
};


}  // namespace tests
}  // namespace snailcore

#endif  // KBNODE_ID_GENERATOR_H_
