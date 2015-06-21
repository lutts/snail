// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_MAIN_WINDOW_PRESENTER_H_
#define SRC_QTUI_MAIN_WINDOW_PRESENTER_H_

#include <memory>  // std::shared_ptr

#include "pfmvp/pf_presenter.h"
#include "snail/i_main_window_model.h"
#include "qtui/i_main_window_view.h"

#include "snail/i_workspace_model.h"
#include "qtui/i_workspace_view.h"

using MainWindowPresenterBase =
    pfmvp::PfPresenterT<snailcore::IMainWindowModel,
                            IMainWindowView>;

class MainWindowPresenter : public MainWindowPresenterBase {
 public:
  MainWindowPresenter(std::shared_ptr<snailcore::IMainWindowModel> model,
                std::shared_ptr<IMainWindowView> view)
      : MainWindowPresenterBase(model, view) {
  }

  void initialize() override {
    // assert(this == shared_from_this().get());
    view()->setWindowTitle2(model()->windowTitle());

    auto workspace_model = model()->getWorkSpaceModel();
    auto workspace_view = createRawViewFor<IWorkSpaceView>(workspace_model);
    view()->setWorkSpaceView(workspace_view);

    model()->whenWindowTitleChanged(
        [this](const utils::U8String& newTitle) {
          view()->setWindowTitle2(newTitle);
        }, shared_from_this());

    view()->whenUserClickAddWork(
        [this](const utils::U8String& work_name) {
          model()->createWork(work_name);
        },
        shared_from_this());

    view()->whenRequestClose(
        [this]() -> bool {
          return model()->requestClose();
        }, shared_from_this());
  }

 private:
  MainWindowPresenter(const MainWindowPresenter& other) = delete;
  MainWindowPresenter& operator=(const MainWindowPresenter& other) = delete;
};

#endif  // SRC_QTUI_MAIN_WINDOW_PRESENTER_H_
