// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_WORK_H_
#define SRC_CORE_WORK_H_

#include <vector>

#include "include/config.h"
#include FTO_HEADER(core, work)
#include "utils/u8string.h"
#include "utils/signal_slot.h"

#include "snail/i_attribute_supplier.h"

namespace snailcore {

class IAttributeSupplier;
class WorkSignalProxy;

class Work : public FTO_NAMESPACE::Work {
 public:
  Work();
  virtual ~Work();

  bool set_name(const utils::U8String& new_name);
  const utils::U8String& name() const;
  std::vector<IAttributeSupplier*> attributeSuppliers() const;

  void setAttributeSuppliers(
      std::vector<std::unique_ptr<IAttributeSupplier> >&& attr_suppliers);

 public:
  SNAIL_SIGSLOT_NONVIRTUAL(NameChanged,
                           void(const utils::U8String& new_name));

 private:
  std::unique_ptr<WorkSignalProxy> signal_proxy_;

 private:
  Work(const Work& other) = delete;
  Work& operator=(const Work& other) = delete;

  utils::U8String name_;
  std::vector<std::unique_ptr<IAttributeSupplier> > attr_suppliers_;
};


}  // namespace snailcore

#endif  // SRC_CORE_WORK_H_
