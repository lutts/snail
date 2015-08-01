// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_MOCK_WORK_ATTRIBUTE_MODEL_H_
#define INCLUDE_SNAIL_MOCK_WORK_ATTRIBUTE_MODEL_H_

#include "snail/i_work_attribute_model.h"

namespace snailcore {
namespace tests {

class MockWorkAttributeModel : public IWorkAttributeModel {
 public:
  MOCK_CONST_METHOD0(isEditMode, bool());
  MOCK_METHOD0(switchToEditMode, void());
  MOCK_METHOD0(switchToDisplayMode, void());

  SNAIL_MOCK_SLOT(AttributesChanged);
  SNAIL_MOCK_SLOT(AttrLabelChanged);
  SNAIL_MOCK_SLOT(ShowPopupFor);

  MOCK_METHOD1(traverseAttributes,
               void(IAttributeDisplayBlockVisitor* visitor));
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_SNAIL_MOCK_WORK_ATTRIBUTE_MODEL_H_
