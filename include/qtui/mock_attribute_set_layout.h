// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_MOCK_ATTRIBUTE_SET_LAYOUT_H_
#define INCLUDE_QTUI_MOCK_ATTRIBUTE_SET_LAYOUT_H_

#include <vector>

#include "qtui/i_attribute_set_layout.h"

class MockAttributeSetLayout : public IAttributeSetLayout {
 public:
  SNAIL_MOCK_SLOT(CreateAttrEditor);
  SNAIL_MOCK_SLOT(CloseAttributeEditors);

  MOCK_METHOD2(
      setAttributeSuppliers,
      void(const std::vector<snailcore::IAttributeSupplier*>& attr_suppliers,
           bool edit_mode));
};

#endif  // INCLUDE_QTUI_MOCK_ATTRIBUTE_SET_LAYOUT_H_
