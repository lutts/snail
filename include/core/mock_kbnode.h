// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_MOCK_KBNODE_H_
#define INCLUDE_CORE_MOCK_KBNODE_H_

#include "core/i_kbnode.h"

namespace snailcore {
namespace tests {

class MockKbNode : public IKbNode {
 public:
  MOCK_CONST_METHOD0(id, KbNodeIdType());
  MOCK_CONST_METHOD0(name, utils::U8String());
  MOCK_CONST_METHOD0(isGroupOnly, bool());

  MOCK_METHOD0(incRef, void());
  MOCK_METHOD0(decRef, void());
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_CORE_MOCK_KBNODE_H_
