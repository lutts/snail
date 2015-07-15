// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_ATTRIBUTE_VIEW_H_
#define MOCK_ATTRIBUTE_VIEW_H_

#include "qtui/i_attribute_view.h"

class MockAttributeView : public IAttributeView {
 public:
  // IPfView mocks
  MOCK_METHOD1(showView, bool(bool modal));
};

#endif  // MOCK_ATTRIBUTE_VIEW_H_
