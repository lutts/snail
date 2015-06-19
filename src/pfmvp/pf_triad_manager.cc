// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/pfmvp/pf_triad_manager.h"

#include <vector>
#include <unordered_map>
#include <forward_list>

#include "utils/basic_utils.h"  // utils::make_unique
#include "pfmvp/i_pf_view_factory.h"
#include "pfmvp/pf_presenter.h"

#include "src/utils/log/log.h"

namespace pfmvp {

class PfTriadManager::PfTriadManagerImpl {
 public:
  explicit PfTriadManagerImpl(const IPfViewFactoryManager& view_factory_mgr)
      : view_factory_mgr_(view_factory_mgr) { }
  virtual ~PfTriadManagerImpl() {
    if (!presenterStore.empty()) {
      ALOGW << "****** presenter not empty when destroy triad manager ******";
      for (auto p : presenterStore) {
        ALOGW << "\t" << p->getModel()->getModelId();
      }
      ALOGW << "************************************************************";
    }
  }

  PfTriadManager::RequestRemoveModelSignalType&
  RequestRemoveModelSignalOf(IPfModel* model) {
    return model_remove_sig_map_[model];
  }

  PfTriadManager::AboutToDestroyModelSignalType&
  AboutToDestroyModelSignalOf(IPfModel* model) {
    return model_destroy_sig_map_[model];
  }

  PfTriadManager::AboutToDestroyViewSignalType&
  AboutToDestroyViewSignalOf(IPfView* view) {
    return view_destroy_sig_map_[view];
  }

 private:
  PfTriadManagerImpl(const PfTriadManagerImpl& other) = delete;
  PfTriadManagerImpl& operator=(const PfTriadManagerImpl& other) = delete;

  const IPfViewFactoryManager& view_factory_mgr_;

  using PresenterStoreType = std::forward_list<std::shared_ptr<PfPresenter> >;
  PresenterStoreType presenterStore;

  std::unordered_map<IPfModel*, int> model_view_count;

  std::unordered_map<IPfModel*, PfTriadManager::RequestRemoveModelSignalType>
  model_remove_sig_map_;

  std::unordered_map<IPfModel*, PfTriadManager::AboutToDestroyModelSignalType>
  model_destroy_sig_map_;

  std::unordered_map<IPfView*, PfTriadManager::AboutToDestroyViewSignalType>
  view_destroy_sig_map_;

  friend class PfTriadManager;
};

PfTriadManager::PfTriadManager(const IPfViewFactoryManager& view_factory_mgr)
    : impl(utils::make_unique<PfTriadManagerImpl>(view_factory_mgr)) {
}

PfTriadManager::~PfTriadManager() = default;

// NOTE: createViewXXX maybe recursively called
std::shared_ptr<IPfView>
PfTriadManager::createViewWithFactory(
    std::shared_ptr<IPfModel> model,
    IPfViewFactory* view_factory) {
  if (model && view_factory) {
    ALOGI << "create view for model " << model->getModelId();
    auto presenter = view_factory->createView(model);

    if (presenter) {
      presenter->set_triad_manager(this);

      // TODO(lutts): LOCK
      // we use FILO policy, because pop-up windows are close
      // before their parents, and MainWindow is the first created
      // and last destroyed
      impl->presenterStore.push_front(presenter);
      ++impl->model_view_count[presenter->getModel().get()];

      // initialize may create sub-triads, so we need to
      // call initialize() after push parent-triad (this-triad) first
      presenter->initialize();

      return presenter->getView();
    }
  }

  return nullptr;
}

std::shared_ptr<IPfView>
PfTriadManager::createViewFor(std::shared_ptr<IPfModel> model) {
  IPfViewFactory* view_factory =
      impl->view_factory_mgr_.getViewFactory(model->getModelId());
  return createViewWithFactory(model, view_factory);
}

std::shared_ptr<IPfView> PfTriadManager::createViewFor(
    std::shared_ptr<IPfModel> model,
    const IPfViewFactory::ViewFactoryIdType& view_factory_id) {
  IPfViewFactory* view_factory =
      impl->view_factory_mgr_.getViewFactory(model->getModelId(),
                                             view_factory_id);
  return createViewWithFactory(model, view_factory);
}

bool PfTriadManager::isModelExist(IPfModel* model) const {
  auto iter = std::find_if(impl->presenterStore.begin(),
                           impl->presenterStore.end(),
                           [model](const std::shared_ptr<PfPresenter>& item) {
                             return model == item->getModel().get();
                           });

  return (iter != impl->presenterStore.end());
}

bool PfTriadManager::isViewExist(IPfView* view) const {
  auto iter = std::find_if(impl->presenterStore.begin(),
                           impl->presenterStore.end(),
                           [view](const std::shared_ptr<PfPresenter>& item) {
                             return view == item->getView().get();
                           });

  return (iter != impl->presenterStore.end());
}

#define SNAIL_PFTRIAD_SIGSLOT_IMPL(THISCLASS, sigName, ObjType, ExistChecker) \
  bool THISCLASS::when##sigName(                                        \
      ObjType* obj,                                                     \
      sigName##SlotType handler,                                        \
      std::shared_ptr<utils::ITrackable> trackObject) {                 \
    if (obj == nullptr) {                                               \
      return false;                                                     \
    }                                                                   \
                                                                        \
    if (!ExistChecker(obj)) {                                           \
      return false;                                                     \
    }                                                                   \
                                                                        \
    auto& sig = impl->sigName##SignalOf(obj);                           \
    sigName##SignalType::slot_type subscriber(handler);                 \
    sig.connect(subscriber.track_foreign(trackObject));                 \
    return true;                                                        \
  }

SNAIL_PFTRIAD_SIGSLOT_IMPL(PfTriadManager,
                           RequestRemoveModel, IPfModel, isModelExist);
SNAIL_PFTRIAD_SIGSLOT_IMPL(PfTriadManager,
                           AboutToDestroyModel, IPfModel, isModelExist);
SNAIL_PFTRIAD_SIGSLOT_IMPL(PfTriadManager,
                           AboutToDestroyView, IPfView, isViewExist);

void PfTriadManager::doAboutToDestroyTriad(PfPresenter* presenter) {
  auto model = presenter->getModel().get();
  auto view = presenter->getView().get();

  // first notify the listeners on this triad
  // 1. view
  auto& view_destroy_sig = impl->AboutToDestroyViewSignalOf(view);
  view_destroy_sig(view);
  // 1.1 remove signal slots
  impl->view_destroy_sig_map_.erase(view);

  // 2. model
  // 2.1. decrease view count of the model
  --impl->model_view_count[model];
  assert(impl->model_view_count[model] >= 0);
  // 2.2 if there's no views left, destroy the model
  if (impl->model_view_count[model] == 0) {
    // 2.2.1. send AboutToDestroy to listeners
    auto& model_destroy_sig = impl->AboutToDestroyModelSignalOf(model);
    model_destroy_sig(model);

    // 2.2.2. remove signal slots
    impl->model_remove_sig_map_.erase(model);
    impl->model_destroy_sig_map_.erase(model);
    impl->model_view_count.erase(model);
  }

  // then notify the triad itself
  // 1. notify the presenter
  presenter->onDestroy();

  // 2. view,
  view->onDestroy();

  // 3. model
  if (impl->model_view_count[model] == 0) {
    model->onDestroy();
  }
}

void PfTriadManager::removeTriadBy(IPfModel* model) {
  // TODO(lutts): LOCK
  impl->presenterStore.remove_if(
      [this, model](const std::shared_ptr<PfPresenter> item) -> bool {
        bool matched = item->getModel().get() == model;
        if (matched) {
          doAboutToDestroyTriad(item.get());
        }

        return matched;
      });
}

void PfTriadManager::removeTriadBy(IPfView* view) {
  // TODO(lutts): LOCK
  impl->presenterStore.remove_if(
      [this, view](const std::shared_ptr<PfPresenter> item) -> bool {
        bool matched = item->getView().get() == view;
        if (matched) {
          doAboutToDestroyTriad(item.get());
        }

        return matched;
      });
}

bool PfTriadManager::requestRemoveTriadByView(IPfView* view) {
  auto iter = std::find_if(
      impl->presenterStore.begin(),
      impl->presenterStore.end(),
      [view](const std::shared_ptr<PfPresenter> item) -> bool {
        return item->getView().get() == view;
      });

  if (iter != impl->presenterStore.end()) {
    auto& presenter = *iter;
    IPfModel* model = presenter->getModel().get();
    auto& sig = impl->RequestRemoveModelSignalOf(model);

    bool allow_remove = sig(presenter->getModel().get());

    if (allow_remove) {
      removeTriadBy(view);
    }

    return allow_remove;
  } else {
    // TODO(lutts): throw exception?????
    return false;
  }
}

std::vector<IPfView*> PfTriadManager::findViewByModel(IPfModel* model) const {
  std::vector<IPfView*> matched_views;

  for (auto& presenter : impl->presenterStore) {
    if (model == presenter->getModel().get()) {
      matched_views.push_back(presenter->getView().get());
    }
  }

  return matched_views;
}

std::vector<IPfView*> PfTriadManager:: findViewsByModelId(
    const IPfModel::ModelIdType& model_id) const {
  std::vector<IPfView*> matched_views;

  for (auto& presenter : impl->presenterStore) {
    auto id = presenter->getModel()->getModelId();
    if (model_id == id) {
      matched_views.push_back(presenter->getView().get());
    }
  }

  return matched_views;
}

IPfModel* PfTriadManager::findModelByView(IPfView* view) const {
  auto iter = std::find_if(
      impl->presenterStore.begin(),
      impl->presenterStore.end(),
      [view](const std::shared_ptr<PfPresenter>& item) -> bool {
        return item->getView().get() == view;
      });

  if (iter != impl->presenterStore.end()) {
    auto& p = *iter;
    return p->getModel().get();
  }

  return nullptr;
}

}  // namespace pfmvp
