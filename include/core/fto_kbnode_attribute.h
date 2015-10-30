// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_FTO_KBNODE_ATTRIBUTE_H_
#define INCLUDE_CORE_FTO_KBNODE_ATTRIBUTE_H_

#include "include/config.h"
#include "snail/i_attribute.h"
#include "core/generic_attribute_supplier.h"
#include "test/test_proxy.h"

namespace snailcore {

class IKbNode;

namespace fto {

#ifndef DISABLE_TEST_CODE

#define INTERFACE_DEFINITION_PHASE
#include "test/interface.h"

class KbNodeAttributeSupplier : public GenericAttributeSupplier {
 public:
  KbNodeAttributeSupplier(const utils::U8String& name, int max_attrs)
      : GenericAttributeSupplier(name, max_attrs) { }
  virtual ~KbNodeAttributeSupplier() = default;

  virtual IKbNode* getRootKbNode() const = 0;
};

#define KbNodeAttribute_METHODS                                         \
  SNAIL_CONST_INTERFACE0(supplier, fto::KbNodeAttributeSupplier*());    \
  SNAIL_INTERFACE1(setKbNode, void(IKbNode* kbnode));

class KbNodeAttribute : public IAttribute {
 public:
  virtual ~KbNodeAttribute() = default;

  SNAIL_CONST_INTERFACE0(clone, fto::KbNodeAttribute*());
  KbNodeAttribute_METHODS
};

#undef INTERFACE_DEFINITION_PHASE

#define INTERFACE_TEST_PROXY_PHASE
#include "test/interface.h"

class KbNodeAttributeTestProxy : public KbNodeAttribute {
  TEST_PROXY_WITHOUT_DEFAULT_CONSTRUCTOR(KbNodeAttribute);

 public:
  utils::U8String displayName() const override {
    return real_obj_->displayName();
  }

  utils::U8String valueText() const override {
    return real_obj_->valueText();
  }

  bool isEmpty() const override {
    return real_obj_->isEmpty();
  }

  void clear() override {
    real_obj_->clear();
  }

  void accept(IAttributeVisitor* visitor) override {
    real_obj_->accept(visitor);
  }

  KbNodeAttribute_METHODS
};

#undef INTERFACE_TEST_PROXY_PHASE

#else  // DISABLE_TEST_CODE

class KbNodeAttributeSupplier : public GenericAttributeSupplier {
 public:
  KbNodeAttributeSupplier(const utils::U8String& name, int max_attrs)
      : GenericAttributeSupplier(name, max_attrs) { }
  virtual ~KbNodeAttributeSupplier() = default;
};

class KbNodeAttribute : public IAttribute { };

#endif  // DISABLE_TEST_CODE

}  // namespace fto
}  // namespace snailcore

#endif  // INCLUDE_CORE_FTO_KBNODE_ATTRIBUTE_H_
