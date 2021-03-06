// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_FTO_KBNODE_MANAGER_H_
#define INCLUDE_CORE_FTO_KBNODE_MANAGER_H_

#include <memory>
#include <vector>

#ifndef DISABLE_TEST_CODE

#include "utils/u8string.h"
#include "core/kb_global.h"

namespace snailcore {

class IKbNode;
class ITreeItemProvider;

namespace fto {

class KbNodeManager {
 public:
  virtual ~KbNodeManager() = default;

  virtual std::shared_ptr<ITreeItemProvider> createTreeItemProvider(
      IKbNode* root_kbnode) = 0;
  virtual IKbNode* idToKbNode(KbNodeIdType kbnode_id) = 0;
  virtual std::vector<IKbNode*> findKbNode(const utils::U8String& pattern,
                                           const IKbNode* parent_node) = 0;
  virtual std::vector<IKbNode*> childItems(const IKbNode* parent_node) = 0;
  virtual IKbNode* addKbNode(const utils::U8String& name, const IKbNode* parent,
                             bool is_category) = 0;
};

}  // namespace fto
}  // namespace snailcore

#else  // DISABLE_TEST_CODE

#include "include/config.h"

namespace snailcore {
namespace fto {

class KbNodeManager {};

}  // namespace fto
}  // namespace snailcore

#endif  // DISABLE_TEST_CODE
#endif  // INCLUDE_CORE_FTO_KBNODE_MANAGER_H_
