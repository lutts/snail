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

#define COMMON_ATTR_SUPPLIER_MOCKS                            \
  SNAIL_MOCK_SLOT(AttributeChanged);                          \
                                                              \
  MOCK_CONST_METHOD0(name, utils::U8String());                \
  MOCK_CONST_METHOD0(attr_count, int());                      \
  MOCK_CONST_METHOD0(attributes, std::vector<IAttribute*>()); \
  MOCK_CONST_METHOD0(max_attrs, int());                       \
                                                              \
  MOCK_METHOD0(addAttribute, IAttribute*());                  \
  MOCK_METHOD1(removeAttribute, void(IAttribute * attr));     \
                                                              \
  MOCK_METHOD1(attributeChanged, void(IAttribute * attr));

class MockAttributeSupplier : public IAttributeSupplier {
 public:
  COMMON_ATTR_SUPPLIER_MOCKS;

  MOCK_CONST_METHOD0(clone, IAttributeSupplier*());
};

class AttrSupplierListener
    : public xtestutils::SimpleMockListener<IAttributeSupplier> {
 public:
  SNAIL_MOCK_LISTENER1(AttrSupplierListener, AttributeChanged,
                       void(IAttribute* attr));

  explicit AttrSupplierListener(IAttributeSupplier* subject)
      : SimpleMockListener(subject) {
    SNAIL_MOCK_LISTENER_REGISTER(AttributeChanged, this);

    attach();
  }

  ~AttrSupplierListener() { detatch(); }
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_SNAIL_MOCK_ATTRIBUTE_SUPPLIER_H_
