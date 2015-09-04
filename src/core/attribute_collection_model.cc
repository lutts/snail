// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/attribute_collection_model.h"

#include <vector>
#include <map>
#include <algorithm>

#include "utils/signal_slot_impl.h"
#include "utils/command.h"
#include "snail/attribute_display_block.h"
#include "snail/i_attribute_model_factory.h"
#include "core/i_attribute_container.h"
#include "core/i_attribute.h"
#include "snail/i_attribute_model.h"
#include "core/i_attribute_supplier.h"

using namespace utils;  // NOLINT
using namespace pfmvp;  // NOLINT

namespace snailcore {

class AttributeCollectionModelImpl {
 public:
  AttributeCollectionModelImpl(
      const std::vector<IAttributeSupplier*>& attr_supplier_list,
      const IAttributeModelFactory& attr_model_factory)
      : attr_supplier_list_(attr_supplier_list)
      , attr_model_factory_(attr_model_factory) { }
  virtual ~AttributeCollectionModelImpl() = default;

  bool isEditMode() const {
    return edit_mode_;
  }
  void switchToEditMode() {
    if (edit_mode_)
      return;

    edit_mode_ = true;
    AttributesChanged();
  }

  void switchToDisplayMode() {
    if (!edit_mode_)
      return;

    clear_empty_attrs_ = true;
    edit_mode_ = false;
    AttributesChanged();
  }

  void traverseAttributes(AttributeCollectionModel* model,
                          IAttributeDisplayBlockVisitor* visitor);
  int attrDisplayCount(IAttributeSupplier* supplier);

  bool shouldGenerateGroupBlock(IAttributeSupplier* supplier) {
    return (supplier->max_attrs() > 1) &&
        (edit_mode_ || (attrDisplayCount(supplier) != 0));
  }

  bool shouldAddAnEmptyAttribute(IAttributeSupplier* supplier) {
    return edit_mode_ &&
        (supplier->max_attrs() == 1) &&
        (supplier->attr_count() == 0);
  }

  bool shouldSetAddCommand(IAttributeSupplier* supplier) {
    return edit_mode_ && (supplier->attr_count() < supplier->max_attrs());
  }
  void clearEmptyAttributes(AttributeCollectionModel* model);
  void updateAttrLabel(IAttribute* attr);

 private:
  class AddAttributeCommand : public Command {
   public:
    virtual ~AddAttributeCommand() = default;

    void redo() override {
      supplier_->addAttribute();
      model_->AttributesChanged();
    }

    void set_model(AttributeCollectionModelImpl* model) { model_ = model;}
    void set_supplier(IAttributeSupplier* supplier) { supplier_ = supplier; }

   private:
    AttributeCollectionModelImpl* model_;
    IAttributeSupplier* supplier_;
  };

  struct GroupBlockCacheItem {
    AttributeGroupDisplayBlock group_block;
    AddAttributeCommand add_command;
  };

  void mayAddGroupBlockCacheFor(IAttributeSupplier* supplier);
  void mayVisitGroupBlock(IAttributeSupplier* supplier,
                          IAttributeDisplayBlockVisitor* visitor);
  void visitAttributes(IAttributeSupplier* supplier,
                          IAttributeDisplayBlockVisitor* visitor);

  std::vector<IAttributeSupplier*> attr_supplier_list_;
  const IAttributeModelFactory& attr_model_factory_;

  std::map<IAttributeSupplier*, GroupBlockCacheItem> group_block_cache_;
  std::map<IAttribute*, AttributeDisplayBlock> attr_block_cache_;

  bool edit_mode_ { false };
  bool clear_empty_attrs_ { false };

 private:
  SNAIL_SIGSLOT_PIMPL(AttributeCollectionModel, AttributesChanged);
  SNAIL_SIGSLOT_PIMPL(AttributeCollectionModel, AttrLabelChanged);

 private:
  SNAIL_DISABLE_COPY(AttributeCollectionModelImpl)

  friend class AttributeCollectionModel;
};

SNAIL_SIGSLOT_PIMPL_RELAY(AttributeCollectionModel, AttributesChanged, impl);
SNAIL_SIGSLOT_PIMPL_RELAY(AttributeCollectionModel, AttrLabelChanged, impl);

void AttributeCollectionModelImpl::mayVisitGroupBlock(
    IAttributeSupplier* supplier, IAttributeDisplayBlockVisitor* visitor) {
  if (!shouldGenerateGroupBlock(supplier))
    return;

  mayAddGroupBlockCacheFor(supplier);

  try {
    auto & group_cache_item = group_block_cache_.at(supplier);
    AttributeGroupDisplayBlock group_block = group_cache_item.group_block;

    if (shouldSetAddCommand(supplier)) {
      group_block.add_command = &group_cache_item.add_command;
    } else {
      group_block.add_command = nullptr;
    }

    group_block.sub_attr_count = attrDisplayCount(supplier);

    auto priv_data = visitor->visitAttributeGroupDisplayBlock(group_block);

    group_block.view_priv_data = priv_data;

    group_cache_item.group_block = group_block;
  } catch (...) { }
}

void AttributeCollectionModelImpl::visitAttributes(
    IAttributeSupplier* supplier, IAttributeDisplayBlockVisitor* visitor) {
  if (shouldAddAnEmptyAttribute(supplier)) {
    supplier->addAttribute();
  }

  auto attrs = supplier->attributes();
  if (!edit_mode_) {
    std::sort(begin(attrs), end(attrs),
              [](const IAttribute* a, const IAttribute* b) -> bool {
                return a->displayName() < b->displayName();
              });
  }

  for (auto & attr : attrs) {
    if (!edit_mode_ && attr->isEmpty())
      continue;

    AttributeDisplayBlock attr_block;

    auto iter = attr_block_cache_.find(attr);
    if (iter != attr_block_cache_.end()) {
      attr_block = iter->second;
    } else {
      auto attr_model = attr_model_factory_.createModel(attr);
      attr_block.attr_model = attr_model;
      attr_block.is_in_group = (supplier->max_attrs() > 1);

      if (attr_model) {
        attr_model->whenDisplayNameChanged(
            [this, attr]() {
              updateAttrLabel(attr);
            });
      }
    }

    attr_block.label = attr->displayName();
    attr_block.edit_mode = edit_mode_;

    auto priv_data = visitor->visitAttributeDisplayBlock(attr_block);

    attr_block.view_priv_data = priv_data;

    attr_block_cache_[attr] = attr_block;
  }
}

void AttributeCollectionModelImpl::traverseAttributes(
    AttributeCollectionModel* model, IAttributeDisplayBlockVisitor* visitor) {
  int total_attrs = 0;

  // 1st pass: determine total attributes
  for (auto & supplier : attr_supplier_list_) {
    int display_count = attrDisplayCount(supplier);
    total_attrs += display_count;

    if (shouldGenerateGroupBlock(supplier)) {
      ++total_attrs;
    } else if (shouldAddAnEmptyAttribute(supplier)) {
      ++total_attrs;
    }
  }

  // 2nd pass: real traverse
  visitor->beginTraverse(total_attrs);

  for (auto & supplier : attr_supplier_list_) {
    mayVisitGroupBlock(supplier, visitor);
    visitAttributes(supplier, visitor);
  }

  visitor->endTraverse(clear_empty_attrs_);

  clearEmptyAttributes(model);
}

int
AttributeCollectionModelImpl::attrDisplayCount(IAttributeSupplier* supplier) {
  if (edit_mode_)
    return supplier->attr_count();

  int count = 0;
  auto attrs = supplier->attributes();
  for (auto & attr : attrs) {
    if (!attr->isEmpty())
      ++count;
  }

  return count;
}

void AttributeCollectionModelImpl::mayAddGroupBlockCacheFor(
    IAttributeSupplier* supplier) {
  auto iter = group_block_cache_.find(supplier);
  if (iter == group_block_cache_.end()) {
    GroupBlockCacheItem group_cache_item;
    group_cache_item.group_block.label = supplier->name();
    group_cache_item.add_command.set_model(this);
    group_cache_item.add_command.set_supplier(supplier);
    group_block_cache_[supplier] = group_cache_item;
  }
}

void AttributeCollectionModelImpl::clearEmptyAttributes(
    AttributeCollectionModel* model) {
  if (!clear_empty_attrs_)
    return;

  for (auto & supplier : attr_supplier_list_) {
    auto attrs = supplier->attributes();
    for (auto & attr : attrs) {
      if (attr->isEmpty()) {
        // remove triad first
        IPfModel* attr_model = nullptr;
        auto iter = attr_block_cache_.find(attr);
        if (iter != attr_block_cache_.end()) {
          attr_model = iter->second.attr_model.get();
          attr_block_cache_.erase(attr);
        }

        if (attr_model)
          model->removeTriadBy(attr_model);

        // then remove from supplier
        supplier->removeAttribute(attr);
      }
    }
  }

  clear_empty_attrs_ = false;
}

void AttributeCollectionModelImpl::updateAttrLabel(IAttribute* attr) {
  auto iter = attr_block_cache_.find(attr);
  if (iter == attr_block_cache_.end())
    return;

  if (iter->second.view_priv_data == nullptr)
    return;

  UpdateAttrLabelData update_data;

  update_data.label = attr->displayName();
  update_data.view_priv_data = iter->second.view_priv_data;

  AttrLabelChanged(update_data);
}

AttributeCollectionModel::AttributeCollectionModel(
    const std::vector<IAttributeSupplier*>& attr_supplier_list,
    const IAttributeModelFactory& attr_model_factory)
    : impl(utils::make_unique<AttributeCollectionModelImpl>(
          attr_supplier_list, attr_model_factory)) { }

AttributeCollectionModel::~AttributeCollectionModel() = default;

bool AttributeCollectionModel::isEditMode() const {
  return impl->isEditMode();
}

void AttributeCollectionModel::switchToEditMode() {
  return impl->switchToEditMode();
}

void AttributeCollectionModel::switchToDisplayMode() {
  return impl->switchToDisplayMode();
}

void AttributeCollectionModel::traverseAttributes(
    IAttributeDisplayBlockVisitor* visitor) {
  impl->traverseAttributes(this, visitor);
}

}  // namespace snailcore
