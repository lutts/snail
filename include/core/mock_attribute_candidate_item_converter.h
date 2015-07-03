// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_ATTRIBUTE_CANDIDATE_ITEM_CONVERTER_H_
#define MOCK_ATTRIBUTE_CANDIDATE_ITEM_CONVERTER_H_

#include "core/i_attribute_candidate_item_converter.h"

namespace snailcore {
namespace tests {

class MockAttrCandidateItemConverter : public IAttrCandidateItemConverter {
 public:
  MOCK_METHOD1(toCandidateItems,
               const CandidateItem*(std::vector<IAttribute*> attr_list));
  MOCK_CONST_METHOD1(toAttribute,
               IAttribute*(const CandidateItem& item));
};

}  // namespace tests
}  // namespace snailcore

#endif  // MOCK_ATTRIBUTE_CANDIDATE_ITEM_CONVERTER_H_
