// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#include "src/qtui/core/qt_uiengine.h"
#include "utils/basic_utils.h"

QtUiEngine::QtUiEngine()
    : qtApplication(utils::make_unique<QApplication>(dummy_argc, nullptr)) {
  // Q_INIT_RESOURCE(images);
  qtApplication->setAttribute(Qt::AA_UseHighDpiPixmaps);
  connect(qtApplication.get(), SIGNAL(aboutToQuit()),
          this, SLOT(aboutToQuitApp()));
}

int QtUiEngine::run() {
  return qtApplication->exec();
}

void QtUiEngine::quit() {
  qtApplication->quit();
}

void QtUiEngine::aboutToQuitApp() {
  AboutToQuit();
}
