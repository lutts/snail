// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_ATTRIBUTE_COLLECTION_MODEL_FACTORY_H_
#define MOCK_ATTRIBUTE_COLLECTION_MODEL_FACTORY_H_

#include "core/i_attribute_collection_model_factory.h"

namespace snailcore {
namespace tests {

class MockAttributeCollectionModelFactory
    : public IAttributeCollectionModelFactory {
 public:
  MOCK_METHOD1(createAttributeCollectionModel,
               std::shared_ptr<IAttributeCollectionModel>(
                   const std::vector<IAttributeSupplier*>& attr_suppliers));
};

}  // namespace tests
}  // namespace snailcore

#endif  // MOCK_ATTRIBUTE_COLLECTION_MODEL_FACTORY_H_
