// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_PF_VIEW_FACTORY_H_
#define INCLUDE_SNAIL_I_PF_VIEW_FACTORY_H_

#include <unordered_map>

#include "snail/i_pf_model.h"
#include "snail/pf_presenter.h"
#include "snail/singleton.h"

namespace snailcore {

class IPfViewFactory {
 public:
  virtual ~IPfViewFactory() = default;

  virtual std::shared_ptr<PfPresenter>
  createView(std::shared_ptr<IPfModel> model) = 0;
};

namespace impl {

class PfViewFactoryManager : public Singleton<PfViewFactoryManager> {
 public:
  static void addViewFactory(const IPfModel::ModelIdType& model_id,
                             IPfViewFactory* view_factory);
  static IPfViewFactory* getViewFactory(const IPfModel::ModelIdType& model_id);

 private:
  PfViewFactoryManager() = default;
  ~PfViewFactoryManager() = default;
  PfViewFactoryManager(const PfViewFactoryManager& other) = delete;
  PfViewFactoryManager& operator=(const PfViewFactoryManager& other) = delete;

  friend class Singleton<PfViewFactoryManager>;

  std::unordered_map<IPfModel::ModelIdType, IPfViewFactory*>
  model_viewfactory_map_;
};

}  // namespace impl

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
    impl::PfViewFactoryManager::addViewFactory(M::modelId(), &factory);
  }
};

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_I_PF_VIEW_FACTORY_H_
