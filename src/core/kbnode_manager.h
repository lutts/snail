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

#include "test_config.h"
#include "utils/basic_utils.h"
#include "utils/u8string.h"
#include "snail/kb_global.h"

namespace snailcore {

class IKbNode;
class ITreeItemProvider;
class KbNodeManagerImpl;

class KbNodeManager {
 public:
  KbNodeManager();
  virtual ~KbNodeManager();

  MOCKPREFIX std::shared_ptr<ITreeItemProvider>
  createTreeItemProvider(IKbNode* root_kbnode) MOCKPOSTFIX;

  MOCKPREFIX IKbNode* idToKbNode(KbNodeIdType kbnode_id) MOCKPOSTFIX;

  MOCKPREFIX std::vector<IKbNode*> findKbNode(const utils::U8String& pattern,
                                   const IKbNode* parent_kbnode) MOCKPOSTFIX;

  MOCKPREFIX std::vector<IKbNode*> childItems(const IKbNode* parent_node) MOCKPOSTFIX;

  MOCKPREFIX IKbNode* addKbNode(const utils::U8String& name,
                     const IKbNode* parent,
                     bool is_category = false) MOCKPOSTFIX;

  MOCKPREFIX void incRef(IKbNode* kbnode) MOCKPOSTFIX;

 private:
  SNAIL_DISABLE_COPY(KbNodeManager);

  std::unique_ptr<KbNodeManagerImpl> impl_;
  friend class KbNodeManagerImpl;
};


}  // namespace snailcore

#endif  // SRC_CORE_KBNODE_MANAGER_H_
