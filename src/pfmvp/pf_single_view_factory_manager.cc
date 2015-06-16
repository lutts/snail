// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "pfmvp/pf_single_view_factory_manager.h"

namespace pfmvp {

SINGLETON_STATIC_INSTANCE(PfSingleViewFactoryManager)

class NullViewFactory : public IPfViewFactory {
 public:
  NullViewFactory() = default;
  ~NullViewFactory() = default;

  std::shared_ptr<PfPresenter>
  createView(std::shared_ptr<IPfModel>) final {
    return nullptr;
  }
};

static NullViewFactory g_null_view_factory;

void PfSingleViewFactoryManager::addViewFactory(
    const IPfModel::ModelIdType& model_id,
    IPfViewFactory* view_factory) {
  auto& self = PfSingleViewFactoryManager::getInstance();
  self.model_viewfactory_map_[model_id] = view_factory;
}

IPfViewFactory* PfSingleViewFactoryManager::getViewFactory(
    const IPfModel::ModelIdType& model_id) const {
  try {
    return model_viewfactory_map_.at(model_id);
  } catch (...) {
    return &g_null_view_factory;
  }
}

}  // namespace pfmvp
