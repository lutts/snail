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

BEGIN_MOCK_LISTENER_DEF(TreeItemProviderListener, ITreeItemProvider)

MOCK_METHOD0(BeginFilter, void());
MOCK_METHOD0(FinishFilter, void());
MOCK_METHOD2(ItemAdded,
             void(const ITreeItem* new_item, const ITreeItem* parent_item));

BEGIN_BIND_SIGNAL(ITreeItemProvider)

BIND_SIGNAL0(BeginFilter, void);
BIND_SIGNAL0(FinishFilter, void);
BIND_SIGNAL2(ItemAdded, void, const ITreeItem*, new_item, const ITreeItem*,
             parent_item);

END_BIND_SIGNAL()
END_MOCK_LISTENER_DEF()

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_SNAIL_MOCK_TREE_ITEM_PROVIDER_H_
