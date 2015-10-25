//-*- TestCaseName: KbNodeManagerTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"

#include "src/core/kbnode_manager.h"
#include "snail/i_kbnode.h"
#include "snail/mock_tree_item_provider.h"

std::ostream& operator<< (std::ostream& os,
                          const std::vector<snailcore::IKbNode*>& kbnodes) {
  for (auto kbnode : kbnodes) {
    os << "(" << kbnode->id() << ", \"" << kbnode->name() << "\"), ";
  }

  return os;
}

namespace snailcore {
namespace tests {

class KbNodeManagerTest : public ::testing::Test {
 protected:
  KbNodeManagerTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~KbNodeManagerTest() { }
  virtual void SetUp() {
    kbnode_manager_ =
        utils::make_unique<KbNodeManager>();
    setupTestKbNodes();
  }
  // virtual void TearDown() { }

  void setupTestKbNodes();

  // region: objects test subject depends on
  IKbNode* top_level_node_ { nullptr };
  std::vector<IKbNode*> all_nodes;
  std::map<IKbNode*, std::vector<IKbNode*> > kbnode_to_subnodes;
  std::vector<IKbNode*> expect_search_result;
  // endregion

  // region: test subject
  std::unique_ptr<fto::KbNodeManager> kbnode_manager_;
  // endregion

  // region: object depends on test subject
  // endregion
};

#define SEARCH_STR "lutts"

bool kbnode_compare(const IKbNode* a, const IKbNode* b) {
  return a->id() < b->id();
}

void KbNodeManagerTest::setupTestKbNodes() {
  // NOTE: words are from The standard Lorem Ipsum passage, used since the 1500s

  // Lorem ipsum dolor sit amet, consectetur adipiscing elit,
  // sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.
  // Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris
  // nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in
  // reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla
  // pariatur. Excepteur sint occaecat cupidatat non proident, sunt in
  // culpa qui officia deserunt mollit anim id est laborum.

  // has search string, but if search use tree as parent, it is not included
  IKbNode* tree = kbnode_manager_->addKbNode("lutts tree", nullptr);
  top_level_node_ = tree;
  all_nodes.push_back(tree);

  {
    // parent is category, child has search str
    const bool is_category = true;
    IKbNode* tree_category1 =  // has search str, but is category
        kbnode_manager_->addKbNode("Lorem lutts ipsum", tree, is_category);
    all_nodes.push_back(tree_category1);
    kbnode_to_subnodes[tree].push_back(tree_category1);

    {  // subnodes
      IKbNode* tree_subnode1_subnode1 =
          kbnode_manager_->addKbNode("empor lutts incididunt", tree_category1);
      all_nodes.push_back(tree_subnode1_subnode1);
      kbnode_to_subnodes[tree_category1].push_back(tree_subnode1_subnode1);
      expect_search_result.push_back(tree_subnode1_subnode1);
    }
  }

  {
    // parent has search str, child also has
    IKbNode* tree_subnode2 =  // no search str
        kbnode_manager_->addKbNode("dolor sit amet", tree);
    all_nodes.push_back(tree_subnode2);
    kbnode_to_subnodes[tree].push_back(tree_subnode2);

    {  // subnodes
      IKbNode* tree_subnode2_subnode1 =
          kbnode_manager_->addKbNode("adipiscing elit", tree_subnode2);
      all_nodes.push_back(tree_subnode2_subnode1);
      kbnode_to_subnodes[tree_subnode2].push_back(tree_subnode2_subnode1);

      IKbNode* tree_subnode2_subnode2 =
          kbnode_manager_->addKbNode("sed do eiusmod lutts", tree_subnode2);
      all_nodes.push_back(tree_subnode2_subnode2);
      kbnode_to_subnodes[tree_subnode2].push_back(tree_subnode2_subnode2);
      expect_search_result.push_back(tree_subnode2_subnode2);
    }
  }

  {
    // parent has no search str, child also no search str
    IKbNode* tree_subnode3 =  // no search str
        kbnode_manager_->addKbNode("consectetur", tree);
    all_nodes.push_back(tree_subnode3);
    kbnode_to_subnodes[tree].push_back(tree_subnode3);

    {  // subnodes
      IKbNode* tree_subnode3_subnode1 =
          kbnode_manager_->addKbNode("quis nostrud", tree_subnode3);
      all_nodes.push_back(tree_subnode3_subnode1);
      kbnode_to_subnodes[tree_subnode3].push_back(tree_subnode3_subnode1);

      {  // grand son has search str
        IKbNode* tree_subnode3_subnode1_subnode1 =
            kbnode_manager_->addKbNode("est laborum lutts",
                                      tree_subnode3_subnode1);
        all_nodes.push_back(tree_subnode3_subnode1_subnode1);
        kbnode_to_subnodes[tree_subnode3_subnode1].
            push_back(tree_subnode3_subnode1_subnode1);
        expect_search_result.push_back(tree_subnode3_subnode1_subnode1);
      }
    }
  }

  {
    // parent has search str, child also has
    IKbNode* tree_subnode4 =  // has search str, and not category
        kbnode_manager_->addKbNode("voluptate velit lutts", tree);
    all_nodes.push_back(tree_subnode4);
    kbnode_to_subnodes[tree].push_back(tree_subnode4);
    expect_search_result.push_back(tree_subnode4);

    {  // sublides
      IKbNode* tree_subnode4_subnode1 =
          kbnode_manager_->addKbNode("occaecat lutts cupidatat", tree_subnode4);
      all_nodes.push_back(tree_subnode4_subnode1);
      kbnode_to_subnodes[tree_subnode4].push_back(tree_subnode4_subnode1);
      expect_search_result.push_back(tree_subnode4_subnode1);
    }
  }

  std::sort(expect_search_result.begin(), expect_search_result.end(),
            kbnode_compare);
}

TEST_F(KbNodeManagerTest,
       should_be_able_to_get_KbNode_instance_by_id) { // NOLINT
  for (auto expect_kbnode : all_nodes) {
    auto actual_kbnode = kbnode_manager_->idToKbNode(expect_kbnode->id());
    ASSERT_EQ(expect_kbnode, actual_kbnode);
  }
}

// TODO(lutts): pattern may treat as regular expression, whitespace means '*'
// and append '*' to the begin and last.
// We currently use simply strstr(), we do not impl this behavior because the
// pattern can be more complex, such as AND or OR, and we do not want these
// complex at this very begining time.
TEST_F(KbNodeManagerTest,
       should_be_able_to_find_by_pattern) { // NOLINT
  auto actual_search_result = kbnode_manager_->findKbNode(SEARCH_STR,
                                                         top_level_node_);
  std::sort(actual_search_result.begin(), actual_search_result.end(),
            kbnode_compare);
  ASSERT_EQ(expect_search_result, actual_search_result)
      << "expect: {" << expect_search_result << "}, "
      << "actual: {" << actual_search_result << "}";

  expect_search_result.push_back(top_level_node_);
  std::sort(expect_search_result.begin(), expect_search_result.end(),
            kbnode_compare);
  auto actual_search_result_top =
      kbnode_manager_->findKbNode(SEARCH_STR, nullptr);
  std::sort(actual_search_result_top.begin(), actual_search_result_top.end(),
            kbnode_compare);
  ASSERT_EQ(expect_search_result, actual_search_result_top)
      << "expect: {" << expect_search_result << "}, "
      << "actual: {" << actual_search_result_top << "}";
}

TEST_F(KbNodeManagerTest,
       should_be_able_to_get_child_nodes_of_specified_parent) { // NOLINT
  std::vector<IKbNode*> expect_top_level_nodes;
  expect_top_level_nodes.push_back(top_level_node_);

  auto actual_top_level_nodes = kbnode_manager_->childItems(nullptr);
  ASSERT_EQ(expect_top_level_nodes, actual_top_level_nodes);

  for (auto kbnode : all_nodes) {
    auto actual_subnodes = kbnode_manager_->childItems(kbnode);
    auto & expect_subnodes = kbnode_to_subnodes[kbnode];
    ASSERT_EQ(expect_subnodes, actual_subnodes)
        << "expect: {" << expect_subnodes << "}, "
        << "actual: {" << actual_subnodes << "}";
  }
}

TEST_F(KbNodeManagerTest,
       should_notify_item_provider_after_add_kbnode) { // NOLINT
  // Setup fixture
  auto item_provider = kbnode_manager_->createTreeItemProvider(top_level_node_);

  const ITreeItem* new_item1 = nullptr;
  const ITreeItem* new_item1_parent = nullptr;

  const ITreeItem* new_item2 = nullptr;
  const ITreeItem* new_item2_parent = nullptr;

  // Expectations
  auto item_provider_listener =
      TreeItemProviderListener::attachTo(item_provider.get());
  EXPECT_CALL(*item_provider_listener, ItemAdded(_, _))
      .WillOnce(DoAll(SaveArg<0>(&new_item1), SaveArg<1>(&new_item1_parent)))
      .WillOnce(DoAll(SaveArg<0>(&new_item2), SaveArg<1>(&new_item2_parent)));

  // Exercise system
  IKbNode* expect_new_item1 =
      kbnode_manager_->addKbNode(xtestutils::genRandomString(),
                                 top_level_node_, false);
  ITreeItem* expect_new_item1_parent = nullptr;

  IKbNode* expect_new_item2 =
      kbnode_manager_->addKbNode(xtestutils::genRandomString(),
                                 expect_new_item1, false);
  ITreeItem* expect_new_item2_parent = expect_new_item1;

  // Verify results
  ASSERT_EQ(expect_new_item1, new_item1);
  ASSERT_EQ(expect_new_item1_parent, new_item1_parent);
  ASSERT_EQ(expect_new_item2, new_item2);
  ASSERT_EQ(expect_new_item2_parent, new_item2_parent);
}

}  // namespace tests
}  // namespace snailcore
