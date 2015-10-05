// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/attribute_collection_model.h"

#include <vector>
#include "core/i_attribute_model_factory.h"
#include "snail/i_attribute_model.h"

namespace snailcore {

AttributeCollectionModel::AttributeCollectionModel(
    const std::vector<IAttributeSupplier*>& attr_suppliers,
    const IAttributeModelFactory& attr_model_factory)
    : attr_suppliers_(attr_suppliers)
    , attr_model_factory_(attr_model_factory) { }

AttributeCollectionModel::~AttributeCollectionModel() = default;

void AttributeCollectionModel::switchMode() {
  if (is_display_mode_) {
    is_display_mode_ = false;
    SwitchToEditMode();
  } else {
    is_display_mode_ = true;
    SwitchToDisplayMode();
  }
}

std::vector<IAttributeSupplier*>
AttributeCollectionModel::getAttributeSuppliers() const {
  return attr_suppliers_;
}

std::shared_ptr<IAttributeModel>
AttributeCollectionModel::createAttributeModel(IAttribute* attr) {
  auto attr_model = attr_model_factory_.createAttributeModel(attr);
  attr_models_.push_back(attr_model.get());

  attr_model->whenValidateComplete(
      [this]() {
        validateComplete();
      });

  return attr_model;
}

void AttributeCollectionModel::validateComplete() {
  bool valid = true;

  for (auto attr_model : attr_models_) {
    valid &= attr_model->isValid();
  }

  ValidateComplete(valid);
}

}  // namespace snailcore
