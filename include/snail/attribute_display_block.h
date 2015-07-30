// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_ATTRIBUTE_DISPLAY_BLOCK_H_
#define INCLUDE_SNAIL_ATTRIBUTE_DISPLAY_BLOCK_H_

#include <memory>

#include "utils/u8string.h"

namespace utils {
class Command;
}  // namespace utils

namespace snailcore {

class IAttributeModel;

struct AttributeGroupDisplayBlock {
  AttributeGroupDisplayBlock()
      : add_command{nullptr}
      , sub_attr_count(0)
      , view_priv_data{nullptr} { }

  utils::U8String label;
  utils::Command* add_command;
  int sub_attr_count;
  void* view_priv_data{ nullptr };
};

struct AttributeDisplayBlock {
  AttributeDisplayBlock()
      : edit_mode{false}
      , erase_command {nullptr}
      , edit_command { nullptr }
      , is_in_group(false)
      , view_priv_data { nullptr } { }

  utils::U8String label;
  bool edit_mode;
  std::shared_ptr<IAttributeModel> attr_model;
  utils::Command* erase_command;
  utils::Command* edit_command;
  bool is_in_group;
  void* view_priv_data;
};

class IAttributeDisplayBlockVisitor {
 public:
  virtual ~IAttributeDisplayBlockVisitor() = default;

  virtual void beginTraverse(int total_block_count) = 0;
  virtual void* visitAttributeGroupDisplayBlock(
      AttributeGroupDisplayBlock attr_group_block) = 0;
  virtual void* visitAttributeDisplayBlock(
      AttributeDisplayBlock attr_block) = 0;
  virtual void endTraverse() = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_ATTRIBUTE_DISPLAY_BLOCK_H_
