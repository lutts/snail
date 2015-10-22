// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_I_WORK_H_
#define INCLUDE_CORE_I_WORK_H_

#include <vector>

#include "utils/u8string.h"
#include "utils/signal_slot.h"

namespace snailcore {

class IAttributeSupplier;

class Work {
 public:
  virtual ~Work() = default;

  SNAIL_SIGSLOT2(NameChanged, void(const utils::U8String& new_name));

  virtual bool set_name(const utils::U8String& name) = 0;
  virtual const utils::U8String& name() const = 0;

  virtual std::vector<IAttributeSupplier*> attributeSuppliers() const = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_CORE_I_WORK_H_
