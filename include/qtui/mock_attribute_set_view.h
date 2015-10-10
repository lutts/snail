// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_MOCK_ATTRIBUTE_SET_VIEW_H_
#define INCLUDE_QTUI_MOCK_ATTRIBUTE_SET_VIEW_H_

#include "qtui/i_attribute_set_view.h"

class MockAttributeSetView : public IAttributeSetView {
 public:
  SNAIL_MOCK_SLOT(UserSwitchMode);

  MOCK_METHOD1(setSwitchModelButtonEnabled, void(bool enabled));
  MOCK_METHOD0(switchToEditMode, void());
  MOCK_METHOD0(switchToDisplayMode, void());
};

#endif  // INCLUDE_QTUI_MOCK_ATTRIBUTE_SET_VIEW_H_
