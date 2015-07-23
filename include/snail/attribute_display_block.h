// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_ATTRIBUTE_DISPLAY_BLOCK_H_
#define INCLUDE_SNAIL_ATTRIBUTE_DISPLAY_BLOCK_H_

#include "utils/u8string.h"

namespace utils {
class Command;
}  // namespace utils

namespace snailcore {

class IAttributeModel;

struct AttributeGroupDisplayBlock {
  utils::U8String label;
  utils::Command* add_command;
  int sub_attr_count;
};

struct AttributeDisplayBlock {
  utils::U8String label;
  std::shared_ptr<IAttributeModel> attr_model;
  utils::Command* erase_command;
  utils::Command* edit_command;
  bool is_in_group;
};

class IAttributeDisplayBlockVisitor {
 public:
  virtual ~IAttributeDisplayBlockVisitor() = default;

  virtual void beginAddAttributeDisplayBlock(int total_block_count) = 0;
  virtual void addAttributeGroupDisplayBlock(
      AttributeGroupDisplayBlock attr_group_block) = 0;
  virtual void addAttributeDisplayBlock(
      AttributeDisplayBlock attr_block) = 0;
  virtual void endAddAttributeDisplayBlock() = 0;
};

}  // namespace snailcore

#endif  // INCLUDE_SNAIL_ATTRIBUTE_DISPLAY_BLOCK_H_
