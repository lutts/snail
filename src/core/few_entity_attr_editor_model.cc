// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/few_entity_attr_editor_model.h"
#include "snail/i_attribute.h"
#include "snail/i_entity.h"
#include "snail/i_entity_provider.h"
#include "core/i_entity_candidate_item_converter.h"

namespace snailcore {

FewEntityAttrEditorModel::FewEntityAttrEditorModel(
    IAttribute* attr,
    IEntityProvider* entity_provider,
    std::unique_ptr<IEntityCandidateItemConverter> entity_item_converter)
    : attr_(attr)
    , entity_provider_(entity_provider)
    , entity_item_converter_(std::move(entity_item_converter)) {
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

const CandidateItem*
FewEntityAttrEditorModel::getCandidateEntities() const {
  return entity_item_converter_->toCandidateItems(
      entity_provider_->getEntities());
}

void FewEntityAttrEditorModel::setCurrentEntity(
    const CandidateItem& entity_item) {
  auto entity = entity_item_converter_->toEntity(entity_item);
  attr_->setEntity(entity);
  entity_provider_->touchEntity(entity);
  ValidateComplete(true);  // always valid
}

utils::U8String FewEntityAttrEditorModel::getCurrentEntityName() const {
  auto entity = attr_->getEntity();
  if (entity) {
    return entity->name();
  } else {
    return utils::U8String{""};
  }
}

}  // namespace snailcore
