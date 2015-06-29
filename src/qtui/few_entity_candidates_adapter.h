// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef FEW_ENTITY_PROVIDER_ADAPTER_H_
#define FEW_ENTITY_PROVIDER_ADAPTER_H_

#include <memory>
#include <QStringListModel>

#include "utils/basic_utils.h"
#include "utils/signal_slot_impl.h"
#include "utils/i_trackable.h"

#include "qtui/i_few_entity_candidates_adapter.h"

class IFewEntityAttrEditorView;

class FewEntityCandidatesAdapter : public IFewEntityCandidatesAdapter
                               , public utils::ITrackable {
 public:
  static std::shared_ptr<FewEntityCandidatesAdapter>
  create(std::shared_ptr<IFewEntityAttrEditorView> view);

  explicit FewEntityCandidatesAdapter(
      std::shared_ptr<IFewEntityAttrEditorView> view)
      : view_{view} { }
  virtual ~FewEntityCandidatesAdapter() = default;

  void setCandidateEntities(
      std::vector<std::shared_ptr<const snailcore::IEntity>> entities) override;
  void setCurrentEntityIndex(int index) override;

 private:
  SNAIL_SIGSLOT_IMPL(EntitySelectionChanged);

  std::shared_ptr<IFewEntityAttrEditorView> view_;
  std::vector<std::shared_ptr<const snailcore::IEntity>> candidate_entities_;
  QStringListModel entity_names_model;

 private:
  SNAIL_DISABLE_COPY(FewEntityCandidatesAdapter)
};

#endif  // FEW_ENTITY_PROVIDER_ADAPTER_H_
