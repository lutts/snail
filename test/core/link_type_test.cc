//-*- TestCaseName: LinkTypeTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"

#include "src/core/link_type.h"
#include "test/core/attribute_test_stub.h"
#include "utils/text/variable_resolver.h"
#include "utils/text/mock_named_string_formatter.h"

using namespace utils::text::tests;  // NOLINT

namespace snailcore {
namespace tests {

class LinkTypeState {
 public:
  LinkTypeState() = default;
  // attr suppliers is not copied
  LinkTypeState(const LinkTypeState& rhs)
      : name_(rhs.name_),
        is_group_only_(rhs.is_group_only_),
        prototype_(rhs.prototype_) {}
  LinkTypeState& operator=(const LinkTypeState& rhs) {
    name_ = rhs.name_;
    is_group_only_ = rhs.is_group_only_;
    prototype_ = rhs.prototype_;
    return *this;
  }

  utils::U8String name_;
  bool is_group_only_{false};
  const LinkType* prototype_{nullptr};
  std::vector<IAttributeSupplier*> attr_suppliers_;
};

void validateLinkType(const LinkType& actual_link_type,
                      const LinkTypeState& expect_link_type) {
  ASSERT_EQ(expect_link_type.name_, actual_link_type.name());
  ASSERT_EQ(expect_link_type.is_group_only_, actual_link_type.isGroupOnly());
  if (!expect_link_type.prototype_) {
    ASSERT_EQ(&actual_link_type, actual_link_type.prototype());
  } else {
    ASSERT_EQ(expect_link_type.prototype_, actual_link_type.prototype());
  }
  ASSERT_EQ(expect_link_type.attr_suppliers_,
            actual_link_type.attributeSuppliers());
}

class LinkTypeTest;

class FormatterFixture : public TestFixture {
 public:
  FormatterFixture(const utils::U8String& name, LinkTypeTest* test_case)
      : TestFixture(name),
        formatter_factory_(MockNamedStringFormatterFactory::getInstance()) {
    (void)test_case;
    Mock::VerifyAndClearExpectations(formatter_factory_.get());

    formatter_ = new MockNamedStringFormatter();
    R_EXPECT_CALL(*formatter_factory_, createInstance())
        .WillOnce(Return(formatter_));
  }

  void setupClonedFormatter() {
    cloned_formatter_ = new MockNamedStringFormatter();
    R_EXPECT_CALL(*formatter_, clone()).WillOnce(Return(cloned_formatter_));
  }

  std::shared_ptr<MockNamedStringFormatterFactory> formatter_factory_;
  MockNamedStringFormatter* formatter_;
  MockNamedStringFormatter* cloned_formatter_;
};

class LinkTypeTest : public ::testing::Test, public FormatterFixture {
 public:
  LinkTypeTest()
      : FormatterFixture(FIXTURE_LOCATION, this),
        link_type{xtestutils::genRandomString(), xtestutils::randomBool()} {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~LinkTypeTest() { }
  void SetUp() override {
    const int TEST_ATTR_SUPPLIER_COUNT = 3;

    std::vector<std::unique_ptr<IAttributeSupplier>> attr_suppliers_up;
    std::vector<IAttributeSupplier*> attr_suppliers;
    for (int i = 0; i < TEST_ATTR_SUPPLIER_COUNT; ++i) {
      int max_attrs = i;
      auto supplier = utils::make_unique<MockAttrSupplierTestStub>(
          xtestutils::genRandomString(), max_attrs);
      attr_suppliers.push_back(supplier.get());
      mock_attr_suppliers_.push_back(supplier.get());
      attr_suppliers_up.push_back(std::move(supplier));
    }

    link_type.setAttributeSuppliers(std::move(attr_suppliers_up));

    link_type_state.name_ = link_type.name();
    link_type_state.is_group_only_ = link_type.isGroupOnly();
    link_type_state.attr_suppliers_ = std::move(attr_suppliers);
    link_type_state.prototype_ = &link_type;

    checkSetup();
  }
  // void TearDown() override { }

  void checkSetup();
  void setupClonedLinkTypeState();
  void fillSupplierWithAttributes();

  LinkType link_type;
  LinkTypeState link_type_state;
  LinkTypeState cloned_link_type_state;
  std::vector<MockAttrSupplierTestStub*> mock_attr_suppliers_;
  std::map<utils::U8String, std::vector<utils::U8String>>
      supplier_to_attr_values;
};

void LinkTypeTest::checkSetup() {
  verify();
  validateLinkType(link_type, link_type_state);
}

void LinkTypeTest::setupClonedLinkTypeState() {
  std::vector<IAttributeSupplier*> cloned_attr_suppliers;
  for (auto& supplier : mock_attr_suppliers_) {
    auto cloned_supplier = new MockAttrSupplierTestStub;
    R_EXPECT_CALL(*supplier, clone()).WillOnce(Return(cloned_supplier));
    cloned_attr_suppliers.push_back(cloned_supplier);
  }

  cloned_link_type_state = link_type_state;
  cloned_link_type_state.attr_suppliers_ = std::move(cloned_attr_suppliers);
}

void LinkTypeTest::fillSupplierWithAttributes() {
  for (auto& supplier : mock_attr_suppliers_) {
    auto attr_value = xtestutils::genRandomString();
    while (supplier->addAttribute(supplier->name(), attr_value)) {
      supplier_to_attr_values[supplier->name()].push_back(attr_value);
    }
  }
}

TEST_F(LinkTypeTest, test_copy_construct) {  // NOLINT
  // Setup fixture
  setupClonedLinkTypeState();

  // Expectations
  setupClonedFormatter();

  // Exercise system
  LinkType cloned_link_type{link_type};

  // Verify results
  CUSTOM_ASSERT(validateLinkType(cloned_link_type, cloned_link_type_state));
  // ensure fixture's link_type is not changed
  CUSTOM_ASSERT(validateLinkType(link_type, link_type_state));
}

TEST_F(LinkTypeTest, test_copy_assignment) {  // NOLINT
  // Setup fixture
  setupClonedLinkTypeState();

  FixtureHelper(FormatterFixture, prepare_create_link_type);
  LinkType cloned_link_type{"", false};

  // Expectations
  setupClonedFormatter();

  // Exercise system
  LinkType& ref = (cloned_link_type = link_type);

  // Verify results
  EXPECT_THAT(ref, Ref(cloned_link_type));
  CUSTOM_ASSERT(validateLinkType(cloned_link_type, cloned_link_type_state));
  // ensure fixture's link_type is not changed
  CUSTOM_ASSERT(validateLinkType(link_type, link_type_state));
}

TEST_F(LinkTypeTest, test_move_construct) {  // NOLINT
  // Setup fixture

  LinkTypeState empty_state;

  // Exercise system
  LinkType target_link_type{std::move(link_type)};

  // Verify results
  CUSTOM_ASSERT(validateLinkType(target_link_type, link_type_state));
  CUSTOM_ASSERT(validateLinkType(link_type, empty_state));
}

TEST_F(LinkTypeTest, test_move_assignment) {  // NOLINT
  // Setup fixture
  FixtureHelper(FormatterFixture, prepare_create_link_type);
  LinkType target_link_type{"", false};
  LinkTypeState empty_state;

  // Exercise system
  LinkType& ref = (target_link_type = std::move(link_type));

  // Verify results
  EXPECT_THAT(ref, Ref(target_link_type));
  CUSTOM_ASSERT(validateLinkType(target_link_type, link_type_state));
  CUSTOM_ASSERT(validateLinkType(link_type, empty_state));
}

TEST_F(LinkTypeTest,
       should_clear_the_prototype_itself_will_do_nothing) {  // NOLINT
  // Exercise system
  link_type.clear();

  // Verify results
  CUSTOM_ASSERT(validateLinkType(link_type, link_type_state));
}

TEST_F(
    LinkTypeTest,
    should_clear_non_prototype_link_type_return_to_prototype_state) {  // NOLINT
  // Setup fixture
  setupClonedLinkTypeState();

  LinkType cloned_link_type = link_type;

  verify();

  setupClonedLinkTypeState();

  // Exercise system
  cloned_link_type.clear();

  // Verify results
  CUSTOM_ASSERT(validateLinkType(cloned_link_type, cloned_link_type_state));
  CUSTOM_ASSERT(validateLinkType(link_type, link_type_state));
}

TEST_F(LinkTypeTest, should_be_an_IVariableResolver) {  // NOLINT
  auto is_variable_resolver =
      std::is_base_of<utils::text::VariableResolver, LinkType>();
  ASSERT_TRUE(is_variable_resolver);
}

TEST_F(
    LinkTypeTest,
    should_lookup_return_all_attr_values_under_the_supplier_with_the_lookuped_name) {  // NOLINT
  fillSupplierWithAttributes();

  // Verify results
  for (auto& supplier : mock_attr_suppliers_) {
    auto var_name = supplier->name();
    auto expect_values = supplier_to_attr_values[var_name];
    auto actual_values = link_type.lookup(var_name);
    ASSERT_EQ(expect_values, actual_values);
  }

  auto unknown_name = xtestutils::genRandomString();
  auto actual_values = link_type.lookup(unknown_name);
  ASSERT_TRUE(actual_values.empty());
}

TEST_F(
    LinkTypeTest,
    should_toString_will_call_named_string_formatter_with_link_type_itself_as_variable_resolver) {  // NOLINT
  // Setup fixture
  auto expect_str = xtestutils::genRandomString();
  auto link_phrase = xtestutils::genRandomString();
  link_type.setLinkPhrase(link_phrase);

  auto formatter = formatter_;

  // Expectations
  EXPECT_CALL(*formatter, format(link_phrase, &link_type))
      .WillOnce(Return(expect_str));

  // Exercise system
  auto actual_str = link_type.toString();

  // Verify results
  ASSERT_EQ(expect_str, actual_str);
}

}  // namespace tests
}  // namespace snailcore
