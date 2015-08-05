//-*- TestCaseName: WorkAttributeModelTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"
#include "pfmvp/mock_pf_triad_manager.h"
#include "src/utils/utils.h"

#include "src/core/work_attribute_model.h"
#include "core/mock_attribute.h"
#include "snail/mock_attribute_model.h"
#include "core/mock_attribute_supplier.h"
#include "snail/mock_attribute_model_factory.h"
#include "utils/mock_relay_command_factory.h"
#include "snail/mock_attribute_display_block.h"
#include "utils/mock_command.h"

using namespace utils;  // NOLINT
using namespace utils::tests;  // NOLINT
using namespace pfmvp;  // NOLINT
using namespace pfmvp::tests;  // NOLINT

namespace snailcore {
namespace tests {

class WorkAttributeModelTest : public ::testing::Test {
 protected:
  WorkAttributeModelTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~WorkAttributeModelTest() { }
  virtual void SetUp() { }
  // virtual void TearDown() { }

  std::unique_ptr<IWorkAttributeModel> createWorkAttributeModel(
      const std::vector<IAttributeSupplier*>& attr_supplier_list);

  // region: objects test subject depends on
  MockAttributeModelFactory attr_model_factory;
  // endregion

  // region: test subject
  // std::unique_ptr<IWorkAttributeModel> model;
  // endregion

  // region: object depends on test subject
  // endregion
};

std::unique_ptr<IWorkAttributeModel>
WorkAttributeModelTest::createWorkAttributeModel(
    const std::vector<IAttributeSupplier*>& attr_supplier_list) {
  return utils::make_unique<WorkAttributeModel>(attr_supplier_list,
                                                attr_model_factory);
}

TEST_F(WorkAttributeModelTest,
       should_isEditMode_initially_be_false) { // NOLINT
  auto model = createWorkAttributeModel({});
  // Verify results
  ASSERT_FALSE(model->isEditMode());
}

namespace {

class AttributeTestStub : public IAttribute {
 public:
  AttributeTestStub(const utils::U8String& name, bool empty)
      : name_(name)
      , empty_(empty) { }
  virtual ~AttributeTestStub() = default;

  utils::U8String displayName() const override {
    return name_;
  }

  void setDisplayName(const utils::U8String& name) {
    name_ = name;
  }

  bool isEmpty() const override {
    return empty_;
  }

  void setEmpty(bool empty) {
    empty_ = empty;
  }

  void clear() override {
    empty_ = true;
  }

 private:
  utils::U8String name_;
  bool empty_ { true };

 private:
  SNAIL_DISABLE_COPY(AttributeTestStub)
};

class AttributeSupplierTestStub : public IAttributeSupplier {
 public:
  AttributeSupplierTestStub(
      const MockAttributeModelFactory& attr_model_factory,
      int num_attrs, int max_attrs, bool edit_mode)
      : attr_model_factory_(attr_model_factory)
      , name_(xtestutils::genRandomString())
      , max_attrs_(max_attrs)
      , edit_mode_(edit_mode) {
    for (int i = 0; i < num_attrs; ++i) {
      addMockAttribute();
    }
  }
  virtual ~AttributeSupplierTestStub() = default;

  //////////////// IAttributeSupplier Impl ////////////////
  utils::U8String name() const override {
    return name_;
  }
  int attr_count() const override { return num_attrs_; }
  int min_attrs() const override { return 0; }
  int max_attrs() const override { return max_attrs_; }

  std::vector<IAttribute*> attributes() const override {
    std::vector<IAttribute*> attrs;
    for (auto& attr : attrs_) {
      attrs.push_back(attr.get());
    }

    return attrs;
  }

  IAttribute* addAttribute() override {
    return addMockAttribute(name_, true);
  }

  void removeAttribute(IAttribute* attr) override {
    do_removeAttribute(attr);
  }

  //////////////// IAttributeSupplier Impl end ////////////////

  void set_name(const utils::U8String& name) {
    name_ = name;
  }

  AttributeTestStub* addMockAttribute() {
    return addMockAttribute(name_, false);
  }

  AttributeTestStub* addMockAttribute(
      const utils::U8String& attr_display_name, bool empty = false) {
    ++num_attrs_;

    [this]() {
      ASSERT_LE(num_attrs_, max_attrs_);
    }();

    auto attr = utils::make_unique<AttributeTestStub>(
        attr_display_name, empty);
    auto attr_ptr = attr.get();

    attrs_.push_back(std::move(attr));

    return attr_ptr;
  }

  AttributeGroupDisplayBlock getGroupBlock() {
    if (max_attrs_ <= 1)
      throw std::logic_error("max_attrs <= 1 has no group block");

    AttributeGroupDisplayBlock group_block;

    group_block.label = name_;
    group_block.sub_attr_count = num_attrs_;
    group_block.view_priv_data = view_priv_data_;

    return group_block;
  }

  AttributeDisplayBlock getAttrBlock(IAttribute* attr) {
    if (!isAttributeExist(attr))
      throw std::logic_error("attr not exist");

    auto iter = attr_to_block.find(attr);
    if (iter != attr_to_block.end())
      return iter->second;

    AttributeDisplayBlock attr_block;

    attr_block.label = attr->displayName();
    attr_block.edit_mode = edit_mode_;

    auto attr_model = std::make_shared<MockAttributeModel>();
    attr_block.attr_model = attr_model;

    EXPECT_CALL(*attr_model, displayName())
        .WillRepeatedly(
            Invoke([attr]() {
                return attr->displayName();
              }));

    attr_block.is_in_group = (max_attrs_ > 1);

    attr_to_block[attr] = attr_block;

    EXPECT_CALL(attr_model_factory_, createModel(attr))
        .Times(AtLeast(1))
        .WillRepeatedly(
            Invoke(this,
                   &AttributeSupplierTestStub::getAttrModelForAttr));

    return attr_block;
  }

  void traverse(MockAttributeDisplayBlockVisitor* attr_visitor,
                Sequence *seq) {
    if (max_attrs_ > 1) {
      auto priv_data =
          xtestutils::genDifferentDummyPointer<void>(view_priv_data_);
      EXPECT_CALL(*attr_visitor,
                  visitAttributeGroupDisplayBlock(getGroupBlock()))
          .WillOnce(Return(priv_data))
          .InSequence(*seq);
      view_priv_data_ = priv_data;
    }

    for (auto & attr : attributes()) {
      auto attr_block = getAttrBlock(attr);
      auto priv_data =
          xtestutils::genDifferentDummyPointer<void>(attr_block.view_priv_data);
      EXPECT_CALL(*attr_visitor,
                  visitAttributeDisplayBlock(attr_block))
          .WillOnce(Return(priv_data))
          .InSequence(*seq);

      auto new_block = attr_block;
      new_block.view_priv_data = priv_data;
      attr_to_block[attr] = new_block;
    }
  }

  MOCK_METHOD1(attrRemoved, void(IAttribute* attr));

 private:
  using AttrElemType = std::unique_ptr<AttributeTestStub>;

  std::shared_ptr<IAttributeModel> getAttrModelForAttr(IAttribute* attr) {
    if (!isAttributeExist(attr))
      std::cout << "attr " << attr
                << " is not exist in this group" << std::endl;

    auto iter = attr_model_created.find(attr);
    if (iter != attr_model_created.end()) {
      throw std::logic_error(
          utils::formatString("attr_model for {1} is already created", attr));
    }

    attr_model_created[attr] = true;

    auto attr_block = attr_to_block.at(attr);
    return attr_block.attr_model;
  }

  bool isAttributeExist(IAttribute* attr) {
    auto iter = std::find_if(attrs_.begin(), attrs_.end(),
                             [attr](const AttrElemType& item) {
                               return item.get() == attr;
                             });
    return iter != attrs_.end();
  }

  void do_removeAttribute(IAttribute* attr) {
    auto iter = std::find_if(attrs_.begin(), attrs_.end(),
                             [attr](const AttrElemType& item) {
                               return item.get() == attr;
                             });
    if (iter != attrs_.end()) {
      attrs_.erase(iter);

      attr_to_block.erase(attr);
      attr_model_created.erase(attr);
      --num_attrs_;

      attrRemoved(attr);
    }
  }

  const MockAttributeModelFactory& attr_model_factory_;
  utils::U8String name_;
  int num_attrs_ { 0 };
  int max_attrs_;
  bool edit_mode_ { false };

  std::vector<AttrElemType> attrs_;

  std::map<IAttribute*, AttributeDisplayBlock> attr_to_block;
  std::map<IAttribute*, bool> attr_model_created;
  void* view_priv_data_ { nullptr };

 private:
  SNAIL_DISABLE_COPY(AttributeSupplierTestStub)
};

}  // namespace

TEST_F(WorkAttributeModelTest,
       test_AttributeSupplierTestStub_max_attrs_gt_1) { // NOLINT
  int expect_max_attrs = 3;
  AttributeSupplierTestStub supplier(attr_model_factory,
                                     0, expect_max_attrs, false);

  AttributeGroupDisplayBlock expect_group_block;
  expect_group_block.label = supplier.name();

  // Verify results
  // 1. initial state
  ASSERT_EQ(0, supplier.attr_count());
  ASSERT_EQ(expect_max_attrs, supplier.max_attrs());
  ASSERT_EQ(0, supplier.attributes().size());

  expect_group_block.sub_attr_count = 0;
  ASSERT_EQ(expect_group_block, supplier.getGroupBlock());

  // 2. add mock attribute
  auto attr0 = supplier.addMockAttribute();
  ASSERT_EQ(supplier.name(), attr0->displayName());
  ASSERT_FALSE(attr0->isEmpty());
  ASSERT_EQ(1, supplier.attr_count());

  expect_group_block.sub_attr_count = 1;
  ASSERT_EQ(expect_group_block, supplier.getGroupBlock());

  auto attr1 = supplier.addMockAttribute();
  ASSERT_EQ(supplier.name(), attr1->displayName());
  ASSERT_FALSE(attr1->isEmpty());
  ASSERT_EQ(2, supplier.attr_count());

  expect_group_block.sub_attr_count = 2;
  ASSERT_EQ(expect_group_block, supplier.getGroupBlock());

  auto attrs = supplier.attributes();
  ASSERT_EQ(attr0, attrs[0]);
  ASSERT_EQ(attr1, attrs[1]);

  // 3. test getAttrBlock
  auto attr0_block_1st_get = supplier.getAttrBlock(attr0);
  auto attr0_block_2nd_get = supplier.getAttrBlock(attr0);

  // same attr get same attr block
  ASSERT_EQ(attr0_block_1st_get, attr0_block_2nd_get);

  auto attr1_block_1st_get = supplier.getAttrBlock(attr1);
  auto attr1_block_2nd_get = supplier.getAttrBlock(attr1);
  ASSERT_EQ(attr1_block_1st_get, attr1_block_2nd_get);

  // different attr get different attr block
  ASSERT_NE(attr0_block_1st_get, attr1_block_1st_get);

  ASSERT_TRUE(attr0_block_1st_get.is_in_group);
  ASSERT_TRUE(attr1_block_1st_get.is_in_group);

  // 4. tes createModel
  auto attr0_model = attr_model_factory.createModel(attr0);
  ASSERT_EQ(attr0_block_1st_get.attr_model, attr0_model);

  try {
    attr_model_factory.createModel(attr0);
    FAIL() << "should only call createModel for the same attribute once";
  } catch (...) { }

  auto attr1_model = attr_model_factory.createModel(attr1);
  ASSERT_EQ(attr1_block_1st_get.attr_model, attr1_model);

  try {
    attr_model_factory.createModel(attr1);
    FAIL() << "should only createModel for the same attribute once";
  } catch (...) { }

  // 5. test removeAttribute
  supplier.removeAttribute(attr0);
  ASSERT_EQ(1, supplier.attr_count());
  ASSERT_EQ(attr1, supplier.attributes()[0]);

  try {
    supplier.getAttrBlock(attr0);
    FAIL() << "get attr of removed attr should failed";
  } catch (...) { }

  try {
    attr_model_factory.createModel(attr0);
    FAIL() << "create attr_model for removed attrs should failed";
  } catch (...) { }
}

TEST_F(WorkAttributeModelTest,
       test_AttributeSupplierTestStub_max_attrs_eq_1) { // NOLINT
  // Setup fixture
  AttributeSupplierTestStub supplier(attr_model_factory,
                                     0, 1, false);
  // Verify results
  auto attr = supplier.addMockAttribute();
  auto attr_block = supplier.getAttrBlock(attr);
  ASSERT_FALSE(attr_block.is_in_group);
  ASSERT_EQ(attr_block.attr_model, attr_model_factory.createModel(attr));

  try {
    supplier.getGroupBlock();
    FAIL() << "max_attrs eq 1 has no group block";
  } catch(...) { }
}

TEST_F(WorkAttributeModelTest,
       should_visit_nothing_when_there_is_no_suppliers) { // NOLINT
  // Setup fixture
  auto model = createWorkAttributeModel({});
  MockAttributeDisplayBlockVisitor attr_visitor;

  // Expectations
  {
    InSequence seq;

    EXPECT_CALL(attr_visitor, beginTraverse(0));
    EXPECT_CALL(attr_visitor, endTraverse(false));
  }

  EXPECT_CALL(attr_visitor, visitAttributeDisplayBlock(_)).Times(0);
  EXPECT_CALL(attr_visitor, visitAttributeGroupDisplayBlock(_)).Times(0);

  // Exercise system
  model->traverseAttributes(&attr_visitor);
}

TEST_F(WorkAttributeModelTest,
       should_visit_nothing_when_suppliers_has_no_attributes) { // NOLINT
  // Setup fixture
  std::vector<IAttributeSupplier*> supplier_list;
  AttributeSupplierTestStub supplier(attr_model_factory,
                                     0, 1, false);
  supplier_list.push_back(&supplier);

  auto model = createWorkAttributeModel(supplier_list);
  MockAttributeDisplayBlockVisitor attr_visitor;

  // Expectations
  {
    InSequence seq;

    EXPECT_CALL(attr_visitor, beginTraverse(0));
    EXPECT_CALL(attr_visitor, endTraverse(false));
  }

  EXPECT_CALL(attr_visitor, visitAttributeDisplayBlock(_)).Times(0);
  EXPECT_CALL(attr_visitor, visitAttributeGroupDisplayBlock(_)).Times(0);

  // Exercise system
  model->traverseAttributes(&attr_visitor);
}

TEST_F(WorkAttributeModelTest,
       should_generate_attr_for_attributes_in_supplier_num_attrs_1_max_attrs_1) { // NOLINT
  // Setup fixture
  std::vector<IAttributeSupplier*> supplier_list;
  AttributeSupplierTestStub supplier(attr_model_factory,
                                     1, 1, false);

  supplier_list.push_back(&supplier);
  auto model = createWorkAttributeModel(supplier_list);

  auto attr = supplier.attributes()[0];
  AttributeDisplayBlock attr_block = supplier.getAttrBlock(attr);


  auto expect_priv_data = xtestutils::genDummyPointer<void>();
  // Expectations

  {
    MockAttributeDisplayBlockVisitor attr_visitor;
    {
      InSequence seq;

      EXPECT_CALL(attr_visitor, beginTraverse(1));
      EXPECT_CALL(attr_visitor, visitAttributeDisplayBlock(attr_block))
          .WillOnce(Return(expect_priv_data));
      EXPECT_CALL(attr_visitor, endTraverse(false));
    }

    EXPECT_CALL(attr_visitor, visitAttributeGroupDisplayBlock(_)).Times(0);

    // Exercise system
    model->traverseAttributes(&attr_visitor);
  }

  // traverse a second time, will got the same data plus the view_priv_data
  // returned by the previous traverse

  attr_block.view_priv_data = expect_priv_data;

  expect_priv_data =
      xtestutils::genDifferentDummyPointer<void>(expect_priv_data);
  {
    MockAttributeDisplayBlockVisitor another_visitor;
    {
      InSequence seq;

      EXPECT_CALL(another_visitor, beginTraverse(1));
      EXPECT_CALL(another_visitor, visitAttributeDisplayBlock(attr_block))
          .WillOnce(Return(expect_priv_data));
      EXPECT_CALL(another_visitor, endTraverse(false));
    }

    model->traverseAttributes(&another_visitor);
  }

  {
    attr_block.view_priv_data = expect_priv_data;

    MockAttributeDisplayBlockVisitor yet_another_visitor;

    {
      InSequence seq;

      EXPECT_CALL(yet_another_visitor, beginTraverse(1));
      EXPECT_CALL(yet_another_visitor, visitAttributeDisplayBlock(attr_block))
          .WillOnce(Return(nullptr));
      EXPECT_CALL(yet_another_visitor, endTraverse(false));
    }

    model->traverseAttributes(&yet_another_visitor);
  }
}

TEST_F(WorkAttributeModelTest,
       should_generate_group_block_for_suppliers_whos_max_attr_gt_1) { // NOLINT
  // Setup fixture
  std::vector<IAttributeSupplier*> supplier_list;
  AttributeSupplierTestStub supplier(attr_model_factory,
                                     1, 2, false);
  supplier_list.push_back(&supplier);
  auto model = createWorkAttributeModel(supplier_list);

  auto group_block = supplier.getGroupBlock();
  auto attr_block = supplier.getAttrBlock(supplier.attributes()[0]);

  auto group_priv_data = xtestutils::genDummyPointer<void>();
  auto attr_priv_data = xtestutils::genDummyPointer<void>();

  // Expectations
  {
    MockAttributeDisplayBlockVisitor attr_visitor;
    {
      InSequence seq;

      EXPECT_CALL(attr_visitor, beginTraverse(2));
      EXPECT_CALL(attr_visitor, visitAttributeGroupDisplayBlock(group_block))
          .WillOnce(Return(group_priv_data));
      EXPECT_CALL(attr_visitor, visitAttributeDisplayBlock(attr_block))
          .WillOnce(Return(attr_priv_data));
      EXPECT_CALL(attr_visitor, endTraverse(false));
    }

    // Exercise system
    model->traverseAttributes(&attr_visitor);
  }

  // traverse a second time, will got the same data plus the view_priv_data
  // returned by the previous traverse
  group_block.view_priv_data = group_priv_data;
  attr_block.view_priv_data = attr_priv_data;

  group_priv_data = xtestutils::genDifferentDummyPointer<void>(group_priv_data);
  attr_priv_data = xtestutils::genDifferentDummyPointer<void>(attr_priv_data);

  {
    MockAttributeDisplayBlockVisitor attr_visitor;
    {
      InSequence seq;

      EXPECT_CALL(attr_visitor, beginTraverse(2));
      EXPECT_CALL(attr_visitor, visitAttributeGroupDisplayBlock(group_block))
          .WillOnce(Return(group_priv_data));
      EXPECT_CALL(attr_visitor, visitAttributeDisplayBlock(attr_block))
          .WillOnce(Return(attr_priv_data));
      EXPECT_CALL(attr_visitor, endTraverse(false));
    }

    // Exercise system
    model->traverseAttributes(&attr_visitor);
  }


  group_block.view_priv_data = group_priv_data;
  attr_block.view_priv_data = attr_priv_data;

  {
    MockAttributeDisplayBlockVisitor attr_visitor;
    {
      InSequence seq;

      EXPECT_CALL(attr_visitor, beginTraverse(2));
      EXPECT_CALL(attr_visitor, visitAttributeGroupDisplayBlock(group_block))
          .WillOnce(Return(group_priv_data));
      EXPECT_CALL(attr_visitor, visitAttributeDisplayBlock(attr_block))
          .WillOnce(Return(attr_priv_data));
      EXPECT_CALL(attr_visitor, endTraverse(false));
    }

    // Exercise system
    model->traverseAttributes(&attr_visitor);
  }
}

TEST_F(WorkAttributeModelTest,
       should_not_generate_group_block_for_supplier_with_max_attrs_gt_1_but_attr_count_is_0) { // NOLINT
  // Setup fixture
  std::vector<IAttributeSupplier*> supplier_list;
  AttributeSupplierTestStub supplier(attr_model_factory,
                                     0, 3, false);
  supplier_list.push_back(&supplier);
  auto model = createWorkAttributeModel(supplier_list);

  MockAttributeDisplayBlockVisitor attr_visitor;
  // Expectations
  {
    InSequence seq;

    EXPECT_CALL(attr_visitor, beginTraverse(0));
    EXPECT_CALL(attr_visitor, endTraverse(false));
  }

  EXPECT_CALL(attr_visitor, visitAttributeGroupDisplayBlock(_)).Times(0);
  EXPECT_CALL(attr_visitor, visitAttributeDisplayBlock(_)).Times(0);

  // Exercise system
  model->traverseAttributes(&attr_visitor);
}

TEST_F(WorkAttributeModelTest,
       should_sort_sub_attrs_by_display_name_for_suppliers_whos_max_attrs_gt_1) { // NOLINT
  // Setup fixture
  std::vector<IAttributeSupplier*> supplier_list;
  AttributeSupplierTestStub supplier(attr_model_factory,
                                     0, 3, false);
  supplier_list.push_back(&supplier);
  auto model = createWorkAttributeModel(supplier_list);

  auto attr_c = supplier.addMockAttribute("c");
  auto attr_b = supplier.addMockAttribute("b");
  auto attr_a = supplier.addMockAttribute("a");

  auto group_block = supplier.getGroupBlock();
  auto attr_block_a = supplier.getAttrBlock(attr_a);
  auto attr_block_b = supplier.getAttrBlock(attr_b);
  auto attr_block_c = supplier.getAttrBlock(attr_c);

  // Expectations
  {
    MockAttributeDisplayBlockVisitor attr_visitor;

    {
      InSequence seq;

      EXPECT_CALL(attr_visitor, beginTraverse(4));
      EXPECT_CALL(attr_visitor, visitAttributeGroupDisplayBlock(group_block))
          .WillOnce(Return(nullptr));
      EXPECT_CALL(attr_visitor, visitAttributeDisplayBlock(attr_block_a))
          .WillOnce(Return(nullptr));
      EXPECT_CALL(attr_visitor, visitAttributeDisplayBlock(attr_block_b))
          .WillOnce(Return(nullptr));
      EXPECT_CALL(attr_visitor, visitAttributeDisplayBlock(attr_block_c))
          .WillOnce(Return(nullptr));
      EXPECT_CALL(attr_visitor, endTraverse(false));
    }

    model->traverseAttributes(&attr_visitor);
  }
}

TEST_F(WorkAttributeModelTest,
       should_supplier_traverse_order_be_the_order_of_the_supplier_list) { // NOLINT
  // Setup fixture
  AttributeSupplierTestStub supplier0(attr_model_factory,
                                      0, 1, false);
  AttributeSupplierTestStub supplier1(attr_model_factory,
                                      0, 3, false);
  AttributeSupplierTestStub supplier2(attr_model_factory,
                                      0, 1, false);

  auto attr0 = supplier0.addMockAttribute();
  auto attr0_block = supplier0.getAttrBlock(attr0);

  auto sub_attr0 = supplier1.addMockAttribute();
  auto sub_attr0_block = supplier1.getAttrBlock(sub_attr0);
  auto sub_attr1 = supplier1.addMockAttribute();
  auto sub_attr1_block = supplier1.getAttrBlock(sub_attr1);

  auto group1_block = supplier1.getGroupBlock();

  auto attr1 = supplier2.addMockAttribute();
  auto attr1_block = supplier2.getAttrBlock(attr1);

  std::vector<IAttributeSupplier*> supplier_list;
  supplier_list.push_back(&supplier0);
  supplier_list.push_back(&supplier1);
  supplier_list.push_back(&supplier2);

  auto model = createWorkAttributeModel(supplier_list);

  // Expectations
  {
    MockAttributeDisplayBlockVisitor attr_visitor;

    {
      InSequence seq;

      EXPECT_CALL(attr_visitor, beginTraverse(5));
      EXPECT_CALL(attr_visitor, visitAttributeDisplayBlock(attr0_block));
      EXPECT_CALL(attr_visitor, visitAttributeGroupDisplayBlock(group1_block));
      EXPECT_CALL(attr_visitor, visitAttributeDisplayBlock(sub_attr0_block));
      EXPECT_CALL(attr_visitor, visitAttributeDisplayBlock(sub_attr1_block));
      EXPECT_CALL(attr_visitor, visitAttributeDisplayBlock(attr1_block));
      EXPECT_CALL(attr_visitor, endTraverse(false));
    }

    model->traverseAttributes(&attr_visitor);
  }
}

BEGIN_MOCK_LISTENER_DEF(MockListener, IWorkAttributeModel)

MOCK_METHOD0(AttributesChanged, void());
MOCK_METHOD1(AttrLabelChanged, void(UpdateAttrLabelData label_data));

BEGIN_BIND_SIGNAL(IWorkAttributeModel)

BIND_SIGNAL0(AttributesChanged, void);
BIND_SIGNAL1(AttrLabelChanged, void, UpdateAttrLabelData, label_data);

END_BIND_SIGNAL()
END_MOCK_LISTENER_DEF()

TEST_F(WorkAttributeModelTest,
       should_fire_AttributesChanged_when_switch_from_display_mode_to_edit_mode) { // NOLINT
  // Setup fixture
  auto model = createWorkAttributeModel({});

  // Expectations
  auto mock_listener = MockListener::attachTo(model.get());
  EXPECT_CALL(*mock_listener, AttributesChanged());

  // Exercise system
  model->switchToEditMode();

  // Verify result
  ASSERT_TRUE(model->isEditMode());
}

TEST_F(WorkAttributeModelTest,
       should_generate_group_block_with_add_command_for_supplier_with_max_attrs_gt_1_and_attr_count_lt_max_attrs_in_edit_mode) { // NOLINT
  // Setup fixture
  std::vector<IAttributeSupplier*> supplier_list;
  AttributeSupplierTestStub supplier(attr_model_factory,
                                     1, 3, true);
  supplier_list.push_back(&supplier);
  auto model = createWorkAttributeModel(supplier_list);
  model->switchToEditMode();

  auto shared_cmd_ptr = std::make_shared<MockCommand>();
  auto shared_cmd_raw_ptr = shared_cmd_ptr.get();

  // Expectations
  auto expect_group_block = supplier.getGroupBlock();
  AttributeGroupDisplayBlock actual_group_block;

  {
    MockAttributeDisplayBlockVisitor attr_visitor;
    {
      InSequence seq;

      EXPECT_CALL(attr_visitor, beginTraverse(2));
      EXPECT_CALL(attr_visitor, visitAttributeGroupDisplayBlock(_))
          .WillOnce(DoAll(SaveArg<0>(&actual_group_block), Return(nullptr)));
      EXPECT_CALL(attr_visitor, visitAttributeDisplayBlock(_));
      EXPECT_CALL(attr_visitor, endTraverse(false));
    }

    // Exercise system
    model->traverseAttributes(&attr_visitor);
  }

  auto add_command = actual_group_block.add_command;
  ASSERT_NE(nullptr, add_command);
  expect_group_block.add_command = add_command;
  ASSERT_EQ(expect_group_block, actual_group_block);

  // traverse a second time will got the same result
  {
    MockAttributeDisplayBlockVisitor attr_visitor;
    {
      InSequence seq;

      EXPECT_CALL(attr_visitor, beginTraverse(2));
      EXPECT_CALL(attr_visitor, visitAttributeGroupDisplayBlock(expect_group_block))
          .WillOnce(Return(nullptr));
      EXPECT_CALL(attr_visitor, visitAttributeDisplayBlock(_));
      EXPECT_CALL(attr_visitor, endTraverse(false));
    }

    // Exercise system
    model->traverseAttributes(&attr_visitor);
  }

  // Verify result
  Mock::VerifyAndClearExpectations(shared_cmd_raw_ptr);
}

TEST_F(WorkAttributeModelTest,
       should_generate_group_block_without_add_command_for_supplier_with_max_attrs_gt_1_and_attr_count_eq_max_attrs_in_edit_mode) { // NOLINT
  // Setup fixture
  std::vector<IAttributeSupplier*> supplier_list;
  AttributeSupplierTestStub supplier(attr_model_factory,
                                     2, 2, true);
  supplier_list.push_back(&supplier);
  auto model = createWorkAttributeModel(supplier_list);
  model->switchToEditMode();

  auto group_block = supplier.getGroupBlock();
  ASSERT_EQ(nullptr, group_block.add_command);

  // Expectations
  MockAttributeDisplayBlockVisitor attr_visitor;
  {
    InSequence seq;

    EXPECT_CALL(attr_visitor, beginTraverse(3));
    EXPECT_CALL(attr_visitor, visitAttributeGroupDisplayBlock(group_block));
    EXPECT_CALL(attr_visitor, visitAttributeDisplayBlock(_));
    EXPECT_CALL(attr_visitor, visitAttributeDisplayBlock(_));
    EXPECT_CALL(attr_visitor, endTraverse(false));
  }

  // Exercise system
  model->traverseAttributes(&attr_visitor);
}

TEST_F(WorkAttributeModelTest,
       DISABLED_should_generate_group_block_with_add_command_for_supplier_with_max_attrs_gt_1_and_attr_count_eq_0) { // NOLINT
  // Setup fixture

  // Expectations

  // Exercise system

  // Verify results

  // Teardown fixture
  //  * ensuring runs even when the test is failed
  //  * do not introduce additional errors
  FAIL() << "Unfinished Test!";
}

TEST_F(WorkAttributeModelTest,
       DISABLED_should_attr_block_edit_mode_be_true_when_generate_attr_blocks_in_edit_mode) { // NOLINT
  // Setup fixture

  // Expectations

  // Exercise system

  // Verify results

  // Teardown fixture
  //  * ensuring runs even when the test is failed
  //  * do not introduce additional errors
  FAIL() << "Unfinished Test!";
}

TEST_F(WorkAttributeModelTest,
       should_automatically_create_empty_attribute_for_edit_for_suppliers_with_max_attrs_eq_1_and_attr_count_eq_0) { // NOLINT
  // Setup fixture
  std::vector<IAttributeSupplier*> supplier_list;
  AttributeSupplierTestStub supplier(attr_model_factory,
                                     0, 1, true);
  supplier_list.push_back(&supplier);
  auto model = createWorkAttributeModel(supplier_list);
  model->switchToEditMode();

  // Expectations
  MockAttributeDisplayBlockVisitor attr_visitor;

  IAttribute* actual_attr = nullptr;
  auto attr_model = std::make_shared<MockAttributeModel>();
  EXPECT_CALL(attr_model_factory, createModel(_))
      .WillOnce(DoAll(SaveArg<0>(&actual_attr), Return(attr_model)));

  AttributeDisplayBlock attr_block;
  attr_block.label = supplier.name();
  attr_block.edit_mode = true;
  attr_block.attr_model = attr_model;
  attr_block.is_in_group = false;

  {
    InSequence seq;

    EXPECT_CALL(attr_visitor, beginTraverse(1));
    EXPECT_CALL(attr_visitor, visitAttributeDisplayBlock(attr_block));
    EXPECT_CALL(attr_visitor, endTraverse(false));
  }

  // Exercise system
  model->traverseAttributes(&attr_visitor);

  // Verify results
  ASSERT_EQ(1, supplier.attr_count());
  auto expect_attr = supplier.attributes()[0];
  ASSERT_EQ(expect_attr, actual_attr);
}

TEST_F(WorkAttributeModelTest,
       should_not_automatically_create_empty_attribue_for_suppliers_with_max_attrs_gt_1_and_attr_count_eq_0) { // NOLINT
  // Setup fixture
  std::vector<IAttributeSupplier*> supplier_list;
  AttributeSupplierTestStub supplier(attr_model_factory,
                                     0, 2, true);
  supplier_list.push_back(&supplier);
  auto model = createWorkAttributeModel(supplier_list);
  model->switchToEditMode();

  // Expectations
  MockAttributeDisplayBlockVisitor attr_visitor;
  {
    InSequence seq;

    EXPECT_CALL(attr_visitor, beginTraverse(1));
    EXPECT_CALL(attr_visitor, visitAttributeGroupDisplayBlock(_));
    EXPECT_CALL(attr_visitor, endTraverse(false));
  }

  EXPECT_CALL(attr_visitor, visitAttributeDisplayBlock(_)).Times(0);

  // Exercise system
  model->traverseAttributes(&attr_visitor);
}

TEST_F(WorkAttributeModelTest,
       should_add_command_create_an_empty_attribute_and_fire_AttributesChanged_signal) { // NOLINT
  // Setup fixture
  std::vector<IAttributeSupplier*> supplier_list;
  AttributeSupplierTestStub supplier(attr_model_factory,
                                     0, 2, true);
  supplier_list.push_back(&supplier);
  auto model = createWorkAttributeModel(supplier_list);
  model->switchToEditMode();

  // Exercise system
  MockAttributeDisplayBlockVisitor attr_visitor;
  AttributeGroupDisplayBlock group_block;
  {
    InSequence seq;

    EXPECT_CALL(attr_visitor, beginTraverse(1));
    EXPECT_CALL(attr_visitor, visitAttributeGroupDisplayBlock(_))
        .WillOnce(DoAll(SaveArg<0>(&group_block), Return(nullptr)));
    EXPECT_CALL(attr_visitor, endTraverse(false));
  }
  model->traverseAttributes(&attr_visitor);

  // Verify results
  // Expectations
  auto mock_listener = MockListener::attachTo(model.get());
  EXPECT_CALL(*mock_listener, AttributesChanged());

  Command* add_command = group_block.add_command;
  ASSERT_NE(nullptr, add_command);
  add_command->redo();
  // and after called, attributes +1
  ASSERT_EQ(1, supplier.attr_count());
}

TEST_F(WorkAttributeModelTest,
       should_fire_AttrLabelChanged_signal_when_attr_DisplayNameChanged) { // NOLINT
  // Setup fixture
  std::vector<IAttributeSupplier*> supplier_list;
  AttributeSupplierTestStub supplier0(attr_model_factory,
                                      0, 1, true);
  AttributeSupplierTestStub supplier1(attr_model_factory,
                                      0, 1, true);
  supplier_list.push_back(&supplier0);
  supplier_list.push_back(&supplier1);
  auto model = createWorkAttributeModel(supplier_list);
  model->switchToEditMode();

  auto attr0 = supplier0.addMockAttribute();
  auto attr0_block = supplier0.getAttrBlock(attr0);

  auto attr1 = supplier1.addMockAttribute();
  auto attr1_block = supplier1.getAttrBlock(attr1);

  // Expectations
  SlotCatcher<IAttributeModel::DisplayNameChangedSlotType> attr0DisplayNameChanged;
  SlotCatcher<IAttributeModel::DisplayNameChangedSlotType> attr1DisplayNameChanged;

  MockAttributeModel* attr0_model = dynamic_cast<MockAttributeModel*>(attr0_block.attr_model.get());
  MockAttributeModel* attr1_model = dynamic_cast<MockAttributeModel*>(attr1_block.attr_model.get());
  EXPECT_CALL(*attr0_model, whenDisplayNameChanged(_, _))
      .WillOnce(SaveArg<0>(&attr0DisplayNameChanged));
  EXPECT_CALL(*attr1_model, whenDisplayNameChanged(_, _))
      .WillOnce(SaveArg<0>(&attr1DisplayNameChanged));

  auto expect_attr0_priv_data = xtestutils::genDummyPointer<void>();
  auto expect_attr1_priv_data = xtestutils::genDummyPointer<void>();

  // Exercise system
  MockAttributeDisplayBlockVisitor attr_visitor;
  {
    InSequence seq;

    EXPECT_CALL(attr_visitor, beginTraverse(2));
    EXPECT_CALL(attr_visitor, visitAttributeDisplayBlock(attr0_block))
        .WillOnce(Return(expect_attr0_priv_data));
    EXPECT_CALL(attr_visitor, visitAttributeDisplayBlock(attr1_block))
        .WillOnce(Return(expect_attr1_priv_data));
    EXPECT_CALL(attr_visitor, endTraverse(false));
  }

  model->traverseAttributes(&attr_visitor);

  // Verify results
  // Expectations
  UpdateAttrLabelData attr0_update_data;
  attr0_update_data.label = attr0->displayName();
  attr0_update_data.view_priv_data = expect_attr0_priv_data;

  UpdateAttrLabelData attr1_update_data;
  attr1_update_data.label = attr1->displayName();
  attr1_update_data.view_priv_data = expect_attr1_priv_data;

  auto mock_listener = MockListener::attachTo(model.get());
  EXPECT_CALL(*mock_listener, AttrLabelChanged(attr0_update_data));
  EXPECT_CALL(*mock_listener, AttrLabelChanged(attr1_update_data));

  attr0DisplayNameChanged();
  attr1DisplayNameChanged();
}

TEST_F(WorkAttributeModelTest,
       should_not_sort_attributes_during_edit_mode_session) { // NOLINT
  // Setup fixture
  std::vector<IAttributeSupplier*> supplier_list;
  AttributeSupplierTestStub supplier(attr_model_factory,
                                     0, 3, true);
  supplier_list.push_back(&supplier);
  auto model = createWorkAttributeModel(supplier_list);
  model->switchToEditMode();

  auto attr_c = supplier.addMockAttribute("c");
  auto attr_b = supplier.addMockAttribute("b");
  auto attr_a = supplier.addMockAttribute("a");

  auto group_block = supplier.getGroupBlock();
  auto attr_block_a = supplier.getAttrBlock(attr_a);
  auto attr_block_b = supplier.getAttrBlock(attr_b);
  auto attr_block_c = supplier.getAttrBlock(attr_c);

  {
    MockAttributeDisplayBlockVisitor attr_visitor;

    {
      InSequence seq;

      EXPECT_CALL(attr_visitor, beginTraverse(4));
      EXPECT_CALL(attr_visitor, visitAttributeGroupDisplayBlock(group_block));
      EXPECT_CALL(attr_visitor, visitAttributeDisplayBlock(attr_block_c));
      EXPECT_CALL(attr_visitor, visitAttributeDisplayBlock(attr_block_b));
      EXPECT_CALL(attr_visitor, visitAttributeDisplayBlock(attr_block_a));
      EXPECT_CALL(attr_visitor, endTraverse(false));
    }

    model->traverseAttributes(&attr_visitor);
  }
}

TEST_F(WorkAttributeModelTest,
       should_fire_AttributesChanged_signal_with_switch_from_edit_mode_to_display_mode) { // NOLINT
  // Setup fixture
  auto model = createWorkAttributeModel({});
  model->switchToEditMode();

  // Expectations
  auto mock_listener = MockListener::attachTo(model.get());
  EXPECT_CALL(*mock_listener, AttributesChanged());

  // Exercise system
  model->switchToDisplayMode();

  // Verify results
  ASSERT_FALSE(model->isEditMode());
}

TEST_F(WorkAttributeModelTest,
       should_clear_empty_attributes_when_switch_from_edit_mode_to_display_mode) { // NOLINT
  // Setup fixture
  std::vector<IAttributeSupplier*> supplier_list;
  AttributeSupplierTestStub supplier(attr_model_factory,
                                     0, 4, true);
  supplier_list.push_back(&supplier);
  auto model = createWorkAttributeModel(supplier_list);
  model->switchToEditMode();

  MockPfTriadManager triad_manager;
  model->set_triad_manager(&triad_manager);

  auto attr0 = supplier.addMockAttribute("000");
  auto attr1_empty = supplier.addMockAttribute("xxx", true);
  auto attr2 = supplier.addMockAttribute("111");
  auto attr3_empty = supplier.addMockAttribute("yyy", true);

  auto attr0_block = supplier.getAttrBlock(attr0);
  auto attr1_block = supplier.getAttrBlock(attr1_empty);
  auto attr2_block = supplier.getAttrBlock(attr2);
  auto attr3_block = supplier.getAttrBlock(attr3_empty);

  NullAttributeDisplayBlockVisitor null_visitor;
  model->traverseAttributes(&null_visitor);

  // Expectations
  attr0_block.edit_mode = false;
  attr2_block.edit_mode = false;


  {
    bool expect_remove_triad = true;

    StrictMock<MockAttributeDisplayBlockVisitor> attr_visitor;
    {
      InSequence seq;

      EXPECT_CALL(attr_visitor, beginTraverse(3));
      EXPECT_CALL(attr_visitor, visitAttributeGroupDisplayBlock(_));
      EXPECT_CALL(attr_visitor, visitAttributeDisplayBlock(attr0_block));
      EXPECT_CALL(attr_visitor, visitAttributeDisplayBlock(attr2_block));
      EXPECT_CALL(attr_visitor, endTraverse(expect_remove_triad));

      EXPECT_CALL(triad_manager, removeTriadBy(attr1_block.attr_model.get()));
      EXPECT_CALL(supplier, attrRemoved(attr1_empty));

      EXPECT_CALL(triad_manager, removeTriadBy(attr3_block.attr_model.get()));
      EXPECT_CALL(supplier, attrRemoved(attr3_empty));
    }

    // Exercise system
    model->switchToDisplayMode();
    model->traverseAttributes(&attr_visitor);
  }

  {
    // traverse again in display mode will not remove triads
    bool expect_remove_triad = false;

    StrictMock<MockAttributeDisplayBlockVisitor> attr_visitor;
    {
      InSequence seq;

      EXPECT_CALL(attr_visitor, beginTraverse(3));
      EXPECT_CALL(attr_visitor, visitAttributeGroupDisplayBlock(_));
      EXPECT_CALL(attr_visitor, visitAttributeDisplayBlock(attr0_block));
      EXPECT_CALL(attr_visitor, visitAttributeDisplayBlock(attr2_block));
      EXPECT_CALL(attr_visitor, endTraverse(expect_remove_triad));
    }

    // switchToDisplayModel when already in display
    // mode will not set remove_triad to true
    model->switchToDisplayMode();
    model->traverseAttributes(&attr_visitor);
  }
}

}  // namespace tests
}  // namespace snailcore
