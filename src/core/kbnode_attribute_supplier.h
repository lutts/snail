// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef KBNODE_ATTRIBUTE_SUPPLIER_H_
#define KBNODE_ATTRIBUTE_SUPPLIER_H_

#include "utils/basic_utils.h"
#include "core/generic_attribute_supplier.h"

namespace snailcore {

class IKbNode;

class KbNodeAttributeSupplier : public GenericAttributeSupplier {
 public:
  KbNodeAttributeSupplier(IKbNode* root_kbnode, int max_attrs);
  virtual ~KbNodeAttributeSupplier();

  // GenericAttributeSupplier impls
  IAttribute* createAttribute() override;

  // KbNodeAttributeSupplier impls
  IKbNode* getRootKbNode() const;

 private:
  SNAIL_DISABLE_COPY(KbNodeAttributeSupplier);

  IKbNode* root_kbnode_ { nullptr };
};

}  // namespace snailcore

#endif  // KBNODE_ATTRIBUTE_SUPPLIER_H_
