// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef I_KBNODE_ATTRIBUTE_SUPPLIER_FACTORY_H_
#define I_KBNODE_ATTRIBUTE_SUPPLIER_FACTORY_H_

#define INTERFACE_DEFINITION_PHASE
#include "test/interface.h"

namespace snailcore {

class IKbNode;

FTO_BEGIN_NAMESPACE
class KbNodeAttributeSupplier;
FTO_END_NAMESPACE

#define IKbNodeAttributeSupplierFactory_METHODS \
  SNAIL_CONST_INTERFACE1(createInstance,        \
                           fto::KbNodeAttributeSupplier*(IKbNode* root_kbnode));

class IKbNodeAttributeSupplierFactory {
 public:
  virtual ~IKbNodeAttributeSupplierFactory() = default;

  IKbNodeAttributeSupplierFactory_METHODS
};

}  // namespace snailcore

#undef INTERFACE_DEFINITION_PHASE

#endif  // I_KBNODE_ATTRIBUTE_SUPPLIER_FACTORY_H_
