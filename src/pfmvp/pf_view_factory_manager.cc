// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "pfmvp/pf_view_factory_manager.h"

namespace pfmvp {

SINGLETON_STATIC_INSTANCE(PfViewFactoryManager)

void PfViewFactoryManager::addViewFactory(
    const IPfModel::ModelIdType& model_id,
    IPfViewFactory* view_factory) {
  auto& factories = model_viewfactory_map_[model_id];
  auto vf_id = view_factory->getViewFactoryId();
  factories[vf_id] = view_factory;
}

IPfViewFactory* PfViewFactoryManager::getViewFactory(
    const IPfModel::ModelIdType& model_id) const {
  try {
    auto& factories = model_viewfactory_map_.at(model_id);
    if (factories.size() > 0) {
      return factories.begin()->second;
    }
  } catch (...) {
  }

  return nullptr;
}

IPfViewFactory* PfViewFactoryManager::getViewFactory(
    const IPfModel::ModelIdType& model_id,
    const IPfViewFactory::ViewFactoryIdType& view_factory_id) const {
  try {
    auto& factories = model_viewfactory_map_.at(model_id);
    return factories.at(view_factory_id);
  } catch (...) {
  }

  return nullptr;
}

std::vector<IPfViewFactory*> PfViewFactoryManager::getAllViewFactory(
    const IPfModel::ModelIdType& model_id) const {
  std::vector<IPfViewFactory*> vec;
  try {
    auto factories = model_viewfactory_map_.at(model_id);
    for (auto f : factories) {
      vec.push_back(f.second);
    }
  } catch (...) {
  }

  return vec;
}

}  // namespace pfmvp
