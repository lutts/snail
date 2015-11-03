// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_KBNODE_ATTRIBUTE_SUPPLIER_FACTORY_H_
#define MOCK_KBNODE_ATTRIBUTE_SUPPLIER_FACTORY_H_

#include "core/i_kbnode_attribute_supplier_factory.h"

namespace snailcore {
namespace tests {

#define INTERFACE_MOCK_PHASE
#include "test/interface.h"

class MockKbNodeAttributeSupplierFactory
    : public IKbNodeAttributeSupplierFactory {
 public:
  IKbNodeAttributeSupplierFactory_METHODS
};

#undef INTERFACE_MOCK_PHASE

}  // namespace tests
}  // namespace snailcore

#endif  // MOCK_KBNODE_ATTRIBUTE_SUPPLIER_FACTORY_H_
