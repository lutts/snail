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

class LinkTypeTest : public ::testing::Test {
 public:
  LinkTypeTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~LinkTypeTest() { }
  void SetUp() override { }
  // void TearDown() override { }
};

class LinkTypeData {
 public:
  LinkTypeData() = default;
  // attr suppliers is not copied
  LinkTypeData(const LinkTypeData& rhs)
      : name_(rhs.name_)
      , is_group_only_(rhs.is_group_only_)
      , prototype_(rhs.prototype_){ }
  LinkTypeData& operator=(const LinkTypeData& rhs) {
    name_ = rhs.name_;
    is_group_only_ = rhs.is_group_only_;
    prototype_ = rhs.prototype_;
    return *this;
  }

  utils::U8String name_;
  bool is_group_only_ { false };
  const LinkType* prototype_ { nullptr };
  std::vector<IAttributeSupplier*> attr_suppliers_;
};

void validateLinkType(const LinkType& actual_link_type,
                      const LinkTypeData& expect_link_type) {
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

TEST_F(LinkTypeTest,
       test_construct) { // NOLINT
  auto expect_name = xtestutils::genRandomString();
  auto expect_group_only = xtestutils::randomBool();

  LinkType link_type { expect_name, expect_group_only };

  // Verify results
  LinkTypeData link_type_data;
  link_type_data.name_ = expect_name;
  link_type_data.is_group_only_ = expect_group_only;

  CUSTOM_ASSERT(validateLinkType(link_type, link_type_data));
}

class AttrSupplierFixture : public TestFixture {
 public:
  AttrSupplierFixture(const utils::U8String& name,
              LinkTypeTest* test_case)
      : TestFixture(name)
      , link_type { xtestutils::genRandomString(),
        xtestutils::randomBool() } {
    (void)test_case;

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

    link_type_data.name_ = link_type.name();
    link_type_data.is_group_only_ = link_type.isGroupOnly();
    link_type_data.attr_suppliers_ = std::move(attr_suppliers);
    link_type_data.prototype_ = &link_type;
  }

  void checkSetup() {
    SCOPED_TRACE(name_);

    validateLinkType(link_type, link_type_data);

    abortIfFailure();
  }

  void setupClonedLinkTypeData() {
    std::vector<IAttributeSupplier*> cloned_attr_suppliers;
    for (auto & supplier : mock_attr_suppliers_) {
      auto cloned_supplier = new MockAttrSupplierTestStub;
      R_EXPECT_CALL(*supplier, clone()).WillOnce(Return(cloned_supplier));
      cloned_attr_suppliers.push_back(cloned_supplier);
    }

    cloned_link_type_data = link_type_data;
    cloned_link_type_data.attr_suppliers_ = std::move(cloned_attr_suppliers);
  }

  void fillSupplierWithAttributes() {
    for (auto & supplier : mock_attr_suppliers_) {
      auto attr_value = xtestutils::genRandomString();
      while (supplier->addAttribute(supplier->name(), attr_value)) {
          supplier_to_attr_values[supplier->name()].push_back(attr_value);
      }
    }
  }

  LinkType link_type;
  LinkTypeData link_type_data;
  LinkTypeData cloned_link_type_data;
  std::vector<MockAttrSupplierTestStub*> mock_attr_suppliers_;
  std::map<utils::U8String,
           std::vector<utils::U8String> > supplier_to_attr_values;
};

TEST_F(LinkTypeTest,
       should_be_able_to_set_and_get_attribute_suppliers) { // NOLINT
  FixtureHelper(AttrSupplierFixture, fixture);
}

TEST_F(LinkTypeTest,
       test_copy_construct) { // NOLINT
  // Setup fixture
  FixtureHelper(AttrSupplierFixture, fixture);
  fixture.setupClonedLinkTypeData();

  // Exercise system
  LinkType cloned_link_type { fixture.link_type };

  // Verify results
  CUSTOM_ASSERT(validateLinkType(cloned_link_type,
                                 fixture.cloned_link_type_data));
  // ensure fixture's link_type is not changed
  CUSTOM_ASSERT(validateLinkType(fixture.link_type, fixture.link_type_data));
}

TEST_F(LinkTypeTest,
       test_copy_assignment) { // NOLINT
  // Setup fixture
  FixtureHelper(AttrSupplierFixture, fixture);
  fixture.setupClonedLinkTypeData();

  LinkType cloned_link_type { "", false };

  // Exercise system
  LinkType& ref = (cloned_link_type = fixture.link_type);

  // Verify results
  EXPECT_THAT(ref, Ref(cloned_link_type));
  CUSTOM_ASSERT(validateLinkType(cloned_link_type,
                                 fixture.cloned_link_type_data));
  // ensure fixture's link_type is not changed
  CUSTOM_ASSERT(validateLinkType(fixture.link_type, fixture.link_type_data));
}

TEST_F(LinkTypeTest,
       test_move_construct) { // NOLINT
  // Setup fixture
  FixtureHelper(AttrSupplierFixture, fixture);

  LinkTypeData empty_data;

  // Exercise system
  LinkType link_type { std::move(fixture.link_type) };

  // Verify results
  CUSTOM_ASSERT(validateLinkType(link_type, fixture.link_type_data));
  CUSTOM_ASSERT(validateLinkType(fixture.link_type, empty_data));
}

TEST_F(LinkTypeTest,
       test_move_assignment) { // NOLINT
  // Setup fixture
  FixtureHelper(AttrSupplierFixture, fixture);

  LinkTypeData empty_data;
  LinkType link_type { "", false };

  // Exercise system
  LinkType& ref = (link_type = std::move(fixture.link_type));

  // Verify results
  EXPECT_THAT(ref, Ref(link_type));
  CUSTOM_ASSERT(validateLinkType(link_type, fixture.link_type_data));
  CUSTOM_ASSERT(validateLinkType(fixture.link_type, empty_data));
}

TEST_F(LinkTypeTest,
       should_clear_the_prototype_itself_will_do_nothing) { // NOLINT
  // Setup fixture
  FixtureHelper(AttrSupplierFixture, fixture);

  // Exercise system
  fixture.link_type.clear();

  // Verify results
  CUSTOM_ASSERT(validateLinkType(fixture.link_type, fixture.link_type_data));
}

TEST_F(LinkTypeTest,
       should_clear_non_prototype_link_type_return_to_prototype_state) { // NOLINT
  // Setup fixture
  FixtureHelper(AttrSupplierFixture, fixture);
  fixture.setupClonedLinkTypeData();

  LinkType cloned_link_type = fixture.link_type;

  fixture.verify();

  fixture.setupClonedLinkTypeData();

  // Exercise system
  cloned_link_type.clear();

  // Verify results
  CUSTOM_ASSERT(validateLinkType(cloned_link_type,
                                 fixture.cloned_link_type_data));
  CUSTOM_ASSERT(validateLinkType(fixture.link_type, fixture.link_type_data));
}

TEST_F(LinkTypeTest,
       should_be_an_IVariableResolver) { // NOLINT
  auto is_variable_resolver =
      std::is_base_of<utils::text::VariableResolver, LinkType>();
  ASSERT_TRUE(is_variable_resolver);
}

TEST_F(LinkTypeTest,
       should_lookup_return_all_attr_values_under_the_supplier_with_the_lookuped_name) { // NOLINT
  FixtureHelper(AttrSupplierFixture, fixture);
  fixture.fillSupplierWithAttributes();

  // Verify results
  for (auto & supplier : fixture.mock_attr_suppliers_) {
    auto var_name = supplier->name();
    auto expect_values = fixture.supplier_to_attr_values[var_name];
    auto actual_values = fixture.link_type.lookup(var_name);
    ASSERT_EQ(expect_values, actual_values);
  }

  auto unknown_name = xtestutils::genRandomString();
  auto actual_values = fixture.link_type.lookup(unknown_name);
  ASSERT_TRUE(actual_values.empty());
}

TEST_F(LinkTypeTest,
       should_toString_will_call_named_string_formatter_with_link_type_itself_as_variable_resolver) { // NOLINT
  // Setup fixture
  MockNamedStringFormatterFactory named_str_formatter_factory;

  auto named_string_formatter = new MockNamedStringFormatter();
  EXPECT_CALL(named_str_formatter_factory, createInstance())
      .WillOnce(Return(named_string_formatter));

  FixtureHelper(AttrSupplierFixture, fixture);

  auto expect_str = xtestutils::genRandomString();
  auto link_phrase = xtestutils::genRandomString();
  fixture.link_type.setLinkPhrase(link_phrase);

  // Expectations
  EXPECT_CALL(*named_string_formatter, format(link_phrase, &fixture.link_type))
      .WillOnce(Return(expect_str));

  // Exercise system
  auto actual_str = fixture.link_type.toString();

  // Verify results
  ASSERT_EQ(expect_str, actual_str);
}

}  // namespace tests
}  // namespace snailcore
