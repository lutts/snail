// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_MOCK_ATTRIBUTE_SET_MODEL_H_
#define INCLUDE_SNAIL_MOCK_ATTRIBUTE_SET_MODEL_H_

#include <vector>

#include "snail/i_attribute_set_model.h"

namespace snailcore {
namespace tests {

class MockAttributeSetModel : public IAttributeSetModel {
 public:
  SNAIL_MOCK_SLOT(SwitchToEditMode);
  SNAIL_MOCK_SLOT(SwitchToDisplayMode);
  SNAIL_MOCK_SLOT(ValidateComplete);

  MOCK_CONST_METHOD0(getAttributeSuppliers, std::vector<IAttributeSupplier*>());
  MOCK_METHOD0(switchMode, void());
  MOCK_METHOD1(createAttributeModel,
               std::shared_ptr<IAttributeModel>(IAttribute* attr));
  MOCK_METHOD1(addAttribute, IAttribute*(IAttributeSupplier* supplier));
  MOCK_METHOD1(closeAttributeEditors,
               void(pfmvp::IPfTriadManager* triad_manager));
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_SNAIL_MOCK_ATTRIBUTE_SET_MODEL_H_
