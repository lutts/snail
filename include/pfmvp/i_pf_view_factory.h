// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_PFMVP_I_PF_VIEW_FACTORY_H_
#define INCLUDE_PFMVP_I_PF_VIEW_FACTORY_H_

#include "pfmvp/i_pf_model.h"
#include "pfmvp/pf_presenter.h"

namespace pfmvp {

class IPfViewFactory {
 public:
  virtual ~IPfViewFactory() = default;

  virtual std::shared_ptr<PfPresenter>
  createView(std::shared_ptr<IPfModel> model) = 0;
};

class IPfViewFactoryManager {
 public:
  virtual ~IPfViewFactoryManager() = default;

  virtual IPfViewFactory*
  getViewFactory(const IPfModel::ModelIdType& model_id) const = 0;
};

}  // namespace pfmvp

#endif  // INCLUDE_PFMVP_I_PF_VIEW_FACTORY_H_
