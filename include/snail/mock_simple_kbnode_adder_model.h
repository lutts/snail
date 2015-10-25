// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_MOCK_SIMPLE_KBNODE_ADDER_MODEL_H_
#define INCLUDE_SNAIL_MOCK_SIMPLE_KBNODE_ADDER_MODEL_H_

#include "snail/i_simple_kbnode_adder_model.h"

namespace snailcore {
namespace tests {

class MockSimpleKbNodeAdderModel : public ISimpleKbNodeAdderModel {
 public:
  MOCK_CONST_METHOD0(name, utils::U8String());
  MOCK_CONST_METHOD0(getKbNodeProvider, ITreeItemProvider*());
  MOCK_CONST_METHOD0(getDefaultNewKbNodeName, utils::U8String());
  MOCK_CONST_METHOD0(getNewKbNodeParent, IKbNode*());
  MOCK_METHOD1(setNewKbNodeParent, void(IKbNode* kbnode));
  MOCK_METHOD1(setNewKbNodeName, void(const utils::U8String& name));
  MOCK_CONST_METHOD0(isNewKbNodeNameValid, bool());
  MOCK_METHOD1(setIsCategory, void(bool category));
  MOCK_METHOD0(addKbNode, void());
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_SNAIL_MOCK_SIMPLE_KBNODE_ADDER_MODEL_H_
