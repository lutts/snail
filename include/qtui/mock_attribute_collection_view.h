// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_MOCK_ATTRIBUTE_COLLECTION_VIEW_H_
#define INCLUDE_QTUI_MOCK_ATTRIBUTE_COLLECTION_VIEW_H_

#include "qtui/i_attribute_collection_view.h"

class MockAttributeCollectionView : public IAttributeCollectionView {
 public:
  SNAIL_MOCK_SLOT(UserSwitchMode);
  SNAIL_MOCK_SLOT(UserMayClickAddAttribute);

  MOCK_METHOD1(setQModel, void(IAttributeCollectionQModel* qmodel));
  MOCK_METHOD1(setAttributeDelegate, void(IAttributeDelegate* attr_delegate));
  MOCK_METHOD1(openAttributeEditor, void(int row));
  MOCK_METHOD1(closeAttributeEditor, void(int row));

  MOCK_METHOD0(switchToDisplayMode, void());
  MOCK_METHOD0(switchToEditMode, void());
  MOCK_METHOD1(setSwitchModelButtonEnabled, void(bool enabled));
};

#endif  // INCLUDE_QTUI_MOCK_ATTRIBUTE_COLLECTION_VIEW_H_
