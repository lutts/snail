// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#include "src/qtui/qt_uiengine.h"

#include "src/utils/log/log.h"

#include "utils/basic_utils.h"
#include "src/pfmvp/pf_triad_manager.h"
#include "pfmvp/pf_view_factory_manager.h"
#include "src/core/main_window_model.h"
//#include "snail/i_command_processor.h"

#if 0
#include "src/core/command/command.h"

class DummyCommand : public snailcore::Command {
 public:
  DummyCommand() { }
  ~DummyCommand() { }

  void redo() override { }
  void undo() override { }
};
#endif

using namespace snailcore;  // NOLINT
using namespace pfmvp;  // NOLINT

int main() {
  initLog();
  QtUiEngine qtUiEngine;
  PfTriadManager triad_manager(PfViewFactoryManager::getInstance());

  {
    auto main_window_model = makeMainWindowModel();
    auto main_window_view = triad_manager.createViewFor(main_window_model);
    main_window_view->showView();
  }

#if 0
  ((snailcore::IUiEngine*)&qtUiEngine)->whenAboutToQuit(
      [raw_appContext]() {
        raw_appContext->aboutToQuitHandler();
      }, appContext);

  auto mainWindow = viewLoader.loadMainWindow();
  mainWindow->setGeometry(800, 600);

  auto mainWindowModel = appContext->getMainWindowModel();
  mainWindowModel->setWindowTitle("hahaaaa, that's the title");
#if 0
  mainWindowModel->whenRequestClose([]() ->bool {
      return false;
    }, appContext);
#endif

  auto commandProcessor = appContext->getCommandProcessor();
  commandProcessor->do_cmd(new DummyCommand());
  commandProcessor->do_cmd(new DummyCommand());
#endif

  return qtUiEngine.run();
}
