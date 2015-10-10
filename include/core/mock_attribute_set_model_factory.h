// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_CORE_MOCK_ATTRIBUTE_SET_MODEL_FACTORY_H_
#define INCLUDE_CORE_MOCK_ATTRIBUTE_SET_MODEL_FACTORY_H_

#include <vector>

#include "core/i_attribute_set_model_factory.h"

namespace snailcore {
namespace tests {

class MockAttributeSetModelFactory
    : public IAttributeSetModelFactory {
 public:
  MOCK_METHOD1(createAttributeSetModel,
               std::shared_ptr<IAttributeSetModel>(
                   const std::vector<IAttributeSupplier*>& attr_suppliers));
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_CORE_MOCK_ATTRIBUTE_SET_MODEL_FACTORY_H_
