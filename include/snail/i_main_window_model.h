// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_MAIN_WINDOW_MODEL_H_
#define INCLUDE_SNAIL_I_MAIN_WINDOW_MODEL_H_

#include "pfmvp/i_pf_model.h"
#include "utils/signal_slot.h"

#include "utils/u8string.h"

namespace snailcore {

class IWorkSpaceModel;

class IMainWindowModel : public pfmvp::IPfModel {
 public:
  virtual ~IMainWindowModel() { }

  DEF_MODEL_ID(MainWindowModel);

  SNAIL_SIGSLOT2(WindowTitleChanged, void(const utils::U8String&));
  SNAIL_SIGSLOT2(RequestClose, bool());

  virtual const utils::U8String& windowTitle() const = 0;
  virtual void setWindowTitle(const utils::U8String& newTitle) = 0;

  virtual std::shared_ptr<IWorkSpaceModel> getWorkSpaceModel() = 0;

  virtual bool requestClose() const = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_I_MAIN_WINDOW_MODEL_H_
