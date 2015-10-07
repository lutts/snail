// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef ATTRIBUTE_COLLECTION_VIEW_FACTORY_H_
#define ATTRIBUTE_COLLECTION_VIEW_FACTORY_H_

#include <memory>

#include "utils/basic_utils.h"
#include "snail/i_attribute_collection_model.h"
#include "src/qtui/core/attribute_collection_presenter.h"
#include "src/qtui/ui/attribute_collection_view.h"
#include "src/qtui/core/attribute_collection_qmodel.h"
#include "src/qtui/core/attribute_delegate.h"
#include "pfmvp/pf_view_factory_manager.h"

using namespace pfmvp;  // NOLINT
using namespace snailcore;  // NOLINT

class AttributeCollectionViewFactory
    : public PfViewFactoryT<IAttributeCollectionModel> {
 public:
  AttributeCollectionViewFactory() = default;
  virtual ~AttributeCollectionViewFactory() = default;

  DEF_VIEW_FACTORY_ID(AttributeCollectionViewFactory)

  std::shared_ptr<PfPresenter>
  createViewFor(std::shared_ptr<IAttributeCollectionModel> model,
                PfCreateViewArgs* args) override {
    (void)args;
    auto view = std::make_shared<AttributeCollectionView>();
    auto qmodel = std::make_shared<AttributeCollectionQModel>();
    auto delegate = utils::make_unique<AttributeDelegate>();
    return std::make_shared<AttributeCollectionPresenter>(model, view,
                                                          qmodel,
                                                          std::move(delegate));
  }

 private:
  SNAIL_DISABLE_COPY(AttributeCollectionViewFactory);
};

static view_factory_t<IAttributeCollectionModel, AttributeCollectionViewFactory>
g_attributecollection_view_factory;

#endif  // ATTRIBUTE_COLLECTION_VIEW_FACTORY_H_
