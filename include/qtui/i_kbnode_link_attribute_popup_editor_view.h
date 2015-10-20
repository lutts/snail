// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_I_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_VIEW_H_
#define INCLUDE_QTUI_I_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_VIEW_H_

#include <QModelIndex>

#include "qtui/generic_view_base.h"
#include "utils/signal_slot.h"

class IAttributeEditorView;
class IAttributeSetView;
class QAbstractItemModel;

class IKbNodeLinkAttributePopupEditorView : public GenericViewBase {
 public:
  virtual ~IKbNodeLinkAttributePopupEditorView() = default;

  SNAIL_SIGSLOT2(UserSelectLinkType, void(const QModelIndex& index));
  SNAIL_SIGSLOT2(UserClickDone, void());

  virtual void setValueAttrEditor(IAttributeEditorView* value_attr_view) = 0;
  virtual void setLinkTypeQModel(QAbstractItemModel* link_type_qmodel) = 0;
  virtual void setCurrentLinkType(
      const QModelIndex& current_link_type_index) = 0;
  virtual void setLinkAttributeSetView(
      IAttributeSetView* link_attr_set_view) = 0;
  virtual void setDoneButtonEnabled(bool enabled) = 0;
};

#endif  // INCLUDE_QTUI_I_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_VIEW_H_
