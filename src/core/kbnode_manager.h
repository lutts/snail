// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_KBNODE_MANAGER_H_
#define SRC_CORE_KBNODE_MANAGER_H_

#include <vector>
#include <unordered_map>

#include "core/i_kbnode_manager.h"
#include "utils/basic_utils.h"

namespace snailcore {

class KbNodeManager : public IKbNodeManager {
 public:
  KbNodeManager();
  virtual ~KbNodeManager();

  std::shared_ptr<IKbNodeProvider>
  createKbNodeProvider(IKbNode* root_kbnode) override;
  IKbNode* idToKbNode(KbNodeIdType kbnode_id) override;
  std::vector<IKbNode*> findKbNode(const utils::U8String& pattern,
                                   const IKbNode* parent_kbnode) override;
  std::vector<IKbNode*> childNodes(const IKbNode* parent_node) override;
  IKbNode* addKbNode(const utils::U8String& name,
                     const IKbNode* parent,
                     bool is_category) override;
  void incRef(IKbNode* kbnode) override;

 private:
  SNAIL_DISABLE_COPY(KbNodeManager);

  KbNodeIdType nextId();
  void erase(IKbNode* parent);

  std::unordered_map<KbNodeIdType, IKbNode*> id_to_kbnode_;
  std::unordered_map<const IKbNode*, std::vector<IKbNode*> > kbnode_to_subnodes_; // NOLINT

  KbNodeIdType id { 0 };

  IKbNode* dummy_root_ { nullptr };
};


}  // namespace snailcore

#endif  // SRC_CORE_KBNODE_MANAGER_H_
