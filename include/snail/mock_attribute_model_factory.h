// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_ATTRIBUTE_MODEL_FACTORY_H_
#define MOCK_ATTRIBUTE_MODEL_FACTORY_H_

#include "snail/i_attribute_model_factory.h"

namespace snailcore {
namespace tests {

class MockAttributeModelFactory : public IAttributeModelFactory {
 public:
  MOCK_CONST_METHOD1(createModel,
                     std::shared_ptr<IAttributeModel>(IAttribute* attr));
};

}  // namespace tests
}  // namespace snailcore

#endif  // MOCK_ATTRIBUTE_MODEL_FACTORY_H_
