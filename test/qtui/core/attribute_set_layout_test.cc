//-*- TestCaseName: AttributeSetLayoutTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <QGridLayout>
#include <QLabel>
#include <QFrame>

#include "test/testutils/gmock_common.h"
#include "test/testutils/qt/gui_tester.h"
#include "test/testutils/qt/gmock_qtapp.h"

#include "src/qtui/core/attribute_set_layout.h"
#include "test/core/attribute_test_stub.h"
#include "qtui/mock_attribute_editor_view.h"

#include "qsint/include/QSint"

using namespace snailcore;  // NOLINT
using namespace snailcore::tests;  // NOLINT

class ExpectLayoutData {
 public:
  ExpectLayoutData(const utils::U8String& name,
                   const utils::U8String& value,
                   IAttribute* attr = nullptr)
      : name_text_{name}
      , value_text_{value}
      , attr_{attr}
      , is_supplier_{attr == nullptr} { }

  utils::U8String name_text_;
  utils::U8String value_text_;
  IAttribute* attr_;
  bool is_supplier_;

  MockAttributeEditorView* editor_view_ { nullptr };
};

class AttributeSetLayoutTest : public ::testing::Test {
 protected:
  AttributeSetLayoutTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~AttributeSetLayoutTest() { }
  void SetUp() override {
    attr_set_layout = std::make_shared<AttributeSetLayout>();
    grid_layout = attr_set_layout->layout();

    ASSERT_NE(nullptr, grid_layout);

    fake_parent_widget = new QFrame();
    fake_parent_widget->setLayout(grid_layout);

    initExpectLayoutDatas();
    switchToDisplayMode();
  }

  void TearDown() override {
    closeAttributeEditorViews();
    delete fake_parent_widget;
  }

  void initExpectLayoutDatas();
  void switchToDisplayMode();
  void switchToEditMode();
  void switchFromEditModeToDisplayMode();
  void jumpAndForthBetweenDisplayAndEditMode();

  void createAttrEditorViewFor(ExpectLayoutData* layout_data);
  void setupAttributeEditorViews(
      std::vector<ExpectLayoutData>* expect_layout_datas);
  void fillExistingAttrEditorViews(
      std::vector<ExpectLayoutData>* expect_layout_datas);
  void closeAttributeEditorViews();
  void destroyAttrEditorView(IAttributeEditorView* editor_view);

  void checkLayoutData(
      const std::vector<ExpectLayoutData>& expect_layout_datas, bool edit_mode);
  template <typename LabelType>
  void assertLabelEqual(
      const utils::U8String expect_text, int row, int column);
  void assertCellEmpty(int row, int column);
  void assertWidgetEqual(QWidget* expect_widget, int row, int column);

  void clickAddAttributeActionLabel(int row);

  void testAddAttribute(int click_row, MockAttrSupplierTestStub* supplier,
                        std::vector<ExpectLayoutData>* expect_layout_datas,
                        bool double_add);

  // region: objects test subject depends on
  QFrame* fake_parent_widget;

  std::vector<ExpectLayoutData> expect_on_display_mode;
  std::vector<ExpectLayoutData> expect_on_edit_mode;
  std::vector<ExpectLayoutData> expect_on_edit_mode_after_click_add_on_attr_count_eq_max_minus_1_supplier; // NOLINT;
  std::vector<ExpectLayoutData> expect_on_edit_mode_after_click_add_on_attr_count_eq_max_minus_2_supplier; // NOLINT
  std::vector<ExpectLayoutData> expect_on_edit_mode_after_another_click_add_on_attr_count_eq_max_minus_2_supplier; // NOLINT

  MockAttrSupplierTestStub* supplier_Max_Gt_1_AttrCount_EQ_Max_minus_1;
  int add_Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_1_row;
  MockAttrSupplierTestStub* supplier_Max_Gt_1_AttrCount_EQ_Max_minus_2;
  int add_Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_2_row;
  // endregion

  // region: test subject
  std::shared_ptr<AttributeSetLayout> attr_set_layout;
  QGridLayout* grid_layout { nullptr };

  std::vector<std::unique_ptr<MockAttrSupplierTestStub> > attr_suppliers_up;
  std::vector<IAttributeSupplier*> attr_suppliers;
  std::map<IAttribute*,
           MockAttributeEditorView*> attr_to_editor_view;
  // endregion

  // region: object depends on test subject
  // endregion
};

void AttributeSetLayoutTest::initExpectLayoutDatas() {
  attr_suppliers_up = MockAttrSupplierTestStub::createAttributeSuppliers();
  std::vector<IAttribute*> all_attrs;
  for (auto & supplier : attr_suppliers_up) {
    attr_suppliers.push_back(supplier.get());
    auto attrs = supplier->attributes();
    all_attrs.insert(all_attrs.end(), attrs.begin(), attrs.end());
  }

  supplier_Max_Gt_1_AttrCount_EQ_Max_minus_1 =
      attr_suppliers_up[MockAttrSupplierTestStub::kIdx_Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_1].get(); // NOLINT

  supplier_Max_Gt_1_AttrCount_EQ_Max_minus_2 =
      attr_suppliers_up[MockAttrSupplierTestStub::kIdx_Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_2].get(); // NOLINT


  auto attr01 = all_attrs[0];
  auto attr02 = all_attrs[1];
  auto attr03 = all_attrs[2];
  auto attr04 = all_attrs[3];
  auto attr05 = all_attrs[4];
  auto attr06 = all_attrs[5];
  auto attr07 = all_attrs[6];
  auto attr07_1 = all_attrs[7];
  auto attr08_1 = all_attrs[8];
  auto attr07_2 = all_attrs[9];
  auto attr08_2 = all_attrs[10];
  auto attr08_3 = all_attrs[11];

  // attribute supplier rows is never showd in display mode
  expect_on_display_mode = {
    // Supplier_Max_Eq_1_AttrCount_Eq_0
    // nothing

    // Supplier_Max_Eq_1_AttrCount_Eq_1
    { "Attribute 01:", "Value 1", attr01},

    // Supplier_Max_Gt_1_AttrCount_Eq_0
    // nothing

    // Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_1
    { "Attribute 02:", "Value 2", attr02},
    { "Attribute 03:", "Value 3", attr03},

    // Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_2
    { "Attribute 04:", "Value 4", attr04},

    // Supplier_Max_Gt_1_AttrCount_Eq_Max
    { "Attribute 05:", "Value 5", attr05},
    { "Attribute 06:", "Value 6", attr06},
    { "Attribute 07:", "Value 7", attr07},

    // Supplier_With_Duplicated_Named_Attrs
    // sorted and duplicate attr name is hidded
    { "Attribute 07:", "Value 7-1", attr07_1},
    { "",             "Value 7-2", attr07_2},
    { "Attribute 08:", "Value 8-1", attr08_1},
    { "",             "Value 8-2", attr08_2},

    // Supplier_With_Attr_Name_Same_As_Previous_Supplier
    { "Attribute 08:", "Value 8-3", attr08_3},
  };

  // 1. attribute suppliers whos attr_count < max & max > 1 is showed
  // 2. attributes are not sorted by name and name are never hidden
  expect_on_edit_mode = {
    // Supplier_Max_Eq_1_AttrCount_Eq_0
    // nothing

    // Supplier_Max_Eq_1_AttrCount_Eq_1
    { "Attribute 01:", "Value 1", attr01},  // 0

    // Supplier_Max_Gt_1_AttrCount_Eq_0
    { "Add Supplier_Max_Gt_1_AttrCount_Eq_0", ""},

    // Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_1
    { "Attribute 02:", "Value 2", attr02},
    { "Attribute 03:", "Value 3", attr03},
    { "Add Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_1", ""},  // 4

    // Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_2
    { "Attribute 04:", "Value 4", attr04},
    { "Add Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_2", ""},  // 6

    // Supplier_Max_Gt_1_AttrCount_Eq_Max
    { "Attribute 05:", "Value 5", attr05},
    { "Attribute 06:", "Value 6", attr06},
    { "Attribute 07:", "Value 7", attr07},

    // Supplier_With_Duplicated_Named_Attrs
    // not sort, not hidden
    { "Attribute 07:", "Value 7-1", attr07_1},
    { "Attribute 08:", "Value 8-1", attr08_1},
    { "Attribute 07:", "Value 7-2", attr07_2},
    { "Attribute 08:", "Value 8-2", attr08_2},

    // Supplier_With_Attr_Name_Same_As_Previous_Supplier
    { "Attribute 08:", "Value 8-3", attr08_3},
  };

  add_Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_1_row = 4;
  add_Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_2_row = 6;

  // after add ,the supplier which becomes full will not show "Add ..." row
  expect_on_edit_mode_after_click_add_on_attr_count_eq_max_minus_1_supplier = {
    // Supplier_Max_Eq_1_AttrCount_Eq_0
    // nothing

    // Supplier_Max_Eq_1_AttrCount_Eq_1
    { "Attribute 01:", "Value 1", attr01},  // 0

    // Supplier_Max_Gt_1_AttrCount_Eq_0
    { "Add Supplier_Max_Gt_1_AttrCount_Eq_0", ""},

    // Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_1
    { "Attribute 02:", "Value 2", attr02},
    { "Attribute 03:", "Value 3", attr03},
    // an empty attribute is added with the name same as the supplier
    { "Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_1:", ""},  // 4
    // and the "Add ..." row is gone

    // Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_2
    { "Attribute 04:", "Value 4", attr04},
    { "Add Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_2", ""},

    // Supplier_Max_Gt_1_AttrCount_Eq_Max
    { "Attribute 05:", "Value 5", attr05},
    { "Attribute 06:", "Value 6", attr06},
    { "Attribute 07:", "Value 7", attr07},

    // Supplier_With_Duplicated_Named_Attrs
    // not sort, not hidden
    { "Attribute 07:", "Value 7-1", attr07_1},
    { "Attribute 08:", "Value 8-1", attr08_1},
    { "Attribute 07:", "Value 7-2", attr07_2},
    { "Attribute 08:", "Value 8-2", attr08_2},

    // Supplier_With_Attr_Name_Same_As_Previous_Supplier
    { "Attribute 08:", "Value 8-3", attr08_3},
  };

  // after add attr, if the supplier is not full, its "Add ..." row is remained
  expect_on_edit_mode_after_click_add_on_attr_count_eq_max_minus_2_supplier = {
    // Supplier_Max_Eq_1_AttrCount_Eq_0
    // nothing

    // Supplier_Max_Eq_1_AttrCount_Eq_1
    { "Attribute 01:", "Value 1", attr01},

    // Supplier_Max_Gt_1_AttrCount_Eq_0
    { "Add Supplier_Max_Gt_1_AttrCount_Eq_0", ""},

    // Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_1
    { "Attribute 02:", "Value 2", attr02},
    { "Attribute 03:", "Value 3", attr03},
    { "Add Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_1", ""},

    // Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_2
    { "Attribute 04:", "Value 4", attr04},
    // an empty attribute is added with the name same as the supplier
    { "Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_2:", ""},
    // and the "Add..." row is remained
    { "Add Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_2", ""},

    // Supplier_Max_Gt_1_AttrCount_Eq_Max
    { "Attribute 05:", "Value 5", attr05},
    { "Attribute 06:", "Value 6", attr06},
    { "Attribute 07:", "Value 7", attr07},

    // Supplier_With_Duplicated_Named_Attrs
    // not sort, not hidden
    { "Attribute 07:", "Value 7-1", attr07_1},
    { "Attribute 08:", "Value 8-1", attr08_1},
    { "Attribute 07:", "Value 7-2", attr07_2},
    { "Attribute 08:", "Value 8-2", attr08_2},

    // Supplier_With_Attr_Name_Same_As_Previous_Supplier
    { "Attribute 08:", "Value 8-3", attr08_3},
  };

  expect_on_edit_mode_after_another_click_add_on_attr_count_eq_max_minus_2_supplier = { // NOLINT
    // Supplier_Max_Eq_1_AttrCount_Eq_0
    // nothing

    // Supplier_Max_Eq_1_AttrCount_Eq_1
    { "Attribute 01:", "Value 1", attr01},

    // Supplier_Max_Gt_1_AttrCount_Eq_0
    { "Add Supplier_Max_Gt_1_AttrCount_Eq_0", ""},

    // Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_1
    { "Attribute 02:", "Value 2", attr02},
    { "Attribute 03:", "Value 3", attr03},
    { "Add Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_1", ""},

    // Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_2
    { "Attribute 04:", "Value 4", attr04},
    { "Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_2:", ""},
    // another empty attribute is added with the name same as the supplier
    { "Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_2:", ""},
    // and the "Add..." row is gone

    // Supplier_Max_Gt_1_AttrCount_Eq_Max
    { "Attribute 05:", "Value 5", attr05},
    { "Attribute 06:", "Value 6", attr06},
    { "Attribute 07:", "Value 7", attr07},

    // Supplier_With_Duplicated_Named_Attrs
    // not sort, not hidden
    { "Attribute 07:", "Value 7-1", attr07_1},
    { "Attribute 08:", "Value 8-1", attr08_1},
    { "Attribute 07:", "Value 7-2", attr07_2},
    { "Attribute 08:", "Value 8-2", attr08_2},

    // Supplier_With_Attr_Name_Same_As_Previous_Supplier
    { "Attribute 08:", "Value 8-3", attr08_3},
  };
}

void AttributeSetLayoutTest::createAttrEditorViewFor(
    ExpectLayoutData* layout_data) {
  using DummyWidget = QFrame;

  auto editor_view = new MockAttributeEditorView();
  auto widget = new DummyWidget();
  EXPECT_CALL(*editor_view, getWidget())
      .WillRepeatedly(Return(widget));

  layout_data->editor_view_ = editor_view;
  if (layout_data->attr_)
    attr_to_editor_view[layout_data->attr_] = editor_view;

  layout_data->is_supplier_ = false;
}

void AttributeSetLayoutTest::setupAttributeEditorViews(
    std::vector<ExpectLayoutData>* expect_layout_datas) {
  for (auto iter = expect_layout_datas->begin();
       iter != expect_layout_datas->end();
       ++iter) {
    ExpectLayoutData& layout_data = *iter;
    if (!layout_data.is_supplier_) {
      createAttrEditorViewFor(&layout_data);
    }
  }
}

void AttributeSetLayoutTest::fillExistingAttrEditorViews(
    std::vector<ExpectLayoutData>* expect_layout_datas) {
  for (auto iter = expect_layout_datas->begin();
       iter != expect_layout_datas->end();
       ++iter) {
    ExpectLayoutData& layout_data = *iter;
    auto attr = layout_data.attr_;
    if (attr) {
      try {
        layout_data.editor_view_ = attr_to_editor_view.at(attr);
      } catch (...) {
        std::cout << "attr's view not found" << std::endl;
        throw;
      }
    }
  }
}

void AttributeSetLayoutTest::destroyAttrEditorView(
    IAttributeEditorView* editor_view) {
  delete editor_view->getWidget();
  delete editor_view;
}

void AttributeSetLayoutTest::closeAttributeEditorViews() {
  for (auto & item : attr_to_editor_view) {
    destroyAttrEditorView(item.second);
  }
  attr_to_editor_view.clear();
}

template <typename LabelType>
void AttributeSetLayoutTest::assertLabelEqual(
    const utils::U8String expect_text, int row, int column) {
  QLayoutItem* item = grid_layout->itemAtPosition(row, column);

  ASSERT_NE(nullptr, item) << "should have item @row" << row
                           << ",column" << column;
  LabelType* label = qobject_cast<LabelType*>(item->widget());
  ASSERT_NE(nullptr, label) << "item @row" << row
                            << ", column" << column
                            << " should be a Label";
  auto actual_text = QStringToU8String(label->text());
  ASSERT_EQ(expect_text, actual_text)
      << "label @row" << row << ", column" << column
      << " should be " << expect_text << ", but actually is " << actual_text;
}

void AttributeSetLayoutTest::assertCellEmpty(int row, int column) {
  QLayoutItem* item = grid_layout->itemAtPosition(row, column);
  ASSERT_EQ(nullptr, item);
}

void AttributeSetLayoutTest::assertWidgetEqual(QWidget* expect_widget,
                                               int row, int column) {
  QLayoutItem* item = grid_layout->itemAtPosition(row, column);

  ASSERT_NE(nullptr, item) << "should have item @row" << row
                           << ",column" << column;
  ASSERT_EQ(expect_widget, item->widget())
      << "widget @row" << row << ", column" << column
      << " should be " << expect_widget
      << ", but actually is " << item->widget();
}

void AttributeSetLayoutTest::checkLayoutData(
    const std::vector<ExpectLayoutData>& expect_layout_datas, bool edit_mode) {
  ASSERT_EQ(2, grid_layout->columnCount());

  int expect_row_count = static_cast<int>(expect_layout_datas.size());

  for (int row = 0; row < expect_row_count; ++row) {
    auto & expect_layout_data = expect_layout_datas[row];

    if (!edit_mode) {
      CUSTOM_ASSERT(
          assertLabelEqual<QLabel>(expect_layout_data.name_text_,
                                   row, AttributeSetLayout::kNameColumn));
      CUSTOM_ASSERT(
          assertLabelEqual<QLabel>(expect_layout_data.value_text_,
                                   row, AttributeSetLayout::kValueColumn));
    } else {
      if (!expect_layout_data.is_supplier_) {
        CUSTOM_ASSERT(
            assertLabelEqual<QLabel>(expect_layout_data.name_text_,
                                     row, AttributeSetLayout::kNameColumn));

        auto expect_widget = expect_layout_data.editor_view_->getWidget();
        CUSTOM_ASSERT(assertWidgetEqual(expect_widget,
                                        row, AttributeSetLayout::kValueColumn));
      } else {  // supplier
        // name spaned two columns
        CUSTOM_ASSERT(
            assertLabelEqual<QSint::ActionLabel>(
                expect_layout_data.name_text_,
                row, AttributeSetLayout::kNameColumn));
        CUSTOM_ASSERT(
            assertLabelEqual<QSint::ActionLabel>(
                expect_layout_data.name_text_,
                row, AttributeSetLayout::kValueColumn));
      }
    }
  }

  int layout_row_count = grid_layout->rowCount();
  if (layout_row_count > expect_row_count) {
    for (int row = expect_row_count; row < layout_row_count; ++row) {
      CUSTOM_ASSERT(assertCellEmpty(row, AttributeSetLayout::kNameColumn));
      CUSTOM_ASSERT(assertCellEmpty(row, AttributeSetLayout::kValueColumn));
    }
  }
}

void AttributeSetLayoutTest::switchToDisplayMode() {
  attr_set_layout->setAttributeSuppliers(attr_suppliers, false);
  // Verify results
  QTest::qWait(10);  // wait for layout cleanup old widgets
  CUSTOM_ASSERT(checkLayoutData(expect_on_display_mode, false));
}

BEGIN_MOCK_LISTENER_DEF(MockListener, fto::AttributeSetLayout)

MOCK_METHOD1(CreateAttrEditor,
             IAttributeEditorView*(snailcore::IAttribute* attr));
MOCK_METHOD0(CloseAttributeEditors, void());

BEGIN_BIND_SIGNAL(fto::AttributeSetLayout)

BIND_SIGNAL1(CreateAttrEditor,
             IAttributeEditorView*, snailcore::IAttribute*, attr);
BIND_SIGNAL0(CloseAttributeEditors, void);

END_BIND_SIGNAL()
END_MOCK_LISTENER_DEF()

void AttributeSetLayoutTest::switchToEditMode() {
  // Setup fixture
  setupAttributeEditorViews(&expect_on_edit_mode);

  // Expectations
  auto mock_listener = MockListener::attachTo(attr_set_layout.get());
  for (auto & map_item : attr_to_editor_view) {
    EXPECT_CALL(*mock_listener, CreateAttrEditor(map_item.first))
        .WillOnce(Return(map_item.second));
  }

  EXPECT_CALL(*mock_listener, CloseAttributeEditors()).Times(0);

  // Excersise system
  attr_set_layout->setAttributeSuppliers(attr_suppliers, true);

  // Verify results
  QTest::qWait(10);  // wait for layout cleanup old widgets
  CUSTOM_ASSERT(checkLayoutData(expect_on_edit_mode, true));
}

TEST_F(AttributeSetLayoutTest,
       check_edit_mode_layout) { // NOLINT
  CUSTOM_ASSERT(switchToEditMode());
}

void AttributeSetLayoutTest::switchFromEditModeToDisplayMode() {
  auto mock_listener = MockListener::attachTo(attr_set_layout.get());
  EXPECT_CALL(*mock_listener, CloseAttributeEditors())
      .WillOnce(Invoke([this](){ closeAttributeEditorViews(); }));

  // Exercise system
  switchToDisplayMode();
}

void AttributeSetLayoutTest::jumpAndForthBetweenDisplayAndEditMode() {
  switchToEditMode();
  switchFromEditModeToDisplayMode();
  switchToEditMode();
}

TEST_F(AttributeSetLayoutTest,
       test_jumpAndForthBetweenDisplayAndEditMode) { // NOLINT
  CUSTOM_ASSERT(jumpAndForthBetweenDisplayAndEditMode());
}

TEST_F(AttributeSetLayoutTest,
       should_not_cause_problem_when_setAttrSuppliers_after_parent_widget_of_the_layout_is_destroyed) { // NOLINT
  // Exercise system
  delete fake_parent_widget;
  fake_parent_widget = nullptr;

  // Verify results
  ASSERT_EQ(nullptr, attr_set_layout->layout());
  attr_set_layout->setAttributeSuppliers(attr_suppliers, true);
}

void AttributeSetLayoutTest::clickAddAttributeActionLabel(int row) {
  QLayoutItem* item = grid_layout->itemAtPosition(
      row, AttributeSetLayout::kNameColumn);
  QTest::mouseClick(item->widget(), Qt::LeftButton);
}

void AttributeSetLayoutTest::testAddAttribute(
    int click_row, MockAttrSupplierTestStub* supplier,
    std::vector<ExpectLayoutData>* expect_layout_datas, bool double_add) {
  // Expectations
  int add_times = 1;
  if (double_add)
    add_times = 2;

  EXPECT_CALL(*supplier, addAttributeCalled(_))
      .Times(add_times)
      .WillRepeatedly(Return(ComplexReturnValue(0)));

  fillExistingAttrEditorViews(expect_layout_datas);

  auto& expect_layout_data =
      expect_layout_datas->at(click_row);
  createAttrEditorViewFor(&expect_layout_data);
  auto first_editor_view = expect_layout_data.editor_view_;

  auto mock_listener = MockListener::attachTo(attr_set_layout.get());

  IAttributeEditorView* second_editor_view = nullptr;
  if (double_add) {
    auto& expect_layout_data_2nd_add =
        expect_layout_datas->at(click_row + 1);
    createAttrEditorViewFor(&expect_layout_data_2nd_add);
    second_editor_view = expect_layout_data_2nd_add.editor_view_;

    EXPECT_CALL(*mock_listener, CreateAttrEditor(_))
        .Times(2)
        .WillOnce(Return(first_editor_view))
        .WillOnce(Return(second_editor_view));
  } else {
    EXPECT_CALL(*mock_listener, CreateAttrEditor(_))
        .WillOnce(Return(first_editor_view));
  }

  // Exercise system
  clickAddAttributeActionLabel(click_row);
  if (double_add) {
    clickAddAttributeActionLabel(click_row + 1);
  }

  // Verify results
  CUSTOM_ASSERT(checkLayoutData(*expect_layout_datas, true));

  // cleanup fixture
  destroyAttrEditorView(first_editor_view);
  if (second_editor_view) {
    destroyAttrEditorView(second_editor_view);
  }
  auto attrs = supplier->attributes();
  EXPECT_CALL(*supplier, attributeRemoved(_)).
      WillRepeatedly(Return(ComplexReturnValue(0)));
  supplier->removeAttribute(attrs[attrs.size() - 1]);
  if (double_add) {
    supplier->removeAttribute(attrs[attrs.size() - 2]);
  }

  Mock::VerifyAndClearExpectations(supplier);
}

TEST_F(AttributeSetLayoutTest,
       should_click_Add_Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_1_add_a_new_attr_before_the_suppliers_row_and_remove_the_full_supplier_row) { // NOLINT
  // Setup fixture
  jumpAndForthBetweenDisplayAndEditMode();

  CUSTOM_ASSERT(testAddAttribute(
      add_Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_1_row,
      supplier_Max_Gt_1_AttrCount_EQ_Max_minus_1,
      &expect_on_edit_mode_after_click_add_on_attr_count_eq_max_minus_1_supplier, // NOLINT
      false));

  // cleanup
  switchFromEditModeToDisplayMode();
}

TEST_F(AttributeSetLayoutTest,
       should_click_Add_Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_2_add_a_new_attr_before_the_supplier_row_and_not_remove_the_supplier_row) { // NOLINT
  // Setup fixture
  jumpAndForthBetweenDisplayAndEditMode();

  CUSTOM_ASSERT(testAddAttribute(
      add_Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_2_row,
      supplier_Max_Gt_1_AttrCount_EQ_Max_minus_2,
      &expect_on_edit_mode_after_click_add_on_attr_count_eq_max_minus_2_supplier, // NOLINT
      false));

  // cleanup
  switchFromEditModeToDisplayMode();
}

TEST_F(AttributeSetLayoutTest,
       double_check_add_attribute) { // NOLINT
  // Setup fixture
  jumpAndForthBetweenDisplayAndEditMode();

  CUSTOM_ASSERT(testAddAttribute(
      add_Supplier_Max_Gt_1_AttrCount_EQ_Max_minus_2_row,
      supplier_Max_Gt_1_AttrCount_EQ_Max_minus_2,
      &expect_on_edit_mode_after_another_click_add_on_attr_count_eq_max_minus_2_supplier, // NOLINT
      true));

  // cleanup
  switchFromEditModeToDisplayMode();
}

TEST_F(AttributeSetLayoutTest,
       should_update_name_label_when_attr_display_name_changed) { // NOLINT
  // Setup fixture
  jumpAndForthBetweenDisplayAndEditMode();

  // Exercise system
  for (auto & layout_data : expect_on_edit_mode) {
    if (!layout_data.is_supplier_) {
      auto new_name = xtestutils::genRandomString();
      layout_data.name_text_ = new_name;
      auto attr = static_cast<AttributeTestStub*>(layout_data.attr_);
      attr->setDisplayName(new_name);
    }
  }

  // Verify results
  CUSTOM_ASSERT(checkLayoutData(expect_on_edit_mode, true));
}
