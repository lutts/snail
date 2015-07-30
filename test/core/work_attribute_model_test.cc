//-*- TestCaseName: WorkAttributeModelTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"
#include "src/utils/utils.h"

#include "src/core/work_attribute_model.h"
#include "core/mock_attribute_container.h"
#include "core/mock_attribute.h"
#include "snail/mock_attribute_model.h"
#include "core/mock_attribute_supplier.h"
#include "snail/mock_attribute_model_factory.h"
#include "utils/mock_relay_command_factory.h"
#include "snail/mock_attribute_display_block.h"

using namespace utils;  // NOLINT
using namespace utils::tests;  // NOLINT

namespace snailcore {
namespace tests {

class WorkAttributeModelTest : public ::testing::Test {
 protected:
  WorkAttributeModelTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~WorkAttributeModelTest() { }
  virtual void SetUp() {
    setupAttributeContainer();

    RECORD_USED_MOCK_OBJECTS_SETUP;

    model = utils::make_unique<WorkAttributeModel>(
        &attr_container,
        attr_model_factory,
        relay_cmd_factory);

    VERIFY_RECORDED_MOCK_OBJECTS;

    // TODO(lutts): check layout results
  }
  // virtual void TearDown() { }

  void setupAttributeContainer();

  // region: objects test subject depends on
  MockAttributeContainer attr_container;
  MockAttributeModelFactory attr_model_factory;
  MockRelayCommandFactory relay_cmd_factory;
  // endregion

  // region: test subject
  std::unique_ptr<IWorkAttributeModel> model;
  // endregion

  // region: object depends on test subject
  // endregion
};

void WorkAttributeModelTest::setupAttributeContainer() {
}

TEST_F(WorkAttributeModelTest,
       should_isEditMode_initially_be_false) { // NOLINT
  // Verify results
  ASSERT_FALSE(model->isEditMode());
}

namespace {

class AttributeSupplierTestStub : public IAttributeSupplier {
 public:
  explicit AttributeSupplierTestStub(int num_attrs = 1,
                                     int min_attrs = 0,
                                     int max_attrs = 1)
      : num_attrs_(num_attrs)
      , min_attrs_(min_attrs)
      , max_attrs_(max_attrs) {
    for (int i = 0; i < num_attrs; ++i) {
      auto attr = utils::make_unique<MockAttribute>();
      auto attr_name =
          utils::formatString("Attribute Of {1}, No.{2}",
                              this->name(), i);
      ON_CALL(*attr, displayName())
          .WillByDefault(Return(attr_name));

      attrs_.push_back(std::move(attr));
    }
  }
  virtual ~AttributeSupplierTestStub() = default;

  //////////////// IAttributeSupplier Impl ////////////////
  utils::U8String name() const override {
    return xtestutils::genRandomString();
  }
  int num_attributes() const override { return num_attrs_; }
  int min_attrs() const override { return min_attrs_; }
  int max_attrs() const override { return max_attrs_; }

  std::vector<IAttribute*> attributes() const override {
    std::vector<IAttribute*> attrs;
    for (auto& attr : attrs_) {
      attrs.push_back(attr.get());
    }

    return attrs;
  }

#if 0
  IAttribute* createEmptyAttribute() override { return nullptr; }
#endif

  //////////////// IAttributeSupplier Impl end ////////////////

 private:
  int num_attrs_;
  int min_attrs_;
  int max_attrs_;

  std::vector<std::unique_ptr<MockAttribute>> attrs_;

 private:
  SNAIL_DISABLE_COPY(AttributeSupplierTestStub)
};

class TestDataGenerator {
 public:
  explicit TestDataGenerator(MockAttributeModelFactory* attr_model_factory)
      : attr_model_factory_(attr_model_factory) { }
  virtual ~TestDataGenerator() = default;

  void addAttributeSupplier(AttributeSupplierTestStub* attr_supplier) {
    for (auto & attr : attr_supplier->attributes()) {
      auto attr_model = std::make_shared<MockAttributeModel>();
      ON_CALL(*attr_model, displayName())
          .WillByDefault(Return(attr->displayName()));
      // createModel for attr should only be called once
      EXPECT_CALL(*attr_model_factory_, createModel(attr))
          .WillOnce(Return(attr_model));

      attr_blocks_.push_back({attr->displayName(),
              attr_model,
              nullptr, nullptr, false});
    }

    num_display_blocks_ += attr_supplier->num_attributes();
    // TODO(lutts): max > 1 MAY have group
    attr_suppliers_.push_back(attr_supplier);
  }

  std::vector<IAttributeSupplier*> attr_suppliers() const {
    std::vector<IAttributeSupplier*> suppliers;
    for (auto & supplier : attr_suppliers_) {
      suppliers.push_back(supplier);
    }

    return suppliers;
  }

  int num_display_blocks() {
    return num_display_blocks_;
  }

  AttributeDisplayBlock attr_block(int index) {
    return attr_blocks_[index];
  }

 private:
  MockAttributeModelFactory* attr_model_factory_ { nullptr };
  int num_display_blocks_ { 0 };
  std::vector<AttributeSupplierTestStub*> attr_suppliers_;
  std::vector<AttributeDisplayBlock> attr_blocks_;

 private:
  SNAIL_DISABLE_COPY(TestDataGenerator)
};

}  // namespace


TEST_F(WorkAttributeModelTest,
       should_be_able_to_create_AttributeDisplayBlock_for_max_equal_1_attr_suppliers) { // NOLINT
  // Setup fixture
  TestDataGenerator test_data_generator { &attr_model_factory };

  AttributeSupplierTestStub attr_supplier1;
  AttributeSupplierTestStub attr_supplier2;
  AttributeSupplierTestStub attr_supplier3;

  test_data_generator.addAttributeSupplier(&attr_supplier1);
  test_data_generator.addAttributeSupplier(&attr_supplier2);
  test_data_generator.addAttributeSupplier(&attr_supplier3);

  ON_CALL(attr_container, attr_suppliers())
      .WillByDefault(Return(test_data_generator.attr_suppliers()));

  auto tester = [this, &test_data_generator]() {
    // Expectations
    MockAttributeDisplayBlockVisitor attr_visitor;

    int expect_num_display_blocks = test_data_generator.num_display_blocks();
    {
      InSequence seq;

      EXPECT_CALL(attr_visitor,
                  beginAddAttributeDisplayBlock(expect_num_display_blocks));
      for (int i = 0; i < expect_num_display_blocks; ++i) {
        EXPECT_CALL(
            attr_visitor,
            addAttributeDisplayBlock(test_data_generator.attr_block(i)));
      }
      EXPECT_CALL(attr_visitor,
                  endAddAttributeDisplayBlock());
    }

    // Exercise system
    model->traverseAttributes(&attr_visitor);
  };

  // traverse multiple times will got the same result
  tester();
//  tester();
}

}  // namespace tests
}  // namespace snailcore
