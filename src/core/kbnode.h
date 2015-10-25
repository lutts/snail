// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_KBNODE_H_
#define SRC_CORE_KBNODE_H_

#include "core/i_kbnode.h"
#include "utils/basic_utils.h"

namespace snailcore {

class KbNode : public IKbNode {
 public:
  KbNode(KbNodeIdType id, const utils::U8String& name, bool is_category = false)
      : id_(id), name_(name), is_category_(is_category) { }
  virtual ~KbNode() = default;

  KbNodeIdType id() const override { return id_; }
  utils::U8String name() const override { return name_; }
  bool isGroupOnly() const override { return is_category_; }

 private:
  SNAIL_DISABLE_COPY(KbNode);

  KbNodeIdType id_;
  utils::U8String name_;
  bool is_category_ { false };
};


}  // namespace snailcore

#endif  // SRC_CORE_KBNODE_H_
