// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_FEW_ENTITY_ATTR_EDITOR_MODEL_H_
#define I_FEW_ENTITY_ATTR_EDITOR_MODEL_H_

#include "snail/i_attribute_editor_model.h"
#include "utils/signal_slot.h"
#include "snail/i_entity_provider.h"

namespace snailcore {

class IEntity;

class IFewEntityAttrEditorModel : public IAttributeEditorModel {
 public:
  virtual ~IFewEntityAttrEditorModel() = default;

  DEF_MODEL_ID(FewEntityAttrEditorModel);

  virtual std::vector<std::shared_ptr<const IEntity>>
  getCandidateEntities() const = 0;

  virtual void setCurrentEntity(std::shared_ptr<const IEntity> entity) = 0;
  virtual int getCurrentEntityIndex() const = 0;
};

}  // namespace snailcore

#endif  // I_FEW_ENTITY_ATTR_EDITOR_MODEL_H_
