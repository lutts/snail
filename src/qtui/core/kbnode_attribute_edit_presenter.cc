// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/qtui/core/kbnode_attribute_edit_presenter.h"

#include <QString>

#include "utils/u8string.h"
#include "snail/i_kbnode.h"
#include "snail/i_tree_item_provider.h"
#include "snail/i_simple_kbnode_adder_model.h"

using namespace snailcore;  // NOLINT

void KbNodeAttributeEditPresenter::initialize() {
  view()->setKbNodeName(U8StringToQString(model()->getKbNodeName()));

  kbnode_qmodel_->setTreeItemProvider(model()->getKbNodeProvider());
  view()->setKbNodeTreeQModel(kbnode_qmodel_.get());

  view()->whenUserClickedIndex(
      [this](const QModelIndex& index) {
        on_UserClickedIndex(index);
      },
      shared_from_this());

  view()->whenFilterPatternChanged(
      [this](const QString& pattern) {
        auto kbnode_provider = model()->getKbNodeProvider();
        kbnode_provider->setFilterPattern(QStringToU8String(pattern));
        view()->clearWarningMessages();
      },
      shared_from_this());

  view()->whenEditingFinished(
      [this](const QString& text) {
        on_editingFinished(text);
      },
      shared_from_this());

  view()->whenUserClickAddKbNode(
      [this]() {
        addKbNode();
      },
      shared_from_this());

  auto kbnode_provider = model()->getKbNodeProvider();
  kbnode_provider->whenBeginFilter(
      [this]() {
        kbnode_qmodel_->beginResetQModel();
      },
      shared_from_this());

  kbnode_provider->whenFinishFilter(
      [this]() {
        kbnode_qmodel_->endResetQModel();
      },
      shared_from_this());
}

void KbNodeAttributeEditPresenter::addKbNode() {
  auto provider_model = model()->createSimpleKbNodeAdderModel();

  provider_model->whenKbNodeAdded(
      [this](IKbNode* new_kbnode, IKbNode* parent_kbnode) {
        kbnode_qmodel_->kbNodeAdded(new_kbnode, parent_kbnode);
      },
      shared_from_this());

  showDialog(provider_model);
}

void KbNodeAttributeEditPresenter::on_UserClickedIndex(
    const QModelIndex& index) {
  if (kbnode_qmodel_->isAddMore(index)) {
    addKbNode();
  } else {
    auto kbnode = kbnode_qmodel_->indexToKbNode(index);
    model()->setKbNode(kbnode);
    view()->setKbNodeName(U8StringToQString(model()->getKbNodeName()));
    view()->clearWarningMessages();
  }
}

void KbNodeAttributeEditPresenter::on_editingFinished(const QString &text) {
  int ret = model()->setKbNodeByName(QStringToU8String(text));
  auto kbnode_provider = model()->getKbNodeProvider();
  QString provider_name = U8StringToQString(kbnode_provider->name());
  switch (ret) {
    case IKbNodeAttributeModel::kSetKbNodeMultpicMatched:
      view()->warnMultipleMatch(provider_name);
      break;
    case IKbNodeAttributeModel::kSetKbNodeNotFound:
      view()->warnNotFound(provider_name);
      break;
    case IKbNodeAttributeModel::kSetKbNodeSuccess:
      view()->clearWarningMessages();
    default: { }
      // do nothing
  }
}
