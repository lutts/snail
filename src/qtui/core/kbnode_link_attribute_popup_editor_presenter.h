// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_PRESENTER_H_
#define KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_PRESENTER_H_

#include <memory>  // std::shared_ptr

#include "pfmvp/pf_presenter.h"
#include "snail/i_kbnode_link_attribute_popup_editor_model.h"
#include "qtui/i_kbnode_link_attribute_popup_editor_view.h"

namespace snailcore {
class IAttributeSetModel;
}  // namespace snailcore

using KbNodeLinkAttributePopupEditorPresenterBase =
    pfmvp::PfPresenterT<snailcore::IKbNodeLinkAttributePopupEditorModel,
                        IKbNodeLinkAttributePopupEditorView>;

class KbNodeLinkAttributePopupEditorPresenter
    : public KbNodeLinkAttributePopupEditorPresenterBase {
 public:
  KbNodeLinkAttributePopupEditorPresenter(
      std::shared_ptr<model_type> model,
      std::shared_ptr<view_type> view,
      std::unique_ptr<ITreeItemQModel<IKbNode>> link_type_qmodel);
  virtual ~KbNodeLinkAttributePopupEditorPresenter();

  void initialize() override;

 private:
  SNAIL_DISABLE_COPY(KbNodeLinkAttributePopupEditorPresenter);

  void createValueAttributeView();
  void initLinkTypeDropDownList();
  void createLinkAttributesView(
      std::shared_ptr<snailcore::IAttributeSetModel> attr_set_model);

  std::unique_ptr<ITreeItemQModel<IKbNode>> link_type_qmodel_;
};

#endif  // KBNODE_LINK_ATTRIBUTE_POPUP_EDITOR_PRESENTER_H_
