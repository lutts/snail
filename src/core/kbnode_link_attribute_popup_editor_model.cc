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
#include "core/fto_link_type.h"
#include "snail/i_attribute_model.h"
#include "snail/i_attribute_set_model.h"

namespace snailcore {

KbNodeLinkAttributePopupEditorModel::KbNodeLinkAttributePopupEditorModel(
    fto::KbNodeLinkAttribute* attr,
    IAttributeModelFactory* attr_model_factory,
    IAttributeSetModelFactory* attr_set_model_factory)
    : attr_(attr)
    , value_attr_copy_(*attr_->valueAttr())
    , proto_link_type_(attr_->protoLinkType())
    , link_type_copy_(*attr_->linkType())
    , attr_model_factory_(attr_model_factory)
    , attr_set_model_factory_(attr_set_model_factory) { }

KbNodeLinkAttributePopupEditorModel::
~KbNodeLinkAttributePopupEditorModel() = default;

utils::U8String KbNodeLinkAttributePopupEditorModel::valueAttrName() const {
  return "";
}

std::shared_ptr<IAttributeModel>
KbNodeLinkAttributePopupEditorModel::createValueAttrModel() {
  auto attr_model =
      attr_model_factory_->createAttributeModel(&value_attr_copy_);

  auto attr_model_raw = attr_model.get();
  attr_model->whenValidateComplete(
      [this, attr_model_raw]() {
        attr_model_valid_ = attr_model_raw->isValid();
        validateComplete();
      }, nullptr);

  return attr_model;
}

ITreeItemProvider*
KbNodeLinkAttributePopupEditorModel::getLinkTypeItemProvider() const {
  return attr_->supplier()->getLinkTypeItemProvider();
}

const ITreeItem*
KbNodeLinkAttributePopupEditorModel::getCurrentProtoLinkType() const {
  return proto_link_type_;
}

std::shared_ptr<IAttributeSetModel>
KbNodeLinkAttributePopupEditorModel::getCurrentLinkAttrSetModel() {
  auto attr_set_model = attr_set_model_factory_->createAttributeSetModel(
      link_type_copy_->attributeSuppliers());

  curr_attr_set_model_ = attr_set_model.get();

  attr_set_model->whenValidateComplete(
      [this](bool result) {
        attr_set_model_valid_ = result;
        validateComplete();
      }, nullptr);

  return attr_set_model;
}

void KbNodeLinkAttributePopupEditorModel::validateComplete() {
  ValidateComplete(attr_model_valid_ & attr_set_model_valid_);
}

void KbNodeLinkAttributePopupEditorModel::setProtoLinkType(
    ITreeItem* proto_link_type_item) {
  proto_link_type_ = static_cast<fto::LinkType*>(proto_link_type_item);

  link_type_copy_ = *(proto_link_type_);

  auto old_attr_set_model = curr_attr_set_model_;
  LinkTypeChanged(getCurrentLinkAttrSetModel(), old_attr_set_model);
}

void KbNodeLinkAttributePopupEditorModel::editFinished() {
  attr_->setValueAttr(UNFOLD_TEST_PROXY(value_attr_copy_));
  attr_->setProtoLinkType(proto_link_type_);
  attr_->setLinkType(UNFOLD_TEST_PROXY(link_type_copy_));
}

}  // namespace snailcore
