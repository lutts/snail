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
#include "core/i_kbnode_attribute_supplier_factory.h"
#include "test/test_proxy.h"

#define INTERFACE_DEFINITION_PHASE
#include "test/interface.h"

namespace snailcore {

class IKbNode;

namespace fto {

#ifndef DISABLE_TEST_CODE

#define KbNodeAttributeSupplier_METHODS                         \
  SNAIL_CONST_INTERFACE0(getRootKbNode, IKbNode*());            \
  SNAIL_CONST_INTERFACE0(clone, KbNodeAttributeSupplier*());

class KbNodeAttributeSupplier : public GenericAttributeSupplier {
 public:
  KbNodeAttributeSupplier(const utils::U8String& name, int max_attrs)
      : GenericAttributeSupplier(name, max_attrs) { }
  virtual ~KbNodeAttributeSupplier() = default;


  KbNodeAttributeSupplier_METHODS
};

#define KbNodeAttribute_METHODS                                         \
  SNAIL_CONST_INTERFACE0(supplier, fto::KbNodeAttributeSupplier*());    \
  SNAIL_INTERFACE1(setKbNode, void(IKbNode* kbnode));

class KbNodeAttribute : public IAttribute {
 public:
  virtual ~KbNodeAttribute() = default;

  SNAIL_CONST_INTERFACE0(clone, KbNodeAttribute*());
  virtual KbNodeAttribute& operator=(KbNodeAttribute&& rhs) = 0;
  KbNodeAttribute_METHODS
};

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

#undef INTERFACE_DEFINITION_PHASE

#define INTERFACE_TEST_PROXY_PHASE
#include "test/interface.h"

namespace snailcore {
namespace fto {

class KbNodeAttributeSupplierTestProxy {
  TEST_PROXY_WITHOUT_DEFAULT_CONSTRUCTOR(KbNodeAttributeSupplier);
  TEST_PROXY_ENABLE_COPY(KbNodeAttributeSupplier);
  TEST_PROXY_ENABLE_FACTORY_SUPPORT(IKbNodeAttributeSupplier);

 public:
  KbNodeAttributeSupplierTestProxy(IKbNode* root_kbnode, int max_attrs) {
    setSelf(getFactory()->createInstance(root_kbnode, max_attrs));
  }

  utils::U8String name() const {
    return self_->name();
  }

  int attr_count() const {
    return self_->attr_count();
  }

  IAttribute* addAttribute() {
    return self_->addAttribute();
  }

  std::vector<IAttribute*> attributes() const {
    return self_->attributes();
  }

  KbNodeAttributeSupplier_METHODS
};

class KbNodeAttributeTestProxy {
  TEST_PROXY_WITHOUT_DEFAULT_CONSTRUCTOR(KbNodeAttribute);
  TEST_PROXY_ENABLE_COPY(KbNodeAttribute);

 public:
  utils::U8String displayName() const {
    return self_->displayName();
  }

  utils::U8String valueText() const {
    return self_->valueText();
  }

  bool isEmpty() const {
    return self_->isEmpty();
  }

  void clear() {
    self_->clear();
  }

  void accept(IAttributeVisitor* visitor) {
    self_->accept(visitor);
  }

  KbNodeAttribute_METHODS
};

}  // namespace fto
}  // namespace snailcore

#undef INTERFACE_TEST_PROXY_PHASE

#endif  // INCLUDE_CORE_FTO_KBNODE_ATTRIBUTE_H_
