// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_MOCK_ATTRIBUTE_CONTAINER_H_
#define INCLUDE_SNAIL_MOCK_ATTRIBUTE_CONTAINER_H_

#include <vector>
#include "snail/i_attribute_container.h"

namespace snailcore {
namespace tests {

class MockAttributeContainer : public IAttributeContainer {
 public:
  MOCK_CONST_METHOD0(getDescription, utils::U8String());
  MOCK_CONST_METHOD0(getAllowedAttributeList, std::vector<IAttribute*>());
  MOCK_CONST_METHOD0(getDefaultAllowedAttribute, IAttribute*());
  MOCK_METHOD1(addAttribute, void(IAttribute* attr));
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_SNAIL_MOCK_ATTRIBUTE_CONTAINER_H_
