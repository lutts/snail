// Copyright (c) 2016
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef KBNODE_ATTRIBUTE_FACTORY_H_
#define KBNODE_ATTRIBUTE_FACTORY_H_

namespace snailcore {

FTO_BEGIN_NAMESPACE
class KbNodeAttributeSupplier;
FTO_END_NAMESPACE

class IKbNode;

class KbNodeAttributeSupplierFactory {
 public:
  virtual ~KbNodeAttributeSupplierFactory() = default;

  virtual fto::KbNodeAttributeSupplier* create(IKbNode* root_kbnode,
                                               int max_attrs) const = 0;
};

}  // namespace snailcore

#endif  // KBNODE_ATTRIBUTE_FACTORY_H_
