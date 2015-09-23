// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_MOCK_KBNODE_ATTRIBUTE_MODEL_H_
#define INCLUDE_SNAIL_MOCK_KBNODE_ATTRIBUTE_MODEL_H_

#include "snail/i_kbnode_attribute_model.h"

namespace snailcore {
namespace tests {

class MockKbNodeAttributeModel : public IKbNodeAttributeModel {
 public:
  // IAttributeModel
  SNAIL_MOCK_SLOT(ValidateComplete);

  // Self
  MOCK_CONST_METHOD0(getKbNodeProvider, IKbNodeProvider*());
  MOCK_CONST_METHOD0(getKbNodeName, utils::U8String());
  MOCK_METHOD1(setKbNode, void(IKbNode* kbnode));
  MOCK_METHOD1(setKbNodeByName, int(const utils::U8String& name));
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_SNAIL_MOCK_KBNODE_ATTRIBUTE_MODEL_H_
