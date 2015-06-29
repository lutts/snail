// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_MOCK_ATTRIBUTE_H_
#define INCLUDE_SNAIL_MOCK_ATTRIBUTE_H_

#include <iostream>  // NOLINT

#include "snail/i_attribute.h"

namespace snailcore {
namespace tests {

class MockAttribute : public IAttribute {
 public:
  MOCK_CONST_METHOD0(clone, IAttribute*());

  MOCK_METHOD1(setEntity, void(std::shared_ptr<const IEntity> entity));
  MOCK_CONST_METHOD0(getEntity, std::shared_ptr<const IEntity>());
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_SNAIL_MOCK_ATTRIBUTE_H_
