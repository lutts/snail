//-*- TestCaseName: WorkTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"

#include "src/core/work.h"

#include "snail/mock_attribute_supplier.h"

namespace snailcore {
namespace tests {

class WorkTest : public ::testing::Test {
 protected:
  WorkTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~WorkTest() { }
  virtual void SetUp() {
    work = utils::make_unique<Work>();

    ASSERT_EQ(utils::U8String{""}, work->name());
  }
  // virtual void TearDown() { }

  // region: objects test subject depends on
  // endregion

  // region: test subject
  std::unique_ptr<Work> work;
  // endregion

  // region: object depends on test subject
  // endregion
};

class MockListener {
 public:
  explicit MockListener(fto::Work* work) {
    work->whenNameChanged([this](const utils::U8String& new_name) {
      NameChanged(new_name);
    }, nullptr);
  }

  MOCK_METHOD1(NameChanged, void(const utils::U8String& new_name));
};

TEST_F(WorkTest, should_be_able_to_set_and_get_name) {  // NOLINT
  // Setup fixture
  auto work_name = xtestutils::genRandomString();

  // Expectations

  // Exercise system
  work->set_name(work_name);

  // Verify results
  ASSERT_EQ(work_name, work->name());
}

TEST_F(WorkTest, should_fire_NameChanged_when_set_a_different_name) {  // NOLINT
  // Setup fixture
  auto new_name = xtestutils::genRandomDifferentString(work->name());

  // Expectations
  // auto mockListener = MockListener::attachTo(work.get());
  MockListener mockListener(work.get());
  EXPECT_CALL(mockListener, NameChanged(new_name));

  // Exercise system
  ASSERT_TRUE(work->set_name(new_name));
}

TEST_F(WorkTest, should_not_fire_NameChnaged_when_set_a_same_name) {  // NOLINT
  // Expectations
  // auto mockListener = MockListener::attachTo(work.get());
  MockListener mockListener(work.get());
  EXPECT_CALL(mockListener, NameChanged(_)).Times(0);

  // Exercise system
  ASSERT_FALSE(work->set_name(work->name()));
}

TEST_F(WorkTest, should_be_able_to_set_and_get_attribute_suppliers) {  // NOLINT
  // Setup fixture
  using AttrSupplierUpVec = std::vector<std::unique_ptr<IAttributeSupplier> >;
  AttrSupplierUpVec expect_attr_suppliers_up_vec;
  std::vector<IAttributeSupplier*> expect_attr_suppliers;

  const int TEST_ATTR_SUPPLIERS_COUNT = 3;
  for (int i = 0; i < TEST_ATTR_SUPPLIERS_COUNT; ++i) {
    std::unique_ptr<IAttributeSupplier> attr_supplier_up =
        utils::make_unique<MockAttributeSupplier>();
    auto raw_attr_supplier = attr_supplier_up.get();

    expect_attr_suppliers_up_vec.push_back(std::move(attr_supplier_up));
    expect_attr_suppliers.push_back(raw_attr_supplier);
  }

  // Exercise system
  work->setAttributeSuppliers(std::move(expect_attr_suppliers_up_vec));
  auto actual_attr_suppliers = work->attributeSuppliers();

  // Verify results
  ASSERT_EQ(expect_attr_suppliers, actual_attr_suppliers);
}

}  // namespace tests
}  // namespace snailcore
