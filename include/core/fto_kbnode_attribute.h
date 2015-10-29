// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_FTO_KBNODE_ATTRIBUTE_H_
#define INCLUDE_CORE_FTO_KBNODE_ATTRIBUTE_H_

#include "snail/i_attribute.h"
#include "core/generic_attribute_supplier.h"
#include "test/test_proxy.h"

#ifndef DISABLE_TEST_CODE

namespace snailcore {

class IKbNode;

namespace fto {

class KbNodeAttributeSupplier : public GenericAttributeSupplier {
 public:
  KbNodeAttributeSupplier(const utils::U8String& name, int max_attrs)
      : GenericAttributeSupplier(name, max_attrs) { }
  virtual ~KbNodeAttributeSupplier() = default;

  virtual IKbNode* getRootKbNode() const = 0;
};

class KbNodeAttribute : public IAttribute {
 public:
  virtual ~KbNodeAttribute() = default;

  virtual KbNodeAttributeSupplier* supplier() const = 0;
  virtual void setKbNode(IKbNode* kbnode) = 0;

  KbNodeAttribute* clone() const override = 0;
};

class KbNodeAttributeTestProxy : public KbNodeAttribute {
  TEST_PROXY_WITH_DEFAULT_CONSTRUCTOR(KbNodeAttribute);

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

  KbNodeAttributeSupplier* supplier() const override {
    return real_obj_->supplier();
  }

  void setKbNode(IKbNode* kbnode) override {
    real_obj_->setKbNode(kbnode);
  }
};

}  // namespace fto
}  // namespace snailcore

#else  // DISABLE_TEST_CODE

#include "include/config.h"

namespace snailcore {
namespace fto {

class KbNodeAttributeSupplier : public GenericAttributeSupplier {
 public:
  KbNodeAttributeSupplier(const utils::U8String& name, int max_attrs)
      : GenericAttributeSupplier(name, max_attrs) { }
  virtual ~KbNodeAttributeSupplier() = default;
};

class KbNodeAttribute : public IAttribute { };

}  // namespace fto
}  // namespace snailcore

#endif  // DISABLE_TEST_CODE
#endif  // INCLUDE_CORE_FTO_KBNODE_ATTRIBUTE_H_
