// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_MOCK_ATTRIBUTE_SUPPLIER_H_
#define INCLUDE_SNAIL_MOCK_ATTRIBUTE_SUPPLIER_H_

#include <vector>

#include "snail/i_attribute_supplier.h"

namespace snailcore {
namespace tests {

class MockAttributeSupplier : public IAttributeSupplier {
 public:
  MOCK_CONST_METHOD0(attributes, std::vector<IAttribute*>());
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_SNAIL_MOCK_ATTRIBUTE_SUPPLIER_H_
