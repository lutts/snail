// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#include "test/testutils/gmock_common.h"

#include "src/core/kbnode_attribute_supplier.h"
//#include "core/kbnode_attribute.h"
#include "snail/mock_kbnode.h"

namespace snailcore {
namespace tests {

class KbNodeAttributeSupplierTest : public ::testing::Test {
 protected:
  KbNodeAttributeSupplierTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~KbNodeAttributeSupplierTest() { }
  virtual void SetUp() {
    expect_max_attrs = std::rand();

    EXPECT_CALL(root_kbnode, name())
        .WillRepeatedly(Return(root_kbnode_name));

    attr_supplier = utils::make_unique<KbNodeAttributeSupplier>(
        &root_kbnode, expect_max_attrs);

    ASSERT_EQ(root_kbnode_name, attr_supplier->name());
    ASSERT_EQ(expect_max_attrs, attr_supplier->max_attrs());
    ASSERT_EQ(&root_kbnode, attr_supplier->getRootKbNode());
  }
  // virtual void TearDown() { }

  // region: objects test subject depends on
  MockKbNode root_kbnode;
  utils::U8String root_kbnode_name;
  int expect_max_attrs;
  // endregion

  // region: test subject
  std::unique_ptr<KbNodeAttributeSupplier> attr_supplier;
  // endregion
};

TEST_F(KbNodeAttributeSupplierTest,
       should_createAttribute_create_KbNodeAttribute_instance_with_this_as_the_supplier) { // NOLINT
#if 0
  // Exercise system
  auto new_attr = attr_supplier->addAttribute();

  // Verify results
  auto actual_attr = dynamic_cast<KbNodeAttribute*>(new_attr);
  ASSERT_NE(nullptr, actual_attr);

  ASSERT_EQ(attr_supplier.get(), actual_attr->supplier());
#endif
}

}  // namespace tests
}  // namespace snailcore
