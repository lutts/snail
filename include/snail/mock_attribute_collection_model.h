// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_MOCK_ATTRIBUTE_COLLECTION_MODEL_H_
#define INCLUDE_SNAIL_MOCK_ATTRIBUTE_COLLECTION_MODEL_H_

#include <vector>

#include "snail/i_attribute_collection_model.h"

namespace snailcore {
namespace tests {

class MockAttributeCollectionModel : public IAttributeCollectionModel {
 public:
  MOCK_CONST_METHOD0(getAttributeSuppliers,
                     std::vector<IAttributeSupplier*>());
  MOCK_METHOD1(createAttributeModel,
               std::shared_ptr<IAttributeModel>(IAttribute* attr));
  MOCK_METHOD0(switchToEditMode, void());
  MOCK_METHOD0(switchToDisplayMode, void());
};

}  // namespace tests
}  // namespace snailcore


#endif  // INCLUDE_SNAIL_MOCK_ATTRIBUTE_COLLECTION_MODEL_H_
