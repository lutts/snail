// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/qtui/core/kbnode_tree_qmodel.h"

#include <QObject>
// #include <QDebug>

#include <vector>

#include "utils/basic_utils.h"
#include "snail/i_kbnode_provider.h"
#include "snail/i_kbnode.h"

class KbNodeItem : QObject {
  Q_OBJECT

 public:
  KbNodeItem() = default;
  KbNodeItem(IKbNodeProvider* kbnode_provider,
             IKbNode* kbnode, KbNodeItem* parent)
      : kbnode_provider_(kbnode_provider)
      , kbnode_(kbnode), parent_(parent) {
    if (kbnode_) {
      text_ = U8StringToQString(kbnode_->name());
    }
  }
  virtual ~KbNodeItem() { }

  virtual std::unique_ptr<KbNodeItem> createKbNodeItem(IKbNode* kbnode) {
    return utils::make_unique<KbNodeItem>(kbnode_provider_, kbnode, this);
  }

  void setKbNodeProvider(IKbNodeProvider* kbnode_provider) {
    kbnode_provider_ = kbnode_provider;
  }

  IKbNodeProvider* kbNodeProvider() const {
    return kbnode_provider_;
  }

  IKbNode* kbnode() const {
    return kbnode_;
  }

  QString text() const {
    return text_;
  }

  void setText(const QString& text) {
    text_ = text;
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

  KbNodeItem* parent() const {
    return parent_;
  }

  bool isRoot() const {
    return parent_ == nullptr;
  }

  int num_children() const {
    lazy_populate_children();
    return children_.size();
  }

  bool children_populated() const {
    return children_populated_;
  }

  KbNodeItem* children(int index) const {
    lazy_populate_children();

    if (index >= static_cast<int>(children_.size()))
      return nullptr;

    return children_[index].get();
  }

  // NOTE: null kbnode is NOT handled because there maybe multi null kbnode
  //       items, and we do NOT know which to return
  KbNodeItem* findByKbNode(IKbNode* kbnode) {
    if (kbnode == nullptr)
      return nullptr;

    if (kbnode == this->kbnode_) {
      return this;
    }

    for (auto & item : children_) {
      auto found_item = item->findByKbNode(kbnode);
      if (found_item)
        return found_item;
    }

    return nullptr;
  }

  virtual int next_append_pos() {
    return children_.size();
  }

  KbNodeItem* appendKbNode(IKbNode* kbnode) {
    auto pos = children_.begin();
    std::advance(pos, next_append_pos());

    auto new_item = createKbNodeItem(kbnode);
    auto new_item_ptr = new_item.get();

    children_.insert(pos, std::move(new_item));

    return new_item_ptr;
  }

  void markAsPopulated() {
    children_populated_ = true;
  }

  void clear() {
    children_.clear();
    children_populated_ = false;
  }

 protected:
  std::vector<std::unique_ptr<KbNodeItem> >& children() {
    return children_;
  }

  virtual void populate_children() {
    auto child_node_iterator = kbnode_provider_->childNodes(kbnode_);
    if (child_node_iterator) {
      while (child_node_iterator->hasNext()) {
        auto kbnode = child_node_iterator->next();
        // qDebug() << "\t " << U8StringToQString(kbnode->name());
        children_.push_back(createKbNodeItem(kbnode));
      }
    }
  }

 private:
  SNAIL_DISABLE_COPY(KbNodeItem);

  void lazy_populate_children() const {
    if (children_populated_)
      return;

#if 0
    if (isRoot()) {
      qDebug() << "populate root idx";
    } else {
      if (kbnode_) {
        qDebug() << "populate kbnode: " << U8StringToQString(kbnode_->name());
      } else {
        qDebug() << "populate kbnode: " << text_;
      }
    }
#endif

    auto that = const_cast<KbNodeItem*>(this);
    that->populate_children();

    children_populated_ = true;
  }

  IKbNodeProvider* kbnode_provider_ { nullptr };
  IKbNode* kbnode_ { nullptr };
  KbNodeItem* parent_ { nullptr };
  QString text_;
  mutable bool children_populated_ { false };

  mutable std::vector<std::unique_ptr<KbNodeItem> > children_;
};

#include "kbnode_tree_qmodel.moc"

////////////////////////////////////////////////////////////////

KbNodeTreeQModelBase::KbNodeTreeQModelBase() { }

KbNodeTreeQModelBase::~KbNodeTreeQModelBase() = default;

//////////////// IKbNodeTreeQModelBase ////////////////

KbNodeItem* KbNodeTreeQModelBase::indexToItem(const QModelIndex& index) const {
  if (!index.isValid())
    return rootItem();

  return reinterpret_cast<KbNodeItem*>(index.internalPointer());
}

QModelIndex KbNodeTreeQModelBase::itemToIndex(KbNodeItem* item) const {
  if (!item || item->isRoot())
    return QModelIndex();

  return createIndex(item->row(), 0, item);
}

IKbNode* KbNodeTreeQModelBase::indexToKbNode(const QModelIndex& index) const {
  auto item = indexToItem(index);
  return item->kbnode();
}

QModelIndex KbNodeTreeQModelBase::kbNodeToIndex(IKbNode* kbnode) const {
  KbNodeItem* item = nullptr;

  if (kbnode == nullptr)
    item = rootItem();
  else
    item = rootItem()->findByKbNode(kbnode);

  return itemToIndex(item);
}

void KbNodeTreeQModelBase::setKbNodeProvider(IKbNodeProvider* kbnode_provider) {
  return rootItem()->setKbNodeProvider(kbnode_provider);
}

bool KbNodeTreeQModelBase::isAddKbNode(const QModelIndex& index) const {
  (void)index;
  return false;
}

void KbNodeTreeQModelBase::beginResetQModel() {
  beginResetModel();
  clear();
}

void KbNodeTreeQModelBase::endResetQModel() {
  endResetModel();
}

void KbNodeTreeQModelBase::kbNodeAdded(
    IKbNode* new_kbnode, IKbNode* parent_kbnode) {
  auto parent_item = indexToItem(kbNodeToIndex(parent_kbnode));
  if (!parent_item)
    return;

  if (parent_item->isRoot() && (parent_kbnode != nullptr))
    return;  // NOT found

  if (!parent_item->children_populated())
    return;

  int row = parent_item->next_append_pos();
  beginInsertRows(itemToIndex(parent_item), row, row);
  parent_item->appendKbNode(new_kbnode);
  endInsertRows();
}

//////////////// QAbstractItemModel ////////////////

QVariant KbNodeTreeQModelBase::data(const QModelIndex &index, int role) const {
  if (!index.isValid())
    return QVariant();

  auto item = indexToItem(index);
  if (!item || item->isRoot())
    return QVariant();

  return itemData(item, role);
}

QVariant KbNodeTreeQModelBase::itemData(KbNodeItem* item, int role) const {
  switch (role) {
    case Qt::DisplayRole:
      return item->text();
    default:
      return QVariant();
  }
}

Qt::ItemFlags KbNodeTreeQModelBase::flags(const QModelIndex &index) const {
  auto flags = QAbstractItemModel::flags(index);

  auto kbnode = indexToKbNode(index);
  if (kbnode && kbnode->isCategory()) {
    flags &= ~Qt::ItemIsSelectable;
  }

  return flags;
}

QModelIndex KbNodeTreeQModelBase::index(
    int row, int column, const QModelIndex &parent) const {
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  auto parent_item = indexToItem(parent);
  if (!parent_item)
    return QModelIndex();

  return itemToIndex(parent_item->children(row));
}

QModelIndex KbNodeTreeQModelBase::parent(const QModelIndex &index) const {
  if (!index.isValid())
    return QModelIndex();

  auto item = indexToItem(index);
  if (!item || item->isRoot())
    return QModelIndex();

  return itemToIndex(item->parent());
}

int KbNodeTreeQModelBase::rowCount(const QModelIndex &parent) const {
  auto item = indexToItem(parent);
  return item->num_children();
}

int KbNodeTreeQModelBase::columnCount(const QModelIndex &parent) const {
  (void)parent;
  return 1;
}

KbNodeItem* KbNodeTreeQModelBase::rootItem() const {
  if (!root_item_)
    root_item_ = createRootItem();
  return root_item_.get();
}

std::unique_ptr<KbNodeItem> KbNodeTreeQModelBase::createRootItem() const {
  return utils::make_unique<KbNodeItem>();
}

void KbNodeTreeQModelBase::clear() {
  if (rootItem())
    rootItem()->clear();
}

////////////////////////////////////////////////////////////////

class KbNodeItemWithEmptyAddMore : public KbNodeItem {
 public:
  KbNodeItemWithEmptyAddMore() = default;
  KbNodeItemWithEmptyAddMore(IKbNodeProvider* kbnode_provider,
                             IKbNode* kbnode, KbNodeItem* parent)
      : KbNodeItem(kbnode_provider, kbnode, parent) { }

  std::unique_ptr<KbNodeItem> createKbNodeItem(IKbNode* kbnode) override {
    return createItem_(kbnode);
  }

  bool isAddMore() const {
    return is_add_more_;
  }

  bool isSpecial() const {
    return is_add_more_ || is_empty_row_;
  }

  int next_append_pos() override {
    int pos = children().size();
    if (pos == 0)
      return pos;

    auto & last = children().back();
    auto item = static_cast<KbNodeItemWithEmptyAddMore*>(last.get());
    if (item->is_add_more_) {
      --pos;
    }

    return pos;
  }

  void populate_children() override {
    if (is_add_more_)
      return;

    if (is_empty_row_)
      return;

    if (isRoot()) {
      auto empty_item = createItem_(nullptr);
      empty_item->is_empty_row_ = true;
      empty_item->setText(tr("(clear)"));

      children().push_back(std::move(empty_item));
    }

    KbNodeItem::populate_children();

    // append "Add More..." as the last row of root index
    if (isRoot()) {
      auto add_more_item = createItem_(nullptr);
      add_more_item->is_add_more_ = true;
      add_more_item->setText(tr("Add More..."));

      children().push_back(std::move(add_more_item));
    }
  }

 private:
  std::unique_ptr<KbNodeItemWithEmptyAddMore> createItem_(IKbNode* kbnode) {
    return utils::make_unique<KbNodeItemWithEmptyAddMore>(kbNodeProvider(),
                                                          kbnode, this);
  }

  bool is_add_more_ { false };
  bool is_empty_row_ { false };
};

KbNodeTreeQModel::KbNodeTreeQModel()
    : KbNodeTreeQModelBase() { }

KbNodeTreeQModel::~KbNodeTreeQModel() = default;

QVariant KbNodeTreeQModel::itemData(KbNodeItem* item, int role) const {
  auto our_item = static_cast<KbNodeItemWithEmptyAddMore*>(item);
  if (our_item->isSpecial() && role == Qt::TextAlignmentRole) {
    return static_cast<int>(Qt::AlignCenter | Qt::AlignVCenter);
  }

  return KbNodeTreeQModelBase::itemData(item, role);
}

std::unique_ptr<KbNodeItem> KbNodeTreeQModel::createRootItem() const {
  return utils::make_unique<KbNodeItemWithEmptyAddMore>();
}

bool KbNodeTreeQModel::isAddKbNode(const QModelIndex& index) const {
  auto item = static_cast<KbNodeItemWithEmptyAddMore*>(indexToItem(index));
  return item->isAddMore();
}

////////////////////////////////////////////////////////////////

KbNodeTreeQModelWithProviderNode::KbNodeTreeQModelWithProviderNode()
    : KbNodeTreeQModelBase() { }

KbNodeTreeQModelWithProviderNode::~KbNodeTreeQModelWithProviderNode() = default;

Qt::ItemFlags KbNodeTreeQModelWithProviderNode::flags(
    const QModelIndex &index) const {
  auto flags = KbNodeTreeQModelBase::flags(index);
  return flags | Qt::ItemIsSelectable;
}

void KbNodeTreeQModelWithProviderNode::clear() {
  if (provider_item_)
    provider_item_->clear();
}

void KbNodeTreeQModelWithProviderNode::setKbNodeProvider(
    IKbNodeProvider* kbnode_provider) {
  if (!provider_item_) {
    provider_item_ = rootItem()->appendKbNode(nullptr);
    rootItem()->markAsPopulated();
  }

  if (!provider_item_)
    return;

  auto text = U8StringToQString(kbnode_provider->name());
  provider_item_->setText(text);
  provider_item_->setKbNodeProvider(kbnode_provider);

  KbNodeTreeQModelBase::setKbNodeProvider(kbnode_provider);
}

QModelIndex KbNodeTreeQModelWithProviderNode::kbNodeToIndex(
    IKbNode* kbnode) const {
  if (kbnode == nullptr) {
    return itemToIndex(provider_item_);
  }

  return KbNodeTreeQModelBase::kbNodeToIndex(kbnode);
}
