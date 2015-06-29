// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_FEW_ENTITY_ATTR_EDITOR_VIEW_H_
#define I_FEW_ENTITY_ATTR_EDITOR_VIEW_H_

#include "qtui/i_attribute_editor_view.h"
#include "utils/signal_slot.h"

class QAbstractItemModel;

class IFewEntityAttrEditorView : public IAttributeEditorView {
 public:
  virtual ~IFewEntityAttrEditorView() = default;

  SNAIL_SIGSLOT2(EntitySelectionChanged, void(int index));

  virtual void setEntitySelectorQModel(QAbstractItemModel* model) = 0;
  virtual void setCurrentEntityIndex(int index) = 0;
};


#endif  // I_FEW_ENTITY_ATTR_EDITOR_VIEW_H_
