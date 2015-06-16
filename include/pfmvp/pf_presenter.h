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

  ////////////////////// triad manager helpers begin //////////////////
  void set_triad_manager(IPfTriadManager* triad_manager) {
    triad_manager_ = triad_manager;
  }

  IPfTriadManager* triad_manager() {
    return triad_manager_;
  }

  std::shared_ptr<IPfView> createViewFor(std::shared_ptr<IPfModel> model) {
    if (triad_manager_) {
      return triad_manager_->createViewFor(model);
    } else {
      return nullptr;
    }
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
    if (triad_manager_) {
      triad_manager_->requestRemoveTriadByView(view);
    } else {
      return false;
    }
  }

  std::vector<IPfView*> findViewByModel(IPfModel* model) const {
    if (triad_manager_) {
      return triad_manager_->findViewByModel(model);
    } else {
      return std::vector<IPfView*>();
    }
  }

  IPfModel* findModelByView(IPfView* view) const {
    if (triad_manager_) {
      return triad_manager_->findModelByView(view);
    } else {
      return nullptr;
    }
  }
  ////////////////////// triad manager helpers end //////////////////

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
  PfPresenterT(std::shared_ptr<MT> model, std::shared_ptr<VT> view)
      : PfPresenter(model, view)
      , model_(dynamic_cast<MT*>(model.get()))
      , view_(dynamic_cast<VT*>(view.get())) { }

  MT* model() { return model_; }
  VT* view() { return view_; }

 private:
  MT* model_;
  VT* view_;
};


}  // namespace pfmvp

#endif  // INCLUDE_PFMVP_PF_PRESENTER_H_
