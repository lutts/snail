// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_CANDIDATE_ITEM_QMODEL_ADAPTER_H_
#define MOCK_CANDIDATE_ITEM_QMODEL_ADAPTER_H_

class MockCandidateItemQModelAdapter : public ICandidateItemQModelAdapter {
 public:
  MOCK_METHOD1(setCandidates, void(const snailcore::CandidateItem& root_item));
};

#endif  // MOCK_CANDIDATE_ITEM_QMODEL_ADAPTER_H_
