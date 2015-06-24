// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_MOCK_ATTRIBUTE_EDITOR_MODEL_FACTORY_H_
#define INCLUDE_SNAIL_MOCK_ATTRIBUTE_EDITOR_MODEL_FACTORY_H_

#include "snail/i_attribute_editor_model_factory.h"

namespace snailcore {
namespace tests {

class MockAttributeEditorModelFactory : public IAttributeEditorModelFactory {
 public:
  MOCK_METHOD1(createAttributeEditorModel,
               std::shared_ptr<IAttributeEditorModel>(IAttribute* attr));
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_SNAIL_MOCK_ATTRIBUTE_EDITOR_MODEL_FACTORY_H_
