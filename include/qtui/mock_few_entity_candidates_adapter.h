// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_FEW_ENTITY_PROVIDER_ADAPTER_H_
#define MOCK_FEW_ENTITY_PROVIDER_ADAPTER_H_

#include "qtui/i_few_entity_candidates_adapter.h"

class MockFewEntityCandidatesAdapter : public IFewEntityCandidatesAdapter {
 public:
  SNAIL_MOCK_SLOT(EntitySelectionChanged);

  MOCK_METHOD1(
      setCandidateEntities,
      void(std::vector<std::shared_ptr<const snailcore::IEntity>> entities));
  MOCK_METHOD1(setCurrentEntityIndex, void(int index));
};

#endif  // MOCK_FEW_ENTITY_PROVIDER_ADAPTER_H_
