// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/attribute_adder_model.h"

#include <vector>

#include "src/utils/utils.h"
#include "utils/i18n.h"
#include "snail/i_attribute_container.h"
#include "snail/i_attribute.h"
#include "snail/i_attribute_editor_model.h"
#include "snail/i_attribute_editor_model_factory.h"
#include "core/i_attribute_candidate_item_converter.h"

namespace snailcore {

AttributeAdderModel::AttributeAdderModel(
    IAttributeContainer* attr_container,
    IAttributeEditorModelFactory* attr_model_factory,
    std::unique_ptr<IAttrCandidateItemConverter> attr_item_converter)
    : attr_container_(attr_container)
    , attr_model_factory_(attr_model_factory)
    , attr_item_converter_(std::move(attr_item_converter)) { }

AttributeAdderModel::~AttributeAdderModel() = default;

const char* AttributeAdderModel::prompt_format_str =
    N_("Add attribute for {1}");

utils::U8String AttributeAdderModel::getPrompt() const {
  return utils::formatString(_(prompt_format_str),
                             attr_container_->getDescription());
}

const CandidateItem* AttributeAdderModel::getAllowedAttributes() const {
  return attr_item_converter_->toCandidateItems(
      attr_container_->getAllowedAttributeList());
}

utils::U8String AttributeAdderModel::getCurrentAttributeName() const {
  IAttribute* attr = curr_attr_prototype_;

  if (!attr) {
    attr = attr_container_->getDefaultAllowedAttribute();
  }

  if (attr)
    return attr->name();
  else
    return utils::U8String{""};
}

void AttributeAdderModel::updateCurrentAttributeEditorModel(
    IAttribute* attr_prototype, bool initial_create) {
  if (!attr_prototype)
    attr_prototype = attr_container_->getDefaultAllowedAttribute();

  if (!attr_prototype)
    return;

  auto new_attr = attr_prototype->clone();
  auto new_model = attr_model_factory_->createAttributeEditorModel(new_attr);

  if (new_model) {
    auto old_attr_model = curr_attr_model_.get();

    curr_attr_model_ = new_model;
    curr_attr_prototype_ = attr_prototype;

    new_model->whenValidateComplete(
        [this](bool result) {
          ValidateComplete(result);
        });

    if (!initial_create) {
      DiscardAttributeEditorModel(old_attr_model);
      CurrentAttributeEditorModelChanged(curr_attr_model_);
    }
  }
}

void AttributeAdderModel::setCurrentAttribute(const CandidateItem& item) {
  auto attr_prototype = attr_item_converter_->toAttribute(item);
  if (curr_attr_prototype_ == attr_prototype)
    return;

  updateCurrentAttributeEditorModel(attr_prototype);
}

std::shared_ptr<IAttributeEditorModel>
AttributeAdderModel::getCurrentAttributeEditorModel() {
  if (!curr_attr_model_) {
    updateCurrentAttributeEditorModel(nullptr, true);
  }

  return curr_attr_model_;
}

bool AttributeAdderModel::validateResult() const {
  if (!curr_attr_model_)
    return false;

  return curr_attr_model_->validateResult();
}

void AttributeAdderModel::doAddAttribute() {
  if (curr_attr_model_) {
    attr_container_->addAttribute(curr_attr_model_->getAttribute());
    updateCurrentAttributeEditorModel(curr_attr_prototype_);
  }
}

}  // namespace snailcore
