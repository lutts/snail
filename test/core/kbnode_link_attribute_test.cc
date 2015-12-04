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

#include "test/core/generic_attribute_supplier_test.h"

namespace snailcore {
namespace tests {

class LinkAttrTestSharedState {
 public:
  LinkAttrTestSharedState() { sInstance = this; }
  ~LinkAttrTestSharedState() { sInstance = nullptr; }

  static LinkAttrTestSharedState& getInstance() { return *sInstance; }

 public:
  MockLinkType default_proto_link_type_;
  MockKbNodeAttributeSupplierFactory value_attr_supplier_factory_;
  MockKbNodeLinkAttributeSupplier link_attr_supplier_;

  static LinkAttrTestSharedState* sInstance;
};

LinkAttrTestSharedState* LinkAttrTestSharedState::sInstance = nullptr;

MockLinkType& defaultProtoLinkType() {
  return LinkAttrTestSharedState::getInstance().default_proto_link_type_;
}

MockKbNodeAttributeSupplierFactory& valueAttrSupplierFactory() {
  return LinkAttrTestSharedState::getInstance().value_attr_supplier_factory_;
}

MockKbNodeLinkAttributeSupplier& linkAttrSupplier() {
  return LinkAttrTestSharedState::getInstance().link_attr_supplier_;
}

class LinkAttrSupplierTestHelper : public xtestutils::TestFixture {
 public:
  LinkAttrSupplierTestHelper()
      : link_type_{new MockLinkType()},
        value_attr_supplier_{new MockKbNodeAttributeSupplier} {
    // setup link type
    R_EXPECT_CALL(linkAttrSupplier(), getDefaultProtoLinkType())
        .WillOnce(Return(&defaultProtoLinkType()));

    R_EXPECT_CALL(defaultProtoLinkType(), clone()).WillOnce(Return(link_type_));

    R_EXPECT_CALL(*link_type_, whenLinkUpdated(_, _))
        .WillOnce(DoAll(SaveArg<0>(&linkUpdated_), Return(SignalConnection())));

    // setup value attr
    auto root_kbnode = xtestutils::genDummyPointer<IKbNode>();
    R_EXPECT_CALL(linkAttrSupplier(), getRootKbNode())
        .WillOnce(Return(root_kbnode));

    ;
    R_EXPECT_CALL(valueAttrSupplierFactory(), createInstance(root_kbnode, 1))
        .WillOnce(Return(value_attr_supplier_));

    R_EXPECT_CALL(*value_attr_supplier_, whenAttributeChanged(_, _))
        .WillOnce(
            DoAll(SaveArg<0>(&valueAttrChanged_), Return(SignalConnection())));
  }

  LinkAttrSupplierTestHelper(const LinkAttrSupplierTestHelper& rhs)
      : xtestutils::TestFixture{rhs},
        link_type_{new MockLinkType()},
        value_attr_supplier_{new MockKbNodeAttributeSupplier} {
    R_EXPECT_CALL(*rhs.link_type_, clone()).WillOnce(Return(link_type_));
    R_EXPECT_CALL(*rhs.value_attr_supplier_, clone())
        .WillOnce(Return(value_attr_supplier_));

    R_EXPECT_CALL(*link_type_, whenLinkUpdated(_, _))
        .WillOnce(DoAll(SaveArg<0>(&linkUpdated_), Return(SignalConnection())));

    R_EXPECT_CALL(*value_attr_supplier_, whenAttributeChanged(_, _))
        .WillOnce(
            DoAll(SaveArg<0>(&valueAttrChanged_), Return(SignalConnection())));
  }

  MockLinkType& linkType() { return *link_type_; }
  MockKbNodeAttributeSupplier& valueAttrSupplier() {
    return *value_attr_supplier_;
  }

  void updateLinkType() { linkUpdated_(); }

  void changeValueAttr() { valueAttrChanged_(nullptr); }

 private:
  MockLinkType* link_type_;
  MockKbNodeAttributeSupplier* value_attr_supplier_;

  // slot catchers (not copyied, not moved)
  using LinkUpdatedSlotType = fto::LinkType::LinkUpdatedSlotType;
  xtestutils::SlotCatcher<LinkUpdatedSlotType> linkUpdated_;

  using ValueAttrChangedSlotType = IAttributeSupplier::AttributeChangedSlotType;
  xtestutils::SlotCatcher<ValueAttrChangedSlotType> valueAttrChanged_;
};

class KbNodeLinkAttrFixture : public xtestutils::TestFixture {
 public:
  KbNodeLinkAttrFixture()
      : xtestutils::TestFixture{}, state_{}, link_attr_{&linkAttrSupplier()} {}

  KbNodeLinkAttrFixture(const KbNodeLinkAttrFixture& rhs)
      : xtestutils::TestFixture{rhs},
        state_{rhs.state_},
        link_attr_{rhs.link_attr_} {}

  KbNodeLinkAttribute* linkAttr() { return &link_attr_; }
  LinkAttrSupplierTestHelper* state() { return &state_; }

  void checkSetup() {
    verify();
    state_.verify();
  }

 private:
  LinkAttrSupplierTestHelper state_;
  KbNodeLinkAttribute link_attr_;
};

class KbNodeLinkAttrFixtureFactory {
 public:
  static KbNodeLinkAttrFixture* createFixture() {
    return new KbNodeLinkAttrFixture();
  }
};

using FixtureHelperGenerator =
    xtestutils::CopyMoveFixtureHelperGenerator<KbNodeLinkAttrFixture,
                                               KbNodeLinkAttrFixtureFactory>;

using KbNodeLinkAttrFixtureHelper =
    xtestutils::TestFixtureHelper<KbNodeLinkAttrFixture>;

class KbNodeLinkAttributeTest : public xtestutils::ErrorVerbosityTestWithParam<
                                    KbNodeLinkAttrFixtureHelper*> {
 public:
  KbNodeLinkAttributeTest()
      : ErrorVerbosityTestWithParam{},
        fixture_{this},
        link_attr{fixture_->linkAttr()} {}
  // ~KbNodeLinkAttributeTest() { }
  // void SetUp() override {}
  // void TearDown() override { }

  MockLinkType& linkType() { return fixture_->state()->linkType(); }

  MockKbNodeAttributeSupplier& valueAttrSupplier() {
    return fixture_->state()->valueAttrSupplier();
  }

  void updateLinkType() { fixture_->state()->updateLinkType(); }
  void changeValueAttr() { fixture_->state()->changeValueAttr(); }

 private:
  LinkAttrTestSharedState shared_state;
  xtestutils::FixtureLoaderFromHelper<KbNodeLinkAttrFixture,
                                      KbNodeLinkAttributeTest> fixture_;

 protected:
  KbNodeLinkAttribute* link_attr;
};

auto link_attr_test_fixture_helpers =
    FixtureHelperGenerator::fixtureHelpers<KbNodeLinkAttrFixtureFactory>(
        TEST_ENABLE_COPY_CONSTRUCT_TEST);
INSTANTIATE_TEST_CASE_P(FixtureSetup, KbNodeLinkAttributeTest,
                        ::testing::ValuesIn((link_attr_test_fixture_helpers)));

TEST_P(KbNodeLinkAttributeTest,
       should_be_able_to_get_link_attr_supplier) {  // NOLINT
  ASSERT_EQ(&linkAttrSupplier(), link_attr->supplier());
}

TEST_P(KbNodeLinkAttributeTest,
       should_be_able_to_get_current_link_type) {  // NOLINT
  EXPECT_EQ(&linkType(), link_attr->linkType());
}

TEST_P(
    KbNodeLinkAttributeTest,
    should_attribute_display_name_be_value_supplier_name_when_link_type_render_to_empty_string) {  // NOLINT
  // Setup fixture
  EXPECT_CALL(linkType(), toString()).WillOnce(Return(""));

  utils::U8String value_attr_supplier_name = xtestutils::genRandomString();
  EXPECT_CALL(valueAttrSupplier(), name())
      .WillOnce(Return(value_attr_supplier_name));

  // Exercise system
  auto actual_name = link_attr->displayName();

  // Verify results
  ASSERT_EQ(value_attr_supplier_name, actual_name);
}

TEST_P(
    KbNodeLinkAttributeTest,
    should_attr_display_name_be_link_type_rendered_string_when_link_type_render_to_non_empty_string) {  // NOLINT
  // Setup fixture
  auto link_name = xtestutils::genRandomString();
  EXPECT_CALL(linkType(), toString()).WillOnce(Return(link_name));

  // Exercise system
  auto actual_name = link_attr->displayName();

  // Verify results
  ASSERT_EQ(link_name, actual_name);
}

TEST_P(KbNodeLinkAttributeTest,
       should_fire_attributeChanged_event_when_link_type_updated) {  // NOLINT
  // Expectations
  EXPECT_CALL(linkAttrSupplier(), attributeChanged(link_attr));

  // Exercise system
  updateLinkType();
}

class ValueAttrFixture : public xtestutils::TestFixture {
 public:
  ValueAttrFixture(const utils::U8String& name,
                   KbNodeLinkAttributeTest* test_case)
      : xtestutils::TestFixture(name) {
    R_EXPECT_CALL(test_case->valueAttrSupplier(), addAttribute())
        .WillOnce(Return(&value_attr_));
  }

  MockKbNodeAttribute value_attr_;
};

TEST_P(KbNodeLinkAttributeTest,
       should_be_able_to_get_current_value_attr) {  // NOLINT
  // Setup fixture
  FixtureHelper(ValueAttrFixture, fixture);

  // Exercise system
  auto actual_value_attr = link_attr->valueAttr();

  // Verify results
  ASSERT_EQ(&fixture.value_attr_, actual_value_attr);

  // get again will got the same attr
  auto actual_value_attr_again = link_attr->valueAttr();
  ASSERT_EQ(&fixture.value_attr_, actual_value_attr_again);
}

TEST_P(KbNodeLinkAttributeTest,
       should_valueText_be_value_attr_s_valueText) {  // NOLINT
  // Setup fixture
  FixtureHelper(ValueAttrFixture, fixture);

  auto expect_value_text = xtestutils::genRandomString();
  EXPECT_CALL(fixture.value_attr_, valueText())
      .WillOnce(Return(expect_value_text));

  // Verify results
  ASSERT_EQ(expect_value_text, link_attr->valueText());
}

TEST_P(KbNodeLinkAttributeTest,
       should_isEmpty_be_value_attr_s_isEmpty) {  // NOLINT
  // Setup fixture
  FixtureHelper(ValueAttrFixture, fixture);

  bool expect_empty = xtestutils::randomBool();
  EXPECT_CALL(fixture.value_attr_, isEmpty()).WillOnce(Return(expect_empty));

  // Verify results
  ASSERT_EQ(expect_empty, link_attr->isEmpty());
}

TEST_P(KbNodeLinkAttributeTest,
       should_clear_clear_both_value_attr_and_link_type) {  // NOLINT
  // Setup fixture
  FixtureHelper(ValueAttrFixture, fixture);
  link_attr->valueAttr();  // init value attr

  // Expectations
  EXPECT_CALL(fixture.value_attr_, clear());
  EXPECT_CALL(linkType(), clear());

  // Exercise system
  link_attr->clear();
}

TEST_P(KbNodeLinkAttributeTest,
       should_fire_attributeChanged_when_value_attr_changed) {  // NOLINT
  // Expectations
  EXPECT_CALL(linkAttrSupplier(), attributeChanged(link_attr));

  // Exercise system
  changeValueAttr();
}

////////////////////////////////////////////////

class MockKbNodeLinkAttrFactory : public KbNodeLinkAttributeFactory {
 public:
  MOCK_CONST_METHOD0(createAttribute, fto::KbNodeLinkAttribute*());
};

using KbNodeLinkAttributeFactoryFixture =
    AttributeFactoryFixtureTemplate<MockKbNodeLinkAttrFactory,
                                    MockKbNodeLinkAttribute>;

class KbNodeLinkAttrSupplierFixture : public GenericAttributeSupplierFixture {
 public:
  KbNodeLinkAttrSupplierFixture()
      : GenericAttributeSupplierFixture{},
        link_item_provider_{xtestutils::genDummyPointer<ITreeItemProvider>()},
        default_proto_link_type_{xtestutils::genDummyPointer<fto::LinkType>()},
        root_kbnode_{xtestutils::genDummyPointer<IKbNode>()},
        attr_supplier_{link_item_provider_, default_proto_link_type_,
                       root_kbnode_, max_attrs()} {}

  IAttributeSupplier* getAttributeSupplier() override {
    return &attr_supplier_;
  }
  GenericAttributeFactoryFixture* getAttributeFactory() override {
    return &attr_factory_fixture_;
  }

  // region: checkers
  void checkSetup() {
    verify();
    validateState();
  }

  void validateState() override {
    GenericAttributeSupplierFixture::validateState();

    ASSERT_EQ(link_item_provider_, attr_supplier_.getLinkTypeItemProvider());
    ASSERT_EQ(default_proto_link_type_,
              attr_supplier_.getDefaultProtoLinkType());
    ASSERT_EQ(root_kbnode_, attr_supplier_.getRootKbNode());
  }
  // endregion: checkers

  void enableMockAttrFactory() {
    attr_supplier_.setAttributeFactory(attr_factory_fixture_.attr_factory());
  }

 private:
  SNAIL_DISABLE_COPY(KbNodeLinkAttrSupplierFixture);

  ITreeItemProvider* link_item_provider_;
  fto::LinkType* default_proto_link_type_;
  IKbNode* root_kbnode_;

  KbNodeLinkAttributeSupplier attr_supplier_;

  KbNodeLinkAttributeFactoryFixture attr_factory_fixture_;
};

class KbNodeLinkAttrSupplierWithMockAttrFactoryFixtureFactory {
 public:
  static KbNodeLinkAttrSupplierFixture* createFixture() {
    auto fixture = new KbNodeLinkAttrSupplierFixture();
    fixture->enableMockAttrFactory();
    return fixture;
  }
};

class KbNodeLinkAttrSupplierFilledWithAttrsFixtureFactory {
 public:
  static KbNodeLinkAttrSupplierFixture* createFixture() {
    auto fixture = new KbNodeLinkAttrSupplierFixture();
    fixture->enableMockAttrFactory();
    fixture->fillAttributes();
    return fixture;
  }
};

using WithMockAttrFactoryFixtureCopyMoveHelper =
    xtestutils::CopyMoveFixtureHelper<
        GenericAttributeSupplierFixture,
        KbNodeLinkAttrSupplierWithMockAttrFactoryFixtureFactory>;
static WithMockAttrFactoryFixtureCopyMoveHelper with_factory_fixture_helper;

INSTANTIATE_TEST_CASE_P(
    FixtureSetup, GenericAttributeSupplierWithMockAttrFactoryTest,
    ::testing::Values((GenericAttributeSupplierFixtureFactory*)(
        &with_factory_fixture_helper)));

using FilledWithAttrsFixtureCopyMoveHelper = xtestutils::CopyMoveFixtureHelper<
    GenericAttributeSupplierFixture,
    KbNodeLinkAttrSupplierFilledWithAttrsFixtureFactory>;
static FilledWithAttrsFixtureCopyMoveHelper
    filled_with_attributes_fixture_helper;
INSTANTIATE_TEST_CASE_P(
    FixtureSetup, GenericAttributeSupplierFilledWithAttrsTest,
    ::testing::Values((GenericAttributeSupplierFixtureFactory*)(
        &filled_with_attributes_fixture_helper)));

}  // namespace tests
}  // namespace snailcore
