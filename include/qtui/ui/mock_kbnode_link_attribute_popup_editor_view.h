// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_UI_MOCK_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_VIEW_H_
#define INCLUDE_QTUI_UI_MOCK_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_VIEW_H_

#include "qtui/ui/i_kbnode_link_attribute_popup_editor_view.h"

class MockKbNodeLinkAttributePopupEditorView
    : public IKbNodeLinkAttributePopupEditorView {
 public:
  MOCK_METHOD1(showView, bool(bool modal));

  SNAIL_MOCK_SLOT(UserSelectLinkType);
  SNAIL_MOCK_SLOT(UserClickDone);

  MOCK_METHOD1(setValueAttrName, void(const QString& value_attr_name));
  MOCK_METHOD1(setValueAttrEditor, void(IAttributeEditorView* value_attr_view));
  MOCK_METHOD1(setLinkTypeQModel, void(QAbstractItemModel* link_type_qmodel));
  MOCK_METHOD1(setCurrentLinkType,
               void(const QModelIndex& current_link_type_index));
  MOCK_METHOD1(setLinkAttributeSetView,
               void(IAttributeSetView* link_attr_set_view));
  MOCK_METHOD1(setDoneButtonEnabled, void(bool enabled));
};

#endif  // INCLUDE_QTUI_UI_MOCK_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_VIEW_H_
