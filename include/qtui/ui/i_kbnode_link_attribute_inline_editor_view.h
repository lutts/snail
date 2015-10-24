// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_I_KBNODE_LINK_ATTRIBUTE_INLINE_EDITOR_VIEW_H_
#define INCLUDE_QTUI_I_KBNODE_LINK_ATTRIBUTE_INLINE_EDITOR_VIEW_H_

#include "qtui/ui/generic_view_base.h"
#include "utils/signal_slot.h"

class QString;

class IKbNodeLinkAttributeInlineEditorView : public GenericViewBase {
 public:
  virtual ~IKbNodeLinkAttributeInlineEditorView() = default;

  SNAIL_SIGSLOT_PURE_VIRTUAL(UserClickShowPopupEditor, void());

  virtual void setValueText(const QString& value_text) = 0;
};


#endif  // INCLUDE_QTUI_I_KBNODE_LINK_ATTRIBUTE_INLINE_EDITOR_VIEW_H_
