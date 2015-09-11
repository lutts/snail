// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_ATTRIBUTE_MODEL_FACTORY_H_
#define I_ATTRIBUTE_MODEL_FACTORY_H_

namespace snailcore {

class IAttribute;
class IAttributeModel;

class IAttributeModelFactory {
 public:
  virtual ~IAttributeModelFactory() = default;

  virtual
  std::shared_ptr<IAttributeModel> createModel(IAttribute* attr) const = 0;
};

}  // namespace snailcore

#endif  // I_ATTRIBUTE_MODEL_FACTORY_H_
