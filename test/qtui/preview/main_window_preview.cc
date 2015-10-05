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
#include "qtui/i_main_window_view.h"
#include "utils/i_trackable.h"

#if 0
#include "snail/i_command_processor.h"
#include "src/core/command/command.h"

class DummyCommand : public snailcore::Command {
 public:
  DummyCommand() { }
  ~DummyCommand() { }

  void redo() override { }
  void undo() override { }
};
#endif

class DummyTracker : public utils::ITrackable { };

using namespace snailcore;  // NOLINT
using namespace pfmvp;  // NOLINT

int main() {
  initLog();
  QtUiEngine qtUiEngine;
  auto app_factory = ApplicationFactory::create();
  std::shared_ptr<IPfTriadManager> triad_manager =
      std::make_shared<PfTriadManager>(PfViewFactoryManager::getInstance());

  auto dummyTrackObj = std::make_shared<DummyTracker>();

  {
    auto main_window_model = app_factory->makeMainWindowModel();
    auto main_window_view = triad_manager->createViewFor(main_window_model);

    main_window_model->setWindowTitle("hahaaaa, that's the title");
    IMainWindowView* main_window =
        dynamic_cast<IMainWindowView*>(main_window_view.get());
    main_window->setGeometry(800, 600);

#if 0
    main_window_model->whenRequestClose([]() ->bool {
        return false;
      }, dummyTrackObj);
#endif

    main_window_view->showView();

    auto raw_triad_manager = triad_manager.get();
    ((snailcore::IUiEngine*)&qtUiEngine)->whenAboutToQuit(
        [raw_triad_manager, main_window]() {
          raw_triad_manager->removeTriadBy(main_window);
        }, triad_manager);
  }

#if 0
  auto commandProcessor = appContext->getCommandProcessor();
  commandProcessor->do_cmd(new DummyCommand());
  commandProcessor->do_cmd(new DummyCommand());
#endif

  int ret = qtUiEngine.run();
  cleanupLog();
  return ret;
}
