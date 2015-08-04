// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/work_attribute_model.h"

#include <vector>
#include <iostream>

#include "utils/signal_slot_impl.h"
#include "utils/i_relay_command_factory.h"
#include "snail/attribute_display_block.h"
#include "snail/i_attribute_model_factory.h"
#include "core/i_attribute_container.h"
#include "core/i_attribute.h"
#include "snail/i_attribute_model.h"
#include "core/i_attribute_supplier.h"

using namespace utils;  // NOLINT
using namespace pfmvp;  // NOLINT

namespace snailcore {

class WorkAttributeModelImpl {
 public:
  WorkAttributeModelImpl(
      const std::vector<IAttributeSupplier*>& attr_supplier_list,
      const IAttributeModelFactory& attr_model_factory,
      const IRelayCommandFactory& cmd_factory)
      : attr_supplier_list_(attr_supplier_list)
      , attr_model_factory_(attr_model_factory)
      , cmd_factory_(cmd_factory) { }
  virtual ~WorkAttributeModelImpl() = default;

  bool isEditMode() const {
    return edit_mode_;
  }
  void switchToEditMode() {
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

  void traverseAttributes(WorkAttributeModel* model,
                          IAttributeDisplayBlockVisitor* visitor);
  int attrDisplayCount(IAttributeSupplier* supplier);
  bool shouldGenerateGroupBlock(IAttributeSupplier* supplier);
  void clearEmptyAttributes(WorkAttributeModel* model);
  void updateAttrLabel(IAttribute* attr);

 private:
  std::vector<IAttributeSupplier*> attr_supplier_list_;
  const IAttributeModelFactory& attr_model_factory_;
  const utils::IRelayCommandFactory& cmd_factory_;

  std::map<IAttribute*, AttributeDisplayBlock> attr_block_cache_;

  struct GroupBlockCacheItem {
    AttributeGroupDisplayBlock group_block;
    std::shared_ptr<Command> add_command;
  };
  std::map<IAttributeSupplier*, GroupBlockCacheItem> group_block_cache_;

  bool edit_mode_ { false };
  bool clear_empty_attrs_ { false };

 private:
  SNAIL_SIGSLOT_PIMPL(WorkAttributeModel, AttributesChanged);
  SNAIL_SIGSLOT_PIMPL(WorkAttributeModel, AttrLabelChanged);

 private:
  SNAIL_DISABLE_COPY(WorkAttributeModelImpl)

  friend class WorkAttributeModel;
};

SNAIL_SIGSLOT_PIMPL_RELAY(WorkAttributeModel, AttributesChanged, impl);
SNAIL_SIGSLOT_PIMPL_RELAY(WorkAttributeModel, AttrLabelChanged, impl);

void WorkAttributeModelImpl::traverseAttributes(
    WorkAttributeModel* model, IAttributeDisplayBlockVisitor* visitor) {
  int total_attrs = 0;
  for (auto & supplier : attr_supplier_list_) {
    int display_count = attrDisplayCount(supplier);
    total_attrs += display_count;

    if (shouldGenerateGroupBlock(supplier)) {
      ++total_attrs;
    } else {
      if (edit_mode_ && (supplier->attr_count() == 0)) {
        ++total_attrs;
      }
    }
  }

  visitor->beginTraverse(total_attrs);

  for (auto & supplier : attr_supplier_list_) {
    if (shouldGenerateGroupBlock(supplier)) {
      AttributeGroupDisplayBlock group_block;
      auto iter = group_block_cache_.find(supplier);
      if (iter != group_block_cache_.end()) {
        group_block = iter->second.group_block;

        if (edit_mode_) {
          group_block.add_command = iter->second.add_command.get();
        } else {
          group_block.add_command = nullptr;
        }
      } else {
        group_block.label = supplier->name();

        GroupBlockCacheItem group_cache_item;
        if (edit_mode_) {
          auto add_cmd = cmd_factory_.createCommand([this, supplier](){
              supplier->addAttribute();
              AttributesChanged();
            });
          // TODO(lutts): may be we can use unique_ptr in GroupBlock instead of raw pointer
          //              to avoid cache add command, and generate fresh cmd on every traverse
          group_block.add_command = add_cmd.get();

          group_cache_item.add_command = add_cmd;
        }

        group_block_cache_[supplier] = group_cache_item;
      }

      group_block.sub_attr_count = attrDisplayCount(supplier);

      auto priv_data = visitor->visitAttributeGroupDisplayBlock(group_block);

      group_block.view_priv_data = priv_data;

      auto & group_cache_item = group_block_cache_[supplier];
      group_cache_item.group_block = group_block;
    }  else if (edit_mode_) {  // no group block and is edit_mode
      if (supplier->attr_count() == 0) {
        supplier->addAttribute();
      }
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

  visitor->endTraverse(clear_empty_attrs_);

  clearEmptyAttributes(model);
}

int WorkAttributeModelImpl::attrDisplayCount(IAttributeSupplier* supplier) {
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

bool WorkAttributeModelImpl::shouldGenerateGroupBlock(
    IAttributeSupplier* supplier) {
  return (supplier->max_attrs() > 1) &&
      (edit_mode_ || (attrDisplayCount(supplier) != 0));
}

void WorkAttributeModelImpl::clearEmptyAttributes(WorkAttributeModel* model) {
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

void WorkAttributeModelImpl::updateAttrLabel(IAttribute* attr) {
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

WorkAttributeModel::WorkAttributeModel(
    const std::vector<IAttributeSupplier*>& attr_supplier_list,
    const IAttributeModelFactory& attr_model_factory,
    const IRelayCommandFactory& cmd_factory)
    : impl(utils::make_unique<WorkAttributeModelImpl>(
          attr_supplier_list, attr_model_factory, cmd_factory)) { }

WorkAttributeModel::~WorkAttributeModel() = default;

bool WorkAttributeModel::isEditMode() const {
  return impl->isEditMode();
}

void WorkAttributeModel::switchToEditMode() {
  return impl->switchToEditMode();
}

void WorkAttributeModel::switchToDisplayMode() {
  return impl->switchToDisplayMode();
}

void WorkAttributeModel::traverseAttributes(
    IAttributeDisplayBlockVisitor* visitor) {
  impl->traverseAttributes(this, visitor);
}

}  // namespace snailcore
