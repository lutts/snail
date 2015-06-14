// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/pf_triad_manager.h"

#include <forward_list>

#include "src/utils/basic_utils.h"  // utils::make_unique
#include "snail/i_pf_view_factory.h"

namespace snailcore {

class PfTriadManager::PfTriadManagerImpl {
 public:
  PfTriadManagerImpl() = default;
  virtual ~PfTriadManagerImpl() = default;

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

  using PresenterStoreType = std::forward_list<std::shared_ptr<PfPresenter> >;
  PresenterStoreType presenterStore;

  std::unordered_map<IPfModel*, PfTriadManager::RequestRemoveModelSignalType>
  model_remove_sig_map_;

  std::unordered_map<IPfModel*, PfTriadManager::AboutToDestroyModelSignalType>
  model_destroy_sig_map_;

  std::unordered_map<IPfView*, PfTriadManager::AboutToDestroyViewSignalType>
  view_destroy_sig_map_;

  friend class PfTriadManager;
};

PfTriadManager::PfTriadManager()
    : impl(utils::make_unique<PfTriadManagerImpl>()) {
}

PfTriadManager::~PfTriadManager() = default;

std::shared_ptr<IPfView>
PfTriadManager::createViewFor(std::shared_ptr<IPfModel> model) {
  IPfViewFactory* view_factory =
      impl::PfViewFactoryManager::getViewFactory(model->getModelId());
  auto presenter = view_factory->createView(model);
  presenter->set_triad_manager(this);

  // TODO(lutts): LOCK
  // we use FILO policy, because pop-up windows are close before their parents,
  // and MainWindow is the first created and last destroyed
  impl->presenterStore.push_front(presenter);

  return presenter->getView();
}

#define SNAIL_PFTRIAD_SIGSLOT_IMPL(THISCLASS, sigName, ObjType)         \
  void THISCLASS::when##sigName(                                        \
      ObjType* obj,                                                     \
      sigName##SlotType handler,                                        \
      std::shared_ptr<snailcore::ITrackable> trackObject) {             \
    if (obj == nullptr) {                                               \
      return;                                                           \
    }                                                                   \
                                                                        \
    auto& sig = impl->sigName##SignalOf(obj);                           \
                sigName##SignalType::slot_type subscriber(handler);     \
                    sig.connect(subscriber.track_foreign(trackObject)); \
  }

SNAIL_PFTRIAD_SIGSLOT_IMPL(PfTriadManager, RequestRemoveModel, IPfModel);
SNAIL_PFTRIAD_SIGSLOT_IMPL(PfTriadManager, AboutToDestroyModel, IPfModel);
SNAIL_PFTRIAD_SIGSLOT_IMPL(PfTriadManager, AboutToDestroyView, IPfView);

void PfTriadManager::emitAboutToDestroySignal(IPfModel* model, IPfView* view) {
  //  AboutToDestroyView(view);
  //  AboutToDestroyModel(model);
  auto& view_destroy_sig = impl->AboutToDestroyViewSignalOf(view);
  view_destroy_sig(view);

  auto& model_destroy_sig = impl->AboutToDestroyModelSignalOf(model);
  model_destroy_sig(model);


  impl->view_destroy_sig_map_.erase(view);
  impl->model_remove_sig_map_.erase(model);
  impl->model_destroy_sig_map_.erase(model);
}

void PfTriadManager::removeTriadBy(IPfModel* model) {
  // TODO(lutts): LOCK
  impl->presenterStore.remove_if(
      [this, model](const std::shared_ptr<PfPresenter> item) -> bool {
        auto matched = item->getModel().get() == model;
        if (matched) {
          emitAboutToDestroySignal(
              item->getModel().get(),
              item->getView().get());
        }

        return matched;
      });
}

void PfTriadManager::removeTriadBy(IPfView* view) {
  // TODO(lutts): LOCK
  impl->presenterStore.remove_if(
      [this, view](const std::shared_ptr<PfPresenter> item) -> bool {
        auto matched = item->getView().get() == view;
        if (matched) {
          emitAboutToDestroySignal(
              item->getModel().get(),
              item->getView().get());
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
    auto presenter = *iter;
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

}  // namespace snailcore
