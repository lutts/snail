// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_I_FEW_ENTITY_ATTR_EDITOR_VIEW_H_
#define INCLUDE_QTUI_I_FEW_ENTITY_ATTR_EDITOR_VIEW_H_

#include "qtui/i_attribute_editor_view.h"
#include "utils/signal_slot.h"
#include "utils/u8string.h"

class ICandidateItemQModelAdapter;

class IFewEntityAttrEditorView : public IAttributeEditorView {
 public:
  virtual ~IFewEntityAttrEditorView() = default;

  SNAIL_SIGSLOT2(EntitySelectionChanged, void(void* item_ptr));

  virtual void setEntitySelectorQModel(ICandidateItemQModelAdapter* model) = 0;
  virtual void setCurrentEntityName(const utils::U8String& entity_name) = 0;
};


#endif  // INCLUDE_QTUI_I_FEW_ENTITY_ATTR_EDITOR_VIEW_H_
