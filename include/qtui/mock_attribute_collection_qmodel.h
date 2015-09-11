// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_MOCK_ATTRIBUTE_COLLECTION_QMODEL_H_
#define INCLUDE_QTUI_MOCK_ATTRIBUTE_COLLECTION_QMODEL_H_

#include <vector>

#include "qtui/i_attribute_collection_qmodel.h"

class MockAttributeCollectionQModel : public IAttributeCollectionQModel {
 public:
  SNAIL_MOCK_SLOT(AttributeAdded);

  MOCK_METHOD1(setAttributeSuppliers,
               void(std::vector<IAttributeSupplier*> attr_suppliers));
  MOCK_METHOD0(aboutToSwitchMode, void());
  MOCK_METHOD0(switchToEditMode, void());
  MOCK_METHOD0(switchToDisplayMode, void());
  MOCK_CONST_METHOD0(attrRowCount, int());
  MOCK_METHOD1(mayAddAttributeIfSupplier, void(int row));

  MOCK_CONST_METHOD1(attrOfRow, IAttribute*(int row));
};

#endif  // INCLUDE_QTUI_MOCK_ATTRIBUTE_COLLECTION_QMODEL_H_
