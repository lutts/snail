// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_MOCK_ATTRIBUTE_EDITOR_MODEL_H_
#define INCLUDE_SNAIL_MOCK_ATTRIBUTE_EDITOR_MODEL_H_

#include "snail/i_attribute_editor_model.h"

namespace snailcore {
namespace tests {

#define ATTRIBUTE_MODEL_COMMON_MOCKS                         \
  SNAIL_MOCK_SLOT(ValidateComplete);                    \
                                                        \
  MOCK_CONST_METHOD0(getAttribute, IAttribute*());

class MockAttributeEditorModel : public IAttributeEditorModel {
 public:
  ATTRIBUTE_MODEL_COMMON_MOCKS
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_SNAIL_MOCK_ATTRIBUTE_EDITOR_MODEL_H_
