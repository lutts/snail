// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_MOCK_ENTITY_PROVIDER_H_
#define INCLUDE_SNAIL_MOCK_ENTITY_PROVIDER_H_

#include <vector>
#include "snail/i_entity_provider.h"

namespace snailcore {
namespace tests {

class MockEntityProvider : public IEntityProvider {
 public:
  MOCK_CONST_METHOD0(getEntities,
                     std::vector<std::shared_ptr<const IEntity>>());
  MOCK_CONST_METHOD0(getDefaultEntity, std::shared_ptr<const IEntity>());
  MOCK_METHOD1(touchEntity,
               void(std::shared_ptr<const IEntity> entity));
};

}  // namespace tests

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_MOCK_ENTITY_PROVIDER_H_
