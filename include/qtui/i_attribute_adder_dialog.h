// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_ATTRIBUTE_ADDER_DIALOG_H_
#define I_ATTRIBUTE_ADDER_DIALOG_H_

#include "src/qtui/generic_view_base.h"
#include "utils/signal_slot.h"
#include "utils/u8string.h"

class IAttributeListQModel;
class IAttributeEditor;

class IAttributeAdderDialog : public GenericViewBase {
 public:
  virtual ~IAttributeAdderDialog() = default;

  SNAIL_SIGSLOT2(CurrentAttributeIndexChanged, void(int index));
  SNAIL_SIGSLOT2(AddButtonClicked, void());

  virtual void setPrompt(const utils::U8String& prompt) = 0;
  virtual void setAttributeListQModel(
      IAttributeListQModel* attr_list_qmodel) = 0;
  virtual void setCurrentAttributeIndex(int index) = 0;
  virtual void setAttributeEditor(IAttributeEditor* attr_editor) = 0;
  virtual void removeAttributeEditor(IAttributeEditor* attr_editor) = 0;
  virtual void setDoneButtonEnabled(bool enabled) = 0;
  virtual void setAddButtonEnabled(bool enabled) = 0;
};


#endif  // I_ATTRIBUTE_ADDER_DIALOG_H_
