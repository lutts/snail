// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef ENTITIES_GENERATOR_H_
#define ENTITIES_GENERATOR_H_

#include <memory>

#include "utils/basic_utils.h"
#include "snail/mock_entity.h"

using namespace snailcore;  // NOLINT
using namespace snailcore::tests;  // NOLINT

class EntityTestHelper {
 public:
  static std::vector<std::shared_ptr<const IEntity>> genEntities() {
    std::vector<std::shared_ptr<const IEntity>> entities;
    const int NUM_ENTITIES = 10;
    for (int i = 0; i < NUM_ENTITIES; ++i) {
      auto entity = std::make_shared<MockEntity>();
      ON_CALL(*entity, name())
          .WillByDefault(Return(xtestutils::genRandomString()));
      entities.push_back(entity);
    }

    return entities;
  }
};

#endif  // ENTITIES_GENERATOR_H_
