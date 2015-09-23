// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_KBNODE_PROVIDER_H_
#define INCLUDE_SNAIL_I_KBNODE_PROVIDER_H_

#include <vector>
#include <iterator>

#include "utils/u8string.h"
#include "utils/signal_slot.h"

namespace snailcore {

class IKbNode;

class IKbNodeProvider {
 public:
  class IChildNodeIterator {
   public:
    virtual ~IChildNodeIterator() = default;

    virtual bool hasNext() const = 0;
    virtual IKbNode* next() = 0;
  };

  struct KbNodeAddResult {
    IKbNode* new_kbnode;
    IKbNode* parent_kbnode;
  };

  virtual ~IKbNodeProvider() = default;

  SNAIL_SIGSLOT2(BeginFilter, void());
  SNAIL_SIGSLOT2(FinishFilter, void());

  // external
  virtual void setFilterPattern(const utils::U8String& filter_pattern) = 0;
  virtual bool isFilterMode() const = 0;
  virtual KbNodeAddResult addKbNode() = 0;

  virtual IChildNodeIterator* childNodes(IKbNode* parent_node) const = 0;

  // TODO(lutts): do we need to move these internal use API to include/core?
  // internal
  virtual void incRef(IKbNode* kbnode) = 0;
  virtual
  std::vector<IKbNode*> findKbNodeByName(const utils::U8String& name) = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_I_KBNODE_PROVIDER_H_
