// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_MOCK_TREE_ITEM_PROVIDER_H_
#define INCLUDE_SNAIL_MOCK_TREE_ITEM_PROVIDER_H_

#include <vector>

#include "snail/i_tree_item_provider.h"

namespace snailcore {
namespace tests {

class MockTreeItemProvider : public ITreeItemProvider {
 public:
  SNAIL_MOCK_SLOT(BeginFilter);
  SNAIL_MOCK_SLOT(FinishFilter);
  SNAIL_MOCK_SLOT(ItemAdded);

  MOCK_CONST_METHOD0(name, utils::U8String());
  MOCK_METHOD1(setFilterPattern, void(const utils::U8String& filter_pattern));
  MOCK_CONST_METHOD0(getFilterPattern, utils::U8String());
  MOCK_CONST_METHOD0(isFilterMode, bool());
  MOCK_CONST_METHOD0(getRootItem, ITreeItem*());
  MOCK_CONST_METHOD1(
      childItems, std::unique_ptr<IChildItemIterator>(ITreeItem* parent_item));
};

class TreeItemProviderListener
    : public xtestutils::SimpleMockListener<ITreeItemProvider> {
 public:
  SNAIL_MOCK_LISTENER0(TreeItemProviderListener, BeginFilter, void());
  SNAIL_MOCK_LISTENER0(TreeItemProviderListener, FinishFilter, void());
  SNAIL_MOCK_LISTENER2(TreeItemProviderListener, ItemAdded,
                       void(const ITreeItem* new_item,
                            const ITreeItem* parent_item));

  explicit TreeItemProviderListener(ITreeItemProvider* subject)
      : SimpleMockListener(subject) {
    SNAIL_MOCK_LISTENER_REGISTER(BeginFilter, this);
    SNAIL_MOCK_LISTENER_REGISTER(FinishFilter, this);
    SNAIL_MOCK_LISTENER_REGISTER(ItemAdded, this);

    attach();
  }

  ~TreeItemProviderListener() { detatch(); }
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_SNAIL_MOCK_TREE_ITEM_PROVIDER_H_
