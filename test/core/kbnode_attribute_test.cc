//-*- TestCaseName: KbNodeAttributeTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"

#include "src/core/kbnode_attribute.h"
#include "core/mock_kbnode_attribute.h"
#include "core/mock_kbnode.h"

#include "core/mock_attribute_visitor.h"

namespace snailcore {
namespace tests {

void assertAttrEmpty(const KbNodeAttribute& attr) {
  ASSERT_TRUE(attr.isEmpty());
  ASSERT_EQ("", attr.valueText());
}

class SupplierTestHelper {
 public:
  SupplierTestHelper()
      : attr_supplier_name_{xtestutils::genRandomString()},
        supplier_{std::make_shared<MockKbNodeAttributeSupplier>()} {}

  void doLateSetup() {
    EXPECT_CALL(*supplier_, name()).WillRepeatedly(Return(attr_supplier_name_));
  }

  utils::U8String attr_supplier_name_;
  std::shared_ptr<MockKbNodeAttributeSupplier> supplier_;
};

class KbNodeHelper {
 public:
  void prepareKbNode() {
    kbnode_ = std::make_shared<MockKbNode>();

    kbnode_name_ = xtestutils::genRandomString();
    EXPECT_CALL(*kbnode_, name()).WillRepeatedly(Return(kbnode_name_));
  }

  void validateAttr(const KbNodeAttribute& attr) {
    if (kbnode_) {
      ASSERT_FALSE(attr.isEmpty());
    } else {
      ASSERT_TRUE(attr.isEmpty());
    }
    ASSERT_EQ(kbnode_name_, attr.valueText());
  }

  utils::U8String kbnode_name_;
  std::shared_ptr<MockKbNode> kbnode_;
};

class KbNodeAttributeStateSet {
 public:
  SupplierTestHelper& supplierHelper() { return state_.get<0>(); }
  KbNodeHelper& kbNodeHelper() { return state_.get<1>(); }

 private:
  TextFixtureStateSet<SupplierTestHelper, KbNodeHelper> state_;
};

class KbNodeAttributeFixture : public TestFixture {
 public:
  std::unique_ptr<KbNodeAttributeStateSet> state_;

 public:
  explicit KbNodeAttributeFixture(
      std::unique_ptr<KbNodeAttributeStateSet> state)
      : TestFixture{},
        state_{std::move(state)},
        attr_{state_->supplierHelper().supplier_.get()} {
    attr_.setKbNode(state_->kbNodeHelper().kbnode_.get());
  }

  KbNodeAttributeFixture(const KbNodeAttributeFixture& rhs)
      : TestFixture{rhs},
        state_{new KbNodeAttributeStateSet{*rhs.state_}},
        attr_{rhs.attr_} {}

  KbNodeAttributeFixture(KbNodeAttributeFixture&& rhs)
      : TestFixture{std::move(rhs)},
        state_{new KbNodeAttributeStateSet{std::move(*rhs.state_)}},
        attr_{std::move(rhs.attr_)} {}

  KbNodeAttributeFixture& operator=(const KbNodeAttributeFixture& rhs) {
    TestFixture::operator=(rhs);
    *state_ = *rhs.state_;

    R_EXPECT_CALL(*state_->supplierHelper().supplier_,
                  attributeChanged(&attr_));
    attr_ = rhs.attr_;

    return *this;
  }

  KbNodeAttributeFixture& operator=(KbNodeAttributeFixture&& rhs) {
    TestFixture::operator=(std::move(rhs));
    *state_ = std::move(*rhs.state_);

    R_EXPECT_CALL(*state_->supplierHelper().supplier_,
                  attributeChanged(&attr_));
    attr_ = std::move(rhs.attr_);

    return *this;
  }

  void checkSetup() override {
    verify();
    validateAttr();
    state_->supplierHelper().doLateSetup();
  }

  void validateAttr() { state_->kbNodeHelper().validateAttr(attr_); }

  KbNodeAttribute attr_;
};

class EmptyKbNodeAttributeFixtureFactory {
 public:
  static KbNodeAttributeFixture* createFixture() {
    auto state = utils::make_unique<KbNodeAttributeStateSet>();
    auto fixture = new KbNodeAttributeFixture(std::move(state));
    fixture->setup();
    return fixture;
  }
};

class NonEmptyKbNodeAttributeFixtureFactory {
 public:
  static KbNodeAttributeFixture* createFixture() {
    auto state = utils::make_unique<KbNodeAttributeStateSet>();
    state->kbNodeHelper().prepareKbNode();
    auto fixture = new KbNodeAttributeFixture(std::move(state));
    fixture->setup();
    return fixture;
  }
};

using FixtureHelperGenerator =
    CopyMoveFixtureHelperGenerator<KbNodeAttributeFixture,
                                   EmptyKbNodeAttributeFixtureFactory,
                                   NonEmptyKbNodeAttributeFixtureFactory>;

using EmptyCopyMoveFixtureHelper =
    CopyMoveFixtureHelper<KbNodeAttributeFixture,
                          EmptyKbNodeAttributeFixtureFactory>;

class EmptyKbNodeAttributeTest
    : public ErrorVerbosityTestWithParam<EmptyCopyMoveFixtureHelper*> {
 private:
  FixtureLoaderFromHelper<KbNodeAttributeFixture, EmptyKbNodeAttributeTest>
      fixture_;

 public:
  EmptyKbNodeAttributeTest()
      : ErrorVerbosityTestWithParam{},
        fixture_{this},
        attr_supplier_name_(
            fixture_->state_->supplierHelper().attr_supplier_name_),
        supplier_(*fixture_->state_->supplierHelper().supplier_.get()),
        attr_{&fixture_->attr_} {}
  // ~EmptyKbNodeAttributeTest() { }
  // void SetUp() override {}
  // void TearDown() override { }

 protected:
  utils::U8String& attr_supplier_name_;
  MockKbNodeAttributeSupplier& supplier_;
  KbNodeAttribute* attr_;
};

auto empty_test_fixture_helpers =
    FixtureHelperGenerator::fixtureHelpers<EmptyKbNodeAttributeFixtureFactory>(
        TEST_ENABLE_COPY_CONSTRUCT_TEST | TEST_ENABLE_COPY_ASSIGNMENT_TEST |
        TEST_ENABLE_MOVE_CONSTRUCT_TEST | TEST_ENABLE_MOVE_ASSIGNMENT_TEST);
INSTANTIATE_TEST_CASE_P(FixtureSetup, EmptyKbNodeAttributeTest,
                        ::testing::ValuesIn(empty_test_fixture_helpers));

// common test
TEST_P(
    EmptyKbNodeAttributeTest,
    should_be_able_to_get_back_the_supplier_passed_in_constructor) {  // NOLINT
  ASSERT_EQ(&supplier_, attr_->supplier());
}
TEST_P(EmptyKbNodeAttributeTest,
       should_attribute_display_name_be_supplier_name) {  // NOLINT
  ASSERT_EQ(attr_supplier_name_, attr_->displayName());
}

TEST_P(EmptyKbNodeAttributeTest,
       should_attribute_visitor_visit_IKbNodeAttribute) {  // NOLINT
  // Setup fixture
  MockAttributeVisitor visitor;

  // Expectations
  EXPECT_CALL(visitor, visit(attr_));

  // Exercise system
  attr_->accept(&visitor);
}

// empty ---> non empty
TEST_P(
    EmptyKbNodeAttributeTest,
    should_become_not_empty_and_value_text_is_kbnode_name_when_set_a_valid_kbnode) {  // NOLINT
  // Setup fixture
  auto kbnode_name = xtestutils::genRandomString();
  MockKbNode kbnode;
  EXPECT_CALL(kbnode, name()).WillOnce(Return(kbnode_name));

  // Exercise system
  attr_->setKbNode(&kbnode);

  // Verify results
  ASSERT_FALSE(attr_->isEmpty());
  ASSERT_EQ(kbnode_name, attr_->valueText());
}

TEST_P(EmptyKbNodeAttributeTest,
       should_set_a_valid_kbnode_emit_attributeChanged) {  // NOLINT
  // Expectations
  EXPECT_CALL(supplier_, attributeChanged(attr_));

  // Exercise system
  attr_->setKbNode(xtestutils::genDummyPointer<IKbNode>());
}

// empty ---> empty
TEST_P(EmptyKbNodeAttributeTest,
       should_set_null_kbnode_do_not_emit_attributeChanged) {  // NOLINT
  // Expectations
  EXPECT_CALL(supplier_, attributeChanged(attr_)).Times(0);

  // Exercise system
  attr_->setKbNode(nullptr);
}

// empty ---> empty
TEST_P(EmptyKbNodeAttributeTest,
       should_not_emit_attributeChanged_when_clear) {  // NOLINT
  // Expectations
  EXPECT_CALL(supplier_, attributeChanged(attr_)).Times(0);

  // Exercise system
  attr_->clear();
}

using NonEmptyCopyMoveFixtureHelper =
    CopyMoveFixtureHelper<KbNodeAttributeFixture,
                          NonEmptyKbNodeAttributeFixtureFactory>;

class NonEmptyKbNodeAttributeTest
    : public ErrorVerbosityTestWithParam<NonEmptyCopyMoveFixtureHelper*> {
 private:
  FixtureLoaderFromHelper<KbNodeAttributeFixture, NonEmptyKbNodeAttributeTest>
      fixture_;

 public:
  NonEmptyKbNodeAttributeTest()
      : ErrorVerbosityTestWithParam{},
        fixture_{this},
        supplier_(*fixture_->state_->supplierHelper().supplier_),
        kbnode_name_(fixture_->state_->kbNodeHelper().kbnode_name_),
        kbnode_(*fixture_->state_->kbNodeHelper().kbnode_),
        attr_{&fixture_->attr_} {}
  // ~NonEmptyKbNodeAttributeTest() { }
  // void SetUp() override {}
  // void TearDown() override { }

  void validateAttr() { fixture_->validateAttr(); }

  MockKbNodeAttributeSupplier& supplier_;
  utils::U8String& kbnode_name_;
  MockKbNode& kbnode_;
  KbNodeAttribute* attr_;
};

auto non_empty_test_fixture_helpers = FixtureHelperGenerator::fixtureHelpers<
    NonEmptyKbNodeAttributeFixtureFactory>(
    TEST_ENABLE_COPY_CONSTRUCT_TEST | TEST_ENABLE_COPY_ASSIGNMENT_TEST |
    TEST_ENABLE_MOVE_CONSTRUCT_TEST | TEST_ENABLE_MOVE_ASSIGNMENT_TEST);

INSTANTIATE_TEST_CASE_P(FixtureSetup, NonEmptyKbNodeAttributeTest,
                        ::testing::ValuesIn(non_empty_test_fixture_helpers));

TEST_P(NonEmptyKbNodeAttributeTest,
       should_attr_valueText_be_kbnode_name) {  // NOLINT
  ASSERT_EQ(kbnode_name_, attr_->valueText());
}

// non empty ---> empty
TEST_P(NonEmptyKbNodeAttributeTest,
       should_set_null_kbnode_becomes_empty) {  // NOLINT
  // Expectations
  EXPECT_CALL(supplier_, attributeChanged(attr_));

  // Exercise system
  attr_->setKbNode(nullptr);

  // Verify results
  assertAttrEmpty(*attr_);
}

// non empty ---> non empty
TEST_P(NonEmptyKbNodeAttributeTest,
       should_set_another_kbnode_will_emit_attribute_changed) {  // NOLINT
  // Setup fixture
  KbNodeHelper kbnode_helper;
  kbnode_helper.prepareKbNode();

  // Expectations
  EXPECT_CALL(supplier_, attributeChanged(attr_));

  // Exercise system
  attr_->setKbNode(kbnode_helper.kbnode_.get());

  // Verify result
  kbnode_helper.validateAttr(*attr_);
}

TEST_P(NonEmptyKbNodeAttributeTest,
       should_set_the_same_kbnode_will_not_emit_attributeChanged) {  // NOLINT
  // Expectations
  EXPECT_CALL(supplier_, attributeChanged(attr_)).Times(0);

  // Exercise system
  attr_->setKbNode(&kbnode_);

  // Verify result
  validateAttr();
}

TEST_P(NonEmptyKbNodeAttributeTest,
       should_clear_emit_attributeChanged_and_attr_becomes_empty) {  // NOLINT
  // Expectations
  EXPECT_CALL(supplier_, attributeChanged(attr_));

  // Exercise system
  attr_->clear();

  // Verify results
  assertAttrEmpty(*attr_);
}

////////////////////////////////////////////////

class KbNodeAttributeSupplierTest : public ::testing::Test {
 protected:
  KbNodeAttributeSupplierTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~KbNodeAttributeSupplierTest() { }
  virtual void SetUp() {
    expect_max_attrs = std::rand();

    EXPECT_CALL(root_kbnode, name()).WillRepeatedly(Return(root_kbnode_name));

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
  std::unique_ptr<fto::KbNodeAttributeSupplier> attr_supplier;
  // endregion
};

TEST_F(
    KbNodeAttributeSupplierTest,
    should_createAttribute_create_KbNodeAttribute_instance_with_this_as_the_supplier) {  // NOLINT
  // Exercise system
  auto new_attr = attr_supplier->addAttribute();

  // Verify results
  auto actual_attr = dynamic_cast<fto::KbNodeAttribute*>(new_attr);
  ASSERT_NE(nullptr, actual_attr);

  ASSERT_EQ(attr_supplier.get(), actual_attr->supplier());
}

}  // namespace tests
}  // namespace snailcore
