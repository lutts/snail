// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_CANDIDATE_ITEM_QMODEL_ADAPTER_H_
#define MOCK_CANDIDATE_ITEM_QMODEL_ADAPTER_H_

#include "qtui/i_candidate_item_qmodel_adapter.h"

class MockCandidateItemQModelAdapter : public ICandidateItemQModelAdapter {
 public:
  MOCK_METHOD1(setCandidates, void(const snailcore::CandidateItem& root_item));
};

#endif  // MOCK_CANDIDATE_ITEM_QMODEL_ADAPTER_H_
