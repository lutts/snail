//-*- TestCaseName: TreeItemQModelTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <QSignalSpy>

#include "test/testutils/gmock_common.h"
#include "test/testutils/qmodelindex_generator.h"

#include "src/qtui/core/tree_item_qmodel.h"
#include "snail/mock_tree_item_provider.h"
#include "snail/i_tree_item.h"

using namespace snailcore;         // NOLINT
using namespace snailcore::tests;  // NOLINT

// test data structure
// -------------------
// (root)
//    |---(empty line)   (0, 0)
//    |---level0 item1   (1, 0)
//    |   |----level1 item1    (0, 0)
//    |   |    |---level2 item1      (0, 0)
//    |   |----level1 item2    (1, 0)
//    |---level0 item2    (2, 0)
//    |---Add More...     (3, 0)
//
// as we use lazy populate for our model impl,
// w using these test data to test the following 4 dynamic add item cases
// 1. add to invisible items
// 2. add to visible but children not populated item
// 3. add to visible and chidren is populated item
// 4. add to root item should add before ADD_MORE_ROW_TEXT row

constexpr static int kEmptySpecialRow = 0;
constexpr static int kAddMoreSpecialRow = 3;
#define PROVIDER_NAME "Provider Name"
#define CLEAR_ROW_TEXT "(clear)"
#define ADD_MORE_ROW_TEXT "Add More..."

class TreeItemTestStub : public ITreeItem {
 public:
  TreeItemTestStub(const utils::U8String& name, bool is_category)
      : name_(name), is_category_(is_category) {}
  virtual ~TreeItemTestStub() = default;

  utils::U8String name() const override { return name_; }

  bool isGroupOnly() const override { return is_category_; }

 private:
  SNAIL_DISABLE_COPY(TreeItemTestStub);

  utils::U8String name_;
  bool is_category_;
};

class ExpectRowData {
 public:
  utils::U8String text;
  ITreeItem* item_ptr;
  ITreeItem* parent_item_ptr;
  bool isSelectable;

  std::vector<ExpectRowData>* subitems;

  bool isSpecialRow() const {
    return (text == CLEAR_ROW_TEXT) || (text == ADD_MORE_ROW_TEXT);
  }
};

std::vector<ExpectRowData> level2_row_data;
std::vector<ExpectRowData> level1_row_data;
std::vector<ExpectRowData> level0_row_data;
std::vector<ExpectRowData> provider_row_data;
ExpectRowData root_row_data;
std::vector<ExpectRowData> level0_row_data_after_reset;
std::vector<ExpectRowData> provider_row_data_after_reset;
ExpectRowData root_row_data_after_reset;

void setupExpectRowDatas() {
  level2_row_data = {{
      .text = "Level2 Item1",
      .item_ptr = nullptr,
      .parent_item_ptr = nullptr,
      .isSelectable = true,
      .subitems = nullptr,
  }};

  level1_row_data = {{
                         .text = "Level1 Item1",
                         .item_ptr = nullptr,
                         .parent_item_ptr = nullptr,
                         .isSelectable = true,
                         .subitems = &level2_row_data,
                     },
                     {
                         .text = "Level1 Item2",
                         .item_ptr = nullptr,
                         .parent_item_ptr = nullptr,
                         .isSelectable = false,
                         .subitems = nullptr,
                     }};

  level0_row_data = {{// level1
                      .text = CLEAR_ROW_TEXT,
                      .item_ptr = nullptr,
                      .parent_item_ptr = nullptr,
                      .isSelectable = true,
                      .subitems = nullptr},
                     {
                         .text = "Level0 Item1",
                         .item_ptr = nullptr,
                         .parent_item_ptr = nullptr,
                         .isSelectable = false,
                         .subitems = &level1_row_data,
                     },
                     {
                         .text = "Level0 Item2",
                         .item_ptr = nullptr,
                         .parent_item_ptr = nullptr,
                         .isSelectable = true,
                         .subitems = nullptr,
                     },
                     {
                         .text = ADD_MORE_ROW_TEXT,
                         .item_ptr = nullptr,
                         .parent_item_ptr = nullptr,
                         .isSelectable = true,
                         .subitems = nullptr,
                     }};

  provider_row_data = {{
      .text = PROVIDER_NAME,
      .item_ptr = nullptr,
      .parent_item_ptr = nullptr,
      .isSelectable = true,
      .subitems = &level0_row_data,
  }};

  root_row_data = {
      .text = "root idx",
      .item_ptr = nullptr,
      .parent_item_ptr = nullptr,
      .isSelectable = false,
      .subitems = &level0_row_data,
  };

  level0_row_data_after_reset = {{// level1
                                  .text = CLEAR_ROW_TEXT,
                                  .item_ptr = nullptr,
                                  .parent_item_ptr = nullptr,
                                  .isSelectable = true,
                                  .subitems = nullptr},
                                 {
                                     .text = "Level0 Item1 After Reset",
                                     .item_ptr = nullptr,
                                     .parent_item_ptr = nullptr,
                                     .isSelectable = false,
                                     .subitems = nullptr,
                                 },
                                 {
                                     .text = "Level0 Item2 After Reset",
                                     .item_ptr = nullptr,
                                     .parent_item_ptr = nullptr,
                                     .isSelectable = true,
                                     .subitems = nullptr,
                                 },
                                 {
                                     .text = ADD_MORE_ROW_TEXT,
                                     .item_ptr = nullptr,
                                     .parent_item_ptr = nullptr,
                                     .isSelectable = true,
                                     .subitems = nullptr,
                                 }};

  provider_row_data_after_reset = {{
      .text = PROVIDER_NAME,
      .item_ptr = nullptr,
      .parent_item_ptr = nullptr,
      .isSelectable = true,
      .subitems = &level0_row_data_after_reset,
  }};

  root_row_data_after_reset = {
      .text = "root idx",
      .item_ptr = nullptr,
      .parent_item_ptr = nullptr,
      .isSelectable = false,
      .subitems = &level0_row_data_after_reset,
  };
}

void cleanupExpectRowDatas() {
  level2_row_data.clear();
  level1_row_data.clear();
  level0_row_data.clear();
  provider_row_data.clear();
  root_row_data = ExpectRowData();
  level0_row_data_after_reset.clear();
  provider_row_data_after_reset.clear();
  root_row_data_after_reset = ExpectRowData();
}

class TreeItemProviderTestStub : public ITreeItemProvider {
 public:
  class ChildItemIteratorTestStub : public IChildItemIterator {
   public:
    ChildItemIteratorTestStub(const TreeItemProviderTestStub& provider,
                              ITreeItem* parent_item)
        : row_data_(provider.itemToRowData(parent_item)) {
      if (row_data_->subitems) {
        max_idx_ = row_data_->subitems->size() - 1;
      }

      if ((row_data_->subitems == &level0_row_data) ||
          (row_data_->subitems == &level0_row_data_after_reset)) {
        cur_idx_ = 1;  // skip empty row
        --max_idx_;    // skip add more row
      }
    }
    virtual ~ChildItemIteratorTestStub() = default;

    bool hasNext() const override {
      if (cur_idx_ > max_idx_) return false;

      return true;
    }

    ITreeItem* next() override {
      if (!hasNext()) throw std::logic_error("no next item");

      auto item = (*row_data_->subitems)[cur_idx_].item_ptr;
      ++cur_idx_;

      return item;
    }

   private:
    SNAIL_DISABLE_COPY(ChildItemIteratorTestStub);

    ExpectRowData* row_data_;
    mutable int cur_idx_{0};
    int max_idx_{-1};
  };

  TreeItemProviderTestStub() {}
  virtual ~TreeItemProviderTestStub() {}

  utils::U8String name() const { return PROVIDER_NAME; }

  std::unique_ptr<IChildItemIterator> childItems(
      ITreeItem* parent_item) const override {
    return utils::make_unique<ChildItemIteratorTestStub>(*this, parent_item);
  }

  void setItemOwnerRow(ITreeItem* item,
                       std::vector<ExpectRowData>* row_data_vec) {
    item_to_row_data_vec_[item] = row_data_vec;
  }

  void fillRowData(ExpectRowData* root_rdata) {
    items_.clear();
    item_to_row_data_vec_.clear();
    fillItemPtrInRowData(root_rdata->subitems);
    root_rdata_ = root_rdata;
  }

  MOCK_CONST_METHOD0(isFilterMode, bool());

  // unintrested method, using mocks
  SNAIL_MOCK_SLOT(BeginFilter);
  SNAIL_MOCK_SLOT(FinishFilter);
  SNAIL_MOCK_SLOT(ItemAdded);

  MOCK_METHOD1(setFilterPattern, void(const utils::U8String& filter_pattern));
  MOCK_CONST_METHOD0(getFilterPattern, utils::U8String());
  MOCK_METHOD1(incRef, void(ITreeItem* item));

  MOCK_CONST_METHOD0(getRootItem, ITreeItem*());

  MOCK_METHOD1(findItemByName,
               std::vector<ITreeItem*>(const utils::U8String& name));

 private:
  SNAIL_DISABLE_COPY(TreeItemProviderTestStub);

  void fillItemPtrInRowData(std::vector<ExpectRowData>* row_datas,
                            ITreeItem* parent_item = nullptr) {
    for (auto& row_data : *row_datas) {
      if (row_data.isSpecialRow()) continue;

      TreeItemTestStub* item = nullptr;
      if (row_data.text != PROVIDER_NAME) {
        auto item_up = utils::make_unique<TreeItemTestStub>(
            row_data.text, !row_data.isSelectable);
        item = item_up.get();
        items_.push_back(std::move(item_up));

        row_data.item_ptr = item;
        item_to_row_data_vec_[item] = row_datas;

        row_data.parent_item_ptr = parent_item;
      }

      if (row_data.subitems) fillItemPtrInRowData(row_data.subitems, item);
    }
  }

  ExpectRowData* itemToRowData(ITreeItem* item) const {
    if (!item) {
      if ((*root_rdata_->subitems)[0].text == PROVIDER_NAME)
        return &(*root_rdata_->subitems)[0];
      else
        return root_rdata_;
    }

    try {
      auto row_data_vec = item_to_row_data_vec_.at(item);
      for (auto& row_data : *row_data_vec) {
        if (row_data.item_ptr == item) return &row_data;
      }
    } catch (...) {
      std::cout << "item: " << item->name() << " row data not found"
                << std::endl;
      /* re */ throw;
    }

    return nullptr;
  }

  std::vector<std::unique_ptr<ITreeItem>> items_;
  std::map<ITreeItem*, std::vector<ExpectRowData>*> item_to_row_data_vec_;
  ExpectRowData* root_rdata_{nullptr};
};

class TreeItemQModelTestBase : public ::testing::Test {
 protected:
  TreeItemQModelTestBase() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~TreeItemQModelTestBase() { }
  void SetUp() override {
    qmodel = createQModel();

    setupExpectRowDatas();
    item_provider.fillRowData(&root_row_data);

    EXPECT_CALL(item_provider, whenBeginFilter(_, _))
        .WillOnce(DoAll(SaveArg<0>(&providerBeginFilter),
                        Return(SignalConnection())));

    EXPECT_CALL(item_provider, whenFinishFilter(_, _))
        .WillOnce(DoAll(SaveArg<0>(&providerFinishFilter),
                        Return(SignalConnection())));

    EXPECT_CALL(item_provider, whenItemAdded(_, _))
        .WillOnce(DoAll(SaveArg<0>(&itemAdded), Return(SignalConnection())));

    qmodel->setTreeItemProvider(&item_provider);
  }

  void TearDown() override { cleanupExpectRowDatas(); }

  virtual std::shared_ptr<TreeItemQModel> createQModel() {
    return std::make_shared<TreeItemQModel>();
  }

  bool shouldRowVisible(const ExpectRowData& row_data);
  int expectItemCount(const ExpectRowData& row_data);

  void checkRowData(const ExpectRowData& root_rdata = root_row_data);
  void checkRowData(const std::vector<ExpectRowData>& row_data,
                    const QModelIndex& parent_index);
  void checkIndexData(const QModelIndex& index, const ExpectRowData& rdata);

  void should_provider_BeginFilter_emit_modelAboutToReset();
  void should_provider_FinishFilter_emit_modelReset();

  virtual int adjustLevel(int level) { return level; }
  void expandToLevel(int level);
  void expandToLevel(int level, const QModelIndex& parent_index);
  void addItemUnderLevel1Item1(ITreeItem** parent_item_ret = nullptr);
  void test_add_item_to_not_visible_parent_item();
  void test_add_item_to_visible_but_children_not_populated_item();
  void test_add_item_to_children_populated_parent_item();

  void addItemUnderRootItem();
  void test_add_item_to_unexpanded_root_item();
  void test_add_item_to_expanded_root_item();

  // region: objects test subject depends on
  TreeItemProviderTestStub item_provider;
  QModelIndexGenerator index_generator;

  bool always_selectable{false};
  bool show_clear_row{false};
  bool show_add_more_row{false};
  // endregion

  // region: test subject
  std::shared_ptr<TreeItemQModel> qmodel;
  // endregion

  // region: object depends on test subject
  using BeginFilterSlotType = ITreeItemProvider::BeginFilterSlotType;
  SlotCatcher<BeginFilterSlotType> providerBeginFilter;

  using FinishFilterSlotType = ITreeItemProvider::FinishFilterSlotType;
  SlotCatcher<FinishFilterSlotType> providerFinishFilter;

  using ItemAddedSlotType = ITreeItemProvider::ItemAddedSlotType;
  SlotCatcher<ItemAddedSlotType> itemAdded;
  // endregion
};

bool TreeItemQModelTestBase::shouldRowVisible(const ExpectRowData& row_data) {
  if (row_data.isSpecialRow()) {
    if (row_data.text == CLEAR_ROW_TEXT)
      return show_clear_row;
    else if (row_data.text == ADD_MORE_ROW_TEXT)
      return show_add_more_row;
    else
      return false;
  } else {
    return true;
  }
}

int TreeItemQModelTestBase::expectItemCount(const ExpectRowData& row_data) {
  if (row_data.subitems == nullptr) return 0;

  int count = 0;
  for (auto& sub_rdata : *row_data.subitems) {
    if (shouldRowVisible(sub_rdata)) {
      ++count;
    }
  }

  return count;
}

void TreeItemQModelTestBase::checkRowData(const ExpectRowData& root_rdata) {
  ASSERT_EQ(expectItemCount(root_rdata),
            qmodel->qmodel()->rowCount(QModelIndex()));
  ASSERT_EQ(1, qmodel->qmodel()->columnCount(QModelIndex()));
  QModelIndex expect_parent_idx;
  ASSERT_EQ(expect_parent_idx, qmodel->qmodel()->parent(QModelIndex()));

  // check sub items
  checkRowData(*root_rdata.subitems, QModelIndex());
}

void TreeItemQModelTestBase::checkRowData(
    const std::vector<ExpectRowData>& row_data,
    const QModelIndex& parent_index) {
  int row = 0;
  for (size_t idx = 0; idx < row_data.size(); ++idx) {
    auto& rdata = row_data[idx];

    if (!shouldRowVisible(rdata)) continue;

    QModelIndex index = qmodel->qmodel()->index(row, 0, parent_index);
    ASSERT_EQ(row, index.row());
    checkIndexData(index, rdata);
    ++row;

    if (rdata.subitems) {
      checkRowData(*rdata.subitems, index);
    }
  }
}

std::ostream& operator<<(std::ostream& os, const QModelIndex& index) {
  os << "[(" << index.row() << "," << index.column() << ")";

  QModelIndex parent = index.parent();

  if (!parent.isValid()) os << " in root";

  while (parent.isValid()) {
    os << " in (" << parent.row() << "," << parent.column() << ")";

    parent = parent.parent();
  }

  os << "]";

  return os;
}

void TreeItemQModelTestBase::checkIndexData(const QModelIndex& index,
                                            const ExpectRowData& expect_data) {
  auto q_display_text = index.data(Qt::DisplayRole).toString();
  auto actual_display_text = QStringToU8String(q_display_text);
  ASSERT_EQ(expect_data.text, actual_display_text)
      << "index: " << index << " text not match"
      << " (text: " << expect_data.text << ")";

  int actual_row_count = qmodel->qmodel()->rowCount(index);
  int expect_row_count = expectItemCount(expect_data);
  ASSERT_EQ(expect_row_count, actual_row_count)
      << "index: " << index << " row count not match"
      << " (text: " << expect_data.text << ")";

  int actual_col_count = qmodel->qmodel()->columnCount(index);
  ASSERT_EQ(1, actual_col_count) << "index: " << index << " col count not match"
                                 << " (text: " << expect_data.text << ")";

  auto actual_item = qmodel->indexToItem(index);
  ASSERT_EQ(expect_data.item_ptr, actual_item)
      << "index: " << index << " item ptr not match"
      << " (text: " << expect_data.text << ")";

  if (expect_data.item_ptr) {
    QModelIndex item_to_index = qmodel->itemToIndex(expect_data.item_ptr);
    ASSERT_EQ(index, item_to_index) << "index: " << index
                                    << " item index not match"
                                    << " (text: " << expect_data.text << ")";
  }

  bool actual_isAddMore = qmodel->isAddMore(index);
  bool expect_isAddMore = (expect_data.text == ADD_MORE_ROW_TEXT);
  ASSERT_EQ(expect_isAddMore, actual_isAddMore)
      << "index: " << index << " isAddMore not match"
      << " (text: " << expect_data.text << ")";

  bool expect_selectable = expect_data.isSelectable;
  if (always_selectable) expect_selectable = true;

  auto actual_selectable =
      qmodel->qmodel()->flags(index) & Qt::ItemIsSelectable;
  ASSERT_EQ(expect_selectable, actual_selectable)
      << "index: " << index << " isSelectable not match"
      << " (text: " << expect_data.text << ")";

  QModelIndex parent = qmodel->qmodel()->parent(index);
  auto actual_parent_ptr = qmodel->indexToItem(parent);
  ASSERT_EQ(expect_data.parent_item_ptr, actual_parent_ptr)
      << "index: " << index << " parent not match"
      << " (text: " << expect_data.text << ")";
}

void TreeItemQModelTestBase::
    should_provider_BeginFilter_emit_modelAboutToReset() {  // NOLINT
  QSignalSpy sigspy(qmodel->qmodel(), SIGNAL(modelAboutToBeReset()));

  // Exercise system
  providerBeginFilter();

  // Verify results
  ASSERT_EQ(1, sigspy.count());
}

void TreeItemQModelTestBase::should_provider_FinishFilter_emit_modelReset() {
  // Setup fixture
  // check before reset
  CUSTOM_ASSERT(checkRowData());

  providerBeginFilter();

  QSignalSpy sigspy(qmodel->qmodel(), SIGNAL(modelReset()));

  // Exercise system
  providerFinishFilter();

  // Verify results
  ASSERT_EQ(1, sigspy.count());

  item_provider.fillRowData(&root_row_data_after_reset);
  CUSTOM_ASSERT(checkRowData(root_row_data_after_reset));
}

void TreeItemQModelTestBase::expandToLevel(int level) {
  level = adjustLevel(level);
  expandToLevel(level, QModelIndex());
}

void TreeItemQModelTestBase::expandToLevel(int level,
                                           const QModelIndex& parent_index) {
  int row_count = qmodel->qmodel()->rowCount(parent_index);

  if (level == 0) return;

  // expand children
  for (int row = 0; row < row_count; ++row) {
    auto index = qmodel->qmodel()->index(row, 0, parent_index);
    expandToLevel(level - 1, index);
  }
}

void TreeItemQModelTestBase::addItemUnderLevel1Item1(
    ITreeItem** parent_item_ret) {  // NOLINT
  auto new_item_name = xtestutils::genRandomString();
  TreeItemTestStub new_item{new_item_name, false};

  // add under "Level1 Item1" @ index 0
  ASSERT_EQ(&level2_row_data, level1_row_data[0].subitems);
  ITreeItem* parent_item = level1_row_data[0].item_ptr;
  if (parent_item_ret) *parent_item_ret = parent_item;

  ExpectRowData new_row_data = {
      .text = new_item_name,
      .item_ptr = &new_item,
      .parent_item_ptr = parent_item,
      .isSelectable = true,
      .subitems = nullptr,
  };

  level2_row_data.push_back(new_row_data);
  item_provider.setItemOwnerRow(&new_item, &level2_row_data);

  // Exercise system
  itemAdded(&new_item, parent_item);

  // Verify results
  CUSTOM_ASSERT(checkRowData());

  // Teardown fixture
  level2_row_data.pop_back();
}

void TreeItemQModelTestBase::test_add_item_to_not_visible_parent_item() {
  // NOTE: initally all items are not pupulated

  QSignalSpy begin_insert_sigspy(
      qmodel->qmodel(),
      SIGNAL(rowsAboutToBeInserted(const QModelIndex&, int, int)));
  QSignalSpy end_insert_sigspy(
      qmodel->qmodel(), SIGNAL(rowsInserted(const QModelIndex&, int, int)));

  CUSTOM_ASSERT(addItemUnderLevel1Item1());

  // should not insert rows because parent is not populated yet
  ASSERT_EQ(0, begin_insert_sigspy.count());
  ASSERT_EQ(0, end_insert_sigspy.count());
}

void TreeItemQModelTestBase::
    test_add_item_to_visible_but_children_not_populated_item() {  // NOLINT
  // ensure "Level1 Item1" is visible, but children not populated
  expandToLevel(1);

  // the result is same as not visible items
  CUSTOM_ASSERT(test_add_item_to_not_visible_parent_item());
}

void TreeItemQModelTestBase::
    test_add_item_to_children_populated_parent_item() {  // NOLINT
  expandToLevel(2);

  QSignalSpy begin_insert_sigspy(
      qmodel->qmodel(),
      SIGNAL(rowsAboutToBeInserted(const QModelIndex&, int, int)));
  QSignalSpy end_insert_sigspy(
      qmodel->qmodel(), SIGNAL(rowsInserted(const QModelIndex&, int, int)));

  // Exercise system
  ITreeItem* parent_item;
  CUSTOM_ASSERT(addItemUnderLevel1Item1(&parent_item));

  // Verify result
  // new row is inserted at pos 1
  QList<QVariant> arguments = begin_insert_sigspy.takeFirst();
  auto parent_idx = qvariant_cast<QModelIndex>(arguments.at(0));
  ASSERT_EQ(parent_item, qmodel->indexToItem(parent_idx));

  int insert_row_first = qvariant_cast<int>(arguments.at(1));
  int insert_row_last = qvariant_cast<int>(arguments.at(2));
  ASSERT_EQ(1, insert_row_first);
  ASSERT_EQ(1, insert_row_last);
}

void TreeItemQModelTestBase::addItemUnderRootItem() {
  auto new_item_name = xtestutils::genRandomString();
  TreeItemTestStub new_item{new_item_name, false};

  ExpectRowData new_row_data = {
      .text = new_item_name,
      .item_ptr = &new_item,
      .parent_item_ptr = nullptr,
      .isSelectable = true,
      .subitems = nullptr,
  };

  auto insert_pos = level0_row_data.end();
  --insert_pos;  // before add more row

  level0_row_data.insert(insert_pos, new_row_data);
  item_provider.setItemOwnerRow(&new_item, &level0_row_data);

  // Exercise system
  itemAdded(&new_item, nullptr);

  // Verify results
  CUSTOM_ASSERT(checkRowData());

  // Teardown fixture
  auto new_item_ptr = &new_item;
  auto iter = std::find_if(level0_row_data.begin(), level0_row_data.end(),
                           [new_item_ptr](const ExpectRowData& item) {
                             if (item.item_ptr == new_item_ptr) return true;

                             return false;
                           });
  if (iter != level0_row_data.end()) {
    level0_row_data.erase(iter);
  }
}

void TreeItemQModelTestBase::test_add_item_to_unexpanded_root_item() {
  QSignalSpy begin_insert_sigspy(
      qmodel->qmodel(),
      SIGNAL(rowsAboutToBeInserted(const QModelIndex&, int, int)));
  QSignalSpy end_insert_sigspy(
      qmodel->qmodel(), SIGNAL(rowsInserted(const QModelIndex&, int, int)));

  // Exercise system
  CUSTOM_ASSERT(addItemUnderRootItem());

  // Verify result
  // should not insert rows
  ASSERT_EQ(0, begin_insert_sigspy.count());
  ASSERT_EQ(0, end_insert_sigspy.count());
}

void TreeItemQModelTestBase::test_add_item_to_expanded_root_item() {
  expandToLevel(0);

  int expect_insert_row = 0;
  for (auto& row_data : level0_row_data) {
    if (row_data.text == ADD_MORE_ROW_TEXT) break;

    if (shouldRowVisible(row_data)) ++expect_insert_row;
  }

  QSignalSpy begin_insert_sigspy(
      qmodel->qmodel(),
      SIGNAL(rowsAboutToBeInserted(const QModelIndex&, int, int)));
  QSignalSpy end_insert_sigspy(
      qmodel->qmodel(), SIGNAL(rowsInserted(const QModelIndex&, int, int)));

  // Exercise system
  CUSTOM_ASSERT(addItemUnderRootItem());

  // Verify result
  ASSERT_EQ(1, begin_insert_sigspy.count());
  ASSERT_EQ(1, end_insert_sigspy.count());

  QList<QVariant> arguments = begin_insert_sigspy.takeFirst();
  auto parent_idx = qvariant_cast<QModelIndex>(arguments.at(0));
  ASSERT_EQ(nullptr, qmodel->indexToItem(parent_idx));

  int insert_row_first = qvariant_cast<int>(arguments.at(1));
  int insert_row_last = qvariant_cast<int>(arguments.at(2));
  ASSERT_EQ(expect_insert_row, insert_row_first);
  ASSERT_EQ(expect_insert_row, insert_row_last);
}

TEST_F(TreeItemQModelTestBase, check_row_data) {  // NOLINT
  CUSTOM_ASSERT(checkRowData());
}

TEST_F(TreeItemQModelTestBase,
       should_provider_BeginFilter_emit_modelAboutToReset) {  // NOLINT
  CUSTOM_ASSERT(should_provider_BeginFilter_emit_modelAboutToReset());
}

TEST_F(TreeItemQModelTestBase,
       should_provider_FinishFilter_emit_modelReset) {  // NOLINT
  CUSTOM_ASSERT(should_provider_FinishFilter_emit_modelReset());
}

TEST_F(TreeItemQModelTestBase,
       test_add_item_to_not_visible_parent_item) {  // NOLINT
  CUSTOM_ASSERT(test_add_item_to_not_visible_parent_item());
}

TEST_F(TreeItemQModelTestBase,
       test_add_item_to_visible_but_children_not_populated_item) {  // NOLINT
  CUSTOM_ASSERT(test_add_item_to_visible_but_children_not_populated_item());
}

TEST_F(TreeItemQModelTestBase,
       test_add_item_to_children_populated_parent_item) {  // NOLINT
  CUSTOM_ASSERT(test_add_item_to_children_populated_parent_item());
}

TEST_F(TreeItemQModelTestBase,
       test_add_item_to_unexpanded_root_item) {  // NOLINT
  CUSTOM_ASSERT(test_add_item_to_unexpanded_root_item());
}

TEST_F(TreeItemQModelTestBase, test_add_item_to_expanded_root_item) {  // NOLINT
  CUSTOM_ASSERT(test_add_item_to_expanded_root_item());
}

TEST_F(TreeItemQModelTestBase,
       should_null_item_to_index_got_invalid_root_idx) {  // NOLINT
  QModelIndex item_to_index = qmodel->itemToIndex(nullptr);
  auto expect_index = QModelIndex();

  ASSERT_EQ(expect_index, item_to_index)
      << "null item_ptr should got root invalid index in completion mode";
}

class TreeItemQModelWithClearAndAddMoreRowTest : public TreeItemQModelTestBase {
 protected:
  void SetUp() override {
    TreeItemQModelTestBase::SetUp();

    show_clear_row = true;
    show_add_more_row = true;
  }

  std::shared_ptr<TreeItemQModel> createQModel() override {
    return std::make_shared<TreeItemQModelWithClearAndAddMoreRow>();
  }
};

TEST_F(TreeItemQModelWithClearAndAddMoreRowTest, check_row_data) {  // NOLINT
  CUSTOM_ASSERT(checkRowData());
}

TEST_F(TreeItemQModelWithClearAndAddMoreRowTest,
       should_provider_BeginFilter_emit_modelAboutToReset) {  // NOLINT
  CUSTOM_ASSERT(should_provider_BeginFilter_emit_modelAboutToReset());
}

TEST_F(TreeItemQModelWithClearAndAddMoreRowTest,
       should_provider_FinishFilter_emit_modelReset) {  // NOLINT
  CUSTOM_ASSERT(should_provider_FinishFilter_emit_modelReset());
}

TEST_F(TreeItemQModelWithClearAndAddMoreRowTest,
       test_add_item_to_not_visible_parent_item) {  // NOLINT
  CUSTOM_ASSERT(test_add_item_to_not_visible_parent_item());
}

TEST_F(TreeItemQModelWithClearAndAddMoreRowTest,
       test_add_item_to_visible_but_children_not_populated_item) {  // NOLINT
  CUSTOM_ASSERT(test_add_item_to_visible_but_children_not_populated_item());
}

TEST_F(TreeItemQModelWithClearAndAddMoreRowTest,
       test_add_item_to_children_populated_parent_item) {  // NOLINT
  CUSTOM_ASSERT(test_add_item_to_children_populated_parent_item());
}

TEST_F(TreeItemQModelWithClearAndAddMoreRowTest,
       test_add_item_to_unexpanded_root_item) {  // NOLINT
  CUSTOM_ASSERT(test_add_item_to_unexpanded_root_item());
}

TEST_F(TreeItemQModelWithClearAndAddMoreRowTest,
       test_add_item_to_expanded_root_item) {  // NOLINT
  CUSTOM_ASSERT(test_add_item_to_expanded_root_item());
}

TEST_F(TreeItemQModelWithClearAndAddMoreRowTest,
       should_null_item_to_index_got_invalid_root_idx) {  // NOLINT
  QModelIndex item_to_index = qmodel->itemToIndex(nullptr);
  auto expect_index = QModelIndex();

  ASSERT_EQ(expect_index, item_to_index)
      << "null item_ptr should got root invalid index in completion mode";
}

class TreeItemQModelWithProviderItemTest : public TreeItemQModelTestBase {
 protected:
  void SetUp() override {
    TreeItemQModelTestBase::SetUp();

    always_selectable = true;

    show_clear_row = false;
    show_add_more_row = false;

    root_row_data.subitems = &provider_row_data;
    root_row_data_after_reset.subitems = &provider_row_data_after_reset;

    ASSERT_EQ(&level0_row_data, provider_row_data[0].subitems);
  }

  void TearDown() override {
    root_row_data.subitems = &level0_row_data;
    root_row_data_after_reset.subitems = &level0_row_data_after_reset;

    TreeItemQModelTestBase::TearDown();
  }

  std::shared_ptr<TreeItemQModel> createQModel() override {
    return std::make_shared<TreeItemQModelWithProviderRoot>();
  }

  int adjustLevel(int level) override { return level + 1; }
};

TEST_F(TreeItemQModelWithProviderItemTest,
       should_display_the_provider_name_as_a_visible_root_item) {  // NOLINT
  CUSTOM_ASSERT(checkRowData());
}

TEST_F(TreeItemQModelWithProviderItemTest,
       should_provider_BeginFilter_emit_modelAboutToReset) {  // NOLINT
  CUSTOM_ASSERT(should_provider_BeginFilter_emit_modelAboutToReset());
}

TEST_F(TreeItemQModelWithProviderItemTest,
       should_provider_FinishFilter_emit_modelReset) {  // NOLINT
  CUSTOM_ASSERT(should_provider_FinishFilter_emit_modelReset());
}

TEST_F(TreeItemQModelWithProviderItemTest,
       test_add_item_to_not_visible_parent_item) {  // NOLINT
  CUSTOM_ASSERT(test_add_item_to_not_visible_parent_item());
}

TEST_F(TreeItemQModelWithProviderItemTest,
       test_add_item_to_visible_but_children_not_populated_item) {  // NOLINT
  CUSTOM_ASSERT(test_add_item_to_visible_but_children_not_populated_item());
}

TEST_F(TreeItemQModelWithProviderItemTest,
       test_add_item_to_children_populated_parent_item) {  // NOLINT
  CUSTOM_ASSERT(test_add_item_to_children_populated_parent_item());
}

TEST_F(TreeItemQModelWithProviderItemTest,
       test_add_item_to_unexpanded_root_item) {  // NOLINT
  CUSTOM_ASSERT(test_add_item_to_unexpanded_root_item());
}

TEST_F(TreeItemQModelWithProviderItemTest,
       test_add_item_to_expanded_root_item) {  // NOLINT
  CUSTOM_ASSERT(test_add_item_to_expanded_root_item());
}

TEST_F(TreeItemQModelWithProviderItemTest,
       should_null_item_to_index_got_provider_item) {  // NOLINT
  // Setup fixture
  auto expect_index = qmodel->qmodel()->index(0, 0, QModelIndex());

  // Verify results
  QModelIndex item_to_index = qmodel->itemToIndex(nullptr);
  ASSERT_EQ(expect_index, item_to_index)
      << "null item_ptr should got provider row index in add item mode";
}
