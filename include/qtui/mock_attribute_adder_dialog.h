// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_ATTRIBUTE_ADDER_DIALOG_H_
#define MOCK_ATTRIBUTE_ADDER_DIALOG_H_

#include "qtui/i_attribute_adder_dialog.h"

class MockAttributeAdderDialog : public IAttributeAdderDialog {
 public:
  MOCK_METHOD1(showView, bool(bool));

  SNAIL_MOCK_SLOT(CurrentAttributeIndexChanged);
  SNAIL_MOCK_SLOT(AddButtonClicked);

  MOCK_METHOD1(setPrompt, void(const utils::U8String& prompt));
  MOCK_METHOD1(setAttributeListQModel,
               void(IAttributeListQModel* attr_list_qmodel));
  MOCK_METHOD1(setCurrentAttributeIndex, void(int index));
  MOCK_METHOD1(setAttributeEditor, void(IAttributeEditor* attr_editor));
  MOCK_METHOD1(removeAttributeEditor, void(IAttributeEditor* attr_editor));
  MOCK_METHOD1(setDoneButtonEnabled, void(bool enabled));
  MOCK_METHOD1(setAddButtonEnabled, void(bool enabled));
};

#endif  // MOCK_ATTRIBUTE_ADDER_DIALOG_H_
