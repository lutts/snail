// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_ATTRIBUTE_COLLECTION_PRESENTER_H_
#define SRC_QTUI_ATTRIBUTE_COLLECTION_PRESENTER_H_

#include <memory>  // std::shared_ptr

#include "utils/basic_utils.h"
#include "pfmvp/pf_presenter.h"
#include "snail/i_attribute_collection_model.h"
#include "qtui/i_work_attribute_view.h"
#include "snail/attribute_display_block.h"

using AttributeCollectionPresenterBase =
    pfmvp::PfPresenterT<snailcore::IAttributeCollectionModel,
                        IAttributeCollectionView>;

class AttributeCollectionPresenter : public AttributeCollectionPresenterBase
                             , public snailcore::IAttributeDisplayBlockVisitor {
 public:
  AttributeCollectionPresenter(
      std::shared_ptr<model_type> model,
      std::shared_ptr<view_type> view,
      IAttributeLayout* attr_layout)
      : AttributeCollectionPresenterBase(model, view)
      , attr_layout_(attr_layout) {
  }

  void initialize() override;

  // IAttributeDisplayBlockVisitor impl
  void beginTraverse(int total_block_count) override;
  void* visitAttributeGroupDisplayBlock(
      snailcore::AttributeGroupDisplayBlock attr_group_block) override;
  void* visitAttributeDisplayBlock(
      snailcore::AttributeDisplayBlock attr_block) override;
  void endTraverse(bool remove_triads) override;

 private:
  SNAIL_DISABLE_COPY(AttributeCollectionPresenter);

  IAttributeLayout* attr_layout_;
};

#endif  // SRC_QTUI_ATTRIBUTE_COLLECTION_PRESENTER_H_
