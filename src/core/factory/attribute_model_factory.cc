// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/factory/attribute_model_factory.h"

#include "core/i_attribute_visitor.h"
#include "core/generic_attribute.h"
#include "src/core/kbnode_attribute_model.h"

namespace snailcore {

class AttributeModelFactory::AttributeVisitor : public IAttributeVisitor {
 public:
  explicit AttributeVisitor(const AttributeModelFactory& factory)
      : factory_(factory) { }
  virtual ~AttributeVisitor() = default;

  void visit(KbNodeAttribute* attr) override {
    attr_model_.reset(
        new KbNodeAttributeModel(attr, factory_.kbnode_manager_,
                                 factory_.simple_kbnode_adder_model_factory_));
  }

 private:
  SNAIL_DISABLE_COPY(AttributeVisitor);

  const AttributeModelFactory& factory_;
  std::shared_ptr<IAttributeModel> attr_model_;

  friend class AttributeModelFactory;
};

std::shared_ptr<IAttributeModel>
AttributeModelFactory::createAttributeModel(IAttribute* attr) const {
  AttributeVisitor visitor{*this};

  GenericAttribute* base_attr = static_cast<GenericAttribute*>(attr);
  base_attr->accept(&visitor);
  return visitor.attr_model_;
}

}  // namespace snailcore
