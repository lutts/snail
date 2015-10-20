// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_FACTORY_ATTRIBUTE_MODEL_FACTORY_H_
#define SRC_CORE_FACTORY_ATTRIBUTE_MODEL_FACTORY_H_

#include "utils/basic_utils.h"
#include "core/i_attribute_model_factory.h"

namespace snailcore {

class IKbNodeManager;
class ISimpleKbNodeAdderModelFactory;

class AttributeModelFactory : public IAttributeModelFactory {
 public:
  class AttributeVisitor;

  AttributeModelFactory(
      IKbNodeManager* kbnode_manager,
      ISimpleKbNodeAdderModelFactory* simple_kbnode_adder_model_factory)
      : kbnode_manager_{kbnode_manager}
      , simple_kbnode_adder_model_factory_{simple_kbnode_adder_model_factory} {}
  virtual ~AttributeModelFactory() = default;

  std::shared_ptr<IAttributeModel>
  createAttributeModel(IAttribute* attr) const override;

 private:
  SNAIL_DISABLE_COPY(AttributeModelFactory);

  IKbNodeManager* kbnode_manager_ { nullptr };
  ISimpleKbNodeAdderModelFactory* simple_kbnode_adder_model_factory_ { nullptr};
};


}  // namespace snailcore

#endif  // SRC_CORE_FACTORY_ATTRIBUTE_MODEL_FACTORY_H_
