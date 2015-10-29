// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_UI_MOCK_KBNODE_LINK_ATTRIBUTE_INLINE_EDITOR_VIEW_H_
#define INCLUDE_QTUI_UI_MOCK_KBNODE_LINK_ATTRIBUTE_INLINE_EDITOR_VIEW_H_

#include "qtui/ui/i_kbnode_link_attribute_inline_editor_view.h"

class MockKbNodeLinkAttributeInlineEditorView
    : public IKbNodeLinkAttributeInlineEditorView {
 public:
  SNAIL_MOCK_SLOT(UserClickShowPopupEditor);

  MOCK_METHOD1(setValueText, void(const QString& value_text));
};

#endif  // INCLUDE_QTUI_UI_MOCK_KBNODE_LINK_ATTRIBUTE_INLINE_EDITOR_VIEW_H_
