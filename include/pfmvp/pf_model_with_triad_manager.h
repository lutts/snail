// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_PFMVP_PF_MODEL_WITH_TRIAD_MANAGER_H_
#define INCLUDE_PFMVP_PF_MODEL_WITH_TRIAD_MANAGER_H_

#include "utils/basic_utils.h"
#include "pfmvp/i_pf_model.h"
#include "pfmvp/i_pf_triad_manager.h"

namespace pfmvp {

class PfModelWithTriadManager : public IPfModel {
 public:
  PfModelWithTriadManager() = default;
  virtual ~PfModelWithTriadManager() = default;

  void set_triad_manager(IPfTriadManager* triad_manager) {
    triad_manager_ = triad_manager;
  }

  void removeTriadBy(IPfModel* model) {
    if (!triad_manager_)
      return;

    triad_manager_->removeTriadBy(model);
  }

 private:
  IPfTriadManager* triad_manager_ { nullptr };

 private:
  SNAIL_DISABLE_COPY(PfModelWithTriadManager)
};

}  // namespace pfmvp

#endif  // INCLUDE_PFMVP_PF_MODEL_WITH_TRIAD_MANAGER_H_
