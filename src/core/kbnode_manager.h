// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_KBNODE_MANAGER_H_
#define SRC_CORE_KBNODE_MANAGER_H_

#include <memory>
#include <vector>
#include <unordered_map>

#include "include/config.h"
#include "utils/basic_utils.h"
#include "utils/u8string.h"
#include "utils/signal_slot_impl.h"
#include "core/kb_global.h"

#include FTO_HEADER(core, kbnode_manager)

namespace snailcore {

class IKbNode;
class ITreeItemProvider;

/** A Node tree is a tree of nodes with name and short descriptions
 *
 */
class KbNodeManager : public FTO_INTERFACE(KbNodeManager) {
 public:
  KbNodeManager();
  virtual ~KbNodeManager();

  std::shared_ptr<ITreeItemProvider>
  createTreeItemProvider(IKbNode* root_kbnode);
  IKbNode* idToKbNode(KbNodeIdType kbnode_id);
  std::vector<IKbNode*> findKbNode(const utils::U8String& pattern,
                                   const IKbNode* parent_kbnode);
  std::vector<IKbNode*> childItems(const IKbNode* parent_node);
  IKbNode* addKbNode(const utils::U8String& name,
                     const IKbNode* parent,
                     bool is_category = false);
  void incRef(IKbNode* kbnode);

 private:
  SNAIL_DISABLE_COPY(KbNodeManager);

  SNAIL_SIGSLOT_IMPL(KbNodeAdded, void(const IKbNode* new_kbnode,
                                       const IKbNode* parent_kbnode));

  KbNodeIdType nextId();
  void erase(IKbNode* parent);

  std::unordered_map<KbNodeIdType, IKbNode*> id_to_kbnode_;
  std::unordered_map<const IKbNode*, std::vector<IKbNode*> > kbnode_to_subnodes_; // NOLINT

  KbNodeIdType id { 0 };

  IKbNode* dummy_root_ { nullptr };
};


}  // namespace snailcore

#endif  // SRC_CORE_KBNODE_MANAGER_H_
