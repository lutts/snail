// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_MOCK_ATTRIBUTE_CONTAINER_H_
#define INCLUDE_CORE_MOCK_ATTRIBUTE_CONTAINER_H_

#include <vector>

#include "core/i_attribute_container.h"

namespace snailcore {
namespace tests {

class MockAttributeContainer : public IAttributeContainer {
 public:
  MOCK_CONST_METHOD0(attr_suppliers, std::vector<IAttributeSupplier*>());
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_CORE_MOCK_ATTRIBUTE_CONTAINER_H_
