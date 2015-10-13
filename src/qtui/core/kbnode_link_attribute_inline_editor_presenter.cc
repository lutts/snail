// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/qtui/core/kbnode_link_attribute_inline_editor_presenter.h"

#include <QString>

#include "snail/i_kbnode_link_attribute_popup_editor_model.h"

void KbNodeLinkAttributeInlineEditorPresenter::initialize() {
  view()->setValueText(U8StringToQString(model()->valueText()));

  view()->whenUserClickShowPopupEditor(
      [this]() {
        auto popup_editor_model = model()->createPopupEditorModel();
        showDialog(popup_editor_model);
      },
      shared_from_this());
}
