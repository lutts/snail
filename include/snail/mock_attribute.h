// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_MOCK_ATTRIBUTE_H_
#define INCLUDE_SNAIL_MOCK_ATTRIBUTE_H_

#include "snail/i_attribute.h"

namespace snailcore {
namespace tests {

class MockAttribute : public IAttribute {
 public:
  MOCK_CONST_METHOD0(displayName, utils::U8String());
  MOCK_CONST_METHOD0(valueText, utils::U8String());
  MOCK_CONST_METHOD0(isEmpty, bool());
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_SNAIL_MOCK_ATTRIBUTE_H_
