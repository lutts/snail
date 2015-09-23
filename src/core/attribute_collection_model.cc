// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/attribute_collection_model.h"

#include <vector>
#include "snail/i_attribute_model_factory.h"

namespace snailcore {

AttributeCollectionModel::AttributeCollectionModel(
    const std::vector<IAttributeSupplier*>& attr_suppliers,
    const IAttributeModelFactory& attr_model_factory)
    : attr_suppliers_(attr_suppliers)
    , attr_model_factory_(attr_model_factory) { }

AttributeCollectionModel::~AttributeCollectionModel() = default;

std::vector<IAttributeSupplier*>
AttributeCollectionModel::getAttributeSuppliers() const {
  return attr_suppliers_;
}

std::shared_ptr<IAttributeModel>
AttributeCollectionModel::createAttributeModel(IAttribute* attr) {
  return attr_model_factory_.createModel(attr);
}

}  // namespace snailcore
