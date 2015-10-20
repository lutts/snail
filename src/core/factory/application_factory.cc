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
#include "src/core/tree_item_provider.h"
#include "src/core/simple_kbnode_adder_model.h"
#include "src/core/attribute_set_model.h"
#include "src/core/work_model.h"

// factories
#include "core/i_kbnode_provider_factory.h"
#include "core/i_simple_kbnode_adder_model_factory.h"
#include "src/core/factory/attribute_model_factory.h"
#include "core/i_attribute_set_model_factory.h"
#include "core/i_work_model_factory.h"
#include "src/core/factory/work_factory.h"

namespace snailcore {

class ApplicationFactoryImpl : public ITreeItemProviderFactory
                             , public ISimpleKbNodeAdderModelFactory
                             , public IAttributeSetModelFactory
                             , public IWorkModelFactory {
 public:
  ApplicationFactoryImpl()
      : kbnode_manager_(utils::make_unique<KbNodeManager>(this))
      , attr_model_factory_(
          utils::make_unique<AttributeModelFactory>(
              kbnode_manager_.get(), this))
      , work_factory_(utils::make_unique<WorkFactory>(kbnode_manager_.get())) {
  }
  ~ApplicationFactoryImpl() = default;

  // ITreeItemProviderFactory
  std::shared_ptr<ITreeItemProvider> createTreeItemProvider(
      IKbNode* root_kbnode,
      IKbNodeManager* kbnode_manager) const override {
    return std::make_shared<TreeItemProvider>(root_kbnode, kbnode_manager);
  }

  // ISimpleKbNodeAdderModelFactory
  std::shared_ptr<ISimpleKbNodeAdderModel>
  createSimpleKbNodeAdderModel(ITreeItemProvider* provider) const override {
    return std::make_shared<SimpleKbNodeAdderModel>(provider,
                                                 kbnode_manager_.get());
  }

  // IAttributeSetModelFactory
  std::shared_ptr<IAttributeSetModel>
  createAttributeSetModel(
      const std::vector<IAttributeSupplier*>& attr_suppliers) override {
    return std::make_shared<AttributeSetModel>(
        attr_suppliers, attr_model_factory_.get());
  }

  // IWorkModelFactory
  std::shared_ptr<IWorkModel> createWorkModel() override {
    return std::make_shared<WorkModel>(this);
  }

 private:
  ApplicationFactoryImpl(const ApplicationFactoryImpl&) = delete;
  ApplicationFactoryImpl& operator=(const ApplicationFactoryImpl&) = delete;

  friend class ApplicationFactory;

  std::unique_ptr<KbNodeManager> kbnode_manager_;
  std::unique_ptr<AttributeModelFactory> attr_model_factory_;
  std::unique_ptr<WorkFactory> work_factory_;
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
        std::make_shared<WorkSpaceModel>(impl_.get(),
                                         impl_->work_factory_.get());
    model = std::make_shared<MainWindowModel>(workspace_model);
    cache = model;
  }

  return model;
}

}  // namespace snailcore
