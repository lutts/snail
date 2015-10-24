// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#include "src/qtui/core/qt_uiengine.h"

#include "src/utils/log/log.h"

#include "utils/basic_utils.h"
#include "src/core/factory/application_factory.h"
#include "src/pfmvp/pf_triad_manager.h"
#include "pfmvp/pf_view_factory_manager.h"
#include "src/core/main_window_model.h"
#include "qtui/ui/i_main_window_view.h"

using namespace snailcore;  // NOLINT
using namespace pfmvp;  // NOLINT

int main() {
  initLog();

  QtUiEngine qtUiEngine;

  auto app_factory = ApplicationFactory::create();
  std::shared_ptr<IPfTriadManager> triad_manager =
      std::make_shared<PfTriadManager>(PfViewFactoryManager::getInstance());

  auto main_window_model = app_factory->makeMainWindowModel();
  auto main_window_view = triad_manager->createViewFor(main_window_model);

  main_window_model->setWindowTitle("Snail Music Reader");
  IMainWindowView* main_window =
      dynamic_cast<IMainWindowView*>(main_window_view.get());
  main_window->setGeometry(800, 600);

  main_window_view->showView();

  auto raw_triad_manager = triad_manager.get();
  ((snailcore::IUiEngine*)&qtUiEngine)->whenAboutToQuit(
      [raw_triad_manager, main_window]() {
        raw_triad_manager->removeTriadBy(main_window);
      }, triad_manager);

  int ret = qtUiEngine.run();
  cleanupLog();
  return ret;
}
