// Copyright (c) 2016
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef ATTRIBUTE_SET_MODEL_FACTORY_H_
#define ATTRIBUTE_SET_MODEL_FACTORY_H_

#include "core/factory/i_attribute_set_model_factory.h"

namespace snailcore {

class IAttributeModelFactory;

class AttributeSetModelFactoryImpl : public IAttributeSetModelFactory {
 public:
  AttributeSetModelFactoryImpl(IAttributeModelFactory* attr_model_factory)
      : attr_model_factory_{attr_model_factory} {}

 private:
  std::shared_ptr<IAttributeSetModel> createAttributeSetModel(
      const std::vector<IAttributeSupplier*>& attr_suppliers) override;

  IAttributeModelFactory* attr_model_factory_;
};

}  // namespace snailcore

#endif  // ATTRIBUTE_SET_MODEL_FACTORY_H_
