// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_PF_PRESENTER_H_
#define INCLUDE_SNAIL_PF_PRESENTER_H_

#include <memory>

#include "snail/i_trackable.h"
#include "snail/i_pf_triad_manager.h"
#include "snail/i_pf_model.h"
#include "snail/i_pf_view.h"

namespace snailcore {

class PfPresenter : public ITrackable {
 public:
  PfPresenter(std::shared_ptr<IPfModel> model,
              std::shared_ptr<IPfView> view)
      : model_(model)
      , view_(view) { }
  virtual ~PfPresenter() = default;

  std::shared_ptr<IPfModel> getModel() { return model_; }
  std::shared_ptr<IPfView> getView() { return view_; }

#if 0
  std::shared_ptr<IPfView> createViewFor(std::shared_ptr<IPfModel> model) {
    if (triad_manager_) {
      return triad_manager_->createViewFor(model);
    } else {
      return nullptr;
    }
  }
#endif

 private:
  PfPresenter(const PfPresenter& other) = delete;
  PfPresenter& operator=(const PfPresenter& other) = delete;

  std::shared_ptr<IPfModel> model_;
  std::shared_ptr<IPfView> view_;
  //  IPfTriadManager* triad_manager_ { nullptr };
};


}  // namespace snailcore

#endif  // INCLUDE_SNAIL_PF_PRESENTER_H_
