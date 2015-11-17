// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/kbnode_link_attribute_inline_editor_model.h"
#include "core/fto_kbnode_link_attribute.h"
#include "core/i_kbnode_link_attribute_popup_editor_model_factory.h"

namespace snailcore {

KbNodeLinkAttributeInlineEditorModel::KbNodeLinkAttributeInlineEditorModel(
    fto::KbNodeLinkAttribute* attr,
    IKbNodeLinkAttributePopupEditorModelFactory* popup_editor_factory)
    : attr_(attr), popup_editor_factory_(popup_editor_factory) {}

KbNodeLinkAttributeInlineEditorModel::~KbNodeLinkAttributeInlineEditorModel() =
    default;

utils::U8String KbNodeLinkAttributeInlineEditorModel::valueText() const {
  return attr_->valueText();
}

std::shared_ptr<IKbNodeLinkAttributePopupEditorModel>
KbNodeLinkAttributeInlineEditorModel::createPopupEditorModel() {
  return popup_editor_factory_->createPopupEditorModel(attr_);
}

}  // namespace snailcore
