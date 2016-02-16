// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/factory/application_factory.h"

#include <memory>
#include <vector>

#include "utils/basic_utils.h"
#include "src/core/main_window_model.h"
#include "src/core/workspace_model.h"

#include "src/core/kbnode_manager.h"

#include "src/core/factory/simple_kbnode_adder_model_factory.h"
#include "src/core/factory/attribute_set_model_factory.h"
#include "src/core/factory/work_model_factory.h"

// factories
#include "src/core/factory/attribute_model_factory.h"
#include "src/core/factory/work_factory.h"

namespace snailcore {

class ApplicationFactoryImpl {
 public:
  ApplicationFactoryImpl() {
    getKbNodeManager();
    getSimpleKbNodeAdderModelFactory();
    getAttrModelFactory();
    getAttributeSetModelFactory();
    getWorkModelFactory();
    getWorkFactory();
  }
  ~ApplicationFactoryImpl() = default;

  /*
   * About using XXX::createSingleInstance() or getXXX
   *
   * * if we use XXX::createSingleInstance, sometimes we have to pass arguments
   *   to it, this may makes it looks strange to some people because they think
   *   pass arguments will got different instance for difference args, although
   *   the arguments we passed is also singletons.
   * * if we use getXXX, besides its name is ambiguous, we do not have to wary
   *   about the arguments, and we can ensure the creation order of the
   *   instances easily.
   *
   * may be some day we should use Dependency-Injection framework to
   * simplify these singleton instances' creation, but currently I don't want
   * to use a third-party DI framework.
   */
  fto::KbNodeManager* getKbNodeManager() {
    if (!kbnode_manager_) kbnode_manager_ = utils::make_unique<KbNodeManager>();

    return kbnode_manager_.get();
  }

  ISimpleKbNodeAdderModelFactory* getSimpleKbNodeAdderModelFactory() {
    if (!simple_kbnode_adder_model_factory_) {
      simple_kbnode_adder_model_factory_ =
          utils::make_unique<SimpleKbNodeAdderModelFactoryImpl>(
              getKbNodeManager());
    }

    return simple_kbnode_adder_model_factory_.get();
  }

  IAttributeModelFactory* getAttrModelFactory() {
    if (!attr_model_factory_) {
      attr_model_factory_ = utils::make_unique<AttributeModelFactory>(
          getKbNodeManager(), getSimpleKbNodeAdderModelFactory());
    }

    return attr_model_factory_.get();
  }

  IAttributeSetModelFactory* getAttributeSetModelFactory() {
    if (!attr_set_model_factory_) {
      attr_set_model_factory_ =
          utils::make_unique<AttributeSetModelFactoryImpl>(
              getAttrModelFactory());
    }

    return attr_set_model_factory_.get();
  }

  IWorkModelFactory* getWorkModelFactory() {
    if (!work_model_factory_) {
      work_model_factory_ = utils::make_unique<WorkModelFactoryImpl>(
          getAttributeSetModelFactory());
    }

    return work_model_factory_.get();
  }

  IWorkFactory* getWorkFactory() {
    if (!work_factory_) {
      work_factory_ = utils::make_unique<WorkFactory>(getKbNodeManager());
    }

    return work_factory_.get();
  }

 private:
  ApplicationFactoryImpl(const ApplicationFactoryImpl&) = delete;
  ApplicationFactoryImpl& operator=(const ApplicationFactoryImpl&) = delete;

  friend class ApplicationFactory;

  std::unique_ptr<fto::KbNodeManager> kbnode_manager_;
  std::unique_ptr<ISimpleKbNodeAdderModelFactory>
      simple_kbnode_adder_model_factory_;
  std::unique_ptr<IAttributeModelFactory> attr_model_factory_;
  std::unique_ptr<IAttributeSetModelFactory> attr_set_model_factory_;
  std::unique_ptr<IWorkModelFactory> work_model_factory_;
  std::unique_ptr<IWorkFactory> work_factory_;
};

ApplicationFactory::ApplicationFactory()
    : impl_(utils::make_unique<ApplicationFactoryImpl>()) {}

ApplicationFactory::~ApplicationFactory() = default;

std::shared_ptr<ApplicationFactory> ApplicationFactory::create() {
  return std::make_shared<ApplicationFactory>();
}

std::shared_ptr<IMainWindowModel> ApplicationFactory::makeMainWindowModel() {
  static std::weak_ptr<IMainWindowModel> cache;

  auto model = cache.lock();
  if (!model) {
    auto workspace_model = std::make_shared<WorkSpaceModel>(
        impl_->getWorkModelFactory(), *impl_->getWorkFactory());
    model = std::make_shared<MainWindowModel>(workspace_model);
    cache = model;
  }

  return model;
}

}  // namespace snailcore
