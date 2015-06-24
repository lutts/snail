// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_ATTRIBUTE_H_
#define MOCK_ATTRIBUTE_H_

#include "snail/i_attribute.h"

namespace snailcore {
namespace tests {

class MockAttribute : public IAttribute {
 public:
  MOCK_CONST_METHOD0(clone, IAttribute*());
};

}  // namespace tests
}  // namespace snailcore

#endif  // MOCK_ATTRIBUTE_H_
