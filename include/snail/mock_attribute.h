// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_MOCK_ATTRIBUTE_H_
#define INCLUDE_SNAIL_MOCK_ATTRIBUTE_H_

#include "snail/i_attribute.h"

#define COMMON_ATTRIBUTE_MOCKS                                  \
  MOCK_CONST_METHOD0(displayName, utils::U8String());           \
  MOCK_CONST_METHOD0(valueText, utils::U8String());             \
  MOCK_CONST_METHOD0(isEmpty, bool());                          \
  MOCK_METHOD0(clear, void());                                  \
  MOCK_METHOD1(accept, void(IAttributeVisitor* visitor));       \
  MOCK_METHOD0(clone, IAttribute*());                           \

namespace snailcore {
namespace tests {

class MockAttribute : public IAttribute {
 public:
  ~MockAttribute() { destroy(); }

  COMMON_ATTRIBUTE_MOCKS

  MOCK_METHOD0(destroy, void());
};

class NullAttribute : public IAttribute {
 public:
  utils::U8String displayName() const override {
    return "";
  }

  utils::U8String valueText() const override {
    return "";
  }

  bool isEmpty() const override {
    return true;
  }

  void clear() override { }

  void accept(IAttributeVisitor* visitor) override { (void)visitor; }
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_SNAIL_MOCK_ATTRIBUTE_H_
