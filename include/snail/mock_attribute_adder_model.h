// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_ATTRIBUTE_ADDER_MODEL_H_
#define MOCK_ATTRIBUTE_ADDER_MODEL_H_

#include "snail/i_attribute_adder_model.h"

namespace snailcore {
namespace tests {

class MockAttributeAdderModel : public IAttributeAdderModel {
 public:
  SNAIL_MOCK_SLOT(ValidateComplete);
  SNAIL_MOCK_SLOT(DiscardAttributeModel);
  SNAIL_MOCK_SLOT(CurrentAttributeModelChanged);

  MOCK_CONST_METHOD0(getPrompt, utils::U8String());
  MOCK_CONST_METHOD0(getAllowedAttributeList, std::vector<IAttribute*>());
  MOCK_CONST_METHOD0(getCurrentAttributeIndex, int());
  MOCK_METHOD1(setCurrentAttributeIndex, void(int index));
  MOCK_METHOD0(getCurrentAttributeModel, std::shared_ptr<IAttributeModel>());

  MOCK_METHOD0(doAddAttribute, void());
};

}  // namespace tests
}  // namespace snailcore

#endif  // MOCK_ATTRIBUTE_ADDER_MODEL_H_
