// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_KBNODE_PROVIDER_H_
#define SRC_CORE_KBNODE_PROVIDER_H_

#include <vector>
#include <unordered_map>

#include "snail/i_tree_item_provider.h"
#include "utils/basic_utils.h"
#include "utils/signal_slot_impl.h"

namespace snailcore {

class IKbNodeManager;

class TreeItemProvider : public ITreeItemProvider {
 public:
  TreeItemProvider(IKbNode* root_kbnode,
                 IKbNodeManager* node_manager);
  virtual ~TreeItemProvider();

  utils::U8String name() const override;
  void setFilterPattern(const utils::U8String& filter_pattern) override;
  utils::U8String getFilterPattern() const override;
  bool isFilterMode() const override;

  IKbNode* getRootItem() const { return nullptr; }  // TODO(lutts): impl this

  std::unique_ptr<IChildItemIterator>
  childItems(IKbNode* parent_node) const override;

 private:
  SNAIL_DISABLE_COPY(TreeItemProvider);

  SNAIL_SIGSLOT_IMPL(BeginFilter);
  SNAIL_SIGSLOT_IMPL(FinishFilter);

  IKbNodeManager* node_manager_;
  IKbNode* root_kbnode_ { nullptr };

  utils::U8String filter_pattern_;
  std::vector<IKbNode*> matched_kbnodes_;
};


}  // namespace snailcore

#endif  // SRC_CORE_KBNODE_PROVIDER_H_
