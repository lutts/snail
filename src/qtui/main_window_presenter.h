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

using MainWindowPresenterBase =
    pfmvp::PfPresenterT<snailcore::IMainWindowModel,
                            IMainWindowView>;

class MainWindowPresenter : public MainWindowPresenterBase {
 public:
  static std::shared_ptr<MainWindowPresenter>
  create(std::shared_ptr<snailcore::IMainWindowModel> model,
         std::shared_ptr<IMainWindowView> view) {
    auto presenter = std::make_shared<MainWindowPresenter>(model, view);
    presenter->initialize(presenter);
    return presenter;
  }

  MainWindowPresenter(std::shared_ptr<snailcore::IMainWindowModel> model,
                std::shared_ptr<IMainWindowView> view)
      : MainWindowPresenterBase(model, view) {
  }

 private:
  MainWindowPresenter(const MainWindowPresenter& other) = delete;
  MainWindowPresenter& operator=(const MainWindowPresenter& other) = delete;

  void initialize(std::shared_ptr<MainWindowPresenter> self) {
    view()->setWindowTitle2(model()->windowTitle());

    model()->whenWindowTitleChanged(
        [this](const utils::U8String& newTitle) {
          view()->setWindowTitle2(newTitle);
        }, self);

    view()->whenRequestClose(
        [this]() -> bool {
          return model()->requestClose();
        }, self);
  }
};

#endif  // SRC_QTUI_MAIN_WINDOW_PRESENTER_H_
