// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_ATTRIBUTE_COLLECTION_VIEW_H_
#define MOCK_ATTRIBUTE_COLLECTION_VIEW_H_

#include "qtui/i_attribute_collection_view.h"

class MockAttributeCollectionView : public IAttributeCollectionView {
 public:
  SNAIL_MOCK_SLOT(SwitchToEditMode);
  SNAIL_MOCK_SLOT(SwitchToDisplayMode);
  SNAIL_MOCK_SLOT(UserClickAddAttribute);

  MOCK_METHOD1(setQModel, void(IAttributeCollectionQModel* qmodel));
  MOCK_METHOD1(setAttributeDelegate, void(IAttributeDelegate* attr_delegate));
  MOCK_METHOD1(openAttributeEditor, void(int row));
  MOCK_METHOD1(closeAttributeEditor, void(int row));
};

#endif  // MOCK_ATTRIBUTE_COLLECTION_VIEW_H_