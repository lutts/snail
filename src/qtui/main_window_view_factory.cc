// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "snail/i_main_window_model.h"
#include "src/qtui/main_window_presenter.h"
#include "src/qtui/main_window.h"
#include "pfmvp/pf_view_factory_manager.h"

using namespace pfmvp;  // NOLINT
using namespace snailcore;  // NOLINT

class MainWindowViewFactory : public IPfViewFactory {
 public:
  MainWindowViewFactory() = default;
  virtual ~MainWindowViewFactory() = default;

  DEF_VIEW_FACTORY_ID(MainWindowViewFactory)

  std::shared_ptr<PfPresenter>
  createView(std::shared_ptr<IPfModel> model) override {
    auto the_model = std::dynamic_pointer_cast<IMainWindowModel>(model);
    if (the_model) {
      auto view = std::make_shared<MainWindow>();
      return MainWindowPresenter::create(the_model, view);
    }

    return nullptr;
  }

 private:
  MainWindowViewFactory(const MainWindowViewFactory& other) = delete;
  MainWindowViewFactory& operator=(const MainWindowViewFactory& other) = delete;
};

static view_factory_single_t<IMainWindowModel, MainWindowViewFactory>
g_main_window_view_factory;
