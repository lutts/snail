// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_I_ATTRIBUTE_SET_MODEL_FACTORY_H_
#define INCLUDE_CORE_I_ATTRIBUTE_SET_MODEL_FACTORY_H_

#include <vector>

namespace snailcore {

class IAttributeSetModel;
class IAttributeSupplier;

class IAttributeSetModelFactory {
 public:
  virtual ~IAttributeSetModelFactory() = default;

  virtual std::shared_ptr<IAttributeSetModel> createAttributeSetModel(
      const std::vector<IAttributeSupplier*>& attr_suppliers) = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_CORE_I_ATTRIBUTE_SET_MODEL_FACTORY_H_
