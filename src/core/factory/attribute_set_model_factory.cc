// Copyright (c) 2016
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/factory/attribute_set_model_factory.h"
#include "src/core/attribute_set_model.h"

namespace snailcore {

std::shared_ptr<IAttributeSetModel>
AttributeSetModelFactoryImpl::createAttributeSetModel(
    const std::vector<IAttributeSupplier*>& attr_suppliers) {
  return std::make_shared<AttributeSetModel>(attr_suppliers,
                                             attr_model_factory_);
}

}  // namespace snailcore
