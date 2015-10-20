// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/qtui/core/simple_kbnode_adder_presenter.h"

#include <QString>
#include <QModelIndex>

#include "qtui/i_kbnode_tree_qmodel.h"

SimpleKbNodeAdderPresenter::SimpleKbNodeAdderPresenter(
    std::shared_ptr<model_type> model,
    std::shared_ptr<view_type> view,
    std::unique_ptr<IKbNodeTreeQModel> kbnode_qmodel)
    : SimpleKbNodeAdderPresenterBase(model, view)
    , kbnode_qmodel_(std::move(kbnode_qmodel)) {
}

SimpleKbNodeAdderPresenter::~SimpleKbNodeAdderPresenter() = default;

void SimpleKbNodeAdderPresenter::initialize() {
  view()->setProviderName(U8StringToQString(model()->name()));

  auto default_new_kbnode_name = model()->getFilterPattern();
  model()->setNewKbNodeName(default_new_kbnode_name);

  view()->setNewKbNodeName(U8StringToQString(default_new_kbnode_name));
  view()->setNameValidateResult(model()->isNewKbNodeNameValid());

  model()->setFilterPattern("");

  kbnode_qmodel_->setTreeItemProvider(model()->getKbNodeProvider());
  view()->setKbNodeTreeQModel(kbnode_qmodel_.get());

  auto new_kbnode_parent = model()->getNewKbNodeParent();
  auto index = kbnode_qmodel_->kbNodeToIndex(new_kbnode_parent);
  view()->selectIndex(index);

  view()->whenUserSelectIndex(
      [this](const QModelIndex& index) {
        auto kbnode = kbnode_qmodel_->indexToKbNode(index);
        model()->setNewKbNodeParent(kbnode);
      },
      shared_from_this());

  view()->whenNewKbNodeNameChanged(
      [this](const QString& name) {
        model()->setNewKbNodeName(QStringToU8String(name));
        view()->setNameValidateResult(model()->isNewKbNodeNameValid());
      },
      shared_from_this());

  view()->whenUserToggleCategoryCheckbox(
      [this](bool checked) {
        model()->setIsCategory(checked);
      },
      shared_from_this());

  view()->whenUserClickAddButton(
      [this]() {
        model()->addKbNode();
      },
      shared_from_this());
}
