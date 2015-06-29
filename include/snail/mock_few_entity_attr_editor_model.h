// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_FEW_ENTITY_ATTR_EDITOR_MODEL_H_
#define MOCK_FEW_ENTITY_ATTR_EDITOR_MODEL_H_

#include "snail/i_few_entity_attr_editor_model.h"
#include "snail/mock_attribute_editor_model.h"

namespace snailcore {
namespace tests {

class MockFewEntityAttrEditorModel : public IFewEntityAttrEditorModel {
 public:
  ATTRIBUTE_EDITOR_MODEL_COMMON_MOCKS

  MOCK_CONST_METHOD0(getCandidateEntities,
                     std::vector<std::shared_ptr<const IEntity>>());

  MOCK_METHOD1(setCurrentEntity,
               void(std::shared_ptr<const IEntity> entity));
  MOCK_CONST_METHOD0(getCurrentEntityIndex, int());
};

}  // namespace tests
}  // namespace snailcore

#endif  // MOCK_FEW_ENTITY_ATTR_EDITOR_MODEL_H_
