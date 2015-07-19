// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef TRIAD_MANAGER_HELPER_H_
#define TRIAD_MANAGER_HELPER_H_

namespace pfmvp {

class TriadManagerHelper {
 public:
  TriadManagerHelper() = default;
  virtual ~TriadManagerHelper() = default;

  void setTriadManager(IPfTriadManager* triad_manager) {
    triad_manager_ = triad_manager;
  }

  IPfTriadManager* triadManager() {
    return triad_manager_;
  }

  void set_default_parent_presenter(PfPresenter* default_parent) {
    default_parent_presenter = default_parent;
  }

  void set_default_auto_remove_child(bool auto_remove_child) {
    default_auto_remove_child = auto_remove_child;
  }

  void removeTriadBy(IPfModel* model) {
    if (triad_manager_) {
      triad_manager_->removeTriadBy(model);
    }
  }

  void removeTriadBy(IPfView* view) {
    if (triad_manager_) {
      triad_manager_->removeTriadBy(view);
    }
  }

  bool requestRemoveTriadByView(IPfView* view) {
    if (!triad_manager_)
      return false;

    triad_manager_->requestRemoveTriadByView(view);
  }

  std::vector<IPfView*> findViewByModel(IPfModel* model) const {
    if (!triad_manager_)
      return std::vector<IPfView*>();

    return triad_manager_->findViewByModel(model);
  }

  std::vector<IPfView*> findViewByModel_if(
      IPfModel* model,
      IPfTriadManager::MementoPredicate pred) {
    if (!triad_manager_)
      return std::vector<IPfView*>();

    return triad_manager_->findViewByModel_if(model, pred);
  }

  IPfModel* findModelByView(IPfView* view) const {
    if (!triad_manager_)
      return nullptr;

    return triad_manager_->findModelByView(view);
  }

  // TODO(lutts): may add monitor support to be notified on dialog exit
  bool showDialog(std::shared_ptr<IPfModel> model,
                  PfCreateViewArgs* args = nullptr,
                  bool modal = true) {
    if (!triad_manager_)
      return false;

    auto view = triad_manager_->createViewFor(model,
                                              default_parent_presenter,
                                              default_auto_remove_child,
                                              args);
    if (view) {
      return view->showView(modal);
    }

    return false;
  }

  template <typename SubVT = IPfView>
  std::shared_ptr<SubVT> createViewFor(
      std::shared_ptr<IPfModel> model,
      PfCreateViewArgs* args = nullptr) {
    if (!triad_manager_)
      return nullptr;

    auto view = triad_manager_->createViewFor(model,
                                              default_parent_presenter,
                                              default_auto_remove_child,
                                              args);
    return std::dynamic_pointer_cast<SubVT>(view);
  }

  template <typename SubVT = IPfView>
  SubVT* createRawViewFor(std::shared_ptr<IPfModel> model,
                          PfCreateViewArgs* args = nullptr) {
    auto view = createViewFor<SubVT>(model, args);
    return view.get();
  }

  template <typename SubVT = IPfView>
  SubVT* findSingleViewByModel(IPfModel* model,
                               const PfCreateViewArgs* args = nullptr) {
    std::vector<IPfView*> matched_views;

    if (args) {
      matched_views =
          findViewByModel_if(model,
                             [args](const PfCreateViewArgsMemento& memento) {
                               if (args->memento_equals(memento)) {
                                 return IPfTriadManager::kMatchedContinue;
                               } else {
                                 return IPfTriadManager::kNotMatched;
                               }
                             });
    } else {
      matched_views = findViewByModel(model);
    }

    if (matched_views.empty())
      return nullptr;

    // TODO(lutts): do we need to warn user if multi views returned?
    for (auto v : matched_views) {
      auto view = dynamic_cast<SubVT*>(v);
      if (view)
        return view;
    }

    return nullptr;
  }

  template <typename SubVT = IPfView>
  SubVT* createRawViewIfNotExist(std::shared_ptr<IPfModel> model,
                                 PfCreateViewArgs* args = nullptr) {
    auto view = findSingleViewByModel<SubVT>(model.get(), args);
    if (view)
      return view;

    return createRawViewFor<SubVT>(model, args);
  }

 private:
  IPfTriadManager* triad_manager_ { nullptr };
  PfPresenter* default_parent_presenter { nullptr };
  bool default_auto_remove_child { true };
};


}  // namespace pfmvp

#endif  // TRIAD_MANAGER_HELPER_H_
