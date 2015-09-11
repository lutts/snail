// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_I_ATTRIBUTE_COLLECTION_QMODEL_H_
#define INCLUDE_QTUI_I_ATTRIBUTE_COLLECTION_QMODEL_H_

#include <vector>

#include "utils/signal_slot.h"

namespace snailcore {
class IAttribute;
class IAttributeSupplier;
}  // namespace snailcore

using snailcore::IAttribute;
using snailcore::IAttributeSupplier;

class IAttributeCollectionQModel {
 public:
  virtual ~IAttributeCollectionQModel() = default;

  SNAIL_SIGSLOT2(AttributeAdded, void(int row));

  virtual void
  setAttributeSuppliers(std::vector<IAttributeSupplier*> attr_suppliers) = 0;
  virtual void aboutToSwitchMode() = 0;
  virtual void switchToEditMode() = 0;
  virtual void switchToDisplayMode() = 0;
  virtual int attrRowCount() const = 0;
  virtual void mayAddAttributeIfSupplier(int row) = 0;

  virtual IAttribute* attrOfRow(int row) const { (void)row; return nullptr; }
};

#endif  // INCLUDE_QTUI_I_ATTRIBUTE_COLLECTION_QMODEL_H_
