// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef ATTRIBUTE_MODEL_FACTORY_H_
#define ATTRIBUTE_MODEL_FACTORY_H_

#include "utils/basic_utils.h"
#include "core/i_attribute_model_factory.h"

namespace snailcore {

class IKbNodeManager;
class IKbNodeProviderModelFactory;

class AttributeModelFactory : public IAttributeModelFactory {
 public:
  class AttributeVisitor;

  AttributeModelFactory(
      IKbNodeManager* kbnode_manager,
      IKbNodeProviderModelFactory* kbnode_provider_model_factory)
      : kbnode_manager_{kbnode_manager}
      , kbnode_provider_model_factory_{kbnode_provider_model_factory} { }
  virtual ~AttributeModelFactory() = default;

  std::shared_ptr<IAttributeModel>
  createAttributeModel(IAttribute* attr) const override;

 private:
  SNAIL_DISABLE_COPY(AttributeModelFactory);

  IKbNodeManager* kbnode_manager_ { nullptr };
  IKbNodeProviderModelFactory* kbnode_provider_model_factory_ { nullptr };
};


}  // namespace snailcore

#endif  // ATTRIBUTE_MODEL_FACTORY_H_
