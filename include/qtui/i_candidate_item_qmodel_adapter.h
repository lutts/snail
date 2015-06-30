// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_CANDIDATE_ITEM_QMODEL_ADAPTER_H_
#define I_CANDIDATE_ITEM_QMODEL_ADAPTER_H_

namespace snailcore {
class CandidateItem;
}  // namespace snailcore

class ICandidateItemQModelAdapter {
 public:
  virtual ~ICandidateItemQModelAdapter() = default;

  virtual void setCandidates(const snailcore::CandidateItem& root_item) = 0;
};

#endif  // I_CANDIDATE_ITEM_QMODEL_ADAPTER_H_
