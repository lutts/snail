// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_QTUI_I_CANDIDATE_ITEM_QMODEL_ADAPTER_H_
#define INCLUDE_QTUI_I_CANDIDATE_ITEM_QMODEL_ADAPTER_H_

namespace snailcore {
class CandidateItem;
}  // namespace snailcore

class ICandidateItemQModelAdapter {
 public:
  virtual ~ICandidateItemQModelAdapter() = default;

  virtual void setCandidates(const snailcore::CandidateItem& root_item) = 0;
};

#endif  // INCLUDE_QTUI_I_CANDIDATE_ITEM_QMODEL_ADAPTER_H_
