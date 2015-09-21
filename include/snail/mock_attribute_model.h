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
  SNAIL_MOCK_SLOT(ValidateComplete);
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_SNAIL_MOCK_ATTRIBUTE_MODEL_H_
