// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_I_ATTRIBUTE_SET_LAYOUT_H_
#define INCLUDE_QTUI_I_ATTRIBUTE_SET_LAYOUT_H_

#include <vector>

#include "utils/signal_slot.h"

namespace snailcore {
class IAttribute;
class IAttributeSupplier;
}  // namespace snailcore

class IAttributeEditorView;

class IAttributeSetLayout {
 public:
  virtual ~IAttributeSetLayout() = default;

  SNAIL_SIGSLOT2(CreateAttrEditor,
                 IAttributeEditorView*(snailcore::IAttribute* attr));
  SNAIL_SIGSLOT2(CloseAttributeEditors, void());

  virtual void setAttributeSuppliers(
      const std::vector<snailcore::IAttributeSupplier*>& attr_suppliers,
      bool edit_mode) = 0;
};

#endif  // INCLUDE_QTUI_I_ATTRIBUTE_SET_LAYOUT_H_
