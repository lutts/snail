// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_KBNODE_PROVIDER_H_
#define I_KBNODE_PROVIDER_H_

#include "utils/u8string.h"
#include "utils/signal_slot.h"

namespace snailcore {

class IKbNode;

class IKbNodeProvider {
 public:
  virtual ~IKbNodeProvider() = default;

  SNAIL_SIGSLOT2(BeginFilter, void());
  SNAIL_SIGSLOT2(FinishFilter, void());

  virtual void setFilterPattern(const utils::U8String& filter_pattern) = 0;
  virtual IKbNode* addKbNode() = 0;
  virtual int totalKbNodes() const = 0;
};

}  // namespace snailcore

#endif  // I_KBNODE_PROVIDER_H_
