// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_MOCK_KBNODE_MANAGER_H_
#define INCLUDE_CORE_MOCK_KBNODE_MANAGER_H_

#include <vector>

#include "core/kbnode_manager.h"

namespace snailcore {
namespace tests {

class MockKbNodeManager : public KbNodeManager {
 public:
  MOCK_METHOD1(createTreeItemProvider,
               std::shared_ptr<ITreeItemProvider>(IKbNode* root_kbnode));
  MOCK_METHOD1(idToKbNode, IKbNode*(KbNodeIdType kbnode_id));
  MOCK_METHOD2(findKbNode,
               std::vector<IKbNode*>(const utils::U8String& pattern,
                                     const IKbNode* parent_node));
  MOCK_METHOD1(childItems, std::vector<IKbNode*>(const IKbNode* parent_node));
  MOCK_METHOD3(addKbNode,
               IKbNode*(const utils::U8String& name, const IKbNode* parent,
                        bool is_category));
  MOCK_METHOD1(incRef, void(IKbNode* kbnode));
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_CORE_MOCK_KBNODE_MANAGER_H_
