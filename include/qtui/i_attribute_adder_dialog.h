// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_I_ATTRIBUTE_ADDER_DIALOG_H_
#define INCLUDE_QTUI_I_ATTRIBUTE_ADDER_DIALOG_H_

#include "src/qtui/generic_view_base.h"
#include "utils/signal_slot.h"
#include "utils/u8string.h"

class IAttributeSelectorQModel;
class IAttributeEditorView;
class ICandidateItemQModelAdapter;

class IAttributeAdderDialog : public GenericViewBase {
 public:
  virtual ~IAttributeAdderDialog() = default;

  SNAIL_SIGSLOT2(CurrentAttributeChanged, void(void* item_ptr));
  SNAIL_SIGSLOT2(AddButtonClicked, void());

  virtual void setPrompt(const utils::U8String& prompt) = 0;
  virtual void setAttributeSelectorQModel(
      ICandidateItemQModelAdapter* model) = 0;
  virtual void setCurrentAttributeName(const utils::U8String& attr_name) = 0;
  virtual void setAttributeEditor(IAttributeEditorView* attr_editor) = 0;
  virtual void removeAttributeEditor(IAttributeEditorView* attr_editor) = 0;
  virtual void setDoneButtonEnabled(bool enabled) = 0;
  virtual void setAddButtonEnabled(bool enabled) = 0;
};


#endif  // INCLUDE_QTUI_I_ATTRIBUTE_ADDER_DIALOG_H_
