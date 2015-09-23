// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/qtui/kbnode_tree_qmodel.h"

#include <QApplication>
#include <QDebug>

#include <vector>

#include "utils/basic_utils.h"
#include "snail/i_kbnode_provider.h"
#include "snail/i_kbnode.h"

class KbNodeItem {
 public:
  KbNodeItem() = default;
  KbNodeItem(IKbNodeProvider* kbnode_provider,
             IKbNode* kbnode, KbNodeItem* parent)
      : kbnode_provider_(kbnode_provider)
      , kbnode_(kbnode), parent_(parent) { }
  virtual ~KbNodeItem() { }

  void setKbNodeProvider(IKbNodeProvider* kbnode_provider) {
    kbnode_provider_ = kbnode_provider;
  }

  IKbNode* kbnode() const {
    return kbnode_;
  }

  QString text() const {
    if (is_add_more_)
      return qApp->translate(0, "Add More...");

    if (!kbnode_)
      return QString();

    return U8StringToQString(kbnode_->name());
  }

  int row() const {
    if (!parent_)
      return -1;

    int index = 0;
    for (auto& item : parent_->children_) {
      if (item.get() == this)
        break;

      ++index;
    }

    return index;
  }

  int num_children() const {
    lazy_populate_children();
    return children_.size();
  }

  KbNodeItem* children(int index) const {
    lazy_populate_children();

    if (index >= static_cast<int>(children_.size()))
      return nullptr;

    return children_[index].get();
  }

  KbNodeItem* parent() const {
    return parent_;
  }

  bool isRoot() const {
    return parent_ == nullptr;
  }

  bool isAddMore() const {
    return is_add_more_;
  }

  KbNodeItem* findByKbNode(IKbNode* kbnode) {
    if (kbnode == this->kbnode_)
      return this;

    for (auto & item : children_) {
      auto found_item = item->findByKbNode(kbnode);
      if (found_item)
        return found_item;
    }

    return nullptr;
  }

  void appendKbNode(IKbNode* kbnode) {
    children_.push_back(
        utils::make_unique<KbNodeItem>(kbnode_provider_, kbnode, this));
  }

 private:
  SNAIL_DISABLE_COPY(KbNodeItem);

  void lazy_populate_children() const {
    if (children_.size() > 0)
      return;

    if (is_add_more_)
      return;

    if (is_empty_row_)
      return;

    if (isRoot()) {
      qDebug() << "populate root idx";
    } else {
      qDebug() << "populate kbnode: " << U8StringToQString(kbnode_->name());
    }

    auto that = const_cast<KbNodeItem*>(this);

    if (isRoot() && !kbnode_provider_->isFilterMode()) {
      children_.push_back(
          utils::make_unique<KbNodeItem>(nullptr, nullptr, that));
      auto& last_item = children_.back();
      last_item->is_empty_row_ = true;
    }

    std::unique_ptr<IKbNodeProvider::IChildNodeIterator>
        child_node_iterator(kbnode_provider_->childNodes(kbnode_));
    if (!child_node_iterator)
      return;

    while (child_node_iterator->hasNext()) {
      auto kbnode = child_node_iterator->next();
      children_.push_back(
          utils::make_unique<KbNodeItem>(kbnode_provider_, kbnode, that));
    }

    // append "Add More..." as the last row of root index
    if (isRoot()) {
      children_.push_back(
          utils::make_unique<KbNodeItem>(nullptr, nullptr, that));
      auto& last_item = children_.back();
      last_item->is_add_more_ = true;
    }
  }

  IKbNodeProvider* kbnode_provider_ { nullptr };
  IKbNode* kbnode_ { nullptr };
  KbNodeItem* parent_ { nullptr };
  bool is_add_more_ { false };
  bool is_empty_row_ { false };

  mutable std::vector<std::unique_ptr<KbNodeItem> > children_;
};

class KbNodeTreeQModelImpl {
 public:

 private:
  KbNodeItem root_item_;
  friend class KbNodeTreeQModel;
};

////////////////////////////////////////////////////////////////

KbNodeTreeQModel::KbNodeTreeQModel()
    : pimpl(utils::make_unique<KbNodeTreeQModelImpl>()) { }

KbNodeTreeQModel::~KbNodeTreeQModel() = default;

//////////////// IKbNodeTreeQModel ////////////////

KbNodeItem* KbNodeTreeQModel::indexToItem(const QModelIndex& index) const {
  if (!index.isValid())
    return &pimpl->root_item_;

  return reinterpret_cast<KbNodeItem*>(index.internalPointer());
}

QModelIndex KbNodeTreeQModel::itemToIndex(KbNodeItem* item) const {
  if (!item || item->isRoot())
    return QModelIndex();

  return createIndex(item->row(), 0, item);
}

void KbNodeTreeQModel::setKbNodeProvider(IKbNodeProvider* kbnode_provider) {
  return pimpl->root_item_.setKbNodeProvider(kbnode_provider);
}

IKbNode* KbNodeTreeQModel::kbNodeOfIndex(const QModelIndex& index) const {
  auto item = indexToItem(index);
  return item->kbnode();
}

bool KbNodeTreeQModel::isAddKbNode(const QModelIndex& index) const {
  auto item = indexToItem(index);
  return item->isAddMore();
}

void KbNodeTreeQModel::beginResetQModel() {
  beginResetModel();
}

void KbNodeTreeQModel::endResetQModel() {
  endResetModel();
}

void KbNodeTreeQModel::kbNodeAdded(
    IKbNode* new_kbnode, IKbNode* parent_kbnode) {
  auto parent_item = pimpl->root_item_.findByKbNode(parent_kbnode);
  if (!parent_item)
    return;

  // append at last
  int row = parent_item->num_children();
  beginInsertRows(itemToIndex(parent_item), row, row);
  parent_item->appendKbNode(new_kbnode);
  endInsertRows();
}

//////////////// QAbstractItemModel ////////////////

QVariant KbNodeTreeQModel::data(const QModelIndex &index, int role) const {
  (void)role;

  if (!index.isValid())
    return QVariant();

  auto item = indexToItem(index);
  if (!item || item->isRoot())
    return QVariant();

  switch (role) {
    case Qt::DisplayRole:
      return item->text();
    default:
      return QVariant();
  }
}

Qt::ItemFlags KbNodeTreeQModel::flags(const QModelIndex &index) const {
  auto flags = QAbstractItemModel::flags(index);

  auto kbnode = kbNodeOfIndex(index);
  if (kbnode && kbnode->isCategory()) {
    flags &= ~Qt::ItemIsSelectable;
  }

  return flags;
}

QModelIndex KbNodeTreeQModel::index(
    int row, int column, const QModelIndex &parent) const {
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  auto parent_item = indexToItem(parent);
  if (!parent_item)
    return QModelIndex();

  return itemToIndex(parent_item->children(row));
}

QModelIndex KbNodeTreeQModel::parent(const QModelIndex &index) const {
  if (!index.isValid())
    return QModelIndex();

  auto item = indexToItem(index);
  if (!item || item->isRoot())
    return QModelIndex();

  return itemToIndex(item->parent());
}

int KbNodeTreeQModel::rowCount(const QModelIndex &parent) const {
  auto item = indexToItem(parent);
  if (item->isAddMore()) {
    return 0;
  } else {
    return item->num_children();
  }
}

int KbNodeTreeQModel::columnCount(const QModelIndex &parent) const {
  (void)parent;
  return 1;
}
