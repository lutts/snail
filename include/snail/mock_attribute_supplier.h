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
  SNAIL_MOCK_SLOT(AttributeChanged);

  MOCK_CONST_METHOD0(name, utils::U8String());
  MOCK_CONST_METHOD0(attr_count, int());
  MOCK_CONST_METHOD0(attributes, std::vector<IAttribute*>());
  MOCK_CONST_METHOD0(max_attrs, int());

  MOCK_METHOD0(addAttribute, IAttribute*());
  MOCK_METHOD1(removeAttribute, void(IAttribute* attr));

  MOCK_METHOD1(attributeChanged, void(IAttribute* attr));
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_SNAIL_MOCK_ATTRIBUTE_SUPPLIER_H_
