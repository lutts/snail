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

bool operator==(const AttributeDisplayBlock& a,
                const AttributeDisplayBlock& b) {
  return (a.label == b.label) &&
      (a.edit_mode == b.edit_mode) &&
      (a.attr_model == b.attr_model) &&
      (a.erase_command == b.erase_command) &&
      (a.edit_command == b.edit_command) &&
      (a.is_in_group == b.is_in_group) &&
      (a.view_priv_data == b.view_priv_data);
}

namespace tests {

class MockAttributeDisplayBlockVisitor : public IAttributeDisplayBlockVisitor {
 public:
  MOCK_METHOD1(beginAddAttributeDisplayBlock, void(int total_block_count));
  MOCK_METHOD1(addAttributeGroupDisplayBlock,
               void*(AttributeGroupDisplayBlock attr_group_block));
  MOCK_METHOD1(addAttributeDisplayBlock,
               void*(AttributeDisplayBlock attr_block));
  MOCK_METHOD0(endAddAttributeDisplayBlock, void());
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_SNAIL_MOCK_ATTRIBUTE_DISPLAY_BLOCK_H_
