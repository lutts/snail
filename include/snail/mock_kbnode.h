// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_KBNODE_H_
#define MOCK_KBNODE_H_

#include "snail/i_kbnode.h"

namespace snailcore {
namespace tests {

class MockKbNode : public IKbNode {
 public:
  MOCK_CONST_METHOD0(name, utils::U8String());
};

}  // namespace tests
}  // namespace snailcore

#endif  // MOCK_KBNODE_H_
