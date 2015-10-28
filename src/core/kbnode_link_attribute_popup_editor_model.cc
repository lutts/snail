// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/kbnode_link_attribute_popup_editor_model.h"
#include "core/fto_kbnode_link_attribute.h"
#include "core/i_attribute_model_factory.h"
#include "core/i_attribute_set_model_factory.h"

namespace snailcore {

KbNodeLinkAttributePopupEditorModel::KbNodeLinkAttributePopupEditorModel(
    fto::KbNodeLinkAttribute* attr,
    IAttributeModelFactory* attr_model_factory,
    IAttributeSetModelFactory* attr_set_model_factory)
    : attr_(attr)
    , attr_model_factory_(attr_model_factory)
    , attr_set_model_factory_(attr_set_model_factory) { }

KbNodeLinkAttributePopupEditorModel::
~KbNodeLinkAttributePopupEditorModel() = default;

KbNodeAttribute*
KbNodeLinkAttributePopupEditorModel::getCurrentValueAttr(){
  if (!value_attr_copy_initialized_) {
    value_attr_copy_ = *(attr_->valueAttr());
    value_attr_copy_initialized_ = true;
  }

  return &value_attr_copy_;
}

std::shared_ptr<IAttributeModel>
KbNodeLinkAttributePopupEditorModel::createValueAttrModel() {
  return attr_model_factory_->createAttributeModel(getCurrentValueAttr());
}

ITreeItemProvider*
KbNodeLinkAttributePopupEditorModel::getLinkTypeItemProvider() const {
  return attr_->supplier()->getLinkTypeItemProvider();
}

void KbNodeLinkAttributePopupEditorModel::mayInitCurrLinkType() {
  if (!curr_link_type_initialized_) {
    curr_link_type_ = *(attr_->linkType());
    curr_link_type_initialized_ = true;
  }
}

ITreeItem* KbNodeLinkAttributePopupEditorModel::getCurrentLinkType() const {
  auto that = const_cast<KbNodeLinkAttributePopupEditorModel*>(this);
  that->mayInitCurrLinkType();
  return &curr_link_type_;
}

std::shared_ptr<IAttributeSetModel>
KbNodeLinkAttributePopupEditorModel::getCurrentLinkAttrSetModel() {
  mayInitCurrLinkType();
  return attr_set_model_factory_->createAttributeSetModel(
      curr_link_type_.attributeSuppliers());
}

void KbNodeLinkAttributePopupEditorModel::setLinkType(
    ITreeItem* link_type_item) {
  LinkType* link_type = static_cast<LinkType*>(link_type_item);
  curr_link_type_ = *link_type;
  curr_link_type_initialized_ = true;
}

void KbNodeLinkAttributePopupEditorModel::editFinished() {
  *(attr_->valueAttr()) = value_attr_copy_;
  *(attr_->linkType()) = curr_link_type_;
}


}  // namespace snailcore
