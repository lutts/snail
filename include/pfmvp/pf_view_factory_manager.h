// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_PFMVP_PF_SINGLE_VIEW_FACTORY_MANAGER_H_
#define INCLUDE_PFMVP_PF_SINGLE_VIEW_FACTORY_MANAGER_H_

#include <vector>
#include <unordered_map>

#include "pfmvp/i_pf_view_factory.h"
#include "pfmvp/singleton.h"

namespace pfmvp {

namespace tests {
class PfViewFactoryManagerTest;
}  // namespace tests

class PfViewFactoryManager :
      public IPfViewFactoryManager,
      public Singleton<PfViewFactoryManager> {
 public:
  void addViewFactory(const IPfModel::ModelIdType& model_id,
                      IPfViewFactory* view_factory) override;

  IPfViewFactory* getViewFactory(
      const IPfModel::ModelIdType& model_id) const override;
  IPfViewFactory* getViewFactory(
      const IPfModel::ModelIdType& model_id,
      const IPfViewFactory::ViewFactoryIdType& view_factory_id) const override;
  std::vector<IPfViewFactory*>
  getAllViewFactory(const IPfModel::ModelIdType& model_id) const override;

 private:
  PfViewFactoryManager() = default;
  ~PfViewFactoryManager() = default;
  PfViewFactoryManager(const PfViewFactoryManager& other) = delete;
  PfViewFactoryManager& operator=(const PfViewFactoryManager&) = delete;

  friend class Singleton<PfViewFactoryManager>;
  friend class tests::PfViewFactoryManagerTest;

  using ViewFactoryStoreType =
      std::unordered_map<IPfViewFactory::ViewFactoryIdType, IPfViewFactory*>;
  std::unordered_map<IPfModel::ModelIdType, ViewFactoryStoreType>
  model_viewfactory_map_;
};

template <typename M, typename F>
class view_factory_single_t {
  F factory;
 public:
  view_factory_single_t() {
    addToViewFactoryManager();
  }

  F& FTO_getFactory() {
    return factory;
  }

 private:
  void addToViewFactoryManager() {
    PfViewFactoryManager::getInstance().addViewFactory(M::modelId(), &factory);
  }
};

}  // namespace pfmvp

#endif  // INCLUDE_PFMVP_PF_SINGLE_VIEW_FACTORY_MANAGER_H_
