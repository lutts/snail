// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_MOCK_ATTRIBUTE_MODEL_FACTORY_H_
#define INCLUDE_SNAIL_MOCK_ATTRIBUTE_MODEL_FACTORY_H_

#include "core/i_attribute_model_factory.h"

namespace snailcore {
namespace tests {

class MockAttributeModelFactory : public IAttributeModelFactory {
 public:
  MOCK_CONST_METHOD1(createModel,
                     std::shared_ptr<IAttributeModel>(IAttribute* attr));
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_SNAIL_MOCK_ATTRIBUTE_MODEL_FACTORY_H_
