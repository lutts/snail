// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef KBNODEATTRIBUTEEDIT_PRESENTER_H_
#define KBNODEATTRIBUTEEDIT_PRESENTER_H_

#include <memory>  // std::shared_ptr

#include "pfmvp/pf_presenter.h"
#include "snail/i_kbnode_attribute_model.h"
#include "qtui/i_kbnode_attribute_edit_view.h"
#include "qtui/i_kbnode_tree_qmodel.h"

using KbNodeAttributeEditPresenterBase =
    pfmvp::PfPresenterT<snailcore::IKbNodeAttributeModel,
                        IKbNodeAttributeEditView>;

class KbNodeAttributeEditPresenter : public KbNodeAttributeEditPresenterBase {
 public:
  KbNodeAttributeEditPresenter(
      std::shared_ptr<model_type> model,
      std::shared_ptr<view_type> view,
      std::unique_ptr<IKbNodeTreeQModel> kbnode_qmodel)
      : KbNodeAttributeEditPresenterBase(model, view)
      , kbnode_qmodel_(std::move(kbnode_qmodel)) {
  }

  void initialize() override;

 private:
  void on_UserClickedIndex(const QModelIndex& index);
  void on_editingFinished(const QString& text);

  std::unique_ptr<IKbNodeTreeQModel> kbnode_qmodel_;

 private:
  SNAIL_DISABLE_COPY(KbNodeAttributeEditPresenter);
};

#endif  // KBNODEATTRIBUTEEDIT_PRESENTER_H_
