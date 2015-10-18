// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_VIEW_H_
#define MOCK_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_VIEW_H_

#include "qtui/i_kbnode_link_attribute_popup_editor_view.h"

class MockKbNodeLinkAttributePopupEditorView
    : public IKbNodeLinkAttributePopupEditorView {
 public:
  MOCK_METHOD1(showView, bool(bool modal));

  SNAIL_MOCK_SLOT(UserSelectLinkType);
  SNAIL_MOCK_SLOT(UserClickDone);

  MOCK_METHOD1(setValueAttrEditor, void(IAttributeEditorView* value_attr_view));
  MOCK_METHOD1(setLinkTypeQModel, void(IKbNodeTreeQModel* link_type_qmodel));
  MOCK_METHOD1(setCurrentLinkType,
               void(const QModelIndex& current_link_type_index));
  MOCK_METHOD1(setLinkAttributeSetView,
               void(IAttributeSetView* link_attr_set_view));
  MOCK_METHOD1(setDoneButtonEnabled, void(bool enabled));
};

#endif  // MOCK_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_VIEW_H_
