// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_I_KBNODE_MANAGER_H_
#define INCLUDE_CORE_I_KBNODE_MANAGER_H_

#include <memory>
#include <vector>

#include "utils/u8string.h"
#include "snail/kb_global.h"

namespace snailcore {

class IKbNode;
class IKbNodeProvider;

class IKbNodeManager {
 public:
  virtual ~IKbNodeManager() = default;

  virtual std::shared_ptr<IKbNodeProvider>
  createKbNodeProvider(IKbNode* root_kbnode) = 0;
  virtual IKbNode* idToKbNode(KbNodeIdType kbnode_id) = 0;
  virtual std::vector<IKbNode*> findKbNode(const utils::U8String& pattern,
                                           const IKbNode* parent_node) = 0;
  virtual std::vector<IKbNode*> childNodes(const IKbNode* parent_node) = 0;
  virtual IKbNode* addKbNode(const utils::U8String& name,
                             const IKbNode* parent,
                             bool is_category = false) = 0;
  virtual void incRef(IKbNode* kbnode) = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_CORE_I_KBNODE_MANAGER_H_
