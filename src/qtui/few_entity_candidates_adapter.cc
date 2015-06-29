// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/qtui/few_entity_candidates_adapter.h"

#include <QStringList>
#include <QStringListModel>

#include "utils/basic_utils.h"
#include "utils/u8string.h"
#include "qtui/i_few_entity_attr_editor_view.h"
#include "snail/i_entity.h"

using namespace snailcore;  // NOLINT

std::shared_ptr<FewEntityCandidatesAdapter>
FewEntityCandidatesAdapter::create(
    std::shared_ptr<IFewEntityAttrEditorView> view) {
  auto adapter = std::make_shared<FewEntityCandidatesAdapter>(view);
  auto raw_adapter = adapter.get();

  view->whenEntitySelectionChanged(
      [raw_adapter](int index) {
        if (index > static_cast<int>(raw_adapter->candidate_entities_.size()))
          return;

        raw_adapter->EntitySelectionChanged(
            raw_adapter->candidate_entities_[index]);
        (void)index;
      },
      adapter);

  return adapter;
}

void FewEntityCandidatesAdapter::setCandidateEntities(
    std::vector<std::shared_ptr<const snailcore::IEntity>> entities) {
  QStringList qstr_list;

  for (auto& entity : entities) {
    qstr_list << U8StringToQString(entity->name());
  }

  entity_names_model.setStringList(qstr_list);
  view_->setEntitySelectorQModel(&entity_names_model);

  candidate_entities_ = entities;
}

void FewEntityCandidatesAdapter::setCurrentEntityIndex(int index) {
  view_->setCurrentEntityIndex(index);
}
