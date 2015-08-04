// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_MOCK_ATTRIBUTE_DISPLAY_BLOCK_H_
#define INCLUDE_SNAIL_MOCK_ATTRIBUTE_DISPLAY_BLOCK_H_

#include "snail/attribute_display_block.h"

namespace snailcore {

bool operator==(const AttributeGroupDisplayBlock& a,
                const AttributeGroupDisplayBlock& b) {
  return (a.label == b.label) &&
      (a.add_command == b.add_command) &&
      (a.sub_attr_count == b.sub_attr_count) &&
      (a.view_priv_data == b.view_priv_data);
}

bool operator!=(const AttributeGroupDisplayBlock& a,
                const AttributeGroupDisplayBlock& b) {
  return !(a == b);
}

bool operator==(const AttributeDisplayBlock& a,
                const AttributeDisplayBlock& b) {
  return (a.label == b.label) &&
      (a.edit_mode == b.edit_mode) &&
      (a.attr_model == b.attr_model) &&
      (a.is_in_group == b.is_in_group) &&
      (a.view_priv_data == b.view_priv_data);
}

bool operator!=(const AttributeDisplayBlock& a,
                const AttributeDisplayBlock& b) {
  return !(a == b);
}

bool operator==(const UpdateAttrLabelData& a,
                const UpdateAttrLabelData& b) {
  return (a.label == b.label) &&
      (a.view_priv_data == b.view_priv_data);
}

void PrintTo(const AttributeDisplayBlock& attr_block, ::std::ostream* os) {
  *os << "AttrBlock: label:" << attr_block.label
      << ", edit_mode: " << attr_block.edit_mode
      << ", attr_model: " << attr_block.attr_model.get()
      << ", in_group: " << attr_block.is_in_group
      << ", priv_data: " << attr_block.view_priv_data
      << std::endl;
}

void PrintTo(const AttributeGroupDisplayBlock& group_block,
             ::std::ostream* os) {
  *os << "GroupBlock: label: " << group_block.label
      << ", add_cmd: " << group_block.add_command
      << ", sub_attr_count: " << group_block.sub_attr_count
      << ", priv_data: " << group_block.view_priv_data
      << std::endl;
}

namespace tests {

class MockAttributeDisplayBlockVisitor : public IAttributeDisplayBlockVisitor {
 public:
  MOCK_METHOD1(beginTraverse, void(int total_block_count));
  MOCK_METHOD1(visitAttributeGroupDisplayBlock,
               void*(AttributeGroupDisplayBlock attr_group_block));
  MOCK_METHOD1(visitAttributeDisplayBlock,
               void*(AttributeDisplayBlock attr_block));
  MOCK_METHOD0(endTraverse, void());
};

class NullAttributeDisplayBlockVisitor : public IAttributeDisplayBlockVisitor {
 public:
  void beginTraverse(int total_block_count) override {
    (void)total_block_count;
  }

  void* visitAttributeGroupDisplayBlock(
      AttributeGroupDisplayBlock attr_group_block) override {
    (void)attr_group_block;
    return nullptr;
  }

  void* visitAttributeDisplayBlock(
      AttributeDisplayBlock attr_block) override {
    (void)attr_block;
    return nullptr;
  }

  void endTraverse() override { }
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_SNAIL_MOCK_ATTRIBUTE_DISPLAY_BLOCK_H_
