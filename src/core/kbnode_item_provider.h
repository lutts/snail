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

#include "include/config.h"
#include "snail/i_tree_item_provider.h"
#include "utils/basic_utils.h"
#include "utils/i_trackable.h"
#include "utils/signal_slot_impl.h"

namespace snailcore {

class IKbNode;

FTO_BEGIN_NAMESPACE
class KbNodeManager;
FTO_END_NAMESPACE

class KbNodeItemProvider : public ITreeItemProvider
                         , public utils::ITrackable {
 public:
  KbNodeItemProvider(IKbNode* root_kbnode,
                 fto::KbNodeManager* node_manager);
  virtual ~KbNodeItemProvider();

  utils::U8String name() const override;
  void setFilterPattern(const utils::U8String& filter_pattern) override;
  utils::U8String getFilterPattern() const override;
  bool isFilterMode() const override;

  ITreeItem* getRootItem() const;

  std::unique_ptr<IChildItemIterator>
  childItems(ITreeItem* parent_item) const override;

  void itemAdded(const ITreeItem* new_item,
                 const ITreeItem* new_item_parent);

 private:
  SNAIL_DISABLE_COPY(KbNodeItemProvider);

  SNAIL_SIGSLOT_OVERRIDE(BeginFilter);
  SNAIL_SIGSLOT_OVERRIDE(FinishFilter);
  SNAIL_SIGSLOT_OVERRIDE(ItemAdded);

  fto::KbNodeManager* node_manager_;
  IKbNode* root_kbnode_ { nullptr };

  utils::U8String filter_pattern_;
  std::vector<IKbNode*> matched_kbnodes_;
};


}  // namespace snailcore

#endif  // SRC_CORE_KBNODE_ITEM_PROVIDER_H_
