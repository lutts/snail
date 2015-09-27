// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_MOCK_KBNODE_MANAGER_H_
#define INCLUDE_CORE_MOCK_KBNODE_MANAGER_H_

#include "core/i_kbnode_manager.h"

namespace snailcore {
namespace tests {

class MockKbNodeManager : public IKbNodeManager {
 public:
  MOCK_METHOD1(createKbNodeProvider,
               std::shared_ptr<IKbNodeProvider>(KbNodeIdType root_kbnode_id));
  MOCK_METHOD1(idToKbNode, IKbNode*(KbNodeIdType kbnode_id));
  MOCK_METHOD1(findKbNode,
               std::vector<IKbNode*>(const utils::U8String& pattern));
  MOCK_METHOD1(childNodes, std::vector<IKbNode*>(IKbNode* parent_node));
  MOCK_METHOD2(addKbNode,
               IKbNode*(const utils::U8String& name, IKbNode* parent));
  MOCK_METHOD1(incRef, void(IKbNode* kbnode));
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_CORE_MOCK_KBNODE_MANAGER_H_
