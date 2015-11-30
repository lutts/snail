// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_FTO_WORK_H_
#define INCLUDE_CORE_FTO_WORK_H_

#include <vector>

#ifndef DISABLE_TEST_CODE

#include "utils/u8string.h"
#include "utils/signal_slot.h"

namespace snailcore {

class IAttributeSupplier;

namespace fto {

class Work {
 public:
  virtual ~Work() = default;

  SNAIL_SIGSLOT_PURE_VIRTUAL(NameChanged,
                             void(const utils::U8String& new_name));

  virtual bool set_name(const utils::U8String& name) = 0;
  virtual utils::U8String name() const = 0;

  virtual std::vector<IAttributeSupplier*> attributeSuppliers() const = 0;
};

}  // namespace fto
}  // namespace snailcore

#else  // DISABLE_TEST_CODE

#include "include/config.h"

namespace snailcore {
namespace fto {

class Work {};

}  // namespace fto
}  // namespace snailcore

#endif  // DISABLE_TEST_CODE
#endif  // INCLUDE_CORE_FTO_WORK_H_
