// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_KBNODE_PROVIDER_H_
#define MOCK_KBNODE_PROVIDER_H_

#include "snail/i_kbnode_provider.h"

namespace snailcore {
namespace tests {

class MockKbNodeProvider : public IKbNodeProvider {
 public:
  SNAIL_MOCK_SLOT(BeginFilter);
  SNAIL_MOCK_SLOT(FinishFilter);

  MOCK_METHOD1(setFilterPattern, void(const utils::U8String& filter_pattern));
  MOCK_METHOD0(addKbNode, IKbNode*());
  MOCK_CONST_METHOD0(totalKbNodes, int());
};

}  // namespace tests
}  // namespace snailcore

#endif  // MOCK_KBNODE_PROVIDER_H_
