// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/application_factory.h"

#include <memory>

#include "utils/basic_utils.h"
#include "src/core/main_window_model.h"
#include "src/core/workspace_model.h"
#include "src/core/work_model_factory.h"

namespace snailcore {

class ApplicationFactoryImpl {
 public:
  ApplicationFactoryImpl()
      : work_model_factory(utils::make_unique<WorkModelFactory>()) { }
  ~ApplicationFactoryImpl() = default;

 private:
  ApplicationFactoryImpl(const ApplicationFactoryImpl&) = delete;
  ApplicationFactoryImpl& operator=(const ApplicationFactoryImpl&) = delete;

  friend class ApplicationFactory;

  std::unique_ptr<IWorkModelFactory> work_model_factory;
};


ApplicationFactory::ApplicationFactory()
    : impl_(utils::make_unique<ApplicationFactoryImpl>()) {
}

ApplicationFactory::~ApplicationFactory() = default;

std::shared_ptr<ApplicationFactory> ApplicationFactory::create() {
  return std::make_shared<ApplicationFactory>();
}

std::shared_ptr<IMainWindowModel> ApplicationFactory::makeMainWindowModel() {
  static std::weak_ptr<IMainWindowModel> cache;

  auto model = cache.lock();
  if (!model) {
    auto workspace_model =
        std::make_shared<WorkSpaceModel>(impl_->work_model_factory.get());
    model = std::make_shared<MainWindowModel>(workspace_model);
    cache = model;
  }

  return model;
}

}  // namespace snailcore
