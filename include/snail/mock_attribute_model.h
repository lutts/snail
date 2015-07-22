// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_MOCK_ATTRIBUTE_MODEL_H_
#define INCLUDE_SNAIL_MOCK_ATTRIBUTE_MODEL_H_

#include "snail/i_attribute_model.h"

namespace snailcore {
namespace tests {

class MockAttributeModel : public IAttributeModel {
 public:
  MOCK_CONST_METHOD0(displayName, utils::U8String());
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_SNAIL_MOCK_ATTRIBUTE_MODEL_H_
