// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "pfmvp/i_pf_view_factory.h"

namespace pfmvp {

SINGLETON_STATIC_INSTANCE(PfViewFactoryManager)

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

void PfViewFactoryManager::addViewFactory(
    const IPfModel::ModelIdType& model_id,
    IPfViewFactory* view_factory) {
  auto& self = PfViewFactoryManager::getInstance();
  self.model_viewfactory_map_[model_id] = view_factory;
}

IPfViewFactory* PfViewFactoryManager::getViewFactory(
    const IPfModel::ModelIdType& model_id) const {
  try {
    return model_viewfactory_map_.at(model_id);
  } catch (...) {
    return &g_null_view_factory;
  }
}

}  // namespace pfmvp
