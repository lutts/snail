// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/qtui/core/tree_item_qmodel_impl.h"

#include <QObject>
// #include <QDebug>

#include <vector>

#include "utils/basic_utils.h"
#include "snail/i_tree_item_provider.h"
#include "snail/i_tree_item.h"

class QtTreeItem : QObject {
  Q_OBJECT

 public:
  QtTreeItem() = default;
  QtTreeItem(ITreeItemProvider* item_provider, ITreeItem* tree_item,
             QtTreeItem* parent)
      : item_provider_(item_provider), tree_item_(tree_item), parent_(parent) {
    if (tree_item_) {
      text_ = U8StringToQString(tree_item_->name());
    }
  }
  virtual ~QtTreeItem() {}

  virtual std::unique_ptr<QtTreeItem> createQtTreeItem(ITreeItem* tree_item) {
    return utils::make_unique<QtTreeItem>(item_provider_, tree_item, this);
  }

  void setTreeItemProvider(ITreeItemProvider* item_provider) {
    item_provider_ = item_provider;
  }

  ITreeItemProvider* itemProvider() const { return item_provider_; }

  ITreeItem* tree_item() const { return tree_item_; }

  QString text() const { return text_; }

  void setText(const QString& text) { text_ = text; }

  int row() const {
    if (!parent_) return -1;

    int index = 0;
    for (auto& item : parent_->children_) {
      if (item.get() == this) break;

      ++index;
    }

    return index;
  }

  QtTreeItem* parent() const { return parent_; }

  bool isRoot() const { return parent_ == nullptr; }

  int num_children() const {
    lazy_populate_children();
    return children_.size();
  }

  bool children_populated() const { return children_populated_; }

  QtTreeItem* children(int index) const {
    lazy_populate_children();

    if (index >= static_cast<int>(children_.size())) return nullptr;

    return children_[index].get();
  }

  // NOTE: null item is NOT handled because there maybe multi null tree_item
  //       items, and we do NOT know which to return
  QtTreeItem* findByTreeItem(const ITreeItem* tree_item) {
    if (tree_item == nullptr) return nullptr;

    if (tree_item == this->tree_item_) {
      return this;
    }

    for (auto& item : children_) {
      auto found_item = item->findByTreeItem(tree_item);
      if (found_item) return found_item;
    }

    return nullptr;
  }

  virtual int next_append_pos() { return children_.size(); }

  QtTreeItem* appendTreeItem(ITreeItem* tree_item) {
    auto pos = children_.begin();
    std::advance(pos, next_append_pos());

    auto new_item = createQtTreeItem(tree_item);
    auto new_item_ptr = new_item.get();

    children_.insert(pos, std::move(new_item));

    return new_item_ptr;
  }

  void markAsPopulated() { children_populated_ = true; }

  void clear() {
    children_.clear();
    children_populated_ = false;
  }

 protected:
  std::vector<std::unique_ptr<QtTreeItem> >& children() { return children_; }

  virtual void populate_children() {
    auto child_node_iterator = item_provider_->childItems(tree_item_);
    if (child_node_iterator) {
      while (child_node_iterator->hasNext()) {
        auto item = child_node_iterator->next();
        // qDebug() << "\t " << U8StringToQString(item->name());
        children_.push_back(createQtTreeItem(item));
      }
    }
  }

 private:
  SNAIL_DISABLE_COPY(QtTreeItem);

  void lazy_populate_children() const {
    if (children_populated_) return;

#if 0
    if (isRoot()) {
      qDebug() << "populate root idx";
    } else {
      if (tree_item_) {
        qDebug() << "populate tree_item: "
                 << U8StringToQString(tree_item_->name());
      } else {
        qDebug() << "populate tree_item: " << text_;
      }
    }
#endif

    auto that = const_cast<QtTreeItem*>(this);
    that->populate_children();

    children_populated_ = true;
  }

  ITreeItemProvider* item_provider_{nullptr};
  ITreeItem* tree_item_{nullptr};
  QtTreeItem* parent_{nullptr};
  QString text_;
  mutable bool children_populated_{false};

  mutable std::vector<std::unique_ptr<QtTreeItem> > children_;
};

#include "tree_item_qmodel_impl.moc"

////////////////////////////////////////////////////////////////

TreeItemQModelImpl::TreeItemQModelImpl() {}

TreeItemQModelImpl::~TreeItemQModelImpl() = default;

//////////////// fto::TreeItemQModelImpl ////////////////

QtTreeItem* TreeItemQModelImpl::indexToQtItem(const QModelIndex& index) const {
  if (!index.isValid()) return rootItem();

  return reinterpret_cast<QtTreeItem*>(index.internalPointer());
}

QModelIndex TreeItemQModelImpl::qtItemToIndex(QtTreeItem* item) const {
  if (!item || item->isRoot()) return QModelIndex();

  return createIndex(item->row(), 0, item);
}

ITreeItem* TreeItemQModelImpl::indexToItem(const QModelIndex& index) const {
  auto item = indexToQtItem(index);
  return item->tree_item();
}

QModelIndex TreeItemQModelImpl::itemToIndex(const ITreeItem* tree_item) const {
  QtTreeItem* item = nullptr;

  if (tree_item == nullptr)
    item = rootItem();
  else
    item = rootItem()->findByTreeItem(tree_item);

  return qtItemToIndex(item);
}

void TreeItemQModelImpl::setTreeItemProvider(ITreeItemProvider* item_provider) {
  return rootItem()->setTreeItemProvider(item_provider);
}

bool TreeItemQModelImpl::isAddMore(const QModelIndex& index) const {
  (void)index;
  return false;
}

void TreeItemQModelImpl::beginResetQModel() {
  beginResetModel();
  clear();
}

void TreeItemQModelImpl::endResetQModel() { endResetModel(); }

void TreeItemQModelImpl::itemAdded(const ITreeItem* new_tree_item,
                                   const ITreeItem* parent_tree_item) {
  auto parent_item = indexToQtItem(itemToIndex(parent_tree_item));
  if (!parent_item) return;

  if (parent_item->isRoot() && (parent_tree_item != nullptr))
    return;  // NOT found

  if (!parent_item->children_populated()) return;

  int row = parent_item->next_append_pos();
  beginInsertRows(qtItemToIndex(parent_item), row, row);
  parent_item->appendTreeItem(const_cast<ITreeItem*>(new_tree_item));
  endInsertRows();
}

//////////////// QAbstractItemModel ////////////////

QVariant TreeItemQModelImpl::data(const QModelIndex& index, int role) const {
  if (!index.isValid()) return QVariant();

  auto item = indexToQtItem(index);
  if (!item || item->isRoot()) return QVariant();

  return qtItemData(item, role);
}

QVariant TreeItemQModelImpl::qtItemData(QtTreeItem* item, int role) const {
  switch (role) {
    case Qt::DisplayRole:
      return item->text();
    default:
      return QVariant();
  }
}

Qt::ItemFlags TreeItemQModelImpl::flags(const QModelIndex& index) const {
  auto flags = QAbstractItemModel::flags(index);

  auto tree_item = indexToItem(index);
  if (tree_item && tree_item->isGroupOnly()) {
    flags &= ~Qt::ItemIsSelectable;
  }

  return flags;
}

QModelIndex TreeItemQModelImpl::index(int row, int column,
                                      const QModelIndex& parent) const {
  if (!hasIndex(row, column, parent)) return QModelIndex();

  auto parent_item = indexToQtItem(parent);
  if (!parent_item) return QModelIndex();

  return qtItemToIndex(parent_item->children(row));
}

QModelIndex TreeItemQModelImpl::parent(const QModelIndex& index) const {
  if (!index.isValid()) return QModelIndex();

  auto item = indexToQtItem(index);
  if (!item || item->isRoot()) return QModelIndex();

  return qtItemToIndex(item->parent());
}

int TreeItemQModelImpl::rowCount(const QModelIndex& parent) const {
  auto item = indexToQtItem(parent);
  return item->num_children();
}

int TreeItemQModelImpl::columnCount(const QModelIndex& parent) const {
  (void)parent;
  return 1;
}

QtTreeItem* TreeItemQModelImpl::rootItem() const {
  if (!root_item_) root_item_ = createRootItem();
  return root_item_.get();
}

std::unique_ptr<QtTreeItem> TreeItemQModelImpl::createRootItem() const {
  return utils::make_unique<QtTreeItem>();
}

void TreeItemQModelImpl::clear() {
  if (rootItem()) rootItem()->clear();
}

////////////////////////////////////////////////////////////////

class QtTreeItemWithEmptyAddMore : public QtTreeItem {
 public:
  QtTreeItemWithEmptyAddMore() = default;
  QtTreeItemWithEmptyAddMore(ITreeItemProvider* item_provider,
                             ITreeItem* tree_item, QtTreeItem* parent)
      : QtTreeItem(item_provider, tree_item, parent) {}

  std::unique_ptr<QtTreeItem> createQtTreeItem(ITreeItem* tree_item) override {
    return createItem_(tree_item);
  }

  bool isAddMore() const { return is_add_more_; }

  bool isSpecial() const { return is_add_more_ || is_empty_row_; }

  int next_append_pos() override {
    int pos = children().size();
    if (pos == 0) return pos;

    auto& last = children().back();
    auto item = static_cast<QtTreeItemWithEmptyAddMore*>(last.get());
    if (item->is_add_more_) {
      --pos;
    }

    return pos;
  }

  void populate_children() override {
    if (is_add_more_) return;

    if (is_empty_row_) return;

    if (isRoot()) {
      auto empty_item = createItem_(nullptr);
      empty_item->is_empty_row_ = true;
      empty_item->setText(tr("(clear)"));

      children().push_back(std::move(empty_item));
    }

    QtTreeItem::populate_children();

    // append "Add More..." as the last row of root index
    if (isRoot()) {
      auto add_more_item = createItem_(nullptr);
      add_more_item->is_add_more_ = true;
      add_more_item->setText(tr("Add More..."));

      children().push_back(std::move(add_more_item));
    }
  }

 private:
  std::unique_ptr<QtTreeItemWithEmptyAddMore> createItem_(
      ITreeItem* tree_item) {
    return utils::make_unique<QtTreeItemWithEmptyAddMore>(itemProvider(),
                                                          tree_item, this);
  }

  bool is_add_more_{false};
  bool is_empty_row_{false};
};

TreeItemQModelImplWithClearAndAddMoreRow::
    TreeItemQModelImplWithClearAndAddMoreRow()
    : TreeItemQModelImpl() {}

TreeItemQModelImplWithClearAndAddMoreRow::
    ~TreeItemQModelImplWithClearAndAddMoreRow() = default;

QVariant TreeItemQModelImplWithClearAndAddMoreRow::qtItemData(QtTreeItem* item,
                                                            int role) const {
  auto our_item = static_cast<QtTreeItemWithEmptyAddMore*>(item);
  if (our_item->isSpecial() && role == Qt::TextAlignmentRole) {
    return static_cast<int>(Qt::AlignCenter | Qt::AlignVCenter);
  }

  return TreeItemQModelImpl::qtItemData(item, role);
}

std::unique_ptr<QtTreeItem>
TreeItemQModelImplWithClearAndAddMoreRow::createRootItem() const {
  return utils::make_unique<QtTreeItemWithEmptyAddMore>();
}

bool TreeItemQModelImplWithClearAndAddMoreRow::isAddMore(
    const QModelIndex& index) const {
  auto item = static_cast<QtTreeItemWithEmptyAddMore*>(indexToQtItem(index));
  return item->isAddMore();
}

////////////////////////////////////////////////////////////////

TreeItemQModelImplWithProviderRoot::TreeItemQModelImplWithProviderRoot()
    : TreeItemQModelImpl() {}

TreeItemQModelImplWithProviderRoot::~TreeItemQModelImplWithProviderRoot() =
    default;

Qt::ItemFlags TreeItemQModelImplWithProviderRoot::flags(
    const QModelIndex& index) const {
  auto flags = TreeItemQModelImpl::flags(index);
  return flags | Qt::ItemIsSelectable;
}

void TreeItemQModelImplWithProviderRoot::clear() {
  if (provider_item_) provider_item_->clear();
}

void TreeItemQModelImplWithProviderRoot::setTreeItemProvider(
    ITreeItemProvider* item_provider) {
  if (!provider_item_) {
    provider_item_ = rootItem()->appendTreeItem(nullptr);
    rootItem()->markAsPopulated();
  }

  if (!provider_item_) return;

  auto text = U8StringToQString(item_provider->name());
  provider_item_->setText(text);
  provider_item_->setTreeItemProvider(item_provider);

  TreeItemQModelImpl::setTreeItemProvider(item_provider);
}

QModelIndex TreeItemQModelImplWithProviderRoot::itemToIndex(
    const ITreeItem* tree_item) const {
  if (tree_item == nullptr) {
    return qtItemToIndex(provider_item_);
  }

  return TreeItemQModelImpl::itemToIndex(tree_item);
}
