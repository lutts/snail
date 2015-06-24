// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_ATTRIBUTE_MODEL_H_
#define MOCK_ATTRIBUTE_MODEL_H_

#include "snail/i_attribute_model.h"

namespace snailcore {
namespace tests {

class MockAttributeModel : public IAttributeModel {
 public:
  SNAIL_MOCK_SLOT(ValidateComplete);

  MOCK_CONST_METHOD0(getAttribute, IAttribute*());
};

}  // namespace tests
}  // namespace snailcore

#endif  // MOCK_ATTRIBUTE_MODEL_H_
