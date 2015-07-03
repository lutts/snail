// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_MOCK_ATTRIBUTE_SELECTOR_QMODEL_H_
#define INCLUDE_QTUI_MOCK_ATTRIBUTE_SELECTOR_QMODEL_H_

#include <vector>
#include "qtui/i_attribute_selector_qmodel.h"

class MockAttributeSelectorQModel : public IAttributeSelectorQModel {
 public:
  MOCK_METHOD1(setAttributeList,
               void(std::vector<snailcore::IAttribute*> attr_list));
};

#endif  // INCLUDE_QTUI_MOCK_ATTRIBUTE_SELECTOR_QMODEL_H_
