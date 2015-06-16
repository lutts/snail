// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef PF_VIEW_FACTORY_MANAGER_H_
#define PF_VIEW_FACTORY_MANAGER_H_

#include <unordered_map>

#include "pfmvp/i_pf_view_factory.h"

#include "pfmvp/singleton.h"

namespace pfmvp {

class PfSingleViewFactoryManager :
      public IPfViewFactoryManager,
      public Singleton<PfSingleViewFactoryManager> {
 public:
  static void addViewFactory(const IPfModel::ModelIdType& model_id,
                             IPfViewFactory* view_factory);
  IPfViewFactory* getViewFactory(const IPfModel::ModelIdType& model_id) const;

 private:
  PfSingleViewFactoryManager() = default;
  ~PfSingleViewFactoryManager() = default;
  PfSingleViewFactoryManager(const PfSingleViewFactoryManager& other) = delete;
  PfSingleViewFactoryManager& operator=(const PfSingleViewFactoryManager& other) = delete;

  friend class Singleton<PfSingleViewFactoryManager>;

  std::unordered_map<IPfModel::ModelIdType, IPfViewFactory*>
  model_viewfactory_map_;
};

template <typename M, typename F>
class view_factory_single_t {
  F factory;
 public:
  view_factory_single_t() {
    addToViewFactoryManager();
  }

 private:
  void addToViewFactoryManager() {
    PfSingleViewFactoryManager::addViewFactory(M::modelId(), &factory);
  }
};

}  // namespace pfmvp

#endif  // PF_VIEW_FACTORY_MANAGER_H_
