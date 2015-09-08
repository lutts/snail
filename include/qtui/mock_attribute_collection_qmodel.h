// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_ATTRIBUTE_COLLECTION_QMODEL_H_
#define MOCK_ATTRIBUTE_COLLECTION_QMODEL_H_

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
  MOCK_METHOD1(addAttributeIfSupplier, void(int row));
};

#endif  // MOCK_ATTRIBUTE_COLLECTION_QMODEL_H_
