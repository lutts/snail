// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/work_attribute_model.h"

#include <vector>

#include "utils/i_relay_command_factory.h"

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

std::vector<std::shared_ptr<IAttributeModel>>
WorkAttributeModel::getAttributeModels() const {
  return std::vector<std::shared_ptr<IAttributeModel>>();
}


std::vector<utils::Command*> WorkAttributeModel::getEraseCommands() const {
  return std::vector<utils::Command*>();
}

std::vector<utils::Command*>
WorkAttributeModel::getPopupEditorCommands() const {
  return std::vector<utils::Command*>();
}


std::vector<utils::Command*>
WorkAttributeModel::getAddAttributeCommands() const {
  return std::vector<utils::Command*>();
}

IWorkAttributeModel::Location
WorkAttributeModel::getLocation(IPfModel* model) const {
  (void)model;
  return {0, 0, 0, 0};
}

IWorkAttributeModel::Location
WorkAttributeModel::getLocation(utils::Command* command) const {
  (void)command;
  return {0, 0, 0, 0};
}

}  // namespace snailcore
