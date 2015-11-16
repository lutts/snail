// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/attribute_set_model.h"

#include <vector>

#include "utils/basic_utils.h"
#include "utils/signal_slot_impl.h"
#include "snail/i_attribute.h"
#include "snail/i_attribute_supplier.h"
#include "core/i_attribute_model_factory.h"
#include "snail/i_attribute_model.h"

#include "pfmvp/i_pf_triad_manager.h"

namespace snailcore {

class AttributeSetModelSignalHelper {
 public:
  SNAIL_SIGSLOT_PIMPL(AttributeSetModel, SwitchToEditMode);
 public:
  SNAIL_SIGSLOT_PIMPL(AttributeSetModel, SwitchToDisplayMode);
 public:
  SNAIL_SIGSLOT_PIMPL(AttributeSetModel, ValidateComplete);
};

SNAIL_SIGSLOT_DELEGATE(AttributeSetModel, SwitchToEditMode, signal_helper_);
SNAIL_SIGSLOT_DELEGATE(AttributeSetModel, SwitchToDisplayMode, signal_helper_);
SNAIL_SIGSLOT_DELEGATE2(AttributeSetModel, ValidateComplete);

AttributeSetModel::AttributeSetModel(
    const std::vector<IAttributeSupplier*>& attr_suppliers,
    IAttributeModelFactory* attr_model_factory)
    : signal_helper_(utils::make_unique<AttributeSetModelSignalHelper>())
    , attr_suppliers_(attr_suppliers)
    , attr_model_factory_(attr_model_factory) { }

AttributeSetModel::~AttributeSetModel() = default;

std::vector<IAttributeSupplier*>
AttributeSetModel::getAttributeSuppliers() const {
  return attr_suppliers_;
}

void AttributeSetModel::switchMode() {
  if (edit_mode_) {
    switchToDisplayMode();
  } else {
    switchToEditMode();
  }
}

void AttributeSetModel::switchToEditMode() {
  for (auto & supplier : attr_suppliers_) {
    if (supplier->max_attrs() == 1 && supplier->attr_count() == 0) {
      supplier->addAttribute();
    }
  }

  edit_mode_ = true;
  signal_helper_->emitSwitchToEditMode();
}

void AttributeSetModel::switchToDisplayMode() {
  for (auto & supplier : attr_suppliers_) {
    auto attrs = supplier->attributes();
    for (auto & attr : attrs) {
      if (attr->isEmpty())
        supplier->removeAttribute(attr);
    }
  }

  edit_mode_ = false;
  signal_helper_->emitSwitchToDisplayMode();
}

std::shared_ptr<IAttributeModel>
AttributeSetModel::createAttributeModel(IAttribute* attr) {
  auto attr_model = attr_model_factory_->createAttributeModel(attr);
  attr_models_.push_back(attr_model);

  attr_model->whenValidateComplete(
      [this]() {
        validateComplete();
      });

  return attr_model;
}

void AttributeSetModel::validateComplete() {
  if (!edit_mode_)
    return;

  bool valid = true;
  bool emit_signal = (attr_models_.size() != 0);

  auto iter = attr_models_.begin();
  while (iter != attr_models_.end()) {
    auto amodel = iter->lock();
    if (amodel) {
      valid &= amodel->isValid();
      ++iter;
    } else {
      attr_models_.erase(iter++);
    }
  }

  if (emit_signal)
    signal_helper_->emitValidateComplete(valid);
}

void AttributeSetModel::closeAttributeEditors(
    pfmvp::IPfTriadManager* triad_manager) {
  for (auto & attr_model : attr_models_) {
    auto amodel = attr_model.lock();
    if (amodel) {
      amodel->cleanupValidateCompleteSlots();
      triad_manager->removeTriadBy(amodel.get());
    }
  }

  attr_models_.clear();
}

}  // namespace snailcore
