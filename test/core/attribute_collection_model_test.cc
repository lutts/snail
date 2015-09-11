//-*- TestCaseName: AttributeCollectionModelTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"

#include "src/core/attribute_collection_model.h"
#include "snail/mock_attribute_model_factory.h"
#include "snail/mock_attribute_model.h"

namespace snailcore {
namespace tests {

class AttributeCollectionModelTest : public ::testing::Test {
 protected:
  AttributeCollectionModelTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~AttributeCollectionModelTest() { }
  virtual void SetUp() {
    attr_suppliers.push_back(xtestutils::genDummyPointer<IAttributeSupplier>());
    attr_suppliers.push_back(xtestutils::genDummyPointer<IAttributeSupplier>());
    attr_suppliers.push_back(xtestutils::genDummyPointer<IAttributeSupplier>());

    model = utils::make_unique<AttributeCollectionModel>(attr_suppliers,
                                                         attr_model_factory);
  }
  // virtual void TearDown() { }

  // region: objects test subject depends on
  std::vector<IAttributeSupplier*> attr_suppliers;
  MockAttributeModelFactory attr_model_factory;
  // endregion

  // region: test subject
  std::unique_ptr<AttributeCollectionModel> model;
  // endregion

  // region: object depends on test subject
  // endregion
};

TEST_F(AttributeCollectionModelTest, should_construct_properly) { // NOLINT
  // See SetUp()
  SUCCEED();
}

TEST_F(AttributeCollectionModelTest,
       should_be_able_to_get_the_attribute_suppliers) { // NOLINT
  // Exercise system
  auto actual_attr_suppliers = model->getAttributeSuppliers();

  // Verify results
  ASSERT_EQ(attr_suppliers, actual_attr_suppliers);
}

TEST_F(AttributeCollectionModelTest,
       should_be_able_to_create_AttributeModel_for_attr) { // NOLINT
  // Setup fixture
  IAttribute* attr = xtestutils::genDummyPointer<IAttribute>();
  auto attr_model = std::make_shared<MockAttributeModel>();

  // Expectations
  EXPECT_CALL(attr_model_factory, createModel(attr))
      .WillOnce(Return(attr_model));

  // Exercise system
  auto actual_attr_model = model->createAttributeModel(attr);

  // Verify results
  ASSERT_EQ(attr_model, actual_attr_model);
}

}  // namespace tests
}  // namespace snailcore
