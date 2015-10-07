// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_MOCK_ATTRIBUTE_MODEL_FACTORY_H_
#define INCLUDE_CORE_MOCK_ATTRIBUTE_MODEL_FACTORY_H_

#include "core/i_attribute_model_factory.h"

namespace snailcore {
namespace tests {

class MockAttributeModelFactory : public IAttributeModelFactory {
 public:
  MOCK_CONST_METHOD1(createAttributeModel,
                     std::shared_ptr<IAttributeModel>(IAttribute* attr));
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_CORE_MOCK_ATTRIBUTE_MODEL_FACTORY_H_
