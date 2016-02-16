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

#include "utils/stacktrace.h"

using namespace utils::text::tests;  // NOLINT

namespace snailcore {
namespace tests {

class LinkTypeTest;

class FormatterFixture : public xtestutils::TestFixture {
 public:
  FormatterFixture()
      : xtestutils::TestFixture{},
        formatter_factory_(MockNamedStringFormatterFactory::getInstance()) {
    Mock::VerifyAndClearExpectations(formatter_factory_.get());

    formatter_ = new MockNamedStringFormatter();
    R_EXPECT_CALL(*formatter_factory_, create()).WillOnce(Return(formatter_));
  }

  FormatterFixture(const FormatterFixture& rhs)
      : xtestutils::TestFixture(rhs),
        formatter_factory_{rhs.formatter_factory_},
        formatter_{new MockNamedStringFormatter()} {
    R_EXPECT_CALL(*rhs.formatter_, clone()).WillOnce(Return(formatter_));
  }

  FormatterFixture(FormatterFixture&& rhs)
      : xtestutils::TestFixture(std::move(rhs)),
        formatter_factory_(std::move(rhs.formatter_factory_)),
        formatter_(new MockNamedStringFormatter()) {
    R_EXPECT_CALL(*rhs.formatter_, moveClone()).WillOnce(Return(formatter_));
  }

  FormatterFixture& operator=(FormatterFixture rhs) {
    rhs.swap(*this);
    return *this;
  }

  void swap(FormatterFixture& rhs) {
    xtestutils::TestFixture::swap(rhs);
    using std::swap;
    swap(formatter_factory_, rhs.formatter_factory_);
    R_EXPECT_CALL(*formatter_, swap_with(Ref(*rhs.formatter_)));
  }

  friend inline void swap(FormatterFixture& v1, FormatterFixture& v2) {
    v1.swap(v2);
  }

  virtual ~FormatterFixture() = default;

 private:
  std::shared_ptr<MockNamedStringFormatterFactory> formatter_factory_;

 public:
  MockNamedStringFormatter* formatter_;
};

class AttrSupplierFixture : public xtestutils::TestFixture {
 public:
  AttrSupplierFixture() : xtestutils::TestFixture{} {
    constexpr int TEST_ATTR_SUPPLIER_COUNT = 3;

    for (int i = 0; i < TEST_ATTR_SUPPLIER_COUNT; ++i) {
      int max_attrs = i;
      auto supplier = utils::make_unique<MockAttrSupplierTestStub>(
          xtestutils::genRandomString(), max_attrs);

      attr_suppliers_.push_back(supplier.get());
      mock_attr_suppliers_.push_back(supplier.get());
      attr_suppliers_up_.push_back(std::move(supplier));
    }
  }

  AttrSupplierFixture(const AttrSupplierFixture& rhs)
      : xtestutils::TestFixture(rhs) {
    for (auto& supplier : rhs.mock_attr_suppliers_) {
      auto cloned_supplier = new MockAttrSupplierTestStub;
      R_EXPECT_CALL(*supplier, clone()).WillOnce(Return(cloned_supplier));

      attr_suppliers_.push_back(cloned_supplier);
      mock_attr_suppliers_.push_back(cloned_supplier);
    }
  }

  AttrSupplierFixture(AttrSupplierFixture&& rhs)
      : xtestutils::TestFixture(std::move(rhs)),
        attr_suppliers_(std::move(rhs.attr_suppliers_)),
        mock_attr_suppliers_(std::move(rhs.mock_attr_suppliers_)),
        attr_suppliers_up_(std::move(rhs.attr_suppliers_up_)),
        supplier_to_attr_values(std::move(rhs.supplier_to_attr_values)) {}

  AttrSupplierFixture& operator=(AttrSupplierFixture rhs) {
    rhs.swap(*this);
    return *this;
  }

  void swap(AttrSupplierFixture& rhs) {
    xtestutils::TestFixture::swap(rhs);

    using std::swap;
    swap(attr_suppliers_, rhs.attr_suppliers_);
    swap(mock_attr_suppliers_, rhs.mock_attr_suppliers_);
    swap(attr_suppliers_up_, rhs.attr_suppliers_up_);
    swap(supplier_to_attr_values, rhs.supplier_to_attr_values);
  }

  friend inline void swap(AttrSupplierFixture& v1, AttrSupplierFixture& v2) {
    v1.swap(v2);
  }

  virtual ~AttrSupplierFixture() = default;

  void fillSupplierWithAttributes() {
    for (auto& supplier : mock_attr_suppliers_) {
      auto attr_value = xtestutils::genRandomString();
      while (supplier->addAttribute(supplier->name(), attr_value)) {
        supplier_to_attr_values[supplier->name()].push_back(attr_value);
      }
    }
  }

 public:
  std::vector<IAttributeSupplier*> attr_suppliers_;
  std::vector<MockAttrSupplierTestStub*> mock_attr_suppliers_;
  std::vector<std::unique_ptr<IAttributeSupplier>> attr_suppliers_up_;
  std::map<utils::U8String, std::vector<utils::U8String>>
      supplier_to_attr_values;
};

class MockListener : public xtestutils::SimpleMockListener<LinkType> {
 public:
  SNAIL_MOCK_LISTENER0(MockListener, LinkUpdated, void());

  explicit MockListener(LinkType* subject) : SimpleMockListener(subject) {
    SNAIL_MOCK_LISTENER_REGISTER(LinkUpdated, this);

    attach();
  }
};

class LinkTypeState {
 public:
  LinkTypeState()
      : formatter_fixture_{},
        attr_supplier_fixture_{},
        name_{xtestutils::genRandomString()},
        is_group_only_{xtestutils::randomBool()},
        link_phrase_{xtestutils::genRandomString()} {}

  LinkTypeState(const LinkTypeState& rhs)
      : formatter_fixture_{rhs.formatter_fixture_},
        attr_supplier_fixture_{rhs.attr_supplier_fixture_},
        name_{rhs.name_},
        is_group_only_{rhs.is_group_only_},
        link_phrase_{rhs.link_phrase_},
        prototype_{rhs.prototype_} {}

  LinkTypeState(LinkTypeState&& rhs)
      : formatter_fixture_(std::move(rhs.formatter_fixture_)),
        attr_supplier_fixture_(std::move(rhs.attr_supplier_fixture_)),
        name_(std::move(rhs.name_)),
        is_group_only_(std::move(rhs.is_group_only_)),
        link_phrase_(std::move(rhs.link_phrase_)),
        prototype_(std::move(rhs.prototype_)) {}

  LinkTypeState& operator=(LinkTypeState rhs) {
    swap(rhs);
    return *this;
  }

  void swap(LinkTypeState& rhs) {
    using std::swap;
    swap(formatter_fixture_, rhs.formatter_fixture_);
    swap(attr_supplier_fixture_, rhs.attr_supplier_fixture_);
    swap(name_, rhs.name_);
    swap(is_group_only_, rhs.is_group_only_);
    swap(link_phrase_, rhs.link_phrase_);
    swap(prototype_, rhs.prototype_);
  }

  void validate(const LinkType& link_type) {
    ASSERT_EQ(name_, link_type.name());
    ASSERT_EQ(is_group_only_, link_type.isGroupOnly());
    ASSERT_EQ(prototype_, link_type.prototype());
    ASSERT_EQ(attr_supplier_fixture_.attr_suppliers_,
              link_type.attributeSuppliers());
  }

  void verifyFixtures() {
    formatter_fixture_.verify();
    attr_supplier_fixture_.verify();
  }

 public:
  FormatterFixture formatter_fixture_;
  AttrSupplierFixture attr_supplier_fixture_;
  utils::U8String name_;
  bool is_group_only_;
  utils::U8String link_phrase_;
  LinkType* prototype_{nullptr};
};

class LinkTypeFixture : public xtestutils::TestFixture {
 public:
  LinkTypeFixture() : LinkTypeFixture{"GlobalFixture", nullptr} {}
  LinkTypeFixture(const utils::U8String& fixture_name, LinkTypeTest* test_case)
      : xtestutils::TestFixture{fixture_name},
        link_type_state{},
        link_type{link_type_state.name_, link_type_state.is_group_only_},
        mock_listener_{&link_type} {
    (void)test_case;
    link_type.setLinkPhrase(link_type_state.link_phrase_);
    link_type.setAttributeSuppliers(
        std::move(link_type_state.attr_supplier_fixture_.attr_suppliers_up_));
    link_type_state.prototype_ = &link_type;
  }

  LinkTypeFixture(const LinkTypeFixture& rhs)
      : xtestutils::TestFixture{rhs},
        link_type_state{rhs.link_type_state},
        link_type{rhs.link_type},
        mock_listener_{&link_type} {}

  LinkTypeFixture(LinkTypeFixture&& rhs)
      : xtestutils::TestFixture(std::move(rhs)),
        link_type_state{std::move(rhs.link_type_state)},
        link_type(std::move(rhs.link_type)),
        mock_listener_(&link_type) {}

  // NOTE: do NOT use copy-and-swap, we are testing copy assignment
  LinkTypeFixture& operator=(const LinkTypeFixture& rhs) {
    xtestutils::TestFixture::operator=(rhs);
    link_type_state = rhs.link_type_state;

    R_EXPECT_CALL(mock_listener_, LinkUpdated());
    LinkType& ref = (link_type = rhs.link_type);
    // mock listener not copied

    EXPECT_THAT(ref, Ref(link_type));

    return *this;
  }

  // NOTE: do NOT use copy-and-swap, we are testing move assignment
  LinkTypeFixture& operator=(LinkTypeFixture&& rhs) {
    xtestutils::TestFixture::operator=(std::move(rhs));
    link_type_state = std::move(rhs.link_type_state);

    R_EXPECT_CALL(mock_listener_, LinkUpdated());
    LinkType& ref = (link_type = std::move(rhs.link_type));
    // mock listener not moved

    EXPECT_THAT(ref, Ref(link_type));

    return *this;
  }

  virtual ~LinkTypeFixture() = default;

  void checkSetup() {
    verify();
    link_type_state.verifyFixtures();
    link_type_state.validate(link_type);
  }

  MockListener& mockListener() { return mock_listener_; }

  std::vector<MockAttrSupplierTestStub*>& mockAttrSuppliers() {
    return link_type_state.attr_supplier_fixture_.mock_attr_suppliers_;
  }

  void fillSupplierWithAttributes() {
    link_type_state.attr_supplier_fixture_.fillSupplierWithAttributes();
  }

  std::vector<utils::U8String>& supplierToAttrValues(
      const utils::U8String& supplier) {
    return link_type_state.attr_supplier_fixture_
        .supplier_to_attr_values[supplier];
  }

  MockNamedStringFormatter* formatter() {
    return link_type_state.formatter_fixture_.formatter_;
  }

  utils::U8String linkPhrase() { return link_type_state.link_phrase_; }

  void validateLinkType() { link_type_state.validate(link_type); }

  LinkTypeState link_type_state;
  LinkType link_type;
  MockListener mock_listener_;
};

class LinkTypeFixtureFactory {
 public:
  static LinkTypeFixture* createFixture() { return new LinkTypeFixture(); }
};

using FixtureHelperGenerator =
    xtestutils::CopyMoveFixtureHelperGenerator<LinkTypeFixture,
                                               LinkTypeFixtureFactory>;

using LinkTypeFixtureHelper = xtestutils::TestFixtureHelper<LinkTypeFixture>;

class LinkTypeTest
    : public xtestutils::ErrorVerbosityTestWithParam<LinkTypeFixtureHelper*> {
 private:
  xtestutils::FixtureLoaderFromHelper<LinkTypeFixture, LinkTypeTest> fixture_;

 public:
  LinkTypeTest()
      : ErrorVerbosityTestWithParam{},
        fixture_{this},
        link_type_{&fixture_->link_type} {}
  // ~LinkTypeTest() { }
  // void SetUp() override {}
  // void TearDown() override { }

  MockListener& mockListener() { return fixture_->mockListener(); }

  std::vector<MockAttrSupplierTestStub*>& mockAttrSuppliers() {
    return fixture_->mockAttrSuppliers();
  }

  void fillSupplierWithAttributes() { fixture_->fillSupplierWithAttributes(); }

  std::vector<utils::U8String>& supplierToAttrValues(
      const utils::U8String& supplier) {
    return fixture_->supplierToAttrValues(supplier);
  }

  MockNamedStringFormatter* formatter() { return fixture_->formatter(); }
  utils::U8String linkPhrase() { return fixture_->linkPhrase(); }

 protected:
  LinkType* link_type_;
};

auto link_type_test_fixture_helpers = FixtureHelperGenerator::fixtureHelpers<
    LinkTypeFixtureFactory, LinkTypeFixture,
    FixtureHelperGenerator::CopyConstructFixtureHelper,
    FixtureHelperGenerator::CopyAssignmentFixtureHelper,
    FixtureHelperGenerator::MoveConstructFixtureHelper,
    FixtureHelperGenerator::MoveAssignmentFixtureHelper>();
INSTANTIATE_TEST_CASE_P(FixtureSetup, LinkTypeTest,
                        ::testing::ValuesIn(link_type_test_fixture_helpers));

TEST_P(LinkTypeTest,
       should_emit_LinkUpdated_signal_when_attributeChanged) {  // NOLINT
  // Setup fixture
  // Expectations
  auto& mock_listener = mockListener();
  EXPECT_CALL(mock_listener, LinkUpdated()).Times(mockAttrSuppliers().size());

  // Exercise system
  for (auto& supplier : mockAttrSuppliers()) {
    supplier->attributeChanged(nullptr);
  }
}

TEST_F(LinkTypeTest,
       should_clear_the_prototype_itself_will_do_nothing) {  // NOLINT
  // Setup fixture
  FixtureHelper(LinkTypeFixture, fixture);

  // Exercise system
  fixture.link_type.clear();

  // Verify results
  fixture.validateLinkType();
}

TEST_F(
    LinkTypeTest,
    should_clear_non_prototype_link_type_return_to_prototype_state) {  // NOLINT
  // Setup fixture
  FixtureHelper(LinkTypeFixture, proto_fixture);
  LinkTypeFixture fixture_cloned = proto_fixture;
  fixture_cloned.checkSetup();

  // Expectations
  // return to prototype state
  fixture_cloned.link_type_state = proto_fixture.link_type_state;
  EXPECT_CALL(fixture_cloned.mock_listener_, LinkUpdated());

  // Exercise system
  fixture_cloned.link_type.clear();

  // Verify results
  proto_fixture.validateLinkType();
  fixture_cloned.validateLinkType();
}

TEST_F(LinkTypeTest, should_be_an_IVariableResolver) {  // NOLINT
  auto is_variable_resolver =
      std::is_base_of<utils::text::VariableResolver, LinkType>();
  ASSERT_TRUE(is_variable_resolver);
}

TEST_P(
    LinkTypeTest,
    should_lookup_return_all_attr_values_under_the_supplier_with_the_lookuped_name) {  // NOLINT
  fillSupplierWithAttributes();

  // Verify results
  for (auto& supplier : mockAttrSuppliers()) {
    auto var_name = supplier->name();
    auto expect_values = supplierToAttrValues(var_name);
    auto actual_values = link_type_->lookup(var_name);
    ASSERT_EQ(expect_values, actual_values);
  }

  auto unknown_name = xtestutils::genRandomString();
  auto actual_values = link_type_->lookup(unknown_name);
  ASSERT_TRUE(actual_values.empty());
}

TEST_P(
    LinkTypeTest,
    should_toString_will_call_named_string_formatter_with_link_type_itself_as_variable_resolver) {  // NOLINT
  // Setup fixture
  auto expect_str = xtestutils::genRandomString();

  // Expectations
  EXPECT_CALL(*formatter(), format(linkPhrase(), link_type_))
      .WillOnce(Return(expect_str));

  // Exercise system
  auto actual_str = link_type_->toString();

  // Verify results
  ASSERT_EQ(expect_str, actual_str);
}

}  // namespace tests
}  // namespace snailcore
