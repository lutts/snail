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
#include "utils/singleton.h"

#include "test/core/generic_attribute_supplier_test.h"

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

  void copyData(const SupplierTestHelper& rhs) {
    (void)rhs;
    // as the name suggested, supplier is not copied
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

  void copyData(const KbNodeHelper& rhs) { this->operator=(rhs); }

  utils::U8String kbnode_name_;
  std::shared_ptr<MockKbNode> kbnode_;
};

class KbNodeAttributeStateSet
    : public xtestutils::TestFixtureStateSet<SupplierTestHelper, KbNodeHelper> {
 public:
  SupplierTestHelper& supplierHelper() { return get<0>(); }
  KbNodeHelper& kbNodeHelper() { return get<1>(); }
};

class KbNodeAttributeFixture : public xtestutils::TestFixture {
 public:
  std::unique_ptr<KbNodeAttributeStateSet> state_;

 public:
  explicit KbNodeAttributeFixture(
      std::unique_ptr<KbNodeAttributeStateSet> state)
      : xtestutils::TestFixture{},
        state_{std::move(state)},
        attr_{state_->supplierHelper().supplier_.get()} {
    auto kbnode = state_->kbNodeHelper().kbnode_.get();
    if (kbnode) EXPECT_CALL(*kbnode, incRef());
    attr_.setKbNode(kbnode);
  }

  KbNodeAttributeFixture(const KbNodeAttributeFixture& rhs)
      : xtestutils::TestFixture{rhs},
        state_{new KbNodeAttributeStateSet{*rhs.state_}},
        attr_{rhs.attr_} {}

  KbNodeAttributeFixture(KbNodeAttributeFixture&& rhs)
      : xtestutils::TestFixture{std::move(rhs)},
        state_{new KbNodeAttributeStateSet{std::move(*rhs.state_)}},
        attr_{std::move(rhs.attr_)} {}

  KbNodeAttributeFixture& operator=(const KbNodeAttributeFixture& rhs) {
    xtestutils::TestFixture::operator=(rhs);
    *state_ = *rhs.state_;

    R_EXPECT_CALL(*state_->supplierHelper().supplier_,
                  attributeChanged(&attr_));
    attr_ = rhs.attr_;

    return *this;
  }

  KbNodeAttributeFixture& operator=(KbNodeAttributeFixture&& rhs) {
    xtestutils::TestFixture::operator=(std::move(rhs));
    *state_ = std::move(*rhs.state_);

    R_EXPECT_CALL(*state_->supplierHelper().supplier_,
                  attributeChanged(&attr_));
    attr_ = std::move(rhs.attr_);

    return *this;
  }

  void copyData(const KbNodeAttributeFixture& rhs) {
    xtestutils::TestFixture::operator=(rhs);

    state_->applyPairwise(*rhs.state_, CopyDataFunctor());
    attr_.copyData(rhs.attr_);
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

using FixtureHelperGenerator = xtestutils::CopyMoveFixtureHelperGenerator<
    KbNodeAttributeFixture, EmptyKbNodeAttributeFixtureFactory,
    NonEmptyKbNodeAttributeFixtureFactory>;

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

using KbNodeAttributeFixtureHelper =
    xtestutils::TestFixtureHelper<KbNodeAttributeFixture>;

class EmptyKbNodeAttributeTest : public xtestutils::ErrorVerbosityTestWithParam<
                                     KbNodeAttributeFixtureHelper*> {
 private:
  xtestutils::FixtureLoaderFromHelper<KbNodeAttributeFixture,
                                      EmptyKbNodeAttributeTest> fixture_;

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

auto empty_test_fixture_helpers = FixtureHelperGenerator::fixtureHelpers<
    EmptyKbNodeAttributeFixtureFactory, KbNodeAttributeFixture,
    FixtureHelperGenerator::CopyConstructFixtureHelper,
    FixtureHelperGenerator::CopyAssignmentFixtureHelper,
    FixtureHelperGenerator::MoveConstructFixtureHelper,
    FixtureHelperGenerator::MoveAssignmentFixtureHelper,
    CopyDataFixtureHelper>();
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

  // Expectations
  EXPECT_CALL(kbnode, incRef());

  // Exercise system
  attr_->setKbNode(&kbnode);

  // Verify results
  ASSERT_FALSE(attr_->isEmpty());
  ASSERT_EQ(kbnode_name, attr_->valueText());
}

// empty ---> non empty
TEST_P(EmptyKbNodeAttributeTest,
       should_set_a_valid_kbnode_emit_attributeChanged) {  // NOLINT
  MockKbNode kbnode;

  // Expectations
  EXPECT_CALL(kbnode, incRef());
  EXPECT_CALL(supplier_, attributeChanged(attr_));

  // Exercise system
  attr_->setKbNode(&kbnode);
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

class NonEmptyKbNodeAttributeTest
    : public xtestutils::ErrorVerbosityTestWithParam<
          KbNodeAttributeFixtureHelper*> {
 private:
  xtestutils::FixtureLoaderFromHelper<KbNodeAttributeFixture,
                                      NonEmptyKbNodeAttributeTest> fixture_;

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
    NonEmptyKbNodeAttributeFixtureFactory, KbNodeAttributeFixture,
    FixtureHelperGenerator::CopyConstructFixtureHelper,
    FixtureHelperGenerator::CopyAssignmentFixtureHelper,
    FixtureHelperGenerator::MoveConstructFixtureHelper,
    FixtureHelperGenerator::MoveAssignmentFixtureHelper,
    CopyDataFixtureHelper>();

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
  EXPECT_CALL(kbnode_, decRef());
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

  auto& old_kbnode = kbnode_;
  auto new_kbnode = kbnode_helper.kbnode_.get();

  // Expectations
  EXPECT_CALL(old_kbnode, decRef());
  EXPECT_CALL(*new_kbnode, incRef());
  EXPECT_CALL(supplier_, attributeChanged(attr_));

  // Exercise system
  attr_->setKbNode(new_kbnode);

  // Verify result
  kbnode_helper.validateAttr(*attr_);
}

TEST_P(NonEmptyKbNodeAttributeTest,
       should_set_the_same_kbnode_will_not_emit_attributeChanged) {  // NOLINT
  // Expectations
  EXPECT_CALL(kbnode_, incRef()).Times(0);
  EXPECT_CALL(kbnode_, decRef()).Times(0);
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

class RootKbNodeHelper : public KbNodeHelper {
 public:
  RootKbNodeHelper() : KbNodeHelper{} { prepareKbNode(); }
};

class MockKbNodeAttrFactory : public KbNodeAttributeFactory {
 public:
  MOCK_CONST_METHOD0(createAttribute, fto::KbNodeAttribute*());
};

using KbNodeAttributeFactoryFixture =
    AttributeFactoryFixtureTemplate<MockKbNodeAttrFactory, MockKbNodeAttribute>;

class KbNodeAttrSupplierFixture : public GenericAttributeSupplierFixture {
 public:
  KbNodeAttrSupplierFixture()
      : GenericAttributeSupplierFixture{},
        attr_supplier_{rootKbNode(), max_attrs()} {}

  KbNodeAttrSupplierFixture(const KbNodeAttrSupplierFixture& rhs)
      : GenericAttributeSupplierFixture{rhs},
        root_kbnode_helper_{rhs.root_kbnode_helper_},
        attr_factory_fixture_{rhs.attr_factory_fixture_},
        attr_supplier_{rhs.attr_supplier_} {}

  KbNodeAttrSupplierFixture& operator=(const KbNodeAttrSupplierFixture& rhs) {
    GenericAttributeSupplierFixture::operator=(rhs);
    root_kbnode_helper_ = rhs.root_kbnode_helper_;
    attr_factory_fixture_ = rhs.attr_factory_fixture_;

    attr_supplier_ = rhs.attr_supplier_;

    return *this;
  }

  utils::U8String getSupplierName() override {
    return root_kbnode_helper_.kbnode_name_;
  }

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

    ASSERT_EQ(rootKbNode(), attr_supplier_.getRootKbNode())
        << "should be able to get back the root kbnode passed in constructor";
  }
  // endregion: checkers

  void enableMockAttrFactory() {
    attr_supplier_.setAttributeFactory(attr_factory_fixture_.attr_factory());
  }

 private:
  IKbNode* rootKbNode() { return root_kbnode_helper_.kbnode_.get(); }

  RootKbNodeHelper root_kbnode_helper_;
  KbNodeAttributeFactoryFixture attr_factory_fixture_;
  KbNodeAttributeSupplier attr_supplier_;
};

class KbNodeAttrSupplierWithMockAttrFactoryFixtureFactory {
 public:
  static KbNodeAttrSupplierFixture* createFixture() {
    auto fixture = new KbNodeAttrSupplierFixture();
    fixture->enableMockAttrFactory();
    return fixture;
  }
};

class KbNodeAttrSupplierFilledWithAttrsFixtureFactory {
 public:
  static KbNodeAttrSupplierFixture* createFixture() {
    auto fixture = new KbNodeAttrSupplierFixture();
    fixture->enableMockAttrFactory();
    fixture->fillAttributes();
    return fixture;
  }
};

using GenericSupplierFixtureHelperGenerator =
    xtestutils::CopyMoveFixtureHelperGenerator<
        GenericAttributeSupplierFixture,
        KbNodeAttrSupplierWithMockAttrFactoryFixtureFactory,
        KbNodeAttrSupplierFilledWithAttrsFixtureFactory>;

INSTANTIATE_GENERIC_ATTR_SUPPLIER_TESTS(
    KbNodeAttrSupplierFixture,
    KbNodeAttrSupplierWithMockAttrFactoryFixtureFactory,
    KbNodeAttrSupplierFilledWithAttrsFixtureFactory,
    GenericSupplierFixtureHelperGenerator::CopyAssignmentFixtureHelper);

class KbNodeAttrSupplierNoAttrFactoryFixtureFactory {
 public:
  static KbNodeAttrSupplierFixture* createFixture() {
    return new KbNodeAttrSupplierFixture();
  }
};

class KbNodeAttributeSupplierNoMockAttrFactoryTest
    : public xtestutils::ErrorVerbosityTest,
      public GenericAttributeSupplierFixtureWrapper {
 protected:
  KbNodeAttributeSupplierNoMockAttrFactoryTest() : ErrorVerbosityTest() {
    setFixture(KbNodeAttrSupplierNoAttrFactoryFixtureFactory::createFixture());
  }
};

TEST_F(KbNodeAttributeSupplierNoMockAttrFactoryTest,
       should_createAttribute_create_KbNodeAttribute_instance) {  // NOLINT
  auto new_attr = attr_supplier_->addAttribute();

  // Verify results
  auto actual_attr = dynamic_cast<fto::KbNodeAttribute*>(new_attr);
  ASSERT_NE(nullptr, actual_attr);

  ASSERT_EQ(attr_supplier_, actual_attr->supplier());
}

}  // namespace tests
}  // namespace snailcore
