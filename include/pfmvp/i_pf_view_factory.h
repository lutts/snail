// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_PF_VIEW_FACTORY_H_
#define INCLUDE_SNAIL_I_PF_VIEW_FACTORY_H_

#include <unordered_map>

#include "pfmvp/i_pf_model.h"
#include "pfmvp/pf_presenter.h"
#include "pfmvp/singleton.h"

namespace pfmvp {

class IPfViewFactory {
 public:
  virtual ~IPfViewFactory() = default;

  virtual std::shared_ptr<PfPresenter>
  createView(std::shared_ptr<IPfModel> model) = 0;
};

class IPfViewFactoryManager {
 public:
  virtual ~IPfViewFactoryManager() = default;

  virtual IPfViewFactory*
  getViewFactory(const IPfModel::ModelIdType& model_id) const = 0;
};

class PfViewFactoryManager :
      public IPfViewFactoryManager,
      public Singleton<PfViewFactoryManager> {
 public:
  static void addViewFactory(const IPfModel::ModelIdType& model_id,
                             IPfViewFactory* view_factory);
  IPfViewFactory* getViewFactory(const IPfModel::ModelIdType& model_id) const;

 private:
  PfViewFactoryManager() = default;
  ~PfViewFactoryManager() = default;
  PfViewFactoryManager(const PfViewFactoryManager& other) = delete;
  PfViewFactoryManager& operator=(const PfViewFactoryManager& other) = delete;

  friend class Singleton<PfViewFactoryManager>;

  std::unordered_map<IPfModel::ModelIdType, IPfViewFactory*>
  model_viewfactory_map_;
};

template <typename M, typename F>
class view_factory_t {
  F factory;
 public:
  view_factory_t() {
    addToViewFactoryManager();
  }

  F& FTO_getFactory() {
    return factory;
  }

 private:
  void addToViewFactoryManager() {
    PfViewFactoryManager::addViewFactory(M::modelId(), &factory);
  }
};

}  // namespace pfmvp

#endif  // INCLUDE_SNAIL_I_PF_VIEW_FACTORY_H_
