// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef FEW_ENTITY_ATTR_EDITOR_PRESENTER_H_
#define FEW_ENTITY_ATTR_EDITOR_PRESENTER_H_

#include <memory>  // std::shared_ptr

#include "pfmvp/pf_presenter.h"
#include "snail/i_few_entity_attr_editor_model.h"
#include "qtui/i_few_entity_attr_editor_view.h"
#include "qtui/i_few_entity_candidates_adapter.h"

using FewEntityAttrEditorPresenterBase =
    pfmvp::PfPresenterT<snailcore::IFewEntityAttrEditorModel,
                        IFewEntityAttrEditorView>;

class FewEntityAttrEditorPresenter : public FewEntityAttrEditorPresenterBase {
 public:
  FewEntityAttrEditorPresenter(
      std::shared_ptr<snailcore::IFewEntityAttrEditorModel> model,
      std::shared_ptr<IFewEntityAttrEditorView> view,
      std::shared_ptr<IFewEntityCandidatesAdapter> entity_candidates_adapter)
      : FewEntityAttrEditorPresenterBase(model, view)
      , entity_candidates_adapter_(entity_candidates_adapter) {
  }

  void initialize() override {
    entity_candidates_adapter_->setCandidateEntities(
        model()->getCandidateEntities());
    entity_candidates_adapter_->setCurrentEntityIndex(
        model()->getCurrentEntityIndex());

    entity_candidates_adapter_->whenEntitySelectionChanged(
        [this](std::shared_ptr<const snailcore::IEntity> entity) {
          model()->setCurrentEntity(entity);
        },
        shared_from_this());
  }

 private:
  FewEntityAttrEditorPresenter(const FewEntityAttrEditorPresenter&) = delete;
  FewEntityAttrEditorPresenter& operator=(
      const FewEntityAttrEditorPresenter&) = delete;

  std::shared_ptr<IFewEntityCandidatesAdapter> entity_candidates_adapter_;
};

#endif  // FEW_ENTITY_ATTR_EDITOR_PRESENTER_H_
