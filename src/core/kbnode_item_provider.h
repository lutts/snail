// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_KBNODE_ITEM_PROVIDER_H_
#define SRC_CORE_KBNODE_ITEM_PROVIDER_H_

#include <vector>
#include <unordered_map>

#include "snail/i_tree_item_provider.h"
#include "utils/basic_utils.h"
#include "utils/signal_slot_impl.h"

namespace snailcore {

class IKbNode;
class KbNodeManager;

class KbNodeItemProvider : public ITreeItemProvider {
 public:
  KbNodeItemProvider(IKbNode* root_kbnode,
                 KbNodeManager* node_manager);
  virtual ~KbNodeItemProvider();

  utils::U8String name() const override;
  void setFilterPattern(const utils::U8String& filter_pattern) override;
  utils::U8String getFilterPattern() const override;
  bool isFilterMode() const override;

  ITreeItem* getRootItem() const;

  std::unique_ptr<IChildItemIterator>
  childItems(ITreeItem* parent_item) const override;

 private:
  SNAIL_DISABLE_COPY(KbNodeItemProvider);

  SNAIL_SIGSLOT_IMPL(BeginFilter);
  SNAIL_SIGSLOT_IMPL(FinishFilter);

  KbNodeManager* node_manager_;
  IKbNode* root_kbnode_ { nullptr };

  utils::U8String filter_pattern_;
  std::vector<IKbNode*> matched_kbnodes_;
};


}  // namespace snailcore

#endif  // SRC_CORE_KBNODE_ITEM_PROVIDER_H_
