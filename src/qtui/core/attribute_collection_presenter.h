// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_ATTRIBUTE_COLLECTION_PRESENTER_H_
#define SRC_QTUI_ATTRIBUTE_COLLECTION_PRESENTER_H_

#include <memory>  // std::shared_ptr

#include "pfmvp/pf_presenter.h"
#include "snail/i_attribute_collection_model.h"
#include "qtui/i_attribute_collection_view.h"

class IAttributeCollectionQModel;
class IAttributeDelegate;

using AttributeCollectionPresenterBase =
    pfmvp::PfPresenterT<snailcore::IAttributeCollectionModel,
                        IAttributeCollectionView>;

class AttributeCollectionPresenter : public AttributeCollectionPresenterBase {
 public:
  AttributeCollectionPresenter(
      std::shared_ptr<model_type> model,
      std::shared_ptr<view_type> view,
      std::shared_ptr<IAttributeCollectionQModel> qmodel,
      std::unique_ptr<IAttributeDelegate> attr_delegate);
  virtual ~AttributeCollectionPresenter();

  void initialize() override;

 private:
  std::shared_ptr<IAttributeCollectionQModel> qmodel_;
  std::unique_ptr<IAttributeDelegate> attr_delegate_;

 private:
  SNAIL_DISABLE_COPY(AttributeCollectionPresenter);
};


#endif  // SRC_QTUI_ATTRIBUTE_COLLECTION_PRESENTER_H_
