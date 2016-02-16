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

  static LinkAttrTestSharedState* sInstance;
};

LinkAttrTestSharedState* LinkAttrTestSharedState::sInstance = nullptr;

MockLinkType& defaultProtoLinkType() {
  return LinkAttrTestSharedState::getInstance().default_proto_link_type_;
}

MockKbNodeAttributeSupplierFactory& valueAttrSupplierFactory() {
  return LinkAttrTestSharedState::getInstance().value_attr_supplier_factory_;
}

class LinkAttrTestHelper : public xtestutils::TestFixture {
 public:
  LinkAttrTestHelper() {
    setupLinkAttrSupplier();
    setupLinkType();
    setupValueAttr();
  }

  void setupLinkAttrSupplier() {
    link_attr_supplier_.reset(new MockKbNodeLinkAttributeSupplier());

    root_kbnode_ = xtestutils::genDummyPointer<IKbNode>();
    R_EXPECT_CALL(*link_attr_supplier_, getRootKbNode())
        .WillOnce(Return(root_kbnode_));

    R_EXPECT_CALL(*link_attr_supplier_, getDefaultProtoLinkType())
        .WillOnce(Return(&defaultProtoLinkType()));
  }

  void setupLinkType() {
    link_type_ = new MockLinkType();

    R_EXPECT_CALL(defaultProtoLinkType(), clone()).WillOnce(Return(link_type_));

    R_EXPECT_CALL(*link_type_, whenLinkUpdated(_, _))
        .WillOnce(DoAll(SaveArg<0>(&linkUpdated_), Return(SignalConnection())));
  }

  void setupValueAttr() {
    value_attr_supplier_ = new MockKbNodeAttributeSupplier;

    R_EXPECT_CALL(valueAttrSupplierFactory(), create(root_kbnode_, 1))
        .WillOnce(Return(value_attr_supplier_));

    R_EXPECT_CALL(*value_attr_supplier_, whenAttributeChanged(_, _))
        .WillOnce(
            DoAll(SaveArg<0>(&valueAttrChanged_), Return(SignalConnection())));
  }

  LinkAttrTestHelper(const LinkAttrTestHelper& rhs)
      : xtestutils::TestFixture{rhs},
        link_attr_supplier_{rhs.link_attr_supplier_} {
    copyConstructLinkType(rhs);
    copyConstructValueAttr(rhs);
  }

  void copyConstructLinkType(const LinkAttrTestHelper& rhs) {
    link_type_ = new MockLinkType();
    R_EXPECT_CALL(*rhs.link_type_, clone()).WillOnce(Return(link_type_));

    R_EXPECT_CALL(*link_type_, whenLinkUpdated(_, _))
        .WillOnce(DoAll(SaveArg<0>(&linkUpdated_), Return(SignalConnection())));
  }

  void copyConstructValueAttr(const LinkAttrTestHelper& rhs) {
    value_attr_supplier_ = new MockKbNodeAttributeSupplier;

    R_EXPECT_CALL(*rhs.value_attr_supplier_, clone())
        .WillOnce(Return(value_attr_supplier_));

    R_EXPECT_CALL(*value_attr_supplier_, whenAttributeChanged(_, _))
        .WillOnce(
            DoAll(SaveArg<0>(&valueAttrChanged_), Return(SignalConnection())));
  }

  void copyData(const LinkAttrTestHelper& rhs) {
    R_EXPECT_CALL(*link_type_, copy_assignment(Ref(*rhs.link_type_)));
    R_EXPECT_CALL(*value_attr_supplier_,
                  copy_assignment(Ref(*rhs.value_attr_supplier_)));
  }

  MockKbNodeLinkAttributeSupplier* link_attr_supplier() {
    return link_attr_supplier_.get();
  }
  MockLinkType& linkType() { return *link_type_; }
  MockKbNodeAttributeSupplier& valueAttrSupplier() {
    return *value_attr_supplier_;
  }

  void updateLinkType() { linkUpdated_(); }

  void changeValueAttr() { valueAttrChanged_(nullptr); }

 private:
  IKbNode* root_kbnode_{nullptr};
  std::shared_ptr<MockKbNodeLinkAttributeSupplier> link_attr_supplier_;
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
      : xtestutils::TestFixture{},
        state_{},
        link_attr_{state_.link_attr_supplier()} {}

  KbNodeLinkAttrFixture(const KbNodeLinkAttrFixture& rhs)
      : xtestutils::TestFixture{rhs},
        state_{rhs.state_},
        link_attr_{rhs.link_attr_} {}

  void copyData(const KbNodeLinkAttrFixture& rhs) {
    xtestutils::TestFixture::operator=(rhs);
    state_.copyData(rhs.state_);
    link_attr_.copyData(rhs.link_attr_);
  }

  KbNodeLinkAttribute* linkAttr() { return &link_attr_; }
  LinkAttrTestHelper* state() { return &state_; }

  void checkSetup() {
    verify();
    state_.verify();
  }

 private:
  LinkAttrTestHelper state_;
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

template <typename BaseFactory, typename TargetFactory,
          typename ConcreteFixtureType>
class CopyDataFixtureHelper
    : public FixtureHelperGenerator::GenericCustomFixtureHelper<
          CopyDataFixtureHelper, BaseFactory, TargetFactory,
          ConcreteFixtureType> {
  void doCustomOperation(ConcreteFixtureType* target_fixture,
                         ConcreteFixtureType* base_fixture) {
    target_fixture->copyData(*base_fixture);
  }
};

using KbNodeLinkAttrFixtureHelper =
    xtestutils::TestFixtureHelper<KbNodeLinkAttrFixture>;

class KbNodeLinkAttributeTest : public xtestutils::ErrorVerbosityTestWithParam<
                                    KbNodeLinkAttrFixtureHelper*> {
 public:
  KbNodeLinkAttributeTest()
      : ErrorVerbosityTestWithParam{},
        fixture_{this},
        link_attr{fixture_->linkAttr()} {}

  MockKbNodeLinkAttributeSupplier& linkAttrSupplier() {
    return *fixture_->state()->link_attr_supplier();
  }

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

auto link_attr_test_fixture_helpers = FixtureHelperGenerator::fixtureHelpers<
    KbNodeLinkAttrFixtureFactory, KbNodeLinkAttrFixture,
    FixtureHelperGenerator::CopyConstructFixtureHelper,
    CopyDataFixtureHelper>();
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
    auto& value_attr_supplier = test_case->valueAttrSupplier();

    {
      InSequence seq;

      R_EXPECT_CALL(value_attr_supplier, attr_count()).WillOnce(Return(0));
      R_EXPECT_CALL(value_attr_supplier, addAttribute())
          .WillOnce(Return(&value_attr_));
      R_EXPECT_CALL(value_attr_supplier, attr_count())
          .WillRepeatedly(Return(1));

      std::vector<IAttribute*> attrs;
      attrs.push_back(&value_attr_);
      R_EXPECT_CALL(value_attr_supplier, attributes())
          .WillRepeatedly(Return(attrs));
    }
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

  KbNodeLinkAttrSupplierFixture(const KbNodeLinkAttrSupplierFixture& rhs)
      : GenericAttributeSupplierFixture{rhs},
        link_item_provider_{rhs.link_item_provider_},
        default_proto_link_type_{rhs.default_proto_link_type_},
        root_kbnode_{rhs.root_kbnode_},
        attr_factory_fixture_{rhs.attr_factory_fixture_},
        attr_supplier_{rhs.attr_supplier_} {}

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
  ITreeItemProvider* link_item_provider_;
  fto::LinkType* default_proto_link_type_;
  IKbNode* root_kbnode_;

  KbNodeLinkAttributeFactoryFixture attr_factory_fixture_;

  KbNodeLinkAttributeSupplier attr_supplier_;
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

using GenericSupplierFixtureHelperGenerator =
    xtestutils::CopyMoveFixtureHelperGenerator<
        GenericAttributeSupplierFixture,
        KbNodeLinkAttrSupplierWithMockAttrFactoryFixtureFactory,
        KbNodeLinkAttrSupplierFilledWithAttrsFixtureFactory>;

INSTANTIATE_GENERIC_ATTR_SUPPLIER_TESTS(
    KbNodeLinkAttrSupplierFixture,
    KbNodeLinkAttrSupplierWithMockAttrFactoryFixtureFactory,
    KbNodeLinkAttrSupplierFilledWithAttrsFixtureFactory);

}  // namespace tests
}  // namespace snailcore
