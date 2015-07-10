// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_PFMVP_PF_PRESENTER_H_
#define INCLUDE_PFMVP_PF_PRESENTER_H_

#include <vector>
#include <memory>

#include "utils/i_trackable.h"
#include "pfmvp/i_pf_triad_manager.h"
#include "pfmvp/i_pf_model.h"
#include "pfmvp/i_pf_view.h"

namespace pfmvp {

class PfPresenter : public utils::ITrackable
                  , public std::enable_shared_from_this<PfPresenter> {
 public:
  PfPresenter(std::shared_ptr<IPfModel> model,
              std::shared_ptr<IPfView> view)
      : model_(model)
      , view_(view) { }
  virtual ~PfPresenter() = default;

  std::shared_ptr<IPfModel> getModel() { return model_; }
  std::shared_ptr<IPfView> getView() { return view_; }

  // NOTE: should be called after set_triad_manager()
  virtual void initialize() { }
  virtual void onDestroy() { }

  ////////////////////// triad manager helpers begin //////////////////
  void set_triad_manager(IPfTriadManager* triad_manager) {
    triad_manager_ = triad_manager;
  }

  IPfTriadManager* triad_manager() {
    return triad_manager_;
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

  std::vector<IPfView*>
  findViewByModelId(const IPfModel::ModelIdType& model_id) {
    if (!triad_manager_)
      return std::vector<IPfView*>();

    return triad_manager_->findViewsByModelId(model_id);
  }

  IPfModel* findModelByView(IPfView* view) const {
    if (!triad_manager_)
      return nullptr;

    return triad_manager_->findModelByView(view);
  }

  bool monitorModelRemoveRequest(IPfModel* model) {
    if (!triad_manager_)
      return false;

    return triad_manager_->whenRequestRemoveModel(
        model,
        [this](IPfModel* model) -> bool {
          return onRequestRemoveModel(model);
        },
        shared_from_this());
  }

  bool monitorModelDestroy(IPfModel* model) {
    if (!triad_manager_)
      return false;

    return triad_manager_->whenAboutToDestroyModel(
        model,
        [this](IPfModel* model) {
          onAboutToDestroyModel(model);
        },
        shared_from_this());
  }

  bool monitorViewDestroy(IPfView* view) {
    if (!triad_manager_)
      return false;

    return triad_manager_->whenAboutToDestroyView(
        view,
        [this](IPfView* view) {
          onAboutToDestroyView(view);
        },
        shared_from_this());
  }

  // TODO(lutts): may add monitor support
  bool showDialog(std::shared_ptr<IPfModel> model,
                  bool modal = true) {
    if (!triad_manager_)
      return false;

    auto view = triad_manager_->createViewFor(model);
    if (view) {
      return view->showView(modal);
    }

    return false;
  }
  ////////////////////// triad manager helpers end //////////////////

  virtual bool onRequestRemoveModel(IPfModel* model) {
    (void)model; return false;
  }
  virtual void onAboutToDestroyModel(IPfModel* model) { (void)model; }
  virtual void onAboutToDestroyView(IPfView* view) { (void)view; }

 private:
  PfPresenter(const PfPresenter& other) = delete;
  PfPresenter& operator=(const PfPresenter& other) = delete;

  std::shared_ptr<IPfModel> model_;
  std::shared_ptr<IPfView> view_;
  IPfTriadManager* triad_manager_ { nullptr };
};

template <typename MT, typename VT>
class PfPresenterT : public PfPresenter {
 public:
  using model_type = MT;
  using view_type = VT;

  PfPresenterT(std::shared_ptr<MT> model, std::shared_ptr<VT> view)
      : PfPresenter(model, view)
      , model_(dynamic_cast<MT*>(model.get()))
      , view_(dynamic_cast<VT*>(view.get())) { }

  model_type* model() { return model_; }
  view_type* view() { return view_; }

  //////////////// Triad Manager Helpers begin ///////////////////
  template <typename SubVT>
  std::shared_ptr<SubVT> createViewFor(
      std::shared_ptr<IPfModel> model,
      const IPfViewFactory::ViewFactoryIdType&
      view_factory_id = INVALID_PF_VIEW_FACTORY_ID) {
    if (!triad_manager())
      return nullptr;

    auto v = triad_manager()->createViewFor(model,
                                            view_factory_id,
                                            this);
    return std::dynamic_pointer_cast<SubVT>(v);
  }

  template <typename SubVT>
  SubVT* createRawViewFor(std::shared_ptr<IPfModel> model,
                          const IPfViewFactory::ViewFactoryIdType&
                          view_factory_id = INVALID_PF_VIEW_FACTORY_ID) {
    if (!triad_manager())
      return nullptr;

    auto v = triad_manager()->createViewFor(model,
                                            view_factory_id,
                                            this);
    return dynamic_cast<SubVT*>(v.get());
  }

  template <typename SubVT>
  SubVT* findSingleViewByModel(IPfModel* model) {
    auto view_vec = findViewByModel(model);

    if (view_vec.empty())
      return nullptr;

    // TODO(lutts): do we need to warn user if multi views returned?
    for (auto v : view_vec) {
      auto view = dynamic_cast<SubVT*>(v);
      if (view)
        return view;
    }

    return nullptr;
  }

  // TODO(lutts): how do we ensure SubVT is matched with view_factory_id?
  template <typename SubVT>
  SubVT* createRawViewIfNotExist(std::shared_ptr<IPfModel> model,
                                 const IPfViewFactory::ViewFactoryIdType&
                                 view_factory_id = INVALID_PF_VIEW_FACTORY_ID) {
    // TODO(lutts): if we can ensure SubVT/view_factory_id connection,
    // we can call this, else we may need a findByModelAndViewFactory impl
    auto view = findSingleViewByModel<SubVT>(model);
    if (view)
      return view;

    return createRawViewFor(model, view_factory_id);
  }

  //////////////// Triad Manager Helpers end  ////////////////////

 private:
  MT* model_;
  VT* view_;
};

}  // namespace pfmvp

#endif  // INCLUDE_PFMVP_PF_PRESENTER_H_
