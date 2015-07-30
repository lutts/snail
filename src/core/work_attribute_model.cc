// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/work_attribute_model.h"

#include <vector>

#include "utils/i_relay_command_factory.h"
#include "snail/attribute_display_block.h"
#include "snail/i_attribute_model_factory.h"
#include "core/i_attribute_container.h"
#include "core/i_attribute.h"
#include "snail/i_attribute_model.h"
#include "core/i_attribute_supplier.h"

using namespace utils;  // NOLINT

namespace snailcore {

WorkAttributeModel::WorkAttributeModel(
    IAttributeContainer* attr_container,
    const IAttributeModelFactory& attr_model_factory,
    const IRelayCommandFactory& cmd_factory)
    : attr_container_(attr_container)
    , attr_model_factory_(attr_model_factory)
    , cmd_factory_(cmd_factory) { }

bool WorkAttributeModel::isEditMode() const {
  return false;
}

void WorkAttributeModel::switchToEditMode() {
}

void WorkAttributeModel::switchToDisplayMode() {
}

void WorkAttributeModel::traverseAttributes(
    IAttributeDisplayBlockVisitor* visitor) {
  auto attr_suppliers = attr_container_->attr_suppliers();

  int total_attrs = 0;
  for (auto & supplier : attr_suppliers) {
    total_attrs += supplier->num_attributes();
  }
  visitor->beginAddAttributeDisplayBlock(total_attrs);

  for (auto & supplier : attr_suppliers) {
    for (auto & attr : supplier->attributes()) {
      auto attr_model = attr_model_factory_.createModel(attr);
      visitor->addAttributeDisplayBlock({
          attr->displayName(),
              attr_model,
              nullptr,
              nullptr,
              false
        });
    }
  }
  visitor->endAddAttributeDisplayBlock();
}

}  // namespace snailcore
