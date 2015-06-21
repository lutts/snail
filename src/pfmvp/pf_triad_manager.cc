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

class TriadInfo {
 public:
  TriadInfo() = default;
  TriadInfo(std::shared_ptr<PfPresenter> presenter,
            TriadInfo* parent = nullptr,
            bool auto_remove_child = true)
      : presenter_(presenter)
      , parent_(parent)
      , auto_remove_child_(auto_remove_child) {
    if (parent_) {
      parent->add_child_triad(this);
    }
  }

  PfPresenter* presenter() const { return presenter_.get(); }
  IPfView* view() const { return presenter_->getView().get(); }
  IPfModel* model() const { return presenter_->getModel().get(); }

  bool auto_remove_child() const { return auto_remove_child_; }
  TriadInfo* parent() const { return parent_; }
  void add_child_triad(TriadInfo* triad) {
    children_.push_front(triad);
  }
  bool hasChildren() const {
    return !children_.empty();
  }

  void markForDelete() {
    waiting_delete_ = true;

    if (!auto_remove_child_)
      return;

    for (auto child : children_) {
      child->markForDelete();
    }
  }

  bool is_waiting_delete() const { return waiting_delete_; }

  bool isRoot() const {
    return (parent_ == nullptr);
  }

  operator bool() const {
    return presenter_.operator bool();
  }

 private:
  std::shared_ptr<PfPresenter> presenter_;
  TriadInfo* parent_;
  bool auto_remove_child_;
  std::forward_list<TriadInfo*> children_;
  bool waiting_delete_ { false };
};

class PfTriadManagerImpl {
 public:
  explicit PfTriadManagerImpl(const IPfViewFactoryManager& view_factory_mgr)
      : view_factory_mgr_(view_factory_mgr) { }
  virtual ~PfTriadManagerImpl() {
    if (!triad_list_.empty()) {
      ALOGW << "****** triad list not empty when destroy triad manager ******";
      for (auto& triad : triad_list_) {
        ALOGW << "\t" << triad.model()->getModelId();
        if (!triad.isRoot()) {
          auto parent = triad.parent();
          ALOGW << "\t\t parent: " << parent->model()->getModelId();
        } else {
          ALOGW << "\t\t parent: (none)";
        }
        ALOGW << "\t\t auto remove child: " << triad.auto_remove_child();
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

  std::shared_ptr<IPfView>
  createViewWithFactory(
      std::shared_ptr<IPfModel> model,
      IPfViewFactory* view_factory,
      IPfTriadManager* triad_manager,
      PfPresenter* parent_presenter,
      bool auto_remove_child);

  void removeTriads(const std::forward_list<TriadInfo*>& triads);

  void removeTriadBy(IPfModel* model);
  void removeTriadBy(IPfView* view);
  bool requestRemoveTriadByView(IPfView* view);
  std::vector<IPfView*> findViewByModel(IPfModel* model) const;
  std::vector<IPfView*>
  findViewsByModelId(const IPfModel::ModelIdType& model_id) const;
  IPfModel* findModelByView(IPfView* view) const;

  bool isModelExist(IPfModel* model) const;
  bool isViewExist(IPfView* view) const;

  void doAboutToDestroyTriad(PfPresenter* presenter);

 private:
  PfTriadManagerImpl(const PfTriadManagerImpl& other) = delete;
  PfTriadManagerImpl& operator=(const PfTriadManagerImpl& other) = delete;

  const IPfViewFactoryManager& view_factory_mgr_;

  std::forward_list<TriadInfo> triad_list_;

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
PfTriadManagerImpl::createViewWithFactory(
    std::shared_ptr<IPfModel> model,
    IPfViewFactory* view_factory,
    IPfTriadManager* triad_manager,
    PfPresenter* parent_presenter,
    bool auto_remove_child) {
  TriadInfo* parent_triad = nullptr;
  if (parent_presenter) {
    for (auto& triad : triad_list_) {
      if (triad.presenter() == parent_presenter) {
        parent_triad = &triad;
        break;
      }
    }

    if (!parent_triad) {
      ALOGE << "create view with not managed parent presenter is not allowed";
      return nullptr;
    }
  }

  if (model && view_factory) {
    ALOGI << "create view for model " << model->getModelId();

    auto presenter = view_factory->createView(model);

    if (presenter) {
      presenter->set_triad_manager(triad_manager);

      // TODO(lutts): LOCK
      // we use FILO policy, because pop-up windows are close
      // before their parents, and MainWindow is the first created
      // and last destroyed
      triad_list_.emplace_front(presenter,
                                      parent_triad,
                                      auto_remove_child);
      ++model_view_count[presenter->getModel().get()];

      // initialize may create sub-triads, so we need to
      // call initialize() after push parent-triad (this-triad) first
      presenter->initialize();

      return presenter->getView();
    }
  }

  return nullptr;
}

bool PfTriadManagerImpl::isModelExist(IPfModel* model) const {
  auto iter = std::find_if(
      triad_list_.begin(),
      triad_list_.end(),
      [model](const TriadInfo& triad) {
        return model == triad.model();
      });

  return (iter != triad_list_.end());
}

bool PfTriadManagerImpl::isViewExist(IPfView* view) const {
  auto iter = std::find_if(
      triad_list_.begin(),
      triad_list_.end(),
      [view](const TriadInfo& triad) {
        return view == triad.view();
      });

  return (iter != triad_list_.end());
}

void PfTriadManagerImpl::doAboutToDestroyTriad(PfPresenter* presenter) {
  auto model = presenter->getModel().get();
  auto view = presenter->getView().get();

  // first notify the listeners on this triad
  // 1. view
  auto& view_destroy_sig = AboutToDestroyViewSignalOf(view);
  view_destroy_sig(view);
  // 1.1 remove signal slots
  view_destroy_sig_map_.erase(view);

  // 2. model
  // 2.1. decrease view count of the model
  --model_view_count[model];
  assert(model_view_count[model] >= 0);
  // 2.2 if there's no views left, destroy the model
  if (model_view_count[model] == 0) {
    // 2.2.1. send AboutToDestroy to listeners
    auto& model_destroy_sig = AboutToDestroyModelSignalOf(model);
    model_destroy_sig(model);

    // 2.2.2. remove signal slots
    model_remove_sig_map_.erase(model);
    model_destroy_sig_map_.erase(model);
    model_view_count.erase(model);
  }

  // then notify the triad itself
  // 1. notify the presenter
  presenter->onDestroy();

  // 2. view,
  view->onDestroy();

  // 3. model
  if (model_view_count[model] == 0) {
    model->onDestroy();
  }
}

void PfTriadManagerImpl::removeTriads(
    const std::forward_list<TriadInfo*>& triads) {
  // 1. mark the triads to be removed
  for (auto triad_ptr : triads) {
    triad_ptr->markForDelete();
  }

  // 2. move the marked triads to a temorary list
  std::forward_list<TriadInfo> triads_to_delete;
  triad_list_.remove_if(
      [&triads_to_delete](const TriadInfo& triad){
        if (triad.is_waiting_delete()) {
          triads_to_delete.push_front(triad);
          return true;
        }

        return false;
      });

  // 3. delete them
  triads_to_delete.reverse();
  triads_to_delete.remove_if(
      [this](const TriadInfo& triad) {
        doAboutToDestroyTriad(triad.presenter());
        return true;
      });
}

void PfTriadManagerImpl::removeTriadBy(IPfModel* model) {
  // TODO(lutts): LOCK

  std::forward_list<TriadInfo*> matched_triads;
  for (auto& triad : triad_list_) {
    if (triad.model() == model) {
      matched_triads.push_front(&triad);
    }
  }

  removeTriads(matched_triads);
}

void PfTriadManagerImpl::removeTriadBy(IPfView* view) {
  // TODO(lutts): LOCK

  std::forward_list<TriadInfo*> matched_triads;
  for (auto& triad : triad_list_) {
    if (triad.view() == view) {
      matched_triads.push_front(&triad);
    }
  }

  removeTriads(matched_triads);
}

// TODO(lutts): what if the view's model has multiple views?
bool PfTriadManagerImpl::requestRemoveTriadByView(IPfView* view) {
  auto iter = std::find_if(
      triad_list_.begin(),
      triad_list_.end(),
      [view](const TriadInfo& triad) -> bool {
        return triad.view() == view;
      });

  if (iter != triad_list_.end()) {
    auto& triad = *iter;
    IPfModel* model = triad.model();
    auto& sig = RequestRemoveModelSignalOf(model);

    bool allow_remove = sig(model);

    if (allow_remove) {
      removeTriadBy(view);
    }

    return allow_remove;
  } else {
    // TODO(lutts): throw exception?????
    return false;
  }
}

std::vector<IPfView*>
PfTriadManagerImpl::findViewByModel(IPfModel* model) const {
  std::vector<IPfView*> matched_views;

  for (auto& triad : triad_list_) {
    if (model == triad.model()) {
      matched_views.push_back(triad.view());
    }
  }

  return matched_views;
}

std::vector<IPfView*> PfTriadManagerImpl:: findViewsByModelId(
    const IPfModel::ModelIdType& model_id) const {
  std::vector<IPfView*> matched_views;

  for (auto& triad : triad_list_) {
    auto id = triad.model()->getModelId();
    if (model_id == id) {
      matched_views.push_back(triad.view());
    }
  }

  return matched_views;
}

IPfModel* PfTriadManagerImpl::findModelByView(IPfView* view) const {
  auto iter = std::find_if(
      triad_list_.begin(),
      triad_list_.end(),
      [view](const TriadInfo& triad) -> bool {
        return triad.view() == view;
      });

  if (iter != triad_list_.end()) {
    auto& triad = *iter;
    return triad.model();
  }

  return nullptr;
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
    if (!impl->ExistChecker(obj)) {                                     \
      return false;                                                     \
    }                                                                   \
                                                                        \
    auto& sig = impl->sigName##SignalOf(obj);                           \
                sigName##SignalType::slot_type subscriber(handler);     \
                    sig.connect(subscriber.track_foreign(trackObject)); \
                    return true;                                        \
  }

SNAIL_PFTRIAD_SIGSLOT_IMPL(PfTriadManager,
                           RequestRemoveModel, IPfModel, isModelExist);
SNAIL_PFTRIAD_SIGSLOT_IMPL(PfTriadManager,
                           AboutToDestroyModel, IPfModel, isModelExist);
SNAIL_PFTRIAD_SIGSLOT_IMPL(PfTriadManager,
                           AboutToDestroyView, IPfView, isViewExist);

std::shared_ptr<IPfView>
PfTriadManager::createViewFor(std::shared_ptr<IPfModel> model,
                              PfPresenter* parent_presenter,
                              bool auto_remove_child) {
  IPfViewFactory* view_factory =
      impl->view_factory_mgr_.getViewFactory(model->getModelId());
  return impl->createViewWithFactory(model, view_factory, this,
                               parent_presenter, auto_remove_child);
}

std::shared_ptr<IPfView>
PfTriadManager::createViewFor(
    std::shared_ptr<IPfModel> model,
    const IPfViewFactory::ViewFactoryIdType& view_factory_id,
    PfPresenter* parent_presenter,
    bool auto_remove_child) {
  IPfViewFactory* view_factory =
      impl->view_factory_mgr_.getViewFactory(model->getModelId(),
                                       view_factory_id);
  return impl->createViewWithFactory(model, view_factory, this,
                               parent_presenter, auto_remove_child);
}

void PfTriadManager::removeTriadBy(IPfModel* model) {
  impl->removeTriadBy(model);
}

void PfTriadManager::removeTriadBy(IPfView* view) {
  impl->removeTriadBy(view);
}

bool PfTriadManager::requestRemoveTriadByView(IPfView* view) {
  return impl->requestRemoveTriadByView(view);
}

std::vector<IPfView*> PfTriadManager::findViewByModel(IPfModel* model) const {
  return impl->findViewByModel(model);
}

std::vector<IPfView*>
PfTriadManager::findViewsByModelId(
    const IPfModel::ModelIdType& model_id) const {
  return impl->findViewsByModelId(model_id);
}

IPfModel* PfTriadManager::findModelByView(IPfView* view) const {
  return impl->findModelByView(view);
}

}  // namespace pfmvp
