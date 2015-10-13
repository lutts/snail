// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_KBNODE_LINK_ATTRIBUTE_INLINE_EDITOR_VIEW_H_
#define MOCK_KBNODE_LINK_ATTRIBUTE_INLINE_EDITOR_VIEW_H_

#include "qtui/i_kbnode_link_attribute_inline_editor_view.h"

class MockKbNodeLinkAttributeInlineEditorView
    : public IKbNodeLinkAttributeInlineEditorView {
 public:
  SNAIL_MOCK_SLOT(UserClickShowPopupEditor);

  MOCK_METHOD1(setValueText, void(const QString& value_text));
};

#endif  // MOCK_KBNODE_LINK_ATTRIBUTE_INLINE_EDITOR_VIEW_H_
