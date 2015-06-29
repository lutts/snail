// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/few_entity_attr_editor_model.h"
#include "snail/i_attribute.h"
#include "snail/i_entity_provider.h"

namespace snailcore {

FewEntityAttrEditorModel::FewEntityAttrEditorModel(
    IAttribute* attr,
    IEntityProvider* entity_provider)
    : attr_(attr), entity_provider_(entity_provider) {
  auto default_entity = entity_provider_->getDefaultEntity();
  if (default_entity) {
    attr_->setEntity(default_entity);
  }
}

bool FewEntityAttrEditorModel::validateResult() const {
  if (attr_->getEntity())
    return true;

  return false;
}

IAttribute* FewEntityAttrEditorModel::getAttribute() const {
  return attr_;
}

std::vector<std::shared_ptr<const IEntity>>
FewEntityAttrEditorModel::getCandidateEntities() const {
  return entity_provider_->getEntities();
}

void FewEntityAttrEditorModel::setCurrentEntity(
    std::shared_ptr<const IEntity> entity) {
  attr_->setEntity(entity);
  ValidateComplete(true);  // always valid
  entity_provider_->touchEntity(entity);
}

int FewEntityAttrEditorModel::getCurrentEntityIndex() const {
  if (!attr_->getEntity())
    return -1;

  int index = 0;
  for (auto& entity : entity_provider_->getEntities()) {
    if (entity = attr_->getEntity()) {
      break;
    }
    ++index;
  }

  return index;
}

}  // namespace snailcore
