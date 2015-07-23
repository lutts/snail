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
namespace tests {

class MockAttributeDisplayBlockVisitor : public IAttributeDisplayBlockVisitor {
 public:
  MOCK_METHOD1(beginAddAttributeDisplayBlock, void(int total_block_count));
  MOCK_METHOD1(addAttributeGroupDisplayBlock,
               void(AttributeGroupDisplayBlock* attr_group_block));
  MOCK_METHOD1(addAttributeDisplayBlock,
               void(AttributeDisplayBlock* attr_block));
  MOCK_METHOD0(endAddAttributeDisplayBlock, void());
};

}  // namespace tests
}  // namespace snailcore

#endif  // INCLUDE_SNAIL_MOCK_ATTRIBUTE_DISPLAY_BLOCK_H_
