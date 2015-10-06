// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_ATTRIBUTE_MODEL_FACTORY_H_
#define INCLUDE_SNAIL_I_ATTRIBUTE_MODEL_FACTORY_H_

#include <memory>

namespace snailcore {

class IAttribute;
class IAttributeModel;

class IAttributeModelFactory {
 public:
  virtual ~IAttributeModelFactory() = default;

  virtual std::shared_ptr<IAttributeModel>
  createAttributeModel(IAttribute* attr) const = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_I_ATTRIBUTE_MODEL_FACTORY_H_
