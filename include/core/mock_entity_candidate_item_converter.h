// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_ENTITY_CANDIDATE_ITEM_CONVERTER_H_
#define MOCK_ENTITY_CANDIDATE_ITEM_CONVERTER_H_

#include "core/i_entity_candidate_item_converter.h"

namespace snailcore {
namespace tests {

class MockEntityCandidateItemConverter : public IEntityCandidateItemConverter {
 public:
  MOCK_METHOD1(toCandidateItems,
               CandidateItem*(const EntitiesContainer& entities));
  MOCK_METHOD1(toEntity,
               std::shared_ptr<const IEntity>(const CandidateItem& item));
};

}  // namespace tests
}  // namespace snailcore

#endif  // MOCK_ENTITY_CANDIDATE_ITEM_CONVERTER_H_
