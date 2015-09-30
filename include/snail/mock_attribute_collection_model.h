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
  SNAIL_MOCK_SLOT(SwitchToEditMode);
  SNAIL_MOCK_SLOT(SwitchToDisplayMode);
  SNAIL_MOCK_SLOT(ValidateComplete);

  MOCK_METHOD0(switchMode, void());
  MOCK_CONST_METHOD0(getAttributeSuppliers,
                     std::vector<IAttributeSupplier*>());
  MOCK_METHOD1(createAttributeModel,
               std::shared_ptr<IAttributeModel>(IAttribute* attr));
};

}  // namespace tests
}  // namespace snailcore


#endif  // INCLUDE_SNAIL_MOCK_ATTRIBUTE_COLLECTION_MODEL_H_
