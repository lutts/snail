//-*- TestCaseName: AttributeCollectionModelTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <QSignalSpy>

#include "test/testutils/gmock_common.h"

#include "src/qtui/core/attribute_collection_qmodel.h"

#include "snail/mock_attribute.h"
#include "snail/mock_attribute_supplier.h"

#include "utils/signal_slot_impl.h"

using namespace snailcore;  // NOLINT
using namespace snailcore::tests;  // NOLINT

class AttributeTestStub :public IAttribute {
 public:
  AttributeTestStub(const utils::U8String& name,
                    const utils::U8String& value,
                    IAttributeSupplier* supplier)
      : name_(name)
      , value_(value)
      , supplier_(supplier) { }
  virtual ~AttributeTestStub() = default;

  utils::U8String displayName() const override {
    return name_;
  }

  void setDisplayName(const utils::U8String& name) {
    name_ = name;
    supplier_->attributeChanged(this);
  }

  utils::U8String valueText() const override {
    return value_;
  }

  bool isEmpty() const override {
    return value_.empty();
  }

  void clear() override {
    value_.clear();
  }

 private:
  SNAIL_DISABLE_COPY(AttributeTestStub);

  utils::U8String name_;
  utils::U8String value_;
  IAttributeSupplier* supplier_;
};


class AttrSupplierTestStub : public IAttributeSupplier {
 public:
  AttrSupplierTestStub(const utils::U8String& name,
                       int max_attrs)
      : name_(name), max_attrs_(max_attrs) { }
  virtual ~AttrSupplierTestStub() {
    for (auto attr : attributes_) {
      delete attr;
    }
  }

  utils::U8String name() const override {
    return name_;
  }

  int max_attrs() const override {
    return max_attrs_;
  }

  int attr_count() const override {
    return attributes_.size();
  }

  std::vector<IAttribute*> attributes() const override {
    return attributes_;
  }

  IAttribute* addAttribute() override {
    addAttributeCalled();
    return addAttribute(name_, "");
  }

  void removeAttribute(IAttribute* attr) override {
    auto iter = std::find(begin(attributes_),
                          end(attributes_),
                          attr);
    if (iter != end(attributes_)) {
      attributes_.erase(iter);
      attributeRemoved(attr);
    }
  }

  AttributeTestStub* addAttribute(const utils::U8String& attr_name,
                                  const utils::U8String& attr_value) {
    if (attributes_.size() > static_cast<size_t>(max_attrs_))
      return nullptr;

    auto attr = new AttributeTestStub(attr_name, attr_value, this);
    attributes_.push_back(attr);

    return attr;
  }

  void attributeChanged(IAttribute* attr) override {
    AttributeChanged(attr);
  }

  virtual void addAttributeCalled() = 0;
  virtual void attributeRemoved(IAttribute* attr) = 0;


 private:
  SNAIL_SIGSLOT_IMPL(AttributeChanged);

 private:
  SNAIL_DISABLE_COPY(AttrSupplierTestStub);

  utils::U8String name_;
  int max_attrs_;
  std::vector<IAttribute*> attributes_;
};

class MockAttrSupplierTestStub : public AttrSupplierTestStub {
 public:
  MockAttrSupplierTestStub(const utils::U8String& name,
                       int max_attrs)
      : AttrSupplierTestStub(name, max_attrs) { }
  virtual ~MockAttrSupplierTestStub() = default;

  MOCK_METHOD0(addAttributeCalled, void());
  MOCK_METHOD1(attributeRemoved, void(IAttribute* attr));
};


class AttributeCollectionModelTest : public ::testing::Test {
 protected:
  AttributeCollectionModelTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~AttributeCollectionModelTest() { }
  virtual void SetUp() {
    qmodel = utils::make_trackable<AttributeCollectionQModel>();
    setupAttributeSuppliers();
  }
  // virtual void TearDown() { }

  void setupAttributeSuppliers();
  void assertCellContentEqual(int row, int column,
                              const utils::U8String& expect_text);
  void checkRowData(std::vector<utils::U8String> expect_row_data[],
                    int expect_row_count);
  void assertCellNotSelectable(int row, int column);
  void assertCellNotEditable(int row, int column);
  void assertCellEditable(int row, int column);
  void switchToEditMode();
  void switchToDisplayMode();

  void checkClickOnAddAttribute(int row, MockAttrSupplierTestStub* supplier,
                                std::vector<utils::U8String> expect_row_data[],
                                int expect_row_count,
                                bool expect_remove_supplier_row);


  // region: objects test subject depends on
  MockObjectGenerator<MockAttrSupplierTestStub> attr_supplier_generator; // NOLINT
  MockAttrSupplierTestStub* max1_attr0_supplier { nullptr };
  MockAttrSupplierTestStub* attr_supplier_with_to_be_cleared_attrs { nullptr };
  MockAttrSupplierTestStub* max_gt_1_attrcount_eq_max_minus_1_supplier { nullptr }; // NOLINT
  MockAttrSupplierTestStub* max_gt_1_attrcount_eq_max_minus_2_supplier { nullptr }; // NOLINT
  MockAttrSupplierTestStub* max_gt_1_attr_eq_max_supplier { nullptr };
  AttributeTestStub* attr_to_be_clear { nullptr };
  AttributeTestStub* attr_to_change { nullptr };
  // endregion

  // region: test subject
  std::shared_ptr<AttributeCollectionQModel> qmodel;
  // endregion

  // region: object depends on test subject
  // endregion
};

void AttributeCollectionModelTest::setupAttributeSuppliers() {
  std::vector<IAttributeSupplier*> attr_suppliers;
  AttrSupplierTestStub* attr_supplier;

  max1_attr0_supplier =
      attr_supplier_generator.generate("Supplier_Max_Eq_1_AttrCount_Eq_0", 1);
  attr_suppliers.push_back(max1_attr0_supplier);

  attr_supplier =
      attr_supplier_generator.generate("Supplier_Max_Eq_1_AttrCount_Eq_1", 1);
  attr_supplier->addAttribute("Attribute 01", "Value 1");
  attr_suppliers.push_back(attr_supplier);

  attr_supplier =
      attr_supplier_generator.generate("Supplier_Max_Gt_1_AttrCount_Eq_0", 3);
  attr_suppliers.push_back(attr_supplier);

  max_gt_1_attrcount_eq_max_minus_1_supplier =
      attr_supplier_generator.generate(
          "Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_1", 3);
  attr_supplier = max_gt_1_attrcount_eq_max_minus_1_supplier;
  attr_supplier->addAttribute("Attribute 02", "Value 2");
  attr_to_change = attr_supplier->addAttribute("Attribute 03", "Value 3");
  attr_suppliers.push_back(attr_supplier);

  max_gt_1_attrcount_eq_max_minus_2_supplier =
      attr_supplier_generator.generate(
          "Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_2", 3);
  attr_supplier = max_gt_1_attrcount_eq_max_minus_2_supplier;
  attr_supplier->addAttribute("Attribute 04", "Value 4");
  attr_suppliers.push_back(attr_supplier);

  max_gt_1_attr_eq_max_supplier =
      attr_supplier_generator.generate("Supplier_Max_Gt_1_AttrCount_Eq_Max", 3);
  attr_supplier = max_gt_1_attr_eq_max_supplier;
  attr_supplier->addAttribute("Attribute 05", "Value 5");
  attr_supplier->addAttribute("Attribute 06", "Value 6");
  attr_supplier->addAttribute("Attribute 07", "Value 7");
  attr_suppliers.push_back(attr_supplier);

  attr_supplier_with_to_be_cleared_attrs =
      attr_supplier_generator.generate("Supplier_With_Duplicated_Named_Attrs",
                                       4);
  attr_supplier = attr_supplier_with_to_be_cleared_attrs;
  attr_supplier->addAttribute("Attribute 08", "Value 8-1");
  attr_to_be_clear =
      attr_supplier->addAttribute("Attribute 09", "Value 9-1-Clear");
  attr_supplier->addAttribute("Attribute 08", "Value 8-2");
  attr_supplier->addAttribute("Attribute 09", "Value 9-2");
  attr_suppliers.push_back(attr_supplier);

  qmodel->setAttributeSuppliers(attr_suppliers);
}

// attribute supplier rows is never showd in display mode
std::vector<utils::U8String> expect_on_display_mode[] = {
  { "Attribute 01", "Value 1" },

  { "Attribute 02", "Value 2" },
  { "Attribute 03", "Value 3" },

  { "Attribute 04", "Value 4" },

  { "Attribute 05", "Value 5" },
  { "Attribute 06", "Value 6" },
  { "Attribute 07", "Value 7" },

  // sorted and duplicate attr name is hidded
  { "Attribute 08", "Value 8-1" },
  { "",            "Value 8-2" },
  { "Attribute 09", "Value 9-1-Clear" },
  { "",            "Value 9-2" },
};

// empty attributes is removed whenswitch from edit mode to display mode
std::vector<utils::U8String>
expect_on_display_mode_after_switch_from_edit_mode[] = {
  { "Attribute 01", "Value 1" },

  { "Attribute 02", "Value 2" },
  { "Attribute 03", "Value 3" },

  { "Attribute 04", "Value 4" },

  { "Attribute 05", "Value 5" },
  { "Attribute 06", "Value 6" },
  { "Attribute 07", "Value 7" },

  // sorted and duplicate attr name is hidded
  { "Attribute 08", "Value 8-1" },
  { "",            "Value 8-2" },
  { "Attribute 09", "Value 9-2" },
};

// attribute suppliers whos attr_count < max & max > 1 is showed in edit mode
// attributes are not sorted by name and name are never hidden
std::vector<utils::U8String> expect_on_edit_mode[] = {
  { "Supplier_Max_Eq_1_AttrCount_Eq_0", "" },  // 0

  { "Attribute 01", "Value 1" },

  { "Add Supplier_Max_Gt_1_AttrCount_Eq_0", ""},

  { "Attribute 02", "Value 2" },
  { "Attribute 03", "Value 3" },
#define ROW_WITH_ATTRIBUTE 4
#define ROW_WITH_SUPPLIER_ATTRCOUNT_EQ_MAX_MINUS_1  5
  { "Add Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_1", "" },  // 5

  { "Attribute 04", "Value 4" },
#define ROW_WITH_SUPPLIER_ATTRCOUNT_EQ_MAX_MINUS_2  7
  { "Add Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_2", "" },

  { "Attribute 05", "Value 5" },
  { "Attribute 06", "Value 6" },
  { "Attribute 07", "Value 7" },  // 10

  { "Attribute 08", "Value 8-1" },
  { "Attribute 09", "Value 9-1-Clear" },
  { "Attribute 08", "Value 8-2" },
  { "Attribute 09", "Value 9-2" },
};

// after add ,the supplier which becomes full will not show "Add ..." row
std::vector<utils::U8String> expect_on_edit_mode_after_click_add_on_attr_count_eq_max_minus_1_supplier[] = { // NOLINT
  { "Supplier_Max_Eq_1_AttrCount_Eq_0", "" },

  { "Attribute 01", "Value 1" },

  { "Add Supplier_Max_Gt_1_AttrCount_Eq_0", ""},

  { "Attribute 02", "Value 2" },
  { "Attribute 03", "Value 3" },
  { "Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_1", "" },

  { "Attribute 04", "Value 4" },
  { "Add Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_2", "" },

  { "Attribute 05", "Value 5" },
  { "Attribute 06", "Value 6" },
  { "Attribute 07", "Value 7" },

  { "Attribute 08", "Value 8-1" },
  { "Attribute 09", "Value 9-1-Clear" },
  { "Attribute 08", "Value 8-2" },
  { "Attribute 09", "Value 9-2" },
};

// after add attr, if the supplier is not full, its "Add ..." row is remained
std::vector<utils::U8String> expect_on_edit_mode_after_click_add_on_attr_count_eq_max_minus_2_supplier[] = {
  { "Supplier_Max_Eq_1_AttrCount_Eq_0", "" },

  { "Attribute 01", "Value 1" },

  { "Add Supplier_Max_Gt_1_AttrCount_Eq_0", ""},

  { "Attribute 02", "Value 2" },
  { "Attribute 03", "Value 3" },
  { "Add Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_1", "" },

  { "Attribute 04", "Value 4" },
  { "Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_2", "" },
  { "Add Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_2", "" },

  { "Attribute 05", "Value 5" },
  { "Attribute 06", "Value 6" },
  { "Attribute 07", "Value 7" },  // 10

  { "Attribute 08", "Value 8-1" },
  { "Attribute 09", "Value 9-1-Clear" },
  { "Attribute 08", "Value 8-2" },
  { "Attribute 09", "Value 9-2" },
};

TEST_F(AttributeCollectionModelTest,
       should_column_count_be_2) { // NOLINT
  ASSERT_EQ(2, qmodel->columnCount());
}

void AttributeCollectionModelTest::assertCellContentEqual(
    int row, int column, const utils::U8String& expect_text) {
  auto index = qmodel->index(row, column);
  auto data = qmodel->data(index, Qt::DisplayRole);
  auto actual_text = QStringToU8String(data.toString());

  ASSERT_EQ(expect_text, actual_text)
      << "cell (" << row << ", " << column << ") text not match";
}

void AttributeCollectionModelTest::checkRowData(
    std::vector<utils::U8String> expect_row_data[], int expect_row_count) {
  int actual_row_count = qmodel->rowCount();
  ASSERT_EQ(expect_row_count, actual_row_count);

  int attr_row_count = qmodel->attrRowCount();
  ASSERT_EQ(expect_row_count, attr_row_count);

  for (int row = 0; row < expect_row_count; ++row) {
    for (int col = 0; col < 2; ++col) {
      CUSTOM_ASSERT(assertCellContentEqual(row, col,
                                           expect_row_data[row][col]));
    }
  }
}

TEST_F(AttributeCollectionModelTest,
       check_row_data_in_display_mode) { // NOLINT
  CUSTOM_ASSERT(checkRowData(expect_on_display_mode,
                             utils::sizeof_array(expect_on_display_mode)));
}

void AttributeCollectionModelTest::assertCellNotSelectable(
    int row, int column) {
  auto index = qmodel->index(row, column);
  auto flags = qmodel->flags(index);

  ASSERT_FALSE(flags & Qt::ItemIsSelectable);
}

void AttributeCollectionModelTest::assertCellNotEditable(
    int row, int column) {
  auto index = qmodel->index(row, column);
  auto flags = qmodel->flags(index);

  ASSERT_FALSE(flags & Qt::ItemIsEditable);
}

void AttributeCollectionModelTest::assertCellEditable(int row, int column) {
  auto index = qmodel->index(row, column);
  auto flags = qmodel->flags(index);

  ASSERT_TRUE(flags & Qt::ItemIsEditable);
}

TEST_F(AttributeCollectionModelTest,
       should_not_editable_in_display_mode) { // NOLINT
  int row_count = qmodel->rowCount();
  for (int row = 0; row < row_count; ++row) {
    CUSTOM_ASSERT(assertCellNotSelectable(row, 0));
    CUSTOM_ASSERT(assertCellNotEditable(row, 0));

    CUSTOM_ASSERT(assertCellNotSelectable(row, 1));
    CUSTOM_ASSERT(assertCellNotEditable(row, 1));
  }
}

void AttributeCollectionModelTest::switchToEditMode() {
  QSignalSpy aboutToReset_sigspy(qmodel.get(), SIGNAL(modelAboutToBeReset()));
  QSignalSpy reset_sigspy(qmodel.get(), SIGNAL(modelReset()));

  // Expectations
  EXPECT_CALL(*max1_attr0_supplier, addAttributeCalled());

  // Exercise system
  qmodel->switchToEditMode();

  // Verify results
  ASSERT_EQ(1, aboutToReset_sigspy.count());
  ASSERT_EQ(1, reset_sigspy.count());
}

void AttributeCollectionModelTest::switchToDisplayMode() {
  QSignalSpy aboutToReset_sigspy(qmodel.get(), SIGNAL(modelAboutToBeReset()));
  QSignalSpy reset_sigspy(qmodel.get(), SIGNAL(modelReset()));

  // Exercise system
  qmodel->switchToDisplayMode();

  // Verify results
  ASSERT_EQ(1, aboutToReset_sigspy.count());
  ASSERT_EQ(1, reset_sigspy.count());
}

TEST_F(AttributeCollectionModelTest,
       should_switch_to_EditMode_create_empty_attributes_for_Supplier_Max_Eq_1_AttrCount_Eq_0) { // NOLINT
  CUSTOM_ASSERT(switchToEditMode());
}

TEST_F(AttributeCollectionModelTest,
       check_row_data_in_edit_mode) { // NOLINT
  // Setup fixture
  switchToEditMode();

  // Verify results
  CUSTOM_ASSERT(checkRowData(expect_on_edit_mode,
                             utils::sizeof_array(expect_on_edit_mode)));
}

TEST_F(AttributeCollectionModelTest,
       should_column_0_not_editor_in_edit_mode) { // NOLINT
  // Setup fixture
  switchToEditMode();

  // Verify results
  int row_count = qmodel->rowCount();
  for (int row = 0; row < row_count; ++row) {
    CUSTOM_ASSERT(assertCellNotSelectable(row, 0));
    CUSTOM_ASSERT(assertCellNotEditable(row, 0));
  }
}

TEST_F(AttributeCollectionModelTest,
       should_column_1_editable_in_edit_mode) { // NOLINT
  // Setup fixture
  switchToEditMode();

  // Verify results
  int row_count = qmodel->rowCount();
  for (int row = 0; row < row_count; ++row) {
    CUSTOM_ASSERT(assertCellNotSelectable(row, 1));
    CUSTOM_ASSERT(assertCellEditable(row, 1));
  }
}

TEST_F(AttributeCollectionModelTest,
       should_remove_empty_attributes_when_switch_from_edit_mode_to_display_mode) { // NOLINT
  // Setup fixture
  switchToEditMode();

    // Expectations
  IAttribute* attr = max1_attr0_supplier->attributes()[0];
  EXPECT_CALL(*max1_attr0_supplier, attributeRemoved(attr));

  attr_to_be_clear->clear();
  EXPECT_CALL(*attr_supplier_with_to_be_cleared_attrs,
              attributeRemoved(attr_to_be_clear));

  // Exercise system
  switchToDisplayMode();

  // Verify results
  int row_count =
      utils::sizeof_array(expect_on_display_mode_after_switch_from_edit_mode);
  CUSTOM_ASSERT(checkRowData(expect_on_display_mode_after_switch_from_edit_mode,
                             row_count));
}

BEGIN_MOCK_LISTENER_DEF(MockListener, IAttributeCollectionQModel)

MOCK_METHOD1(AttributeAdded, void(int row));

BEGIN_BIND_SIGNAL(IAttributeCollectionQModel)

BIND_SIGNAL1(AttributeAdded, void, int, row);

END_BIND_SIGNAL()
END_MOCK_LISTENER_DEF()

void checkRowsInsertRemoveSigspy(QSignalSpy* sigspy, int expect_row) {
  ASSERT_EQ(1, sigspy->count());
  QList<QVariant> arguments = sigspy->takeFirst();
  ASSERT_TRUE(arguments.at(0).type() == QVariant::ModelIndex);
  ASSERT_TRUE(arguments.at(1).type() == QVariant::Int);
  ASSERT_TRUE(arguments.at(2).type() == QVariant::Int);

  QModelIndex parent = qvariant_cast<QModelIndex>(arguments.at(0));
  ASSERT_FALSE(parent.isValid());

  int actual_row = qvariant_cast<int>(arguments.at(1));
  ASSERT_EQ(expect_row, actual_row);

  actual_row = qvariant_cast<int>(arguments.at(2));
  ASSERT_EQ(expect_row, actual_row);
}

void AttributeCollectionModelTest::checkClickOnAddAttribute(
    int row, MockAttrSupplierTestStub* supplier,
    std::vector<utils::U8String> expect_row_data[],
    int expect_row_count, bool expect_remove_supplier_row) {
  // Setup fixture
  switchToEditMode();

  QSignalSpy rowsAboutToBeInserted_sigspy(
      qmodel.get(),
      SIGNAL(rowsAboutToBeInserted(const QModelIndex&, int, int)));
  QSignalSpy rowsInserted_sigspy(
      qmodel.get(),
      SIGNAL(rowsInserted(const QModelIndex&, int, int)));

  QSignalSpy rowsAboutToBeRemoved_sigspy(
      qmodel.get(),
      SIGNAL(rowsAboutToBeRemoved(const QModelIndex&, int, int)));
  QSignalSpy rowsRemoved_sigspy(
      qmodel.get(),
      SIGNAL(rowsRemoved(const QModelIndex&, int, int)));

  // Expectations
  EXPECT_CALL(*supplier, addAttributeCalled());

  auto mock_listener = MockListener::attachTo(qmodel.get());
  EXPECT_CALL(*mock_listener, AttributeAdded(row));

  // Exercise system
  qmodel->mayAddAttributeIfSupplier(row);

  // Verify result
  CUSTOM_ASSERT(checkRowData(expect_row_data, expect_row_count));
  CUSTOM_ASSERT(checkRowsInsertRemoveSigspy(&rowsAboutToBeInserted_sigspy,
                                            row));
  CUSTOM_ASSERT(checkRowsInsertRemoveSigspy(&rowsInserted_sigspy,
                                            row));

  if (expect_remove_supplier_row) {
    CUSTOM_ASSERT(checkRowsInsertRemoveSigspy(&rowsAboutToBeRemoved_sigspy,
                                              row + 1));
    CUSTOM_ASSERT(checkRowsInsertRemoveSigspy(&rowsRemoved_sigspy,
                                              row + 1));
  } else {
    ASSERT_EQ(0, rowsAboutToBeRemoved_sigspy.count());
    ASSERT_EQ(0, rowsRemoved_sigspy.count());
  }
}

TEST_F(AttributeCollectionModelTest,
       should_add_empty_attribute_and_remove_supplier_row_when_user_clicked_on_a_supplier_whos_attrcount_eq_max_minus_1) { // NOLINT
  int expect_row_count =
      utils::sizeof_array(expect_on_edit_mode_after_click_add_on_attr_count_eq_max_minus_1_supplier); // NOLINT
  CUSTOM_ASSERT(checkClickOnAddAttribute(
      ROW_WITH_SUPPLIER_ATTRCOUNT_EQ_MAX_MINUS_1,
      max_gt_1_attrcount_eq_max_minus_1_supplier,
      expect_on_edit_mode_after_click_add_on_attr_count_eq_max_minus_1_supplier,
      expect_row_count, true));
}

TEST_F(AttributeCollectionModelTest,
       should_add_empty_attribute_but_not_remove_supplier_row_when_user_clicked_on_a_supplier_whos_attrcount_eq_max_minus_2) { // NOLINT
  int expect_row_count =
      utils::sizeof_array(expect_on_edit_mode_after_click_add_on_attr_count_eq_max_minus_2_supplier); // NOLINT
  CUSTOM_ASSERT(checkClickOnAddAttribute(
      ROW_WITH_SUPPLIER_ATTRCOUNT_EQ_MAX_MINUS_2,
      max_gt_1_attrcount_eq_max_minus_2_supplier,
      expect_on_edit_mode_after_click_add_on_attr_count_eq_max_minus_2_supplier,
      expect_row_count, false));
}

TEST_F(AttributeCollectionModelTest,
       should_not_add_empty_attr_when_user_clicked_on_normal_attr_row) { // NOLINT
  // Setup fixture
  int row = ROW_WITH_ATTRIBUTE;

  switchToEditMode();

  QSignalSpy sigspy(qmodel.get(),
                    SIGNAL(rowsInserted(QModelIndex, int, int)));

  // Expectations
  EXPECT_CALL(*max_gt_1_attrcount_eq_max_minus_1_supplier,
              addAttributeCalled()).Times(0);

  auto mock_listener = MockListener::attachTo(qmodel.get());
  EXPECT_CALL(*mock_listener, AttributeAdded(row)).Times(0);

  // Exercise system
  qmodel->mayAddAttributeIfSupplier(row);

  // Verify results
  int expect_row_count =
      utils::sizeof_array(expect_on_edit_mode);
  CUSTOM_ASSERT(checkRowData(expect_on_edit_mode, expect_row_count));

  ASSERT_EQ(0, sigspy.count());
}

TEST_F(AttributeCollectionModelTest,
       should_emit_dataChanged_signal_with_the_correct_index_when_attr_changed) { // NOLINT
  // Setup fixture
  switchToEditMode();

  QSignalSpy sigspy(qmodel.get(), SIGNAL(dataChanged(const QModelIndex&,
                                                     const QModelIndex&,
                                                     const QVector<int>&)));

  // Exercise system
  auto new_name = xtestutils::genRandomString();
  int row_to_change = ROW_WITH_ATTRIBUTE;
  expect_on_edit_mode[row_to_change][0] = new_name;

  attr_to_change->setDisplayName(new_name);

  // Verify results
  ASSERT_EQ(1, sigspy.count());
  QModelIndex expect_topLeft_index = qmodel->index(row_to_change, 0);
  QModelIndex expect_bottomRight_index = qmodel->index(row_to_change, 1);

  QList<QVariant> arguments = sigspy.takeFirst();
  auto actual_topLeft_index = qvariant_cast<QModelIndex>(arguments.at(0));
  auto actual_bottomRight_index = qvariant_cast<QModelIndex>(arguments.at(1));
  ASSERT_EQ(actual_topLeft_index, expect_topLeft_index)
      << "expect (" << row_to_change << ", 0) got ("
      << actual_topLeft_index.row() << ", "
      << actual_topLeft_index.column() << ")";
  ASSERT_EQ(actual_bottomRight_index, expect_bottomRight_index)
      << "expect (" << row_to_change << ", 1) got ("
      << actual_bottomRight_index.row() << ", "
      << actual_bottomRight_index.column() << ")";
}

TEST_F(AttributeCollectionModelTest,
       should_be_able_to_got_the_attr_of_a_row) { // NOLINT
  switchToEditMode();

  ASSERT_EQ(attr_to_change, qmodel->attrOfRow(ROW_WITH_ATTRIBUTE));
  ASSERT_EQ(nullptr,
            qmodel->attrOfRow(ROW_WITH_SUPPLIER_ATTRCOUNT_EQ_MAX_MINUS_1));
}
