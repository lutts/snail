// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_MOCK_ATTRIBUTE_H_
#define INCLUDE_CORE_MOCK_ATTRIBUTE_H_

#include "core/i_attribute.h"

namespace snailcore {
namespace tests {

class MockAttribute : public IAttribute {
 public:
  MOCK_CONST_METHOD0(displayName, utils::U8String());
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_CORE_MOCK_ATTRIBUTE_H_
