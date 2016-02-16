// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_FACTORY_ATTRIBUTE_MODEL_FACTORY_H_
#define SRC_CORE_FACTORY_ATTRIBUTE_MODEL_FACTORY_H_

#include "include/config.h"
#include "utils/basic_utils.h"
#include "core/factory/i_attribute_model_factory.h"

namespace snailcore {

FTO_BEGIN_NAMESPACE
class KbNodeManager;
FTO_END_NAMESPACE

class ISimpleKbNodeAdderModelFactory;

class AttributeModelFactory : public IAttributeModelFactory {
 public:
  class AttributeVisitor;

  AttributeModelFactory(
      fto::KbNodeManager* kbnode_manager,
      ISimpleKbNodeAdderModelFactory* simple_kbnode_adder_model_factory)
      : kbnode_manager_{kbnode_manager},
        simple_kbnode_adder_model_factory_{simple_kbnode_adder_model_factory} {}
  virtual ~AttributeModelFactory() = default;

  std::shared_ptr<IAttributeModel> createAttributeModel(
      IAttribute* attr) const override;

 private:
  SNAIL_DISABLE_COPY(AttributeModelFactory);

  fto::KbNodeManager* kbnode_manager_{nullptr};
  ISimpleKbNodeAdderModelFactory* simple_kbnode_adder_model_factory_{nullptr};
};

}  // namespace snailcore

#endif  // SRC_CORE_FACTORY_ATTRIBUTE_MODEL_FACTORY_H_
