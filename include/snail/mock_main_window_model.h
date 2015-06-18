// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_MOCK_MAIN_WINDOW_MODEL_H_
#define INCLUDE_SNAIL_MOCK_MAIN_WINDOW_MODEL_H_

#include "snail/i_main_window_model.h"

namespace snailcore {
namespace tests {

class MockMainWindowModel : public IMainWindowModel {
 public:
  SNAIL_MOCK_SLOT(WindowTitleChanged);
  SNAIL_MOCK_SLOT(RequestClose);

  MOCK_CONST_METHOD0(windowTitle, const utils::U8String& ());
  MOCK_METHOD1(setWindowTitle, void(const utils::U8String& newTitle));
  MOCK_METHOD0(getWorkSpaceModel, std::shared_ptr<IWorkSpaceModel>());
  MOCK_CONST_METHOD0(requestClose, bool());
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_SNAIL_MOCK_MAIN_WINDOW_MODEL_H_
