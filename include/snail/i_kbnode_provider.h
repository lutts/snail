// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_KBNODE_PROVIDER_H_
#define I_KBNODE_PROVIDER_H_

#include <vector>

#include "utils/u8string.h"
#include "utils/signal_slot.h"

namespace snailcore {

class IKbNode;

class IKbNodeProvider {
 public:
  virtual ~IKbNodeProvider() = default;

  SNAIL_SIGSLOT2(BeginFilter, void());
  SNAIL_SIGSLOT2(FinishFilter, void());

  // external
  virtual void setFilterPattern(const utils::U8String& filter_pattern) = 0;
  virtual IKbNode* addKbNode() = 0;

  // TODO(lutts): do we need to move these internal use API to include/core?
  // internal
  virtual void incRef(IKbNode* kbnode) = 0;
  virtual
  std::vector<IKbNode*> findKbNodeByName(const utils::U8String& name) = 0;
};

}  // namespace snailcore

#endif  // I_KBNODE_PROVIDER_H_
