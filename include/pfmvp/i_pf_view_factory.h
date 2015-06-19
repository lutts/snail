// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_PFMVP_I_PF_VIEW_FACTORY_H_
#define INCLUDE_PFMVP_I_PF_VIEW_FACTORY_H_

#include <memory>

#include "pfmvp/i_pf_model.h"
// #include "pfmvp/pf_presenter.h"
#include "utils/u8string.h"

namespace pfmvp {

class PfPresenter;
class IPfModel;

class IPfViewFactory {
 public:
  using ViewFactoryIdType = utils::U8String;

  virtual ~IPfViewFactory() = default;

  virtual const ViewFactoryIdType& getViewFactoryId() const = 0;

  virtual std::shared_ptr<PfPresenter>
  createView(std::shared_ptr<IPfModel> model) = 0;
};

#define INVALID_PF_VIEW_FACTORY_ID utils::U8String{""}
#define isViewFactoryIdValid(id) (id != INVALID_PF_VIEW_FACTORY_ID)

#define DEF_VIEW_FACTORY_ID(id)                                 \
  const ViewFactoryIdType& getViewFactoryId() const override {  \
    return viewFactoryId();                                     \
  }                                                             \
                                                                \
  static const ViewFactoryIdType& viewFactoryId() {             \
    static const ViewFactoryIdType view_factory_id { #id };     \
    return view_factory_id;                                     \
  }

class IPfViewFactoryManager {
 public:
  virtual ~IPfViewFactoryManager() = default;

  virtual void addViewFactory(const IPfModel::ModelIdType& model_id,
                              IPfViewFactory* view_factory) = 0;

  // return a view factory for this model_id, if the model has multiple
  // view factories and factroy id is not specified, which one will return
  // is not undefined
  virtual IPfViewFactory* getViewFactory(
      const IPfModel::ModelIdType& model_id,
      const IPfViewFactory::ViewFactoryIdType&
      view_factory_id = INVALID_PF_VIEW_FACTORY_ID) const = 0;

  virtual std::vector<IPfViewFactory*>
  getAllViewFactory(const IPfModel::ModelIdType& model_id) const = 0;
};

}  // namespace pfmvp

#endif  // INCLUDE_PFMVP_I_PF_VIEW_FACTORY_H_
