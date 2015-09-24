// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/qtui/kbnode_provider_presenter.h"

#include <QString>
#include <QModelIndex>

#include "qtui/i_kbnode_tree_qmodel.h"

KbNodeProviderPresenter::KbNodeProviderPresenter(
    std::shared_ptr<model_type> model,
    std::shared_ptr<view_type> view,
    std::unique_ptr<IKbNodeTreeQModel> kbnode_qmodel)
    : KbNodeProviderPresenterBase(model, view)
    , kbnode_qmodel_(std::move(kbnode_qmodel)) {
}

KbNodeProviderPresenter::~KbNodeProviderPresenter() = default;

void KbNodeProviderPresenter::initialize() {
  view()->setNewKbNodeName(U8StringToQString(model()->getFilterPattern()));
  model()->setFilterPattern("");

  kbnode_qmodel_->setKbNodeProvider(model()->getKbNodeProvider());
  kbnode_qmodel_->enterAddNewNodeMode();
  view()->setKbNodeTreeQModel(kbnode_qmodel_.get());

  auto new_kbnode_parent = model()->getNewKbNodeParent();
  auto index = kbnode_qmodel_->indexOfKbNode(new_kbnode_parent);
  view()->selectIndex(index);

  view()->whenUserSelectIndex(
      [this](const QModelIndex& index) {
        auto kbnode = kbnode_qmodel_->kbNodeOfIndex(index);
        model()->setNewKbNodeParent(kbnode);
      },
      shared_from_this());

  view()->whenNewKbNodeNameChanged(
      [this](const QString& name) {
        model()->setNewKbNodeName(QStringToU8String(name));
        view()->setNameValidateResult(model()->isNewKbNodeNameValid());
      },
      shared_from_this());

  view()->whenUserClickAddButton(
      [this]() {
        model()->addKbNode();
      },
      shared_from_this());

  view()->whenUserReject(
      [this]() {
        removeTriadBy(model());
      },
      shared_from_this());
}
