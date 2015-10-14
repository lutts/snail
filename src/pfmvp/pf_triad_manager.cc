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
#include <memory>

#include "utils/basic_utils.h"  // utils::make_unique
#include "pfmvp/i_pf_view_factory.h"
#include "pfmvp/pf_presenter.h"

#include "src/utils/log/log.h"

namespace pfmvp {

class TriadInfo {
 public:
  TriadInfo() = default;
  TriadInfo(std::shared_ptr<PfPresenter> presenter,
            TriadInfo* parent,
            bool auto_remove_child,
            std::shared_ptr<PfCreateViewArgsMemento> creation_args_memento)
      : presenter_{presenter}
      , parent_{parent}
      , auto_remove_child_{auto_remove_child}
      , creation_args_memento_{creation_args_memento} {
    if (parent_) {
      parent->add_child_triad(this);
    }
      }

  ~TriadInfo() {
    ALOGI << "delete triad " << model()->getModelId();
    if (parent_)
      parent_->remove_child_triad(this);

    for (auto child : children_) {
      child->parent_ = nullptr;
    }
  }

  PfPresenter* presenter() const { return presenter_.get(); }
  IPfView* view() const { return presenter_->getView().get(); }
  IPfModel* model() const { return presenter_->getModel().get(); }
  IPfViewFactory::ViewFactoryIdType view_factory_id() const {
    return creation_args_memento_->view_factory_id();
  }

  const PfCreateViewArgsMemento* memento() const {
    return creation_args_memento_.get();
  }

  bool auto_remove_child() const { return auto_remove_child_; }
  TriadInfo* parent() const { return parent_; }

  void add_child_triad(TriadInfo* triad) {
    children_.push_front(triad);
  }

  void remove_child_triad(TriadInfo* triad) {
    children_.remove(triad);
  }

  bool hasChildren() const {
    return !children_.empty();
  }

  void markForDelete() {
    waiting_delete_ = true;
    ALOGI << "mark triad " << model()->getModelId() << " for delete"
          << ", has child: " << !children_.empty()
          << ", auto_remove_child: " << auto_remove_child_;

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
  SNAIL_DISABLE_COPY(TriadInfo);

  std::shared_ptr<PfPresenter> presenter_;
  TriadInfo* parent_ { nullptr };
  bool auto_remove_child_ { true };
  std::shared_ptr<PfCreateViewArgsMemento> creation_args_memento_;

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
        ALOGW << "\t" << triad->model()->getModelId();
        if (!triad->isRoot()) {
          auto parent = triad->parent();
          ALOGW << "\t\t parent: " << parent->model()->getModelId();
        } else {
          ALOGW << "\t\t parent: (none)";
        }
        ALOGW << "\t\t auto remove child: " << triad->auto_remove_child();
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
  createViewFor(
      std::shared_ptr<IPfModel> model,
      PfPresenter* parent,
      bool auto_remove_child,
      PfCreateViewArgs* args,
      IPfTriadManager* triad_manager);

  void removeTriads(const std::forward_list<TriadInfo*>& triads);

  void removeTriadBy(IPfModel* model);
  void removeTriadBy(IPfView* view);
  bool requestRemoveTriadByView(IPfView* view);
  std::vector<IPfView*> findViewByModel(IPfModel* model) const;
  IPfModel* findModelByView(IPfView* view) const;

  std::vector<IPfView*> findViewByModelAndViewFactory(
      IPfModel* model,
      const IPfViewFactory::ViewFactoryIdType& view_factory_id) const;

  std::vector<IPfView*> findViewByModel_if(
      IPfModel* model,
      IPfTriadManager::MementoPredicate pred) const;

  bool isModelExist(IPfModel* model) const;
  bool isViewExist(IPfView* view) const;

  void doAboutToDestroyTriad(PfPresenter* presenter);

 private:
  PfTriadManagerImpl(const PfTriadManagerImpl& other) = delete;
  PfTriadManagerImpl& operator=(const PfTriadManagerImpl& other) = delete;

  const IPfViewFactoryManager& view_factory_mgr_;

  using TriadListItemType = std::unique_ptr<TriadInfo>;
  std::forward_list<TriadListItemType> triad_list_;

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
PfTriadManagerImpl::createViewFor(
    std::shared_ptr<IPfModel> model,
    PfPresenter* parent,
    bool auto_remove_child,
    PfCreateViewArgs* args,
    IPfTriadManager* triad_manager) {
  PfCreateViewArgs defaultArgs;

  auto orig_args = args;
  if (!args) {
    args = &defaultArgs;
  }

  TriadInfo* parent_triad = nullptr;
  if (parent) {
    for (auto& triad : triad_list_) {
      if (triad->presenter() == parent) {
        parent_triad = triad.get();
        break;
      }
    }

    if (!parent_triad) {
      ALOGE << "create view with not managed parent presenter is not allowed";
      return nullptr;
    }
  }

  IPfViewFactory* view_factory =
      view_factory_mgr_.getViewFactory(model->getModelId(),
                                       args->view_factory_id());
  if (!view_factory) {
    ALOGW << "view factory '" << args->view_factory_id()
          << "' for model " << model->getModelId() << " not found!";
  }

  if (model && view_factory) {
    ALOGI << "create view for model " << model->getModelId()
          << ", parent is "
          << (parent_triad ? parent_triad->model()->getModelId() : "(none)");

    auto presenter = view_factory->createView(model, orig_args);

    if (presenter) {
      model->set_triad_manager(triad_manager);
      presenter->set_triad_manager(triad_manager);

      // TODO(lutts): LOCK

      // we use FILO policy, because pop-up windows are close
      // before their parents, and MainWindow is the first created
      // and last destroyed
      std::unique_ptr<PfCreateViewArgsMemento> memento;
      if (orig_args)
        memento = orig_args->getMemento();

      triad_list_.push_front(
          utils::make_unique<TriadInfo>(presenter,
                                        parent_triad,
                                        auto_remove_child,
                                        std::move(memento)));
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
      [model](const TriadListItemType& triad) {
        return model == triad->model();
      });

  return (iter != triad_list_.end());
}

bool PfTriadManagerImpl::isViewExist(IPfView* view) const {
  auto iter = std::find_if(
      triad_list_.begin(),
      triad_list_.end(),
      [view](const TriadListItemType& triad) {
        return view == triad->view();
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
  ALOGI << "about to destroy view of model " << model->getModelId();
  view->onDestroy();

  // 3. model
  if (model_view_count[model] == 0) {
    ALOGI << "about to destroy model " << model->getModelId();
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
  std::forward_list<TriadListItemType> triads_to_delete;
  triad_list_.remove_if(
      [&triads_to_delete](TriadListItemType& triad){
        if (triad->is_waiting_delete()) {
          triads_to_delete.push_front(std::move(triad));
          return true;
        }

        return false;
      });

  // 3. delete them
  triads_to_delete.reverse();
  triads_to_delete.remove_if(
      [this](const TriadListItemType& triad) {
        doAboutToDestroyTriad(triad->presenter());
        return true;
      });
}

void PfTriadManagerImpl::removeTriadBy(IPfModel* model) {
  // TODO(lutts): LOCK

  std::forward_list<TriadInfo*> matched_triads;
  for (auto& triad : triad_list_) {
    if (triad->model() == model) {
      matched_triads.push_front(triad.get());
    }
  }

  removeTriads(matched_triads);
}

void PfTriadManagerImpl::removeTriadBy(IPfView* view) {
  // TODO(lutts): LOCK

  std::forward_list<TriadInfo*> matched_triads;
  for (auto& triad : triad_list_) {
    if (triad->view() == view) {
      matched_triads.push_front(triad.get());
    }
  }

  removeTriads(matched_triads);
}

// TODO(lutts): what if the view's model has multiple views?
bool PfTriadManagerImpl::requestRemoveTriadByView(IPfView* view) {
  auto iter = std::find_if(
      triad_list_.begin(),
      triad_list_.end(),
      [view](const TriadListItemType& triad) -> bool {
        return triad->view() == view;
      });

  if (iter != triad_list_.end()) {
    auto& triad = *iter;
    IPfModel* model = triad->model();
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
    if (model == triad->model()) {
      matched_views.push_back(triad->view());
    }
  }

  return matched_views;
}

IPfModel* PfTriadManagerImpl::findModelByView(IPfView* view) const {
  auto iter = std::find_if(
      triad_list_.begin(),
      triad_list_.end(),
      [view](const TriadListItemType& triad) -> bool {
        return triad->view() == view;
      });

  if (iter != triad_list_.end()) {
    auto& triad = *iter;
    return triad->model();
  }

  return nullptr;
}

std::vector<IPfView*> PfTriadManagerImpl::findViewByModelAndViewFactory(
    IPfModel* model,
    const IPfViewFactory::ViewFactoryIdType& view_factory_id) const {
  std::vector<IPfView*> matched_views;

  for (auto& triad : triad_list_) {
    if (model == triad->model() &&
        view_factory_id == triad->view_factory_id()) {
      matched_views.push_back(triad->view());
    }
  }

  return matched_views;
}

std::vector<IPfView*> PfTriadManagerImpl::findViewByModel_if(
    IPfModel* model,
    IPfTriadManager::MementoPredicate pred) const {
  std::vector<IPfView*> matched_views;

  PfCreateViewArgsMemento default_memento;

  for (auto& triad : triad_list_) {
    if (model == triad->model()) {
      int result = IPfTriadManager::kNotMatched;
      if (triad->memento()) {
        result = pred(*(triad->memento()));
      } else {
        result = pred(default_memento);
      }

      if (result == IPfTriadManager::kMatchedContinue) {
        matched_views.push_back(triad->view());
        // continue
      } else if (result == IPfTriadManager::kMatchedBreak) {
        matched_views.push_back(triad->view());
        break;
      }
    }
  }

  return matched_views;
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
                              PfPresenter* parent,
                              bool auto_remove_child,
                              PfCreateViewArgs* args) {
  return impl->createViewFor(model, parent, auto_remove_child, args, this);
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

IPfModel* PfTriadManager::findModelByView(IPfView* view) const {
  return impl->findModelByView(view);
}

std::vector<IPfView*> PfTriadManager::findViewByModelAndViewFactory(
    IPfModel* model,
    const IPfViewFactory::ViewFactoryIdType& view_factory_id) const {
  return impl->findViewByModelAndViewFactory(model, view_factory_id);
}

std::vector<IPfView*> PfTriadManager::findViewByModel_if(
    IPfModel* model,
    MementoPredicate pred) const {
  return impl->findViewByModel_if(model, pred);
}

}  // namespace pfmvp
