// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef FEW_ENTITY_ATTR_EDITOR_MODEL_H_
#define FEW_ENTITY_ATTR_EDITOR_MODEL_H_

#include <memory>

#include "snail/i_few_entity_attr_editor_model.h"
#include "utils/signal_slot_impl.h"

namespace snailcore {

class IAttribute;
class IEntityProvider;
class IEntityCandidateItemConverter;

class FewEntityAttrEditorModel : public IFewEntityAttrEditorModel {
 public:
  FewEntityAttrEditorModel(
      IAttribute* attr,
      IEntityProvider* entity_provider,
      std::unique_ptr<IEntityCandidateItemConverter> entity_item_converter);
  virtual ~FewEntityAttrEditorModel() = default;

  bool validateResult() const override;
  IAttribute* getAttribute() const override;

  const CandidateItem* getCandidateEntities() const override;
  void setCurrentEntity(const CandidateItem& entity_item) override;
  utils::U8String getCurrentEntityName() const override;

 private:
  FewEntityAttrEditorModel(const FewEntityAttrEditorModel&) = delete;
  FewEntityAttrEditorModel& operator=(const FewEntityAttrEditorModel&) = delete;

  IAttribute* attr_;
  IEntityProvider* entity_provider_;
  std::unique_ptr<IEntityCandidateItemConverter> entity_item_converter_;

 private:
  SNAIL_SIGSLOT_IMPL(ValidateComplete);
};


}  // namespace snailcore

#endif  // FEW_ENTITY_ATTR_EDITOR_MODEL_H_
