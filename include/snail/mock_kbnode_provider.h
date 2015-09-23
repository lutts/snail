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
  MOCK_CONST_METHOD0(isFilterMode, bool());
  MOCK_METHOD0(addKbNode, KbNodeAddResult());
  MOCK_CONST_METHOD1(childNodes, IChildNodeIterator*(IKbNode* parent_node));

  MOCK_METHOD1(incRef, void(IKbNode* kbnode));
  MOCK_METHOD1(findKbNodeByName,
               std::vector<IKbNode*>(const utils::U8String& name));
};

}  // namespace tests
}  // namespace snailcore

#endif  // MOCK_KBNODE_PROVIDER_H_
