// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_WORK_ATTRIBUTE_MODEL_H_
#define MOCK_WORK_ATTRIBUTE_MODEL_H_

#include "snail/i_work_attribute_model.h"

namespace snailcore {
namespace tests {

class MockWorkAttributeModel : public IWorkAttributeModel {
 public:
  MOCK_CONST_METHOD0(isEditMode, bool());
  MOCK_METHOD0(switchToEditMode, void());
  MOCK_METHOD0(switchToDisplayMode, void());

  SNAIL_MOCK_SLOT(AttributesChanged);
  SNAIL_MOCK_SLOT(ShowPopupFor);

  MOCK_CONST_METHOD0(getAttributeModels,
                     std::vector<std::shared_ptr<IAttributeModel>>());
  MOCK_CONST_METHOD0(getEraseCommands, std::vector<utils::Command*>());
  MOCK_CONST_METHOD0(getPopupEditorCommands, std::vector<utils::Command*>());
  MOCK_CONST_METHOD0(getAddAttributeCommands, std::vector<utils::Command*>());
  MOCK_CONST_METHOD1(getLocation, Location(IPfModel* model));
  MOCK_CONST_METHOD1(getLocation, Location(utils::Command* command));
};

}  // namespace tests
}  // namespace snailcore

#endif  // MOCK_WORK_ATTRIBUTE_MODEL_H_
