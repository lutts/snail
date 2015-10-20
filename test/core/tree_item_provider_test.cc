//-*- TestCaseName: TreeItemProviderTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <vector>
#include <map>

#include "test/testutils/gmock_common.h"
#include "test/core/kbnode_id_generator.h"

#include "src/core/tree_item_provider.h"
#include "core/mock_kbnode_manager.h"
#include "snail/mock_kbnode.h"

namespace snailcore {
namespace tests {

template <typename TestBase>
class TreeItemProviderTestBase : public TestBase {
 protected:
  TreeItemProviderTestBase() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~TreeItemProviderTestBase() { }
  virtual void SetUp() {
    root_kbnode_name_ = xtestutils::genRandomString();
    EXPECT_CALL(root_kbnode_, name())
        .WillRepeatedly(Return(root_kbnode_name_));

    kbnode_provider_ = utils::make_unique<TreeItemProvider>(&root_kbnode_,
                                                          &node_manager_);
  }
  // virtual void TearDown() { }

  void checkSubNodes(
      IKbNode* parent_node,
      std::map<const IKbNode*, std::vector<IKbNode*> >*  kbnode_to_subnodes);
  void checkNonFilterModelKbNodeHierarchy();

  // region: objects test subject depends on
  MockKbNode root_kbnode_;
  utils::U8String root_kbnode_name_;

  MockKbNodeManager node_manager_;
  // endregion

  // region: test subject
  std::unique_ptr<TreeItemProvider> kbnode_provider_;
  // endregion

  // region: object depends on test subject
  // endregion
};

class TreeItemProviderTest : public TreeItemProviderTestBase<::testing::Test> { };

TEST_F(TreeItemProviderTest,
       should_provider_name_be_root_kbnode_name) { // NOLINT
  ASSERT_EQ(root_kbnode_name_, kbnode_provider_->name());
}

BEGIN_MOCK_LISTENER_DEF(MockListener, ITreeItemProvider)

MOCK_METHOD0(BeginFilter, void());
MOCK_METHOD0(FinishFilter, void());

BEGIN_BIND_SIGNAL(ITreeItemProvider)

BIND_SIGNAL0(BeginFilter, void);
BIND_SIGNAL0(FinishFilter, void);

END_BIND_SIGNAL()
END_MOCK_LISTENER_DEF()


TEST_F(TreeItemProviderTest,
       test_setFilterPattern_with_non_empty_string) { // NOLINT
  // Setup fixture
  auto filter_pattern = xtestutils::genRandomString();

  ASSERT_FALSE(filter_pattern.empty());
  // initially not in filter model
  ASSERT_FALSE(kbnode_provider_->isFilterMode());

  std::vector<IKbNode*> expect_kbnodes;
  expect_kbnodes.push_back(xtestutils::genDummyPointer<IKbNode>());
  expect_kbnodes.push_back(xtestutils::genDummyPointer<IKbNode>());
  expect_kbnodes.push_back(xtestutils::genDummyPointer<IKbNode>());

  // Expectations
  auto mock_listener = MockListener::attachTo(kbnode_provider_.get());
  {
    InSequence seq;

    EXPECT_CALL(*mock_listener, BeginFilter());
    EXPECT_CALL(node_manager_, findKbNode(filter_pattern, &root_kbnode_))
        .WillOnce(Return(expect_kbnodes));
    EXPECT_CALL(*mock_listener, FinishFilter());
  }

  // Exercise system
  kbnode_provider_->setFilterPattern(filter_pattern);
  // set a second time will not trigger any signals or requests to node manager
  kbnode_provider_->setFilterPattern(filter_pattern);

  // Verify results
  auto actual_filter_pattern = kbnode_provider_->getFilterPattern();
  ASSERT_EQ(filter_pattern, actual_filter_pattern);

  ASSERT_TRUE(kbnode_provider_->isFilterMode());

  // in filter mode, kbnodes should be a flatten list, not a tree
  std::vector<IKbNode*> actual_kbnodes;
  auto iterator = kbnode_provider_->childItems(nullptr);
  while (iterator->hasNext()) {
    auto kbnode = iterator->next();
    actual_kbnodes.push_back(kbnode);

    auto sub_iter = kbnode_provider_->childItems(kbnode);
    ASSERT_FALSE(sub_iter->hasNext());
  }

  ASSERT_EQ(expect_kbnodes, actual_kbnodes);
}

TEST_F(TreeItemProviderTest,
       should_setFilterPattern_to_empty_will_emit_signals_but_not_call_node_manager_findKbNode) { // NOLINT
  // Setup fixture
  kbnode_provider_->setFilterPattern(xtestutils::genRandomString());

  // Expectations
  auto mock_listener = MockListener::attachTo(kbnode_provider_.get());
  {
    InSequence seq;

    EXPECT_CALL(*mock_listener, BeginFilter());
    EXPECT_CALL(*mock_listener, FinishFilter());
  }

  EXPECT_CALL(node_manager_, findKbNode(_, _)).Times(0);

  // Exercise system
  kbnode_provider_->setFilterPattern("");

  // Verify results
  CUSTOM_ASSERT(checkNonFilterModelKbNodeHierarchy());
}

template <typename TestBase>
void TreeItemProviderTestBase<TestBase>::checkSubNodes(
    IKbNode* parent_node,
    std::map<const IKbNode*, std::vector<IKbNode*> >*  kbnode_to_subnodes) {
  std::vector<IKbNode*> actual_subnodes;
  auto iter = kbnode_provider_->childItems(parent_node);
  while (iter->hasNext()) {
    auto kbnode = iter->next();
    actual_subnodes.push_back(kbnode);;

    checkSubNodes(kbnode, kbnode_to_subnodes);
  }

  if (parent_node == nullptr)
    parent_node = &root_kbnode_;
  ASSERT_EQ((*kbnode_to_subnodes)[parent_node], actual_subnodes);
}

template <typename TestBase>
void TreeItemProviderTestBase<TestBase>::checkNonFilterModelKbNodeHierarchy() {
  std::vector<IKbNode*> level1_nodes;
  level1_nodes.push_back(xtestutils::genDummyPointer<IKbNode>());
  level1_nodes.push_back(xtestutils::genDummyPointer<IKbNode>());
  level1_nodes.push_back(xtestutils::genDummyPointer<IKbNode>());

  std::map<const IKbNode*, std::vector<IKbNode*> > kbnode_to_subnodes;

  std::vector<IKbNode*> level2_subnodes0;
  level2_subnodes0.push_back(xtestutils::genDummyPointer<IKbNode>());
  level2_subnodes0.push_back(xtestutils::genDummyPointer<IKbNode>());
  level2_subnodes0.push_back(xtestutils::genDummyPointer<IKbNode>());

  std::vector<IKbNode*> level2_subnodes2;
  level2_subnodes2.push_back(xtestutils::genDummyPointer<IKbNode>());
  level2_subnodes2.push_back(xtestutils::genDummyPointer<IKbNode>());
  level2_subnodes2.push_back(xtestutils::genDummyPointer<IKbNode>());

  std::vector<IKbNode*> level3_subnodes1;
  level3_subnodes1.push_back(xtestutils::genDummyPointer<IKbNode>());
  level3_subnodes1.push_back(xtestutils::genDummyPointer<IKbNode>());
  level3_subnodes1.push_back(xtestutils::genDummyPointer<IKbNode>());

  kbnode_to_subnodes[&root_kbnode_] = level1_nodes;
  kbnode_to_subnodes[level1_nodes[0]] = level2_subnodes0;
  kbnode_to_subnodes[level1_nodes[2]] = level2_subnodes2;

  kbnode_to_subnodes[level2_subnodes2[1]] = level3_subnodes1;

  EXPECT_CALL(node_manager_, childItems(_))
      .WillRepeatedly(
          Invoke([&kbnode_to_subnodes](const IKbNode* parent_kbnode) {
            return kbnode_to_subnodes[parent_kbnode];
          }));

  // Verify results
  ASSERT_FALSE(kbnode_provider_->isFilterMode());
  checkSubNodes(nullptr, &kbnode_to_subnodes);
}

TEST_F(TreeItemProviderTest,
       should_be_able_to_iterator_the_kbnode_hierarchy_when_not_in_filter_mode) { // NOLINT
  CUSTOM_ASSERT(checkNonFilterModelKbNodeHierarchy());
}

}  // namespace tests
}  // namespace snailcore
