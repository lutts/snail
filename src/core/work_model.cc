// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/work_model.h"
#include "core/fto_work.h"
#include "core/i_attribute_set_model_factory.h"

namespace snailcore {

WorkModel::WorkModel(
    IAttributeSetModelFactory* attr_set_model_factory)
    : attr_set_model_factory_{attr_set_model_factory} { }

WorkModel::~WorkModel() {
  // TODO(lutts): delete work is not our responsibility,
  // remove these codes some day
  // delete work_;
}

void WorkModel::set_work(fto::Work* work) {
  work_ = work;

  work_->whenNameChanged(
      [this](const utils::U8String& new_name) {
        NameChanged(new_name);
      },
      shared_from_this());
}

utils::U8String WorkModel::name() const {
  return work_->name();
}

bool WorkModel::set_name(const utils::U8String& new_name) {
  return work_->set_name(new_name);
}

std::shared_ptr<IAttributeSetModel>
WorkModel::createAttributeSetModel() {
  return attr_set_model_factory_->createAttributeSetModel(
      work_->attributeSuppliers());
}

}  // namespace snailcore
