// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_ATTRIBUTE_DELEGATE_H_
#define MOCK_ATTRIBUTE_DELEGATE_H_

#include "qtui/i_attribute_delegate.h"

class MockAttributeDelegate : public IAttributeDelegate {
 public:
  SNAIL_MOCK_SLOT(CreateEditorFor);
  SNAIL_MOCK_SLOT(CloseEditor);
};

#endif  // MOCK_ATTRIBUTE_DELEGATE_H_
