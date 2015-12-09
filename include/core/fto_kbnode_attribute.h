// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_FTO_KBNODE_ATTRIBUTE_H_
#define INCLUDE_CORE_FTO_KBNODE_ATTRIBUTE_H_

// region: shared with impl

#include <vector>

#include "include/config.h"
#include "snail/i_attribute.h"
#include "snail/i_attribute_supplier.h"

namespace snailcore {
class IKbNode;
}  // namespace snailcore

// endregion: shared

// region: Interface

#ifndef DISABLE_TEST_CODE

#include "test/test_proxy.h"

#define INTERFACE_DEFINITION_PHASE
#include "test/interface.h"

namespace snailcore {
namespace fto {

#define KbNodeAttributeSupplier_METHODS \
  SNAIL_CONST_INTERFACE0(getRootKbNode, IKbNode*());

class KbNodeAttributeSupplier : public IAttributeSupplier {
 public:
  virtual ~KbNodeAttributeSupplier() = default;

  SNAIL_CONST_INTERFACE0(clone, KbNodeAttributeSupplier*());
  TEST_PROXY_COPY_ASSIGN_INTERFACE(KbNodeAttributeSupplier);

  KbNodeAttributeSupplier_METHODS
};

#define KbNodeAttributeSupplierFactory_METHODS \
  SNAIL_CONST_INTERFACE2(                      \
      createInstance,                          \
      fto::KbNodeAttributeSupplier*(IKbNode * root_kbnode, int max_attrs));

class KbNodeAttributeSupplierFactory {
 public:
  virtual ~KbNodeAttributeSupplierFactory() = default;

  KbNodeAttributeSupplierFactory_METHODS
};

class KbNodeAttribute;

#define KbNodeAttribute_METHODS                                      \
  SNAIL_CONST_INTERFACE0(supplier, fto::KbNodeAttributeSupplier*()); \
  SNAIL_INTERFACE1(setKbNode, void(IKbNode * kbnode));               \
  SNAIL_INTERFACE1(copyExceptSupplier, void(const KbNodeAttribute& other));

class KbNodeAttribute : public IAttribute {
 public:
  virtual ~KbNodeAttribute() = default;

  SNAIL_CONST_INTERFACE0(clone, KbNodeAttribute*());
  virtual KbNodeAttribute& operator=(KbNodeAttribute&& rhs) = 0;

  KbNodeAttribute_METHODS
};

}  // namespace fto
}  // namespace snailcore

#undef INTERFACE_DEFINITION_PHASE

#else  // DISABLE_TEST_CODE

namespace snailcore {
namespace fto {

class KbNodeAttributeSupplier : public IAttributeSupplier {};

class KbNodeAttribute : public IAttribute {};

}  // namespace fto
}  // namespace snailcore

#endif  // DISABLE_TEST_CODE
// endregion: Interface

// region: TestProxy
#ifndef DISABLE_TEST_CODE

#define INTERFACE_TEST_PROXY_PHASE
#include "test/interface.h"

namespace snailcore {
namespace fto {

class KbNodeAttributeSupplierTestProxy {
  TEST_PROXY_WITHOUT_DEFAULT_CONSTRUCTOR(KbNodeAttributeSupplier);
  TEST_PROXY_ENABLE_COPY_CONSTRUCT(KbNodeAttributeSupplier);
  TEST_PROXY_ENABLE_COPY_ASSIGNMENT(KbNodeAttributeSupplier);
  TEST_PROXY_DISABLE_MOVE_CONSTRUCT(KbNodeAttributeSupplier);
  TEST_PROXY_DISABLE_MOVE_ASSIGNMENT(KbNodeAttributeSupplier);
  TEST_PROXY_DISABLE_SWAP(KbNodeAttributeSupplier);

  TEST_PROXY_ENABLE_FACTORY_SUPPORT(KbNodeAttributeSupplier);

 public:
  KbNodeAttributeSupplierTestProxy(IKbNode* root_kbnode, int max_attrs) {
    createInstance(root_kbnode, max_attrs);
  }

  utils::U8String name() const { return self_->name(); }

  int attr_count() const { return self_->attr_count(); }

  IAttribute* addAttribute() { return self_->addAttribute(); }

  std::vector<IAttribute*> attributes() const { return self_->attributes(); }

  SNAIL_SIGSLOT_PROXY(KbNodeAttributeSupplier, AttributeChanged);

  KbNodeAttributeSupplier_METHODS
};

class KbNodeAttributeTestProxy {
  TEST_PROXY_WITHOUT_DEFAULT_CONSTRUCTOR(KbNodeAttribute);
  TEST_PROXY_ENABLE_COPY_CONSTRUCT(KbNodeAttribute);
  TEST_PROXY_DISABLE_COPY_ASSIGNMENT(KbNodeAttribute);
  TEST_PROXY_DISABLE_MOVE_CONSTRUCT(KbNodeAttribute);
  TEST_PROXY_ENABLE_MOVE_ASSIGNMENT(KbNodeAttribute);
  TEST_PROXY_DISABLE_SWAP(KbNodeAttribute);

 public:
  utils::U8String displayName() const { return self_->displayName(); }

  utils::U8String valueText() const { return self_->valueText(); }

  bool isEmpty() const { return self_->isEmpty(); }

  void clear() { self_->clear(); }

  void accept(IAttributeVisitor* visitor) { self_->accept(visitor); }

  KbNodeAttribute_METHODS
};

#undef INTERFACE_TEST_PROXY_PHASE

}  // namespace fto
}  // namespace snailcore

#undef INTERFACE_TEST_PROXY_PHASE
#endif  // DISABLE_TEST_CODE

// endregion: TestProxy

#endif  // INCLUDE_CORE_FTO_KBNODE_ATTRIBUTE_H_
