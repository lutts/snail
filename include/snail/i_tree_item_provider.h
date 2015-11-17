// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_TREE_ITEM_PROVIDER_H_
#define INCLUDE_SNAIL_I_TREE_ITEM_PROVIDER_H_

#include <vector>
#include <iterator>

#include "utils/u8string.h"
#include "utils/signal_slot.h"

namespace snailcore {

class ITreeItem;

class ITreeItemProvider {
 public:
  class IChildItemIterator {
   public:
    virtual ~IChildItemIterator() = default;

    virtual bool hasNext() const = 0;
    virtual ITreeItem* next() = 0;
  };

  virtual ~ITreeItemProvider() = default;

  SNAIL_SIGSLOT_PURE_VIRTUAL(BeginFilter, void());
  SNAIL_SIGSLOT_PURE_VIRTUAL(FinishFilter, void());
  SNAIL_SIGSLOT_PURE_VIRTUAL(ItemAdded, void(const ITreeItem* new_item,
                                             const ITreeItem* parent_item));

  // external
  virtual utils::U8String name() const = 0;
  virtual void setFilterPattern(const utils::U8String& filter_pattern) = 0;
  virtual utils::U8String getFilterPattern() const = 0;
  virtual bool isFilterMode() const = 0;
  virtual ITreeItem* getRootItem() const = 0;
  virtual std::unique_ptr<IChildItemIterator> childItems(
      ITreeItem* parent_item) const = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_I_TREE_ITEM_PROVIDER_H_
