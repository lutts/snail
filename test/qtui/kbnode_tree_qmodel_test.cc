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

#include "src/qtui/kbnode_tree_qmodel.h"
#include "snail/mock_kbnode_provider.h"
#include "snail/mock_kbnode.h"

using namespace snailcore;  // NOLINT
using namespace snailcore::tests;  // NOLINT

// test data structure
// -------------------
// (root)
//    |---(empty line if not in filter mode)
//    |---level1 node1   (0, 0)
//    |   |----level2 node1    (0, 0)
//    |   |----level2 node2    (1, 0)
//    |   |    |---level3 node1    (0, 0)
//    |   |    |---(level3 node2 Later Added)
//    |   |----level2 node3    (2, 0)
//    |---level1 node2    (1, 0)
//    |---Add More... (special node)

class ExpectRowData {
 public:
  utils::U8String text;
  IKbNode* node_ptr;
  IKbNode* parent_node_ptr;
  bool isAddMore;
  bool isSelectable;

  std::vector<ExpectRowData>* subnodes;
};

std::vector<ExpectRowData> level3_row_data = {
  {
    .text = "Level3 Node1",
    .node_ptr = nullptr,
    .parent_node_ptr = nullptr,
    .isAddMore = false,
    .isSelectable = true,
    .subnodes = nullptr,
  }
};

std::vector<ExpectRowData> level2_row_data = {
  {
    .text = "Level2 Node1",
    .node_ptr = nullptr,
    .parent_node_ptr = nullptr,
    .isAddMore = false,
    .isSelectable = true,
    .subnodes = nullptr,
  },
  {
    .text = "Level2 Node2",
    .node_ptr = nullptr,
    .parent_node_ptr = nullptr,
    .isAddMore = false,
    .isSelectable = false,
    .subnodes = &level3_row_data,
  },
  {
    .text = "Level2 Node3",
    .node_ptr = nullptr,
    .parent_node_ptr = nullptr,
    .isAddMore = false,
    .isSelectable = true,
    .subnodes = nullptr,
  }
};

std::vector<ExpectRowData> level1_row_data = {
  {  // level1
    .text = "",
    .node_ptr = nullptr,
    .parent_node_ptr = nullptr,
    .isAddMore = false,
    .isSelectable = true,
    .subnodes = nullptr
  },
  {
    .text = "Level1 Node1",
    .node_ptr = nullptr,
    .parent_node_ptr = nullptr,
    .isAddMore = false,

    .isSelectable = false,
    .subnodes = &level2_row_data,
  },
  {
    .text = "Level1 Node2",
    .node_ptr = nullptr,
    .parent_node_ptr = nullptr,
    .isAddMore = false,
    .isSelectable = true,
    .subnodes = nullptr,
  },
  {
    .text = "Add More...",
    .node_ptr = nullptr,
    .parent_node_ptr = nullptr,
    .isAddMore = true,
    .isSelectable = true,
    .subnodes = nullptr,
  }
};

ExpectRowData root_row_data = {
  .text = "root idx",
  .node_ptr = nullptr,
  .parent_node_ptr = nullptr,
  .isAddMore = false,
  .isSelectable = false,
  .subnodes = &level1_row_data,
};

class KbNodeProviderTestStub : public IKbNodeProvider {
 public:
  class ChildNodeIteratorTestStub : public IChildNodeIterator {
   public:
    ChildNodeIteratorTestStub(const KbNodeProviderTestStub& provider,
                              IKbNode* parent_node)
        : row_data_(provider.nodeToRowData(parent_node)) {
      if (row_data_->subnodes) {
        max_idx_ = row_data_->subnodes->size() - 1;
      } else {
        max_idx_ = -1;
      }
      if (parent_node == nullptr) {  // root
        cur_idx_ = 1;  // skip empty row
        --max_idx_;  // skip add more row
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
    int cur_idx_ { 0 };
    int max_idx_ { 0 };
  };

  KbNodeProviderTestStub() {
    fillNodePtrInRowData(root_row_data.subnodes);
  }
  virtual ~KbNodeProviderTestStub() = default;

  MOCK_CONST_METHOD0(isFilterMode, bool());

  IChildNodeIterator* childNodes(IKbNode* parent_node) const override {
    return new ChildNodeIteratorTestStub(*this, parent_node);
  }

  void addKbNodeRowData(IKbNode* node, std::vector<ExpectRowData>* row_data_vec) {
    node_to_row_data_vec_[node] = row_data_vec;
  }

  // unintrested method, using mocks
  SNAIL_MOCK_SLOT(BeginFilter);
  SNAIL_MOCK_SLOT(FinishFilter);

  MOCK_METHOD1(setFilterPattern, void(const utils::U8String& filter_pattern));
  MOCK_METHOD0(addKbNode, KbNodeAddResult());

  MOCK_METHOD1(incRef, void(IKbNode* kbnode));
  MOCK_METHOD1(findKbNodeByName,
               std::vector<IKbNode*>(const utils::U8String& name));

 private:
  SNAIL_DISABLE_COPY(KbNodeProviderTestStub);

  void fillNodePtrInRowData(std::vector<ExpectRowData>* row_datas,
                            IKbNode* parent_node = nullptr) {
    for (auto & row_data : *row_datas) {
      if (row_data.text == "")
        continue;

      if (row_data.isAddMore)
        continue;

      kbnodes_.push_back(utils::make_unique<MockKbNode>());
      auto & last_add = kbnodes_.back();
      MockKbNode* kbnode = last_add.get();

      EXPECT_CALL(*kbnode, name())
          .WillRepeatedly(Return(row_data.text));
      EXPECT_CALL(*kbnode, isCategory())
          .WillRepeatedly(Return(!row_data.isSelectable));

      row_data.node_ptr = kbnode;
      node_to_row_data_vec_[kbnode] = row_datas;

      row_data.parent_node_ptr = parent_node;

      if (row_data.subnodes)
        fillNodePtrInRowData(row_data.subnodes, kbnode);
    }
  }

  ExpectRowData* nodeToRowData(IKbNode* node) const {
    if (!node)
      return &root_row_data;

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
};

class KbNodeTreeQModelTest : public ::testing::Test {
 protected:
  KbNodeTreeQModelTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~KbNodeTreeQModelTest() { }
  virtual void SetUp() {
    qmodel.setKbNodeProvider(&kbnode_provider);
  }
  // virtual void TearDown() { }

  void checkRowData(bool filter_mode);
  void checkRowData(const std::vector<ExpectRowData>& row_data,
                    const QModelIndex& parent_index,
                    size_t start_data_idx);
  void checkIndexData(const QModelIndex& index,
                      const ExpectRowData& rdata);

  // region: objects test subject depends on
  KbNodeProviderTestStub kbnode_provider;
  QModelIndexGenerator index_generator;
  // endregion

  // region: test subject
  KbNodeTreeQModel qmodel;
  // endregion

  // region: object depends on test subject
  // endregion
};

void KbNodeTreeQModelTest::checkRowData(bool filter_mode) {
  size_t level1_start_idx = 0;
  if (filter_mode)
    level1_start_idx = 1;

  // check root
  int expect_row_count = root_row_data.subnodes->size();
  if (filter_mode)
    --expect_row_count;
  ASSERT_EQ(expect_row_count, qmodel.rowCount(QModelIndex()));
  ASSERT_EQ(1, qmodel.columnCount(QModelIndex()));
  QModelIndex expect_parent_idx;
  ASSERT_EQ(expect_parent_idx, qmodel.parent(QModelIndex()));

  // check sub items
  checkRowData(*root_row_data.subnodes, QModelIndex(), level1_start_idx);
}

void KbNodeTreeQModelTest::checkRowData(
    const std::vector<ExpectRowData>& row_data,
    const QModelIndex& parent_index,
    size_t start_data_idx) {
  for (size_t idx = start_data_idx; idx < row_data.size(); ++idx) {
    auto& rdata = row_data[idx];

    int row = idx - start_data_idx;
    QModelIndex index = qmodel.index(row, 0, parent_index);
    checkIndexData(index, rdata);

    if (rdata.subnodes) {
      checkRowData(*rdata.subnodes, index, 0);
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

void KbNodeTreeQModelTest::checkIndexData(const QModelIndex& index,
                                          const ExpectRowData& expect_data) {
  auto q_display_text = index.data(Qt::DisplayRole).toString();
  auto actual_display_text = QStringToU8String(q_display_text);
  ASSERT_EQ(expect_data.text, actual_display_text)
      << "index: " << index << " text not match"
      << " (text: " << expect_data.text << ")";

  int actual_row_count = qmodel.rowCount(index);
  int expect_row_count =
      (expect_data.subnodes == nullptr) ? 0 : expect_data.subnodes->size();
  ASSERT_EQ(expect_row_count, actual_row_count)
      << "index: " << index << " row count not match"
      << " (text: " << expect_data.text << ")";

  int actual_col_count = qmodel.columnCount(index);
  ASSERT_EQ(1, actual_col_count)
      << "index: " << index << " col count not match"
      << " (text: " << expect_data.text << ")";

  auto actual_kbnode = qmodel.kbNodeOfIndex(index);
  ASSERT_EQ(expect_data.node_ptr, actual_kbnode)
      << "index: " << index << " kbnode ptr not match"
      << " (text: " << expect_data.text << ")";

  auto actual_isAddMore = qmodel.isAddKbNode(index);
  ASSERT_EQ(expect_data.isAddMore, actual_isAddMore)
      << "index: " << index << " isAddMore not match"
      << " (text: " << expect_data.text << ")";

  auto actual_selectable = qmodel.flags(index) & Qt::ItemIsSelectable;
  ASSERT_EQ(expect_data.isSelectable, actual_selectable)
      << "index: " << index << " isSelectable not match"
      << " (text: " << expect_data.text << ")";

  QModelIndex parent = qmodel.parent(index);
  auto actual_parent_ptr = qmodel.kbNodeOfIndex(parent);
  ASSERT_EQ(expect_data.parent_node_ptr, actual_parent_ptr)
      << "index: " << index << " parent not match"
      << " (text: " << expect_data.text << ")";
}

TEST_F(KbNodeTreeQModelTest,
       check_non_filter_mode_row_data) { // NOLINT
  // Setup fixture
  bool filter_mode = false;

  EXPECT_CALL(kbnode_provider, isFilterMode())
      .WillRepeatedly(Return(filter_mode));

  // Verify results
  CUSTOM_ASSERT(checkRowData(filter_mode));
}

TEST_F(KbNodeTreeQModelTest,
       check_filter_mode_row_data) { // NOLINT
  // Setup fixture
  bool filter_mode = true;

  EXPECT_CALL(kbnode_provider, isFilterMode())
      .WillRepeatedly(Return(filter_mode));

  // Verify results
  CUSTOM_ASSERT(checkRowData(filter_mode));
}

TEST_F(KbNodeTreeQModelTest,
       should_beginResetQModel_emit_modelAboutToReset) { // NOLINT
  // Setup fixture
  QSignalSpy sigspy(&qmodel, SIGNAL(modelAboutToBeReset()));

  // Exercise system
  qmodel.beginResetQModel();

  // Verify results
  ASSERT_EQ(1, sigspy.count());
}

TEST_F(KbNodeTreeQModelTest,
       should_endResetQModel_emit_modelReset) { // NOLINT
  // Setup fixture
  QSignalSpy sigspy(&qmodel, SIGNAL(modelReset()));

  // Exercise system
  qmodel.endResetQModel();

  // Verify results
  ASSERT_EQ(1, sigspy.count());
}

TEST_F(KbNodeTreeQModelTest,
       test_dynamically_add_kbnode) { // NOLINT
  // Setup fixture
  bool filter_mode = true;

  EXPECT_CALL(kbnode_provider, isFilterMode())
      .WillRepeatedly(Return(filter_mode));

  CUSTOM_ASSERT(checkRowData(filter_mode));

  auto new_kbnode_name = xtestutils::genRandomString();
  MockKbNode new_kbnode;

  EXPECT_CALL(new_kbnode, name())
      .WillRepeatedly(Return(new_kbnode_name));
  EXPECT_CALL(new_kbnode, isCategory())
      .WillRepeatedly(Return(false));

  ASSERT_EQ(&level3_row_data, level2_row_data[1].subnodes);
  IKbNode* parent_kbnode = level2_row_data[1].node_ptr;

  ExpectRowData new_row_data = {
    .text = new_kbnode_name,
    .node_ptr = &new_kbnode,
    .parent_node_ptr = parent_kbnode,
    .isAddMore = false,
    .isSelectable = true,
    .subnodes = nullptr,
  };

  level3_row_data.push_back(new_row_data);

  kbnode_provider.addKbNodeRowData(&new_kbnode, &level3_row_data);

  // Exercise system
  qmodel.kbNodeAdded(&new_kbnode, parent_kbnode);

  // Verify results
  CUSTOM_ASSERT(checkRowData(filter_mode));
}

// TODO: parent() is not tested
