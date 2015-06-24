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

namespace snailcore {

AttributeAdderModel::~AttributeAdderModel() = default;

const char* AttributeAdderModel::prompt_format_str =
    N_("Add attribute for {1}");

utils::U8String AttributeAdderModel::getPrompt() const {
  return utils::formatString(_(prompt_format_str),
                             attr_container_->getDescription());
}

std::vector<IAttribute*>
AttributeAdderModel::getAllowedAttributeList() const {
  return attr_container_->getAllowedAttributeList();
}

int AttributeAdderModel::getCurrentAttributeIndex() const {
  if (curr_attr_index_ < 0) {
    auto def_attr = attr_container_->getDefaultAllowedAttribute();
    auto attr_vec = getAllowedAttributeList();
    for (size_t idx = 0; idx < attr_vec.size(); ++idx) {
      if (attr_vec[idx] == def_attr) {
        curr_attr_index_ = static_cast<int>(idx);
      }
    }
  }

  return curr_attr_index_;
}

void AttributeAdderModel::updateCurrentAttributeEditorModel(
    int attr_index, bool initial_create) {
  auto allowed_attr_list = getAllowedAttributeList();
  auto attr_prototype = allowed_attr_list[attr_index];
  auto new_attr = attr_prototype->clone();
  auto new_model = attr_model_factory_->createAttributeEditorModel(new_attr);

  if (new_model) {
    auto old_attr_model = curr_attr_model_.get();

    curr_attr_model_ = new_model;
    curr_attr_index_ = attr_index;

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

void AttributeAdderModel::setCurrentAttributeIndex(int index) {
  if (index == getCurrentAttributeIndex())
    return;

  auto allowed_attr_list = getAllowedAttributeList();

  if ((index < 0) || (index > static_cast<int>(allowed_attr_list.size())))
    return;

  updateCurrentAttributeEditorModel(index);
}

std::shared_ptr<IAttributeEditorModel>
AttributeAdderModel::getCurrentAttributeEditorModel() {
  if (!curr_attr_model_) {
    updateCurrentAttributeEditorModel(getCurrentAttributeIndex(), true);
  }

  return curr_attr_model_;
}

void AttributeAdderModel::doAddAttribute() {
  attr_container_->addAttribute(curr_attr_model_->getAttribute());
  updateCurrentAttributeEditorModel(getCurrentAttributeIndex());
}

}  // namespace snailcore
