// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_CANDIDATE_ITEM_H_
#define I_CANDIDATE_ITEM_H_

#include "utils/u8string.h"

namespace snailcore {

class CandidateItem {
 public:
  explicit CandidateItem() { }
  CandidateItem(
      utils::U8String text,
      utils::U8String description,
      CandidateItem* parent = nullptr)
      : text_(text)
      , description_(description)
      , parent_(parent) {
    if (parent_) {
      parent_->addChild(this);
    }
  }

  virtual ~CandidateItem() {
    if (parent_)
      parent_->removeChild(this);

    removeAllChilddren();
  }

  utils::U8String text() const {
    return text_;
  }

  utils::U8String description() const {
    return description_;
  }

  bool isSelectable() const {
    return selectable_;
  }

  void setUnselectable() {
    selectable_ = false;
  }

  CandidateItem* parent() const {
    return parent_;
  }

  bool isRoot() const {
    return parent_ == nullptr;
  }

  const CandidateItem* operator[](int index) const {
    return at(index);
  }

  CandidateItem* operator[](int index) {
    return at(index);
  }

  int row() const {
    if (!parent_)
      return -1;

    int index = -1;
    for (auto item : parent_->children_) {
      if (item == this)
        break;

      ++index;
    }

    return index;
  }

  int num_children() const {
    return children_.size();
  }

 private:
  void addChild(CandidateItem* item) {
    children_.push_back(item);
  }

  void removeChild(CandidateItem* item) {
    // TODO(lutts): Lock
    auto iter = std::find(children_.begin(),
                          children_.end(),
                          item);
    if (iter != children_.end()) {
      children_.erase(iter);
      item->parent_ = nullptr;
    }
  }

  void removeAllChilddren() {
    // TODO(lutts): Lock
    while(!children_.empty()) {
      auto item = *children_.begin();
      removeChild(item);
      delete item;
    }
  }

  CandidateItem* at(int index) const {
    if (index > static_cast<int>(children_.size()))
      return nullptr;

    return children_[index];
  }

  utils::U8String text_;
  utils::U8String description_;
  CandidateItem* parent_ { nullptr };
  bool selectable_ { true };

  std::vector<CandidateItem*> children_;
};

}  // namespace snailcore

#endif  // I_CANDIDATE_ITEM_H_
