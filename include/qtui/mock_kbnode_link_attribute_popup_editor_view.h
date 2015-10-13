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
};

#endif  // MOCK_KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_VIEW_H_
