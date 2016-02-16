// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_MOCK_KBNODE_ATTRIBUTE_H_
#define INCLUDE_CORE_MOCK_KBNODE_ATTRIBUTE_H_

#include "core/fto_kbnode_attribute.h"
#include "snail/mock_attribute.h"
#include "snail/mock_attribute_supplier.h"

#define INTERFACE_MOCK_PHASE
#include "test/interface.h"

namespace snailcore {
namespace tests {

class MockKbNodeAttribute : public fto::KbNodeAttribute {
 public:
  COMMON_ATTRIBUTE_MOCKS

  SNAIL_CONST_INTERFACE0(clone, fto::KbNodeAttribute*());

  KbNodeAttribute& operator=(KbNodeAttribute&& rhs) override {
    move_assignment(rhs);
    return *this;
  }

  MOCK_METHOD1(move_assignment, void(fto::KbNodeAttribute& rhs));  // NOLINT

  KbNodeAttribute_METHODS
};

class MockKbNodeAttributeSupplier : public fto::KbNodeAttributeSupplier {
 public:
  COMMON_ATTR_SUPPLIER_MOCKS;

  SNAIL_CONST_INTERFACE0(clone, KbNodeAttributeSupplier*());
  TEST_PROXY_COPY_ASSIGN_MOCK(KbNodeAttributeSupplier);

  KbNodeAttributeSupplier_METHODS
};

class MockKbNodeAttributeSupplierFactory
    : public KbNodeAttributeSupplierFactory {
 public:
  MockKbNodeAttributeSupplierFactory() {
    fto::KbNodeAttributeSupplierTestProxy::setFactory(this);
  }

  virtual ~MockKbNodeAttributeSupplierFactory() {
    fto::KbNodeAttributeSupplierTestProxy::setFactory(nullptr);
  }

  MOCK_CONST_METHOD2(create, fto::KbNodeAttributeSupplier*(IKbNode* root_kbnode,
                                                           int max_attrs));
};

}  // namespace tests
}  // namespace snailcore

#undef INTERFACE_MOCK_PHASE

#endif  // INCLUDE_CORE_MOCK_KBNODE_ATTRIBUTE_H_
