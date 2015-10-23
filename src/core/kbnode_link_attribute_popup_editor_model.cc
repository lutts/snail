// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/kbnode_link_attribute_popup_editor_model.h"
#include "core/i_kbnode_link_attribute.h"
#include "core/i_attribute_model_factory.h"

namespace snailcore {

KbNodeLinkAttributePopupEditorModel::KbNodeLinkAttributePopupEditorModel(
    fto::KbNodeLinkAttribute* attr,
    IAttributeModelFactory* attr_model_factory)
    : attr_(attr)
    , attr_model_factory_(attr_model_factory) { }

KbNodeLinkAttributePopupEditorModel::
~KbNodeLinkAttributePopupEditorModel() = default;

std::shared_ptr<IAttributeModel>
KbNodeLinkAttributePopupEditorModel::createValueKbNodeAttrModel() {
  value_attr_copy_ = attr_->valueAttr()->clone();
  return attr_model_factory_->createAttributeModel(value_attr_copy_);
}

ITreeItemProvider*
KbNodeLinkAttributePopupEditorModel::getLinkTypeProvider() const {
  return attr_->supplier()->getLinkTypeProvider();
}

IKbNode* KbNodeLinkAttributePopupEditorModel::getCurrentLinkType() const {
  return nullptr;
}

std::shared_ptr<IAttributeSetModel>
KbNodeLinkAttributePopupEditorModel::getCurrentLinkAttrSetModel() {
  return nullptr;
}

void KbNodeLinkAttributePopupEditorModel::setLinkType(IKbNode* link_type) {
  (void)link_type;
}

void KbNodeLinkAttributePopupEditorModel::editFinished() {
}


}  // namespace snailcore
