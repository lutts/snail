// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_I_ENTITY_CANDIDATE_ITEM_CONVERTER_H_
#define INCLUDE_CORE_I_ENTITY_CANDIDATE_ITEM_CONVERTER_H_

#include <vector>

namespace snailcore {

class CandidateItem;
class IEntity;

class IEntityCandidateItemConverter {
 public:
  virtual ~IEntityCandidateItemConverter() = default;

  using EntitiesContainer = std::vector<std::shared_ptr<const IEntity>>;

  virtual CandidateItem* toCandidateItems(
      const EntitiesContainer& entities) = 0;

  virtual std::shared_ptr<const IEntity>
  toEntity(const CandidateItem& item) = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_CORE_I_ENTITY_CANDIDATE_ITEM_CONVERTER_H_
