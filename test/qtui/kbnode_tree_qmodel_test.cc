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
  bool isVisible;

  std::vector<ExpectRowData>* subnodes;
};

std::vector<ExpectRowData> level3_row_data = {
  {
    .text = "Level3 Node1",
    .node_ptr = nullptr,
    .parent_node_ptr = nullptr,
    .isAddMore = false,
    .isSelectable = true,
    .isVisible = true,
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
    .isVisible = true,
    .subnodes = nullptr,
  },
  {
    .text = "Level2 Node2",
    .node_ptr = nullptr,
    .parent_node_ptr = nullptr,
    .isAddMore = false,
    .isSelectable = false,
    .isVisible = true,
    .subnodes = &level3_row_data,
  },
  {
    .text = "Level2 Node3",
    .node_ptr = nullptr,
    .parent_node_ptr = nullptr,
    .isAddMore = false,
    .isSelectable = true,
    .isVisible = true,
    .subnodes = nullptr,
  }
};

constexpr static int kEmptySpecialRow = 0;
constexpr static int kAddMoreSpecialRow = 3;
std::vector<ExpectRowData> level1_row_data = {
  {  // level1
    .text = "",
    .node_ptr = nullptr,
    .parent_node_ptr = nullptr,
    .isAddMore = false,
    .isSelectable = true,
    .isVisible = true,
    .subnodes = nullptr
  },
  {
    .text = "Level1 Node1",
    .node_ptr = nullptr,
    .parent_node_ptr = nullptr,
    .isAddMore = false,
    .isSelectable = false,
    .isVisible = true,
    .subnodes = &level2_row_data,
  },
  {
    .text = "Level1 Node2",
    .node_ptr = nullptr,
    .parent_node_ptr = nullptr,
    .isAddMore = false,
    .isSelectable = true,
    .isVisible = true,
    .subnodes = nullptr,
  },
  {
    .text = "Add More...",
    .node_ptr = nullptr,
    .parent_node_ptr = nullptr,
    .isAddMore = true,
    .isSelectable = true,
    .isVisible = true,
    .subnodes = nullptr,
  }
};

#define PROVIDER_NAME "Provider Name"
std::vector<ExpectRowData> provider_row_data = {
  {
    .text = PROVIDER_NAME,
    .node_ptr = nullptr,
    .parent_node_ptr = nullptr,
    .isAddMore = false,
    .isSelectable = true,
    .isVisible = true,
    .subnodes = &level1_row_data,
  }
};

ExpectRowData root_row_data = {
  .text = "root idx",
  .node_ptr = nullptr,
  .parent_node_ptr = nullptr,
  .isAddMore = false,
  .isSelectable = false,
  .isVisible = true,
  .subnodes = &level1_row_data,
};

std::vector<ExpectRowData> level1_row_data_after_reset = {
  {  // level1
    .text = "",
    .node_ptr = nullptr,
    .parent_node_ptr = nullptr,
    .isAddMore = false,
    .isSelectable = true,
    .isVisible = true,
    .subnodes = nullptr
  },
  {
    .text = "Level1 Node1 After Reset",
    .node_ptr = nullptr,
    .parent_node_ptr = nullptr,
    .isAddMore = false,
    .isSelectable = false,
    .isVisible = true,
    .subnodes = nullptr,
  },
  {
    .text = "Level1 Node2 After Reset",
    .node_ptr = nullptr,
    .parent_node_ptr = nullptr,
    .isAddMore = false,
    .isSelectable = true,
    .isVisible = true,
    .subnodes = nullptr,
  },
  {
    .text = "Add More...",
    .node_ptr = nullptr,
    .parent_node_ptr = nullptr,
    .isAddMore = true,
    .isSelectable = true,
    .isVisible = true,
    .subnodes = nullptr,
  }
};

std::vector<ExpectRowData> provider_row_data_after_reset = {
  {
    .text = PROVIDER_NAME,
    .node_ptr = nullptr,
    .parent_node_ptr = nullptr,
    .isAddMore = false,
    .isSelectable = true,
    .isVisible = true,
    .subnodes = &level1_row_data_after_reset,
  }
};

ExpectRowData root_row_data_after_reset = {
  .text = "root idx",
  .node_ptr = nullptr,
  .parent_node_ptr = nullptr,
  .isAddMore = false,
  .isSelectable = false,
  .isVisible = true,
  .subnodes = &level1_row_data_after_reset,
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
      }

      if ((row_data_->subnodes == &level1_row_data) ||
          (row_data_->subnodes == &level1_row_data_after_reset)) {
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

      std::cout << "populate one..." << kbnode->name() << std::endl;

      return kbnode;
    }

   private:
    SNAIL_DISABLE_COPY(ChildNodeIteratorTestStub);

    ExpectRowData* row_data_;
    mutable int cur_idx_ { 0 };
    int max_idx_ { -1 };
  };

  KbNodeProviderTestStub() {
    fillRowData(&root_row_data);
  }
  virtual ~KbNodeProviderTestStub() = default;

  utils::U8String name() const {
    return PROVIDER_NAME;
  }

  std::unique_ptr<IChildNodeIterator>
  childNodes(IKbNode* parent_node) const override {
    return utils::make_unique<ChildNodeIteratorTestStub>(*this, parent_node);
  }

  void addKbNodeRowData(IKbNode* node,
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
  MOCK_METHOD2(addKbNode,
               IKbNode*(const utils::U8String& name, IKbNode* parent));

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
  virtual void SetUp() {
    qmodel = createQModel();
    qmodel->setKbNodeProvider(&kbnode_provider);
  }
  // virtual void TearDown() { }

  virtual std::unique_ptr<KbNodeTreeQModelBase> createQModel() = 0;

  void expectEmptyRowVisible(bool visible);
  void expectAddMoreRowVisible(bool visible);

  int visibleNodeCount(const ExpectRowData& row_data);

  void checkRowData(bool after_reset = false);
  void checkRowData(const std::vector<ExpectRowData>& row_data,
                    const QModelIndex& parent_index);
  void checkIndexData(const QModelIndex& index,
                      const ExpectRowData& rdata);

  void should_beginResetQModel_emit_modelAboutToReset();
  void should_endResetQModel_emit_modelReset();
  void test_dynamically_add_kbnode();
  void test_dynamically_add_kbnode_in_level1();

  // region: objects test subject depends on
  KbNodeProviderTestStub kbnode_provider;
  QModelIndexGenerator index_generator;
  // endregion

  // region: test subject
  std::unique_ptr<KbNodeTreeQModelBase> qmodel;
  // endregion

  // region: object depends on test subject
  // endregion
};

void KbNodeTreeQModelTestBase::expectEmptyRowVisible(bool visible) {
  level1_row_data[kEmptySpecialRow].isVisible = visible;
  level1_row_data_after_reset[kEmptySpecialRow].isVisible = visible;
}

void KbNodeTreeQModelTestBase::expectAddMoreRowVisible(bool visible) {
  level1_row_data[kAddMoreSpecialRow].isVisible = visible;
  level1_row_data_after_reset[kAddMoreSpecialRow].isVisible = visible;
}

int KbNodeTreeQModelTestBase::visibleNodeCount(const ExpectRowData& row_data) {
  if (row_data.subnodes == nullptr)
    return 0;

  int count = 0;
  for (auto & sub_rdata : *row_data.subnodes) {
    if (sub_rdata.isVisible)
      ++count;
  }

  return count;
}

void KbNodeTreeQModelTestBase::checkRowData(bool after_reset) {
  // check root
  auto root_rdata = &root_row_data;
  if (after_reset)
    root_rdata = &root_row_data_after_reset;

  ASSERT_EQ(visibleNodeCount(*root_rdata), qmodel->rowCount(QModelIndex()));
  ASSERT_EQ(1, qmodel->columnCount(QModelIndex()));
  QModelIndex expect_parent_idx;
  ASSERT_EQ(expect_parent_idx, qmodel->parent(QModelIndex()));

  // check sub items
  checkRowData(*root_rdata->subnodes, QModelIndex());
}

void KbNodeTreeQModelTestBase::checkRowData(
    const std::vector<ExpectRowData>& row_data,
    const QModelIndex& parent_index) {
  int row = 0;
  for (size_t idx = 0; idx < row_data.size(); ++idx) {
    auto& rdata = row_data[idx];

    if (!rdata.isVisible)
      continue;

    QModelIndex index = qmodel->index(row, 0, parent_index);
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

void KbNodeTreeQModelTestBase::checkIndexData(const QModelIndex& index,
                                          const ExpectRowData& expect_data) {
  auto q_display_text = index.data(Qt::DisplayRole).toString();
  auto actual_display_text = QStringToU8String(q_display_text);
  ASSERT_EQ(expect_data.text, actual_display_text)
      << "index: " << index << " text not match"
      << " (text: " << expect_data.text << ")";

  int actual_row_count = qmodel->rowCount(index);
  int expect_row_count = visibleNodeCount(expect_data);
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

  auto actual_isAddMore = qmodel->isAddKbNode(index);
  ASSERT_EQ(expect_data.isAddMore, actual_isAddMore)
      << "index: " << index << " isAddMore not match"
      << " (text: " << expect_data.text << ")";

  auto actual_selectable = qmodel->flags(index) & Qt::ItemIsSelectable;
  ASSERT_EQ(expect_data.isSelectable, actual_selectable)
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

  std::cout << "check after reset" << std::endl;
  kbnode_provider.fillRowData(&root_row_data_after_reset);
  CUSTOM_ASSERT(checkRowData(true));
}

void KbNodeTreeQModelTestBase::test_dynamically_add_kbnode() {
  // check before add
  CUSTOM_ASSERT(checkRowData());

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
    .isVisible = true,
    .subnodes = nullptr,
  };

  level3_row_data.push_back(new_row_data);
  kbnode_provider.addKbNodeRowData(&new_kbnode, &level3_row_data);

  QSignalSpy begin_insert_sigspy(qmodel.get(),
                                 SIGNAL(rowsAboutToBeInserted(
                                     const QModelIndex&, int, int)));
  QSignalSpy end_insert_sigspy(qmodel.get(),
                               SIGNAL(rowsInserted(
                                   const QModelIndex&, int, int)));

  // Exercise system
  qmodel->kbNodeAdded(&new_kbnode, parent_kbnode);

  // Verify results
  CUSTOM_ASSERT(checkRowData());

  ASSERT_EQ(1, begin_insert_sigspy.count());
  ASSERT_EQ(1, end_insert_sigspy.count());

  QList<QVariant> arguments = begin_insert_sigspy.takeFirst();
  auto parent_idx = qvariant_cast<QModelIndex>(arguments.at(0));
  ASSERT_EQ(parent_kbnode, qmodel->indexToKbNode(parent_idx));

  int insert_row_first = qvariant_cast<int>(arguments.at(1));
  int insert_row_last = qvariant_cast<int>(arguments.at(2));
  ASSERT_EQ(1, insert_row_first);
  ASSERT_EQ(1, insert_row_last);

  // Teardown fixture
  level3_row_data.pop_back();
}

void KbNodeTreeQModelTestBase::test_dynamically_add_kbnode_in_level1() {
  // check before add
  CUSTOM_ASSERT(checkRowData());

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
    .isAddMore = false,
    .isSelectable = true,
    .isVisible = true,
    .subnodes = nullptr,
  };

  auto insert_pos = level1_row_data.end();
  --insert_pos;  // before add more row

  level1_row_data.insert(insert_pos, new_row_data);
  kbnode_provider.addKbNodeRowData(&new_kbnode, &level1_row_data);

  QSignalSpy begin_insert_sigspy(qmodel.get(),
                                 SIGNAL(rowsAboutToBeInserted(
                                     const QModelIndex&, int, int)));
  QSignalSpy end_insert_sigspy(qmodel.get(),
                               SIGNAL(rowsInserted(
                                   const QModelIndex&, int, int)));

  // Exercise system
  qmodel->kbNodeAdded(&new_kbnode, nullptr);

  // Verify results
  CUSTOM_ASSERT(checkRowData());

  ASSERT_EQ(1, begin_insert_sigspy.count());
  ASSERT_EQ(1, end_insert_sigspy.count());

  QList<QVariant> arguments = begin_insert_sigspy.takeFirst();
  auto parent_idx = qvariant_cast<QModelIndex>(arguments.at(0));
  ASSERT_EQ(nullptr, qmodel->indexToKbNode(parent_idx));

  int insert_row_first = qvariant_cast<int>(arguments.at(1));
  int insert_row_last = qvariant_cast<int>(arguments.at(2));
  ASSERT_EQ(2, insert_row_first);
  ASSERT_EQ(2, insert_row_last);

  // Teardown fixture
  auto new_kbnode_ptr = &new_kbnode;
  auto iter = std::find_if(level1_row_data.begin(),
                           level1_row_data.end(),
                           [new_kbnode_ptr](const ExpectRowData& item) {
                             if (item.node_ptr == new_kbnode_ptr)
                               return true;

                             return false;
                           });
  if (iter != level1_row_data.end()) {
    level1_row_data.erase(iter);
  }
}

class KbNodeTreeQModelTest : public KbNodeTreeQModelTestBase {
 protected:
  virtual void SetUp() {
    KbNodeTreeQModelTestBase::SetUp();

    // default special visibilities
    expectEmptyRowVisible(false);
    expectAddMoreRowVisible(true);
  }

  std::unique_ptr<KbNodeTreeQModelBase> createQModel() override {
    return utils::make_unique<KbNodeTreeQModel>();
  }
};

TEST_F(KbNodeTreeQModelTest,
       check_non_filter_mode_row_data) { // NOLINT
  // Setup fixture
  bool filter_mode = false;
  expectEmptyRowVisible(true);

  EXPECT_CALL(kbnode_provider, isFilterMode())
      .WillRepeatedly(Return(filter_mode));

  // Verify results
  CUSTOM_ASSERT(checkRowData());
}

TEST_F(KbNodeTreeQModelTest,
       check_filter_mode_row_data) { // NOLINT
  // Setup fixture
  bool filter_mode = true;

  EXPECT_CALL(kbnode_provider, isFilterMode())
      .WillRepeatedly(Return(filter_mode));

  // Verify results
  CUSTOM_ASSERT(checkRowData());
}

TEST_F(KbNodeTreeQModelTest,
       should_beginResetQModel_emit_modelAboutToReset) { // NOLINT
  CUSTOM_ASSERT(should_beginResetQModel_emit_modelAboutToReset());
}

TEST_F(KbNodeTreeQModelTest,
       should_endResetQModel_emit_modelReset) { // NOLINT
  bool filter_mode = true;

  EXPECT_CALL(kbnode_provider, isFilterMode())
      .WillRepeatedly(Return(filter_mode));

  CUSTOM_ASSERT(should_endResetQModel_emit_modelReset());
}

TEST_F(KbNodeTreeQModelTest,
       test_dynamically_add_kbnode) { // NOLINT
  // Setup fixture
  bool filter_mode = true;

  EXPECT_CALL(kbnode_provider, isFilterMode())
      .WillRepeatedly(Return(filter_mode));

  CUSTOM_ASSERT(test_dynamically_add_kbnode());
}

TEST_F(KbNodeTreeQModelTest,
       test_dynamically_add_kbnode_in_level1) { // NOLINT
  // Setup fixture
  bool filter_mode = true;

  EXPECT_CALL(kbnode_provider, isFilterMode())
      .WillRepeatedly(Return(filter_mode));

  CUSTOM_ASSERT(test_dynamically_add_kbnode_in_level1());
}

TEST_F(KbNodeTreeQModelTest,
       should_null_kbnode_to_index_got_invalid_root_idx_in_completion_mode) { // NOLINT
  QModelIndex kbnode_to_index = qmodel->kbNodeToIndex(nullptr);
  auto expect_index = QModelIndex();

  ASSERT_EQ(expect_index, kbnode_to_index)
      << "null node_ptr should got root invalid index in completion mode";
}

class KbNodeTreeQModelWithProviderNodeTest : public KbNodeTreeQModelTestBase {
 protected:
  void SetUp() override {
    KbNodeTreeQModelTestBase::SetUp();

    // default special visibilities
    expectEmptyRowVisible(false);
    expectAddMoreRowVisible(false);

    root_row_data.subnodes = &provider_row_data;
    root_row_data_after_reset.subnodes = &provider_row_data_after_reset;

    ASSERT_EQ(&level1_row_data, provider_row_data[0].subnodes);
  }

  void TearDown() override {
    root_row_data.subnodes = &level1_row_data;
    root_row_data_after_reset.subnodes = &level1_row_data_after_reset;

    KbNodeTreeQModelTestBase::TearDown();
  }

  std::unique_ptr<KbNodeTreeQModelBase> createQModel() override {
    return utils::make_unique<KbNodeTreeQModelWithProviderNode>();
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
       test_dynamically_add_kbnode) { // NOLINT
  CUSTOM_ASSERT(test_dynamically_add_kbnode());
}

TEST_F(KbNodeTreeQModelWithProviderNodeTest,
       test_dynamically_add_kbnode_in_level1) { // NOLINT
  CUSTOM_ASSERT(test_dynamically_add_kbnode_in_level1());
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
