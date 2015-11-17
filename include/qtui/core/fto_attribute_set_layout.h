// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_CORE_FTO_ATTRIBUTE_SET_LAYOUT_H_
#define INCLUDE_QTUI_CORE_FTO_ATTRIBUTE_SET_LAYOUT_H_

#include <vector>

#include "include/config.h"
#include "utils/signal_slot.h"

namespace snailcore {
class IAttribute;
class IAttributeSupplier;
}  // namespace snailcore

class IAttributeEditorView;

namespace fto {

#ifndef DISABLE_TEST_CODE

class AttributeSetLayout {
 public:
  virtual ~AttributeSetLayout() = default;

  SNAIL_SIGSLOT_PURE_VIRTUAL(
      CreateAttrEditor, IAttributeEditorView*(snailcore::IAttribute* attr));
  SNAIL_SIGSLOT_PURE_VIRTUAL(CloseAttributeEditors, void());

  virtual void setAttributeSuppliers(
      const std::vector<snailcore::IAttributeSupplier*>& attr_suppliers,
      bool edit_mode) = 0;
};

#else  // DISABLE_TEST_CODE

class AttributeSetLayout {};

#endif  // DISABLE_TEST_CODE

}  // namespace fto

#endif  // INCLUDE_QTUI_CORE_FTO_ATTRIBUTE_SET_LAYOUT_H_
