// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef MOCK_WORKSPACE_MODEL_H_
#define MOCK_WORKSPACE_MODEL_H_

#include "snail/i_workspace_model.h"

namespace snailcore {
namespace tests {

class MockWorkSpaceModel : public IWorkSpaceModel {
 public:
  SNAIL_MOCK_SLOT(WorkModelAdded);
  SNAIL_MOCK_SLOT(ActiveWorkModelChanged);
  SNAIL_MOCK_SLOT(WorkModelActivelyRemoved);

  MOCK_METHOD1(createWork, void(const utils::U8String& work_name));
  MOCK_METHOD1(removeWorkModel, void(IWorkModel* model));
};

}  // namespace tests
}  // namespace snailcore

#endif  // MOCK_WORKSPACE_MODEL_H_
