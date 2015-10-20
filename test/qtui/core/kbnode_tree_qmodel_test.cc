//-*- TestCaseName: KbNodeTreeQModelTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <QSignalSpy>

#include "test/testutils/gmock_common.h"
#include "test/testutils/qmodelindex_generator.h"

#include "src/qtui/core/kbnode_tree_qmodel.h"
#include "snail/mock_tree_item_provider.h"
#include "snail/mock_kbnode.h"

using namespace snailcore;  // NOLINT
using namespace snailcore::tests;  // NOLINT

// test data structure
// -------------------
// (root)
//    |---(empty line)   (0, 0)
//    |---level0 node1   (1, 0)
//    |   |----level1 node1    (0, 0)
//    |   |    |---level2 node1      (0, 0)
//    |   |----level1 node2    (1, 0)
//    |---level0 node2    (2, 0)
//    |---Add More...     (3, 0)
//
// as we use lazy populate for our model impl,
// w using these test data to test the following 4 dynamic add node cases
// 1. add to invisible kbnodes
// 2. add to visible but children not populated node
// 3. add to visible and chidren is populated node
// 4. add to root node should add before ADD_MORE_ROW_TEXT row

constexpr static int kEmptySpecialRow = 0;
constexpr static int kAddMoreSpecialRow = 3;
#define PROVIDER_NAME "Provider Name"
#define CLEAR_ROW_TEXT "(clear)"
#define ADD_MORE_ROW_TEXT "Add More..."

class ExpectRowData {
 public:
  utils::U8String text;
  IKbNode* node_ptr;
  IKbNode* parent_node_ptr;
  bool isSelectable;

  std::vector<ExpectRowData>* subnodes;

  bool isSpecialRow() const {
    return (text == CLEAR_ROW_TEXT) ||
        (text == ADD_MORE_ROW_TEXT);
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
  level2_row_data = {
    {
      .text = "Level2 Node1",
      .node_ptr = nullptr,
      .parent_node_ptr = nullptr,
      .isSelectable = true,
      .subnodes = nullptr,
    }
  };

  level1_row_data = {
    {
      .text = "Level1 Node1",
      .node_ptr = nullptr,
      .parent_node_ptr = nullptr,
      .isSelectable = true,
      .subnodes = &level2_row_data,
    },
    {
      .text = "Level1 Node2",
      .node_ptr = nullptr,
      .parent_node_ptr = nullptr,
      .isSelectable = false,
      .subnodes = nullptr,
    }
  };

  level0_row_data = {
    {  // level1
      .text = CLEAR_ROW_TEXT,
      .node_ptr = nullptr,
      .parent_node_ptr = nullptr,
      .isSelectable = true,
      .subnodes = nullptr
    },
    {
      .text = "Level0 Node1",
      .node_ptr = nullptr,
      .parent_node_ptr = nullptr,
      .isSelectable = false,
      .subnodes = &level1_row_data,
    },
    {
      .text = "Level0 Node2",
      .node_ptr = nullptr,
      .parent_node_ptr = nullptr,
      .isSelectable = true,
      .subnodes = nullptr,
    },
    {
      .text = ADD_MORE_ROW_TEXT,
      .node_ptr = nullptr,
      .parent_node_ptr = nullptr,
      .isSelectable = true,
      .subnodes = nullptr,
    }
  };

  provider_row_data = {
    {
      .text = PROVIDER_NAME,
      .node_ptr = nullptr,
      .parent_node_ptr = nullptr,
      .isSelectable = true,
      .subnodes = &level0_row_data,
    }
  };

  root_row_data = {
    .text = "root idx",
    .node_ptr = nullptr,
    .parent_node_ptr = nullptr,
    .isSelectable = false,
    .subnodes = &level0_row_data,
  };

  level0_row_data_after_reset = {
    {  // level1
      .text = CLEAR_ROW_TEXT,
      .node_ptr = nullptr,
      .parent_node_ptr = nullptr,
      .isSelectable = true,
      .subnodes = nullptr
    },
    {
      .text = "Level0 Node1 After Reset",
      .node_ptr = nullptr,
      .parent_node_ptr = nullptr,
      .isSelectable = false,
      .subnodes = nullptr,
    },
    {
      .text = "Level0 Node2 After Reset",
      .node_ptr = nullptr,
      .parent_node_ptr = nullptr,
      .isSelectable = true,
      .subnodes = nullptr,
    },
    {
      .text = ADD_MORE_ROW_TEXT,
      .node_ptr = nullptr,
      .parent_node_ptr = nullptr,
      .isSelectable = true,
      .subnodes = nullptr,
    }
  };

  provider_row_data_after_reset = {
    {
      .text = PROVIDER_NAME,
      .node_ptr = nullptr,
      .parent_node_ptr = nullptr,
      .isSelectable = true,
      .subnodes = &level0_row_data_after_reset,
    }
  };

  root_row_data_after_reset = {
    .text = "root idx",
    .node_ptr = nullptr,
    .parent_node_ptr = nullptr,
    .isSelectable = false,
    .subnodes = &level0_row_data_after_reset,
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
  class ChildNodeIteratorTestStub : public IChildItemIterator {
   public:
    ChildNodeIteratorTestStub(const TreeItemProviderTestStub& provider,
                              IKbNode* parent_node)
        : row_data_(provider.nodeToRowData(parent_node)) {
      if (row_data_->subnodes) {
        max_idx_ = row_data_->subnodes->size() - 1;
      }

      if ((row_data_->subnodes == &level0_row_data) ||
          (row_data_->subnodes == &level0_row_data_after_reset)) {
        cur_idx_ = 1;  // skip empty row
        --max_idx_;     // skip add more row
      }
    }
    virtual ~ChildNodeIteratorTestStub() = default;

    bool hasNext() const override {
      if (cur_idx_ > max_idx_)
        return false;

      return true;
    }

    IKbNode* next() override {
      if (!hasNext())
        throw std::logic_error("no next kbnode");

      auto kbnode = (*row_data_->subnodes)[cur_idx_].node_ptr;
      ++cur_idx_;

      return kbnode;
    }

   private:
    SNAIL_DISABLE_COPY(ChildNodeIteratorTestStub);

    ExpectRowData* row_data_;
    mutable int cur_idx_ { 0 };
    int max_idx_ { -1 };
  };

  TreeItemProviderTestStub() { }
  virtual ~TreeItemProviderTestStub() { }

  utils::U8String name() const {
    return PROVIDER_NAME;
  }

  std::unique_ptr<IChildItemIterator>
  childItems(IKbNode* parent_node) const override {
    return utils::make_unique<ChildNodeIteratorTestStub>(*this, parent_node);
  }

  void setKbNodeOwnerRow(IKbNode* node,
                         std::vector<ExpectRowData>* row_data_vec) {
    node_to_row_data_vec_[node] = row_data_vec;
  }

  void fillRowData(ExpectRowData* root_rdata) {
    kbnodes_.clear();
    node_to_row_data_vec_.clear();
    fillNodePtrInRowData(root_rdata->subnodes);
    root_rdata_ = root_rdata;
  }

  MOCK_CONST_METHOD0(isFilterMode, bool());

  // unintrested method, using mocks
  SNAIL_MOCK_SLOT(BeginFilter);
  SNAIL_MOCK_SLOT(FinishFilter);
  MOCK_METHOD1(setFilterPattern, void(const utils::U8String& filter_pattern));
  MOCK_CONST_METHOD0(getFilterPattern, utils::U8String());
  MOCK_METHOD1(incRef, void(IKbNode* kbnode));

  MOCK_CONST_METHOD0(getRootItem, IKbNode*());

  MOCK_METHOD1(findItemByName,
               std::vector<IKbNode*>(const utils::U8String& name));

 private:
  SNAIL_DISABLE_COPY(TreeItemProviderTestStub);

  void fillNodePtrInRowData(std::vector<ExpectRowData>* row_datas,
                            IKbNode* parent_node = nullptr) {
    for (auto & row_data : *row_datas) {
      if (row_data.isSpecialRow())
        continue;

      MockKbNode* kbnode = nullptr;
      if (row_data.text != PROVIDER_NAME) {
        auto kbnode_up = utils::make_unique<MockKbNode>();
        kbnode = kbnode_up.get();
        kbnodes_.push_back(std::move(kbnode_up));

        EXPECT_CALL(*kbnode, name())
            .WillRepeatedly(Return(row_data.text));
        EXPECT_CALL(*kbnode, isCategory())
            .WillRepeatedly(Return(!row_data.isSelectable));

        row_data.node_ptr = kbnode;
        node_to_row_data_vec_[kbnode] = row_datas;

        row_data.parent_node_ptr = parent_node;
      }

      if (row_data.subnodes)
        fillNodePtrInRowData(row_data.subnodes, kbnode);
    }
  }

  ExpectRowData* nodeToRowData(IKbNode* node) const {
    if (!node) {
      if ((*root_rdata_->subnodes)[0].text == PROVIDER_NAME)
        return &(*root_rdata_->subnodes)[0];
      else
        return root_rdata_;
    }

    try {
      auto row_data_vec = node_to_row_data_vec_.at(node);
      for (auto & row_data : *row_data_vec) {
        if (row_data.node_ptr == node)
          return &row_data;
      }
    } catch (...) {
      std::cout << "node: " << node->name()
                << " row data not found" << std::endl;
      /* re */ throw;
    }

    return nullptr;
  }

  std::vector<std::unique_ptr<MockKbNode>> kbnodes_;
  std::map<IKbNode*, std::vector<ExpectRowData>*> node_to_row_data_vec_;
  ExpectRowData* root_rdata_ { nullptr };
};

class KbNodeTreeQModelTestBase : public ::testing::Test {
 protected:
  KbNodeTreeQModelTestBase() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~KbNodeTreeQModelTestBase() { }
  void SetUp() override {
    qmodel = createQModel();

    setupExpectRowDatas();
    kbnode_provider.fillRowData(&root_row_data);

    qmodel->setTreeItemProvider(&kbnode_provider);
  }

  void TearDown() override {
    cleanupExpectRowDatas();
  }

  virtual std::unique_ptr<KbNodeTreeQModelBasic> createQModel() {
    return utils::make_unique<KbNodeTreeQModelBasic>();
  }

  bool shouldRowVisible(const ExpectRowData& row_data);
  int expectNodeCount(const ExpectRowData& row_data);

  void checkRowData(const ExpectRowData& root_rdata = root_row_data);
  void checkRowData(const std::vector<ExpectRowData>& row_data,
                    const QModelIndex& parent_index);
  void checkIndexData(const QModelIndex& index,
                      const ExpectRowData& rdata);

  void should_beginResetQModel_emit_modelAboutToReset();
  void should_endResetQModel_emit_modelReset();

  virtual int adjustLevel(int level) { return level; }
  void expandToLevel(int level);
  void expandToLevel(int level, const QModelIndex& parent_index);
  void addKbNodeUnderLevel1Node1(IKbNode** parent_kbnode_ret = nullptr);
  void test_add_kbnode_to_not_visible_parent_kbnode();
  void test_add_kbnode_to_visible_but_children_not_populated_kbnode();
  void test_add_kbnode_to_children_populated_parent_kbnode();

  void addKbNodeUnderRootNode();
  void test_add_kbnode_to_unexpanded_root_node();
  void test_add_kbnode_to_expanded_root_node();

  // region: objects test subject depends on
  TreeItemProviderTestStub kbnode_provider;
  QModelIndexGenerator index_generator;

  bool always_selectable { false };
  bool show_clear_row { false };
  bool show_add_more_row { false };
  // endregion

  // region: test subject
  std::unique_ptr<KbNodeTreeQModelBasic> qmodel;
  // endregion

  // region: object depends on test subject
  // endregion
};

bool KbNodeTreeQModelTestBase::shouldRowVisible(const ExpectRowData& row_data) {
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

int KbNodeTreeQModelTestBase::expectNodeCount(const ExpectRowData& row_data) {
  if (row_data.subnodes == nullptr)
    return 0;

  int count = 0;
  for (auto & sub_rdata : *row_data.subnodes) {
    if (shouldRowVisible(sub_rdata)) {
      ++count;
    }
  }

  return count;
}

void KbNodeTreeQModelTestBase::checkRowData(const ExpectRowData& root_rdata) {
  ASSERT_EQ(expectNodeCount(root_rdata), qmodel->rowCount(QModelIndex()));
  ASSERT_EQ(1, qmodel->columnCount(QModelIndex()));
  QModelIndex expect_parent_idx;
  ASSERT_EQ(expect_parent_idx, qmodel->parent(QModelIndex()));

  // check sub items
  checkRowData(*root_rdata.subnodes, QModelIndex());
}

void KbNodeTreeQModelTestBase::checkRowData(
    const std::vector<ExpectRowData>& row_data,
    const QModelIndex& parent_index) {
  int row = 0;
  for (size_t idx = 0; idx < row_data.size(); ++idx) {
    auto& rdata = row_data[idx];

    if (!shouldRowVisible(rdata))
      continue;

    QModelIndex index = qmodel->index(row, 0, parent_index);
    ASSERT_EQ(row, index.row());
    checkIndexData(index, rdata);
    ++row;

    if (rdata.subnodes) {
      checkRowData(*rdata.subnodes, index);
    }
  }
}

std::ostream &operator<<(std::ostream &os, const QModelIndex& index) {
  os << "[(" << index.row() << "," << index.column() << ")";

  QModelIndex parent = index.parent();

  if (!parent.isValid())
    os << " in root";

  while (parent.isValid()) {
    os << " in (" << parent.row() << "," << parent.column() << ")";

    parent = parent.parent();
  }

  os << "]";

  return os;
}

void KbNodeTreeQModelTestBase::checkIndexData(
    const QModelIndex& index, const ExpectRowData& expect_data) {
  auto q_display_text = index.data(Qt::DisplayRole).toString();
  auto actual_display_text = QStringToU8String(q_display_text);
  ASSERT_EQ(expect_data.text, actual_display_text)
      << "index: " << index << " text not match"
      << " (text: " << expect_data.text << ")";

  int actual_row_count = qmodel->rowCount(index);
  int expect_row_count = expectNodeCount(expect_data);
  ASSERT_EQ(expect_row_count, actual_row_count)
      << "index: " << index << " row count not match"
      << " (text: " << expect_data.text << ")";

  int actual_col_count = qmodel->columnCount(index);
  ASSERT_EQ(1, actual_col_count)
      << "index: " << index << " col count not match"
      << " (text: " << expect_data.text << ")";

  auto actual_kbnode = qmodel->indexToKbNode(index);
  ASSERT_EQ(expect_data.node_ptr, actual_kbnode)
      << "index: " << index << " kbnode ptr not match"
      << " (text: " << expect_data.text << ")";

  if (expect_data.node_ptr) {
    QModelIndex kbnode_to_index = qmodel->kbNodeToIndex(expect_data.node_ptr);
    ASSERT_EQ(index, kbnode_to_index)
        << "index: " << index << " kbnode index not match"
        << " (text: " << expect_data.text << ")";
  }

  bool actual_isAddMore = qmodel->isAddKbNode(index);
  bool expect_isAddMore = (expect_data.text == ADD_MORE_ROW_TEXT);
  ASSERT_EQ(expect_isAddMore, actual_isAddMore)
      << "index: " << index << " isAddMore not match"
      << " (text: " << expect_data.text << ")";

  bool expect_selectable = expect_data.isSelectable;
  if (always_selectable)
    expect_selectable = true;

  auto actual_selectable = qmodel->flags(index) & Qt::ItemIsSelectable;
  ASSERT_EQ(expect_selectable, actual_selectable)
      << "index: " << index << " isSelectable not match"
      << " (text: " << expect_data.text << ")";

  QModelIndex parent = qmodel->parent(index);
  auto actual_parent_ptr = qmodel->indexToKbNode(parent);
  ASSERT_EQ(expect_data.parent_node_ptr, actual_parent_ptr)
      << "index: " << index << " parent not match"
      << " (text: " << expect_data.text << ")";
}

void KbNodeTreeQModelTestBase::should_beginResetQModel_emit_modelAboutToReset() { // NOLINT
  QSignalSpy sigspy(qmodel.get(), SIGNAL(modelAboutToBeReset()));

  // Exercise system
  qmodel->beginResetQModel();

  // Verify results
  ASSERT_EQ(1, sigspy.count());
}

void KbNodeTreeQModelTestBase::should_endResetQModel_emit_modelReset() {
  // Setup fixture
  // check before reset
  CUSTOM_ASSERT(checkRowData());

  qmodel->beginResetQModel();

  QSignalSpy sigspy(qmodel.get(), SIGNAL(modelReset()));

  // Exercise system
  qmodel->endResetQModel();

  // Verify results
  ASSERT_EQ(1, sigspy.count());

  kbnode_provider.fillRowData(&root_row_data_after_reset);
  CUSTOM_ASSERT(checkRowData(root_row_data_after_reset));
}

void KbNodeTreeQModelTestBase::expandToLevel(int level) {
  level = adjustLevel(level);
  expandToLevel(level, QModelIndex());
}

void KbNodeTreeQModelTestBase::expandToLevel(
    int level, const QModelIndex& parent_index) {
  int row_count = qmodel->rowCount(parent_index);

  if (level ==  0)
    return;

  // expand children
  for (int row = 0; row < row_count; ++row) {
    auto index = qmodel->index(row, 0, parent_index);
    expandToLevel(level - 1, index);
  }
}

void KbNodeTreeQModelTestBase::addKbNodeUnderLevel1Node1(IKbNode** parent_kbnode_ret) { // NOLINT
  auto new_kbnode_name = xtestutils::genRandomString();
  MockKbNode new_kbnode;

  EXPECT_CALL(new_kbnode, name())
      .WillRepeatedly(Return(new_kbnode_name));
  EXPECT_CALL(new_kbnode, isCategory())
      .WillRepeatedly(Return(false));

  // add under "Level1 Node1" @ index 0
  ASSERT_EQ(&level2_row_data, level1_row_data[0].subnodes);
  IKbNode* parent_kbnode = level1_row_data[0].node_ptr;
  if (parent_kbnode_ret)
    *parent_kbnode_ret = parent_kbnode;

  ExpectRowData new_row_data = {
    .text = new_kbnode_name,
    .node_ptr = &new_kbnode,
    .parent_node_ptr = parent_kbnode,
    .isSelectable = true,
    .subnodes = nullptr,
  };

  level2_row_data.push_back(new_row_data);
  kbnode_provider.setKbNodeOwnerRow(&new_kbnode, &level2_row_data);

  // Exercise system
  qmodel->kbNodeAdded(&new_kbnode, parent_kbnode);

  // Verify results
  CUSTOM_ASSERT(checkRowData());

  // Teardown fixture
  level2_row_data.pop_back();
}

void KbNodeTreeQModelTestBase::test_add_kbnode_to_not_visible_parent_kbnode() {
  // NOTE: initally all kbnodes are not pupulated

  QSignalSpy begin_insert_sigspy(qmodel.get(),
                                 SIGNAL(rowsAboutToBeInserted(
                                     const QModelIndex&, int, int)));
  QSignalSpy end_insert_sigspy(qmodel.get(),
                               SIGNAL(rowsInserted(
                                   const QModelIndex&, int, int)));

  CUSTOM_ASSERT(addKbNodeUnderLevel1Node1());

  // should not insert rows because parent is not populated yet
  ASSERT_EQ(0, begin_insert_sigspy.count());
  ASSERT_EQ(0, end_insert_sigspy.count());
}

void KbNodeTreeQModelTestBase::test_add_kbnode_to_visible_but_children_not_populated_kbnode() { // NOLINT
  // ensure "Level1 Node1" is visible, but children not populated
  expandToLevel(1);

  // the result is same as not visible kbnodes
  CUSTOM_ASSERT(test_add_kbnode_to_not_visible_parent_kbnode());
}

void KbNodeTreeQModelTestBase::test_add_kbnode_to_children_populated_parent_kbnode() { // NOLINT
  expandToLevel(2);

  QSignalSpy begin_insert_sigspy(qmodel.get(),
                                 SIGNAL(rowsAboutToBeInserted(
                                     const QModelIndex&, int, int)));
  QSignalSpy end_insert_sigspy(qmodel.get(),
                               SIGNAL(rowsInserted(
                                   const QModelIndex&, int, int)));

  // Exercise system
  IKbNode* parent_kbnode;
  CUSTOM_ASSERT(addKbNodeUnderLevel1Node1(&parent_kbnode));

  // Verify result
  // new row is inserted at pos 1
  QList<QVariant> arguments = begin_insert_sigspy.takeFirst();
  auto parent_idx = qvariant_cast<QModelIndex>(arguments.at(0));
  ASSERT_EQ(parent_kbnode, qmodel->indexToKbNode(parent_idx));

  int insert_row_first = qvariant_cast<int>(arguments.at(1));
  int insert_row_last = qvariant_cast<int>(arguments.at(2));
  ASSERT_EQ(1, insert_row_first);
  ASSERT_EQ(1, insert_row_last);
}

void KbNodeTreeQModelTestBase::addKbNodeUnderRootNode() {
  auto new_kbnode_name = xtestutils::genRandomString();
  MockKbNode new_kbnode;

  EXPECT_CALL(new_kbnode, name())
      .WillRepeatedly(Return(new_kbnode_name));
  EXPECT_CALL(new_kbnode, isCategory())
      .WillRepeatedly(Return(false));

  ExpectRowData new_row_data = {
    .text = new_kbnode_name,
    .node_ptr = &new_kbnode,
    .parent_node_ptr = nullptr,
    .isSelectable = true,
    .subnodes = nullptr,
  };

  auto insert_pos = level0_row_data.end();
  --insert_pos;  // before add more row

  level0_row_data.insert(insert_pos, new_row_data);
  kbnode_provider.setKbNodeOwnerRow(&new_kbnode, &level0_row_data);

  // Exercise system
  qmodel->kbNodeAdded(&new_kbnode, nullptr);

  // Verify results
  CUSTOM_ASSERT(checkRowData());

  // Teardown fixture
  auto new_kbnode_ptr = &new_kbnode;
  auto iter = std::find_if(level0_row_data.begin(),
                           level0_row_data.end(),
                           [new_kbnode_ptr](const ExpectRowData& item) {
                             if (item.node_ptr == new_kbnode_ptr)
                               return true;

                             return false;
                           });
  if (iter != level0_row_data.end()) {
    level0_row_data.erase(iter);
  }
}

void KbNodeTreeQModelTestBase::test_add_kbnode_to_unexpanded_root_node() {
  QSignalSpy begin_insert_sigspy(qmodel.get(),
                                 SIGNAL(rowsAboutToBeInserted(
                                     const QModelIndex&, int, int)));
  QSignalSpy end_insert_sigspy(qmodel.get(),
                               SIGNAL(rowsInserted(
                                   const QModelIndex&, int, int)));

  // Exercise system
  CUSTOM_ASSERT(addKbNodeUnderRootNode());

  // Verify result
  // should not insert rows
  ASSERT_EQ(0, begin_insert_sigspy.count());
  ASSERT_EQ(0, end_insert_sigspy.count());
}

void KbNodeTreeQModelTestBase::test_add_kbnode_to_expanded_root_node() {
  expandToLevel(0);

  int expect_insert_row = 0;
  for (auto & row_data : level0_row_data) {
    if (row_data.text == ADD_MORE_ROW_TEXT)
      break;

    if (shouldRowVisible(row_data))
      ++expect_insert_row;
  }

  QSignalSpy begin_insert_sigspy(qmodel.get(),
                                 SIGNAL(rowsAboutToBeInserted(
                                     const QModelIndex&, int, int)));
  QSignalSpy end_insert_sigspy(qmodel.get(),
                               SIGNAL(rowsInserted(
                                   const QModelIndex&, int, int)));

  // Exercise system
  CUSTOM_ASSERT(addKbNodeUnderRootNode());

  // Verify result
  ASSERT_EQ(1, begin_insert_sigspy.count());
  ASSERT_EQ(1, end_insert_sigspy.count());

  QList<QVariant> arguments = begin_insert_sigspy.takeFirst();
  auto parent_idx = qvariant_cast<QModelIndex>(arguments.at(0));
  ASSERT_EQ(nullptr, qmodel->indexToKbNode(parent_idx));

  int insert_row_first = qvariant_cast<int>(arguments.at(1));
  int insert_row_last = qvariant_cast<int>(arguments.at(2));
  ASSERT_EQ(expect_insert_row, insert_row_first);
  ASSERT_EQ(expect_insert_row, insert_row_last);
}

TEST_F(KbNodeTreeQModelTestBase,
       check_row_data) { // NOLINT
  CUSTOM_ASSERT(checkRowData());
}

TEST_F(KbNodeTreeQModelTestBase,
       should_beginResetQModel_emit_modelAboutToReset) { // NOLINT
  CUSTOM_ASSERT(should_beginResetQModel_emit_modelAboutToReset());
}

TEST_F(KbNodeTreeQModelTestBase,
       should_endResetQModel_emit_modelReset) { // NOLINT
  CUSTOM_ASSERT(should_endResetQModel_emit_modelReset());
}

TEST_F(KbNodeTreeQModelTestBase,
       test_add_kbnode_to_not_visible_parent_kbnode) { // NOLINT
  CUSTOM_ASSERT(test_add_kbnode_to_not_visible_parent_kbnode());
}

TEST_F(KbNodeTreeQModelTestBase,
       test_add_kbnode_to_visible_but_children_not_populated_kbnode) { // NOLINT
  CUSTOM_ASSERT(test_add_kbnode_to_visible_but_children_not_populated_kbnode());
}

TEST_F(KbNodeTreeQModelTestBase,
       test_add_kbnode_to_children_populated_parent_kbnode) { // NOLINT
  CUSTOM_ASSERT(test_add_kbnode_to_children_populated_parent_kbnode());
}

TEST_F(KbNodeTreeQModelTestBase,
       test_add_kbnode_to_unexpanded_root_node) { // NOLINT
  CUSTOM_ASSERT(test_add_kbnode_to_unexpanded_root_node());
}

TEST_F(KbNodeTreeQModelTestBase,
       test_add_kbnode_to_expanded_root_node) { // NOLINT
  CUSTOM_ASSERT(test_add_kbnode_to_expanded_root_node());
}

TEST_F(KbNodeTreeQModelTestBase,
       should_null_kbnode_to_index_got_invalid_root_idx) { // NOLINT
  QModelIndex kbnode_to_index = qmodel->kbNodeToIndex(nullptr);
  auto expect_index = QModelIndex();

  ASSERT_EQ(expect_index, kbnode_to_index)
      << "null node_ptr should got root invalid index in completion mode";
}

class KbNodeTreeQModelWithClearAndAddMoreRowTest : public KbNodeTreeQModelTestBase {
 protected:
  void SetUp() override {
    KbNodeTreeQModelTestBase::SetUp();

    show_clear_row = true;
    show_add_more_row = true;
  }

  std::unique_ptr<KbNodeTreeQModelBasic> createQModel() override {
    return utils::make_unique<KbNodeTreeQModelWithClearAndAddMoreRow>();
  }
};

TEST_F(KbNodeTreeQModelWithClearAndAddMoreRowTest,
       check_row_data) { // NOLINT
  CUSTOM_ASSERT(checkRowData());
}

TEST_F(KbNodeTreeQModelWithClearAndAddMoreRowTest,
       should_beginResetQModel_emit_modelAboutToReset) { // NOLINT
  CUSTOM_ASSERT(should_beginResetQModel_emit_modelAboutToReset());
}

TEST_F(KbNodeTreeQModelWithClearAndAddMoreRowTest,
       should_endResetQModel_emit_modelReset) { // NOLINT
  CUSTOM_ASSERT(should_endResetQModel_emit_modelReset());
}

TEST_F(KbNodeTreeQModelWithClearAndAddMoreRowTest,
       test_add_kbnode_to_not_visible_parent_kbnode) { // NOLINT
  CUSTOM_ASSERT(test_add_kbnode_to_not_visible_parent_kbnode());
}

TEST_F(KbNodeTreeQModelWithClearAndAddMoreRowTest,
       test_add_kbnode_to_visible_but_children_not_populated_kbnode) { // NOLINT
  CUSTOM_ASSERT(test_add_kbnode_to_visible_but_children_not_populated_kbnode());
}

TEST_F(KbNodeTreeQModelWithClearAndAddMoreRowTest,
       test_add_kbnode_to_children_populated_parent_kbnode) { // NOLINT
  CUSTOM_ASSERT(test_add_kbnode_to_children_populated_parent_kbnode());
}

TEST_F(KbNodeTreeQModelWithClearAndAddMoreRowTest,
       test_add_kbnode_to_unexpanded_root_node) { // NOLINT
  CUSTOM_ASSERT(test_add_kbnode_to_unexpanded_root_node());
}

TEST_F(KbNodeTreeQModelWithClearAndAddMoreRowTest,
       test_add_kbnode_to_expanded_root_node) { // NOLINT
  CUSTOM_ASSERT(test_add_kbnode_to_expanded_root_node());
}

TEST_F(KbNodeTreeQModelWithClearAndAddMoreRowTest,
       should_null_kbnode_to_index_got_invalid_root_idx) { // NOLINT
  QModelIndex kbnode_to_index = qmodel->kbNodeToIndex(nullptr);
  auto expect_index = QModelIndex();

  ASSERT_EQ(expect_index, kbnode_to_index)
      << "null node_ptr should got root invalid index in completion mode";
}

class KbNodeTreeQModelWithProviderNodeTest : public KbNodeTreeQModelTestBase {
 protected:
  void SetUp() override {
    KbNodeTreeQModelTestBase::SetUp();

    always_selectable = true;

    show_clear_row = false;
    show_add_more_row = false;

    root_row_data.subnodes = &provider_row_data;
    root_row_data_after_reset.subnodes = &provider_row_data_after_reset;

    ASSERT_EQ(&level0_row_data, provider_row_data[0].subnodes);
  }

  void TearDown() override {
    root_row_data.subnodes = &level0_row_data;
    root_row_data_after_reset.subnodes = &level0_row_data_after_reset;

    KbNodeTreeQModelTestBase::TearDown();
  }

  std::unique_ptr<KbNodeTreeQModelBasic> createQModel() override {
    return utils::make_unique<KbNodeTreeQModelWithProviderNode>();
  }

  int adjustLevel(int level) override {
    return level + 1;
  }
};

TEST_F(KbNodeTreeQModelWithProviderNodeTest,
       should_display_the_provider_name_as_a_visible_root_node) { // NOLINT
  CUSTOM_ASSERT(checkRowData());
}

TEST_F(KbNodeTreeQModelWithProviderNodeTest,
       should_beginResetQModel_emit_modelAboutToReset) { // NOLINT
  CUSTOM_ASSERT(should_beginResetQModel_emit_modelAboutToReset());
}

TEST_F(KbNodeTreeQModelWithProviderNodeTest,
       should_endResetQModel_emit_modelReset) { // NOLINT
  CUSTOM_ASSERT(should_endResetQModel_emit_modelReset());
}

TEST_F(KbNodeTreeQModelWithProviderNodeTest,
       test_add_kbnode_to_not_visible_parent_kbnode) { // NOLINT
  CUSTOM_ASSERT(test_add_kbnode_to_not_visible_parent_kbnode());
}

TEST_F(KbNodeTreeQModelWithProviderNodeTest,
       test_add_kbnode_to_visible_but_children_not_populated_kbnode) { // NOLINT
  CUSTOM_ASSERT(test_add_kbnode_to_visible_but_children_not_populated_kbnode());
}

TEST_F(KbNodeTreeQModelWithProviderNodeTest,
       test_add_kbnode_to_children_populated_parent_kbnode) { // NOLINT
  CUSTOM_ASSERT(test_add_kbnode_to_children_populated_parent_kbnode());
}

TEST_F(KbNodeTreeQModelWithProviderNodeTest,
       test_add_kbnode_to_unexpanded_root_node) { // NOLINT
  CUSTOM_ASSERT(test_add_kbnode_to_unexpanded_root_node());
}

TEST_F(KbNodeTreeQModelWithProviderNodeTest,
       test_add_kbnode_to_expanded_root_node) { // NOLINT
  CUSTOM_ASSERT(test_add_kbnode_to_expanded_root_node());
}

TEST_F(KbNodeTreeQModelWithProviderNodeTest,
       should_null_kbnode_to_index_got_provider_node) { // NOLINT
  // Setup fixture
  auto expect_index = qmodel->index(0, 0, QModelIndex());

  // Verify results
  QModelIndex kbnode_to_index = qmodel->kbNodeToIndex(nullptr);
  ASSERT_EQ(expect_index, kbnode_to_index)
      << "null node_ptr should got provider node index in add kbnode mode";
}
