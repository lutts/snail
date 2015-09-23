// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/qtui/kbnode_attribute_edit_presenter.h"

#include <QString>

#include "utils/u8string.h"
#include "snail/i_kbnode.h"
#include "snail/i_kbnode_provider.h"

using namespace snailcore;  // NOLINT

void KbNodeAttributeEditPresenter::initialize() {
  view()->setKbNodeName(U8StringToQString(model()->getKbNodeName()));

  kbnode_qmodel_->setKbNodeProvider(model()->getKbNodeProvider());
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
      },
      shared_from_this());

  view()->whenEditingFinished(
      [this](const QString& text) {
        on_editingFinished(text);
      },
      shared_from_this());

  view()->whenUserClickAddKbNode(
      [this]() {
        auto kbnode_provider = model()->getKbNodeProvider();
        auto new_kbnode_pair = kbnode_provider->addKbNode();
        if (new_kbnode_pair.new_kbnode) {
          kbnode_qmodel_->kbNodeAdded(new_kbnode_pair.new_kbnode,
                                      new_kbnode_pair.parent_kbnode);
        }
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

void KbNodeAttributeEditPresenter::on_UserClickedIndex(
    const QModelIndex& index) {
  if (kbnode_qmodel_->isAddKbNode(index)) {
    auto kbnode_provider = model()->getKbNodeProvider();
    auto new_kbnode_pair = kbnode_provider->addKbNode();
    if (new_kbnode_pair.new_kbnode) {
      kbnode_qmodel_->kbNodeAdded(new_kbnode_pair.new_kbnode,
                                  new_kbnode_pair.parent_kbnode);
    }
  } else {
    auto kbnode = kbnode_qmodel_->kbNodeOfIndex(index);
    model()->setKbNode(kbnode);
    view()->setKbNodeName(U8StringToQString(model()->getKbNodeName()));
  }
}

void KbNodeAttributeEditPresenter::on_editingFinished(const QString &text) {
  int ret = model()->setKbNodeByName(QStringToU8String(text));
  switch (ret) {
    case IKbNodeAttributeModel::kSetKbNodeMultpicMatched:
      view()->warnMultipleMatch();
      break;
    case IKbNodeAttributeModel::kSetKbNodeNotFound:
      view()->warnNotFound();
      break;
    case IKbNodeAttributeModel::kSetKbNodeSuccess:
    default: { }
      // do nothing
  }
}
