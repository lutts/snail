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
#include "qtui/i_candidate_item_qmodel_adapter.h"

using FewEntityAttrEditorPresenterBase =
    pfmvp::PfPresenterT<snailcore::IFewEntityAttrEditorModel,
                        IFewEntityAttrEditorView>;

class FewEntityAttrEditorPresenter : public FewEntityAttrEditorPresenterBase {
 public:
  FewEntityAttrEditorPresenter(
      std::shared_ptr<model_type> model,
      std::shared_ptr<view_type> view,
      std::unique_ptr<ICandidateItemQModelAdapter> entity_candidates_adapter)
      : FewEntityAttrEditorPresenterBase(model, view)
      , entity_candidates_adapter_(std::move(entity_candidates_adapter)) {
  }

  void initialize() override {
    auto entity_candidates_item_root = model()->getCandidateEntities();
    entity_candidates_adapter_->setCandidates(*entity_candidates_item_root);
    view()->setEntitySelectorQModel(entity_candidates_adapter_.get());
    view()->setCurrentEntityName(
        model()->getCurrentEntityName());

    view()->whenEntitySelectionChanged(
        [this](void* item_ptr) {
          auto entity_item = static_cast<snailcore::CandidateItem*>(item_ptr);
          model()->setCurrentEntity(*entity_item);
        },
        shared_from_this());
  }

 private:
  FewEntityAttrEditorPresenter(const FewEntityAttrEditorPresenter&) = delete;
  FewEntityAttrEditorPresenter& operator=(
      const FewEntityAttrEditorPresenter&) = delete;

  std::unique_ptr<ICandidateItemQModelAdapter> entity_candidates_adapter_;
};

#endif  // FEW_ENTITY_ATTR_EDITOR_PRESENTER_H_
