// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_PFMVP_I_PF_VIEW_FACTORY_H_
#define INCLUDE_PFMVP_I_PF_VIEW_FACTORY_H_

#include <vector>
#include <memory>

#include "pfmvp/i_pf_model.h"
#include "pfmvp/pf_view_factory_id.h"
#include "utils/u8string.h"

namespace pfmvp {

class PfPresenter;
class IPfModel;
class PfCreateViewArgs;

class IPfViewFactory {
 public:
  using ViewFactoryIdType = utils::U8String;

  virtual ~IPfViewFactory() = default;

  virtual const ViewFactoryIdType& getViewFactoryId() const = 0;

  virtual std::shared_ptr<PfPresenter> createView(
      std::shared_ptr<IPfModel> model, PfCreateViewArgs* args) = 0;
};

template <typename ModelType>
class PfViewFactoryT : public IPfViewFactory {
 public:
  std::shared_ptr<PfPresenter> createView(std::shared_ptr<IPfModel> model,
                                          PfCreateViewArgs* args) final {
    auto the_model = std::dynamic_pointer_cast<ModelType>(model);
    if (!the_model) return nullptr;

    return createViewFor(the_model, args);
  };

  virtual std::shared_ptr<PfPresenter> createViewFor(
      std::shared_ptr<ModelType> model, PfCreateViewArgs* args) = 0;
};

#define DEF_VIEW_FACTORY_ID(id)                                \
  const ViewFactoryIdType& getViewFactoryId() const override { \
    return viewFactoryId();                                    \
  }                                                            \
                                                               \
  static const ViewFactoryIdType& viewFactoryId() {            \
    static const ViewFactoryIdType view_factory_id{#id};       \
    return view_factory_id;                                    \
  }

class IPfViewFactoryManager {
 public:
  virtual ~IPfViewFactoryManager() = default;

  virtual void addViewFactory(const IPfModel::ModelIdType& model_id,
                              IPfViewFactory* view_factory) = 0;

  virtual void removeViewFactory(const IPfModel::ModelIdType& model_id,
                                 IPfViewFactory* view_factory) = 0;

  // return a view factory for this model_id, if the model has multiple
  // view factories and factroy id is not specified, which one will return
  // is not undefined
  virtual IPfViewFactory* getViewFactory(
      const IPfModel::ModelIdType& model_id,
      const IPfViewFactory::ViewFactoryIdType&
          view_factory_id = INVALID_PF_VIEW_FACTORY_ID) const = 0;

  virtual std::vector<IPfViewFactory*> getAllViewFactory(
      const IPfModel::ModelIdType& model_id) const = 0;
};

}  // namespace pfmvp

#endif  // INCLUDE_PFMVP_I_PF_VIEW_FACTORY_H_
