// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_I_ATTRIBUTE_CANDIDATE_ITEM_CONVERTER_H_
#define INCLUDE_CORE_I_ATTRIBUTE_CANDIDATE_ITEM_CONVERTER_H_

#include <vector>

namespace snailcore {

class IAttrCandidateItemConverter {
 public:
  virtual ~IAttrCandidateItemConverter() = default;

  virtual const CandidateItem* toCandidateItems(
      std::vector<IAttribute*> attr_list) = 0;
  virtual IAttribute* toAttribute(const CandidateItem& item) const = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_CORE_I_ATTRIBUTE_CANDIDATE_ITEM_CONVERTER_H_
