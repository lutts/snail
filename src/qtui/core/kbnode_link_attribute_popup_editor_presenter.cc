// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/qtui/core/kbnode_link_attribute_popup_editor_presenter.h"
#include "pfmvp/i_pf_create_view_args.h"
#include "snail/i_attribute_model.h"
#include "qtui/ui/i_attribute_editor_view.h"
#include "snail/i_attribute_set_model.h"
#include "qtui/ui/i_attribute_set_view.h"

using snailcore::IAttributeSetModel;

KbNodeLinkAttributePopupEditorPresenter::
    KbNodeLinkAttributePopupEditorPresenter(
        std::shared_ptr<model_type> model, std::shared_ptr<view_type> view,
        std::shared_ptr<fto::TreeItemQModel> link_type_qmodel)
    : KbNodeLinkAttributePopupEditorPresenterBase(model, view),
      link_type_qmodel_(link_type_qmodel) {}

KbNodeLinkAttributePopupEditorPresenter::
    ~KbNodeLinkAttributePopupEditorPresenter() = default;

void KbNodeLinkAttributePopupEditorPresenter::initialize() {
  createValueAttributeView();
  initLinkTypeDropDownList();
  createLinkAttributesView(model()->getCurrentLinkAttrSetModel());

  view()->whenUserSelectLinkType([this](const QModelIndex& index) {
                                   auto link_type =
                                       link_type_qmodel_->indexToItem(index);
                                   model()->setProtoLinkType(link_type);
                                 },
                                 shared_from_this());

  model()->whenLinkTypeChanged(
      [this](std::shared_ptr<IAttributeSetModel> new_attr_set_model,
             IAttributeSetModel* old_attr_set_model) {
        removeTriadBy(old_attr_set_model);
        createLinkAttributesView(new_attr_set_model);
      },
      shared_from_this());

  model()->whenValidateComplete(
      [this](bool result) { view()->setDoneButtonEnabled(result); },
      shared_from_this());

  view()->whenUserClickDone([this]() { model()->editFinished(); },
                            shared_from_this());
}

void KbNodeLinkAttributePopupEditorPresenter::createValueAttributeView() {
  view()->setValueAttrName(U8StringToQString(model()->valueAttrName()));

  auto value_attr_model = model()->createValueAttrModel();
  auto value_attr_editor =
      createRawViewFor<IAttributeEditorView>(value_attr_model);
  view()->setValueAttrEditor(value_attr_editor);
}

void KbNodeLinkAttributePopupEditorPresenter::initLinkTypeDropDownList() {
  auto link_type_provider = model()->getLinkTypeItemProvider();
  link_type_qmodel_->setTreeItemProvider(link_type_provider);
  view()->setLinkTypeQModel(link_type_qmodel_->qmodel());

  // select the current link type
  auto current_link_type = model()->getCurrentProtoLinkType();
  auto current_index = link_type_qmodel_->itemToIndex(current_link_type);
  view()->setCurrentLinkType(current_index);
}

void KbNodeLinkAttributePopupEditorPresenter::createLinkAttributesView(
    std::shared_ptr<IAttributeSetModel> attr_set_model) {
  pfmvp::PfCreateViewArgs args;
  args.set_view_factory_id(
      PF_VIEW_FACTORY(AttributeSetViewNonSwitchableFactory));

  auto attr_set_view =
      createRawViewFor<IAttributeSetView>(attr_set_model, &args);
  view()->setLinkAttributeSetView(attr_set_view);
}
