// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_MOCK_KBNODE_H_
#define INCLUDE_SNAIL_MOCK_KBNODE_H_

#include "snail/i_kbnode.h"

namespace snailcore {
namespace tests {

class MockKbNode : public IKbNode {
 public:
  MOCK_CONST_METHOD0(name, utils::U8String());
  MOCK_CONST_METHOD0(isCategory, bool());
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_SNAIL_MOCK_KBNODE_H_
