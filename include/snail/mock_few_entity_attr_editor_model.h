// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_MOCK_FEW_ENTITY_ATTR_EDITOR_MODEL_H_
#define INCLUDE_SNAIL_MOCK_FEW_ENTITY_ATTR_EDITOR_MODEL_H_

#include "snail/i_few_entity_attr_editor_model.h"
#include "snail/mock_attribute_editor_model.h"

namespace snailcore {
namespace tests {

class MockFewEntityAttrEditorModel : public IFewEntityAttrEditorModel {
 public:
  ATTRIBUTE_EDITOR_MODEL_COMMON_MOCKS

  MOCK_CONST_METHOD0(getCandidateEntities, const CandidateItem*());
  MOCK_METHOD1(setCurrentEntity, void(const CandidateItem& entity_item));
  MOCK_CONST_METHOD0(getCurrentEntityName, utils::U8String());
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_SNAIL_MOCK_FEW_ENTITY_ATTR_EDITOR_MODEL_H_
