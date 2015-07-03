// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_MOCK_ENTITY_H_
#define INCLUDE_SNAIL_MOCK_ENTITY_H_

#include "snail/i_entity.h"

namespace snailcore {
namespace tests {

class MockEntity : public IEntity {
 public:
  MOCK_CONST_METHOD0(name, utils::U8String());
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_SNAIL_MOCK_ENTITY_H_
