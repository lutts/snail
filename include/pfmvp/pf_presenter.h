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
#include "pfmvp/triad_manager_helper.h"
#include "pfmvp/i_pf_model.h"
#include "pfmvp/i_pf_view.h"

namespace pfmvp {

class PfPresenter : public utils::ITrackable,
                    public std::enable_shared_from_this<PfPresenter> {
 public:
  PfPresenter(std::shared_ptr<IPfModel> model, std::shared_ptr<IPfView> view)
      : model_(model), view_(view) {}
  virtual ~PfPresenter() = default;

  std::shared_ptr<IPfModel> getModel() { return model_; }
  std::shared_ptr<IPfView> getView() { return view_; }

  virtual void initialize() {}
  virtual void onDestroy() {}

  void set_triad_manager(IPfTriadManager* triad_manager) {
    triad_manager_ = triad_manager;
    on_set_triad_manager(triad_manager);
  }

  IPfTriadManager* triad_manager() { return triad_manager_; }

 private:
  PfPresenter(const PfPresenter& other) = delete;
  PfPresenter& operator=(const PfPresenter& other) = delete;

  virtual void on_set_triad_manager(IPfTriadManager* triad_manager) {
    (void)triad_manager;
  }

  std::shared_ptr<IPfModel> model_;
  std::shared_ptr<IPfView> view_;
  IPfTriadManager* triad_manager_{nullptr};
};

template <typename MT, typename VT>
class PfPresenterT : public PfPresenter, public TriadManagerHelper {
 public:
  using model_type = MT;
  using view_type = VT;

  PfPresenterT(std::shared_ptr<MT> model, std::shared_ptr<VT> view)
      : PfPresenter(model, view),
        model_(dynamic_cast<MT*>(model.get())),
        view_(dynamic_cast<VT*>(view.get())) {}

  model_type* model() { return model_; }
  view_type* view() { return view_; }

 private:
  void on_set_triad_manager(IPfTriadManager* triad_manager) override {
    setTriadManager(triad_manager);
    set_default_parent_presenter(this);
  }

  MT* model_;
  VT* view_;
};

}  // namespace pfmvp

#endif  // INCLUDE_PFMVP_PF_PRESENTER_H_
