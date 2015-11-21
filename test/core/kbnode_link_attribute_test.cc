//-*- TestCaseName: KbNodeLinkAttributeTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"

#include "src/core/kbnode_link_attribute.h"
#include "core/mock_kbnode_link_attribute.h"
#include "core/mock_kbnode_attribute.h"
#include "core/mock_link_type.h"
#include "core/mock_kbnode.h"

#include "core/mock_attribute_visitor.h"

namespace snailcore {
namespace tests {

class KbNodeLinkAttributeTest : public ::testing::Test {
 public:
  KbNodeLinkAttributeTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~KbNodeLinkAttributeTest() { }
  void SetUp() override {
    MockObjectRecorder mock_obj_recorder;

    // setup link type
    MockLinkType default_proto_link_type;
    R_EXPECT_CALL(link_attr_supplier, getDefaultProtoLinkType())
        .WillOnce(Return(&default_proto_link_type));
    link_type = new MockLinkType();
    R_EXPECT_CALL(default_proto_link_type, clone()).WillOnce(Return(link_type));

    R_EXPECT_CALL(*link_type, whenLinkUpdated(_, _))
        .WillOnce(SaveArg<0>(&linkUpdated));

    // setup value attr
    auto root_kbnode = xtestutils::genDummyPointer<IKbNode>();
    R_EXPECT_CALL(link_attr_supplier, getRootKbNode())
        .WillOnce(Return(root_kbnode));

    value_attr_supplier =
        new MockKbNodeAttributeSupplier(value_attr_supplier_name_, 1);
    R_EXPECT_CALL(value_attr_supplier_factory, createInstance(root_kbnode, 1))
        .WillOnce(Return(value_attr_supplier));

    link_attr = utils::make_unique<KbNodeLinkAttribute>(&link_attr_supplier);

    mock_obj_recorder.verify();
  }
  // void TearDown() override { }

  // region: objects test subject depends on
  MockLinkType* link_type;

  utils::U8String value_attr_supplier_name_{xtestutils::genRandomString()};
  MockKbNodeAttributeSupplier* value_attr_supplier;
  MockKbNodeAttributeSupplierFactory value_attr_supplier_factory;
  // endregion

  // region: test subject
  std::unique_ptr<KbNodeLinkAttribute> link_attr;
  MockKbNodeLinkAttributeSupplier link_attr_supplier;
  // endregion

  // region: object depends on test subject
  using LinkUpdatedSlotType = fto::LinkType::LinkUpdatedSlotType;
  SlotCatcher<LinkUpdatedSlotType> linkUpdated;
  // endregion
};

TEST_F(KbNodeLinkAttributeTest,
       should_be_able_to_get_link_attr_supplier) {  // NOLINT
  ASSERT_EQ(&link_attr_supplier, link_attr->supplier());
}

TEST_F(KbNodeLinkAttributeTest,
       should_be_able_to_get_current_link_type) {  // NOLINT
  EXPECT_EQ(link_type, link_attr->linkType());
}

TEST_F(
    KbNodeLinkAttributeTest,
    should_attribute_display_name_be_value_supplier_name_when_link_type_render_to_empty_string) {  // NOLINT
  // Setup fixture
  EXPECT_CALL(*link_type, toString()).WillOnce(Return(""));

  // Exercise system
  auto actual_name = link_attr->displayName();

  // Verify results
  ASSERT_EQ(value_attr_supplier_name_, actual_name);
}

TEST_F(
    KbNodeLinkAttributeTest,
    should_attr_display_name_be_link_type_rendered_string_when_link_type_render_to_non_empty_string) {  // NOLINT
  // Setup fixture
  auto link_name = xtestutils::genRandomString();
  EXPECT_CALL(*link_type, toString()).WillOnce(Return(link_name));

  // Exercise system
  auto actual_name = link_attr->displayName();

  // Verify results
  ASSERT_EQ(link_name, actual_name);
}

TEST_F(KbNodeLinkAttributeTest,
       should_fire_attributeChanged_event_when_link_type_updated) {  // NOLINT
  // Expectations
  AttrSupplierListener link_attr_supplier_listener(&link_attr_supplier);
  EXPECT_CALL(link_attr_supplier_listener, AttributeChanged(link_attr.get()));

  // Exercise system
  linkUpdated();
}

class ValueAttrFixture : public TestFixture {
 public:
  ValueAttrFixture(const utils::U8String& name,
                   KbNodeLinkAttributeTest* test_case)
      : TestFixture(name) {
    value_attr_ = new MockKbNodeAttribute();
    R_EXPECT_CALL(*(test_case->value_attr_supplier), createAttribute())
        .WillOnce(Return(value_attr_));
  }

  MockKbNodeAttribute* value_attr_;
};

TEST_F(KbNodeLinkAttributeTest,
       should_be_able_to_get_current_value_attr) {  // NOLINT
  // Setup fixture
  FixtureHelper(ValueAttrFixture, fixture);

  // Exercise system
  auto actual_value_attr = link_attr->valueAttr();

  // Verify results
  ASSERT_EQ(fixture.value_attr_, actual_value_attr);

  // get again will got the same attr
  auto actual_value_attr_again = link_attr->valueAttr();
  ASSERT_EQ(fixture.value_attr_, actual_value_attr_again);
}

TEST_F(KbNodeLinkAttributeTest,
       should_valueText_be_value_attr_s_valueText) {  // NOLINT
  // Setup fixture
  FixtureHelper(ValueAttrFixture, fixture);

  auto expect_value_text = xtestutils::genRandomString();
  EXPECT_CALL(*fixture.value_attr_, valueText())
      .WillOnce(Return(expect_value_text));

  // Verify results
  ASSERT_EQ(expect_value_text, link_attr->valueText());
}

TEST_F(KbNodeLinkAttributeTest,
       should_isEmpty_be_value_attr_s_isEmpty) {  // NOLINT
  // Setup fixture
  FixtureHelper(ValueAttrFixture, fixture);

  bool expect_empty = xtestutils::randomBool();
  EXPECT_CALL(*fixture.value_attr_, isEmpty()).WillOnce(Return(expect_empty));

  // Verify results
  ASSERT_EQ(expect_empty, link_attr->isEmpty());
}

TEST_F(KbNodeLinkAttributeTest,
       should_clear_clear_both_value_attr_and_link_type) {  // NOLINT
  // Setup fixture
  FixtureHelper(ValueAttrFixture, fixture);
  link_attr->valueAttr();  // init value attr

  // Expectations
  EXPECT_CALL(*fixture.value_attr_, clear());
  EXPECT_CALL(*link_type, clear());

  // Exercise system
  link_attr->clear();
}

TEST_F(KbNodeLinkAttributeTest,
       should_fire_attributeChanged_when_value_attr_changed) {  // NOLINT
  // Expectations
  AttrSupplierListener link_attr_supplier_listener(&link_attr_supplier);
  EXPECT_CALL(link_attr_supplier_listener, AttributeChanged(link_attr.get()));

  // Exercise system
  value_attr_supplier->attributeChanged(nullptr);
}

TEST(KbNodeLinkAttributeSupplierTest,
     should_get_back_components_passed_in_constructor) {  // NOLINT
  auto link_item_provider = xtestutils::genDummyPointer<ITreeItemProvider>();
  auto default_proto_link_type = xtestutils::genDummyPointer<fto::LinkType>();
  auto root_kbnode = xtestutils::genDummyPointer<IKbNode>();

  KbNodeLinkAttributeSupplier attr_supplier(
      link_item_provider, default_proto_link_type, root_kbnode, std::rand());
  ASSERT_EQ(link_item_provider, attr_supplier.getLinkTypeItemProvider());
  ASSERT_EQ(default_proto_link_type, attr_supplier.getDefaultProtoLinkType());
  ASSERT_EQ(root_kbnode, attr_supplier.getRootKbNode());
}

}  // namespace tests
}  // namespace snailcore
