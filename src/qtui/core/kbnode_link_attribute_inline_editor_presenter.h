// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_CORE_KBNODE_LINK_ATTRIBUTE_INLINE_EDITOR_PRESENTER_H_
#define SRC_QTUI_CORE_KBNODE_LINK_ATTRIBUTE_INLINE_EDITOR_PRESENTER_H_

#include <memory>  // std::shared_ptr

#include "pfmvp/pf_presenter.h"
#include "snail/i_kbnode_link_attribute_inline_editor_model.h"
#include "qtui/i_kbnode_link_attribute_inline_editor_view.h"

using KbNodeLinkAttributeInlineEditorPresenterBase =
    pfmvp::PfPresenterT<snailcore::IKbNodeLinkAttributeInlineEditorModel,
                        IKbNodeLinkAttributeInlineEditorView>;

class KbNodeLinkAttributeInlineEditorPresenter
    : public KbNodeLinkAttributeInlineEditorPresenterBase {
 public:
  KbNodeLinkAttributeInlineEditorPresenter(
      std::shared_ptr<model_type> model,
      std::shared_ptr<view_type> view)
      : KbNodeLinkAttributeInlineEditorPresenterBase(model, view) {
  }

  void initialize() override;

 private:
  SNAIL_DISABLE_COPY(KbNodeLinkAttributeInlineEditorPresenter);
};

#endif  // SRC_QTUI_CORE_KBNODE_LINK_ATTRIBUTE_INLINE_EDITOR_PRESENTER_H_
