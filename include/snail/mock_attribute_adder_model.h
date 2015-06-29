// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_MOCK_ATTRIBUTE_ADDER_MODEL_H_
#define INCLUDE_SNAIL_MOCK_ATTRIBUTE_ADDER_MODEL_H_

#include <vector>
#include "snail/i_attribute_adder_model.h"

namespace snailcore {
namespace tests {

class MockAttributeAdderModel : public IAttributeAdderModel {
 public:
  SNAIL_MOCK_SLOT(ValidateComplete);
  SNAIL_MOCK_SLOT(DiscardAttributeEditorModel);
  SNAIL_MOCK_SLOT(CurrentAttributeEditorModelChanged);

  MOCK_CONST_METHOD0(getPrompt, utils::U8String());
  MOCK_CONST_METHOD0(getAllowedAttributeList, std::vector<IAttribute*>());
  MOCK_CONST_METHOD0(getCurrentAttributeIndex, int());
  MOCK_METHOD1(setCurrentAttributeIndex, void(int index));
  MOCK_METHOD0(getCurrentAttributeEditorModel,
               std::shared_ptr<IAttributeEditorModel>());
  MOCK_CONST_METHOD0(validateResult, bool());

  MOCK_METHOD0(doAddAttribute, void());
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_SNAIL_MOCK_ATTRIBUTE_ADDER_MODEL_H_
