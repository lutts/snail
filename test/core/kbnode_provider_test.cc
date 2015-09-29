//-*- TestCaseName: KbNodeProviderTest;-*-
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

#include "src/core/kbnode_provider.h"
#include "core/mock_kbnode_manager.h"
#include "snail/mock_kbnode.h"

namespace snailcore {
namespace tests {

template <typename TestBase>
class KbNodeProviderTestBase : public TestBase {
 protected:
  KbNodeProviderTestBase() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~KbNodeProviderTestBase() { }
  virtual void SetUp() {
    root_kbnode_name_ = xtestutils::genRandomString();
    EXPECT_CALL(root_kbnode_, name())
        .WillRepeatedly(Return(root_kbnode_name_));

    kbnode_provider_ = utils::make_unique<KbNodeProvider>(&root_kbnode_,
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
  std::unique_ptr<KbNodeProvider> kbnode_provider_;
  // endregion

  // region: object depends on test subject
  // endregion
};

class KbNodeProviderTest : public KbNodeProviderTestBase<::testing::Test> { };

TEST_F(KbNodeProviderTest,
       should_provider_name_be_root_kbnode_name) { // NOLINT
  ASSERT_EQ(root_kbnode_name_, kbnode_provider_->name());
}

BEGIN_MOCK_LISTENER_DEF(MockListener, IKbNodeProvider)

MOCK_METHOD0(BeginFilter, void());
MOCK_METHOD0(FinishFilter, void());

BEGIN_BIND_SIGNAL(IKbNodeProvider)

BIND_SIGNAL0(BeginFilter, void);
BIND_SIGNAL0(FinishFilter, void);

END_BIND_SIGNAL()
END_MOCK_LISTENER_DEF()


TEST_F(KbNodeProviderTest,
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
  auto iterator = kbnode_provider_->childNodes(nullptr);
  while (iterator->hasNext()) {
    auto kbnode = iterator->next();
    actual_kbnodes.push_back(kbnode);

    auto sub_iter = kbnode_provider_->childNodes(kbnode);
    ASSERT_FALSE(sub_iter->hasNext());
  }

  ASSERT_EQ(expect_kbnodes, actual_kbnodes);
}

TEST_F(KbNodeProviderTest,
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
void KbNodeProviderTestBase<TestBase>::checkSubNodes(
    IKbNode* parent_node,
    std::map<const IKbNode*, std::vector<IKbNode*> >*  kbnode_to_subnodes) {
  std::vector<IKbNode*> actual_subnodes;
  auto iter = kbnode_provider_->childNodes(parent_node);
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
void KbNodeProviderTestBase<TestBase>::checkNonFilterModelKbNodeHierarchy() {
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

  EXPECT_CALL(node_manager_, childNodes(_))
      .WillRepeatedly(
          Invoke([&kbnode_to_subnodes](const IKbNode* parent_kbnode) {
            return kbnode_to_subnodes[parent_kbnode];
          }));

  // Verify results
  ASSERT_FALSE(kbnode_provider_->isFilterMode());
  checkSubNodes(nullptr, &kbnode_to_subnodes);
}

TEST_F(KbNodeProviderTest,
       should_be_able_to_iterator_the_kbnode_hierarchy_when_not_in_filter_mode) { // NOLINT
  CUSTOM_ASSERT(checkNonFilterModelKbNodeHierarchy());
}

class KbNodeProviderTest_BoolParam :
      public KbNodeProviderTestBase<::testing::TestWithParam<bool>> { };

INSTANTIATE_TEST_CASE_P(BoolParam,
                        KbNodeProviderTest_BoolParam,
                        ::testing::Bool());

TEST_P(KbNodeProviderTest_BoolParam,
       should_addKbNode_actual_call_node_manager_addKbNode) { // NOLINT
  // Setup fixture
  auto expect_name = xtestutils::genRandomString();
  auto foo1_parent_node = xtestutils::genDummyPointer<IKbNode>();
  auto foo1_child_node = xtestutils::genDummyPointer<IKbNode>();
  auto root_child_node = xtestutils::genDummyPointer<IKbNode>();
  auto is_category = GetParam();

  // Expectations
  EXPECT_CALL(node_manager_, addKbNode(expect_name,
                                       foo1_parent_node, is_category))
      .WillOnce(Return(foo1_child_node));
  // when called with null parent, root is the actual parent
  EXPECT_CALL(node_manager_, addKbNode(expect_name, &root_kbnode_, is_category))
      .WillOnce(Return(root_child_node));

  // Exercise system
  auto actual_foo1_child_node =
      kbnode_provider_->addKbNode(expect_name, foo1_parent_node, is_category);
  auto actual_root_child_node =
      kbnode_provider_->addKbNode(expect_name, nullptr, is_category);

  // Verify results
  ASSERT_EQ(foo1_child_node, actual_foo1_child_node);
  ASSERT_EQ(root_child_node, actual_root_child_node);
}

TEST_F(KbNodeProviderTest,
       should_incRef_call_node_manager_incRef) { // NOLINT
  // Setup fixture
  auto kbnode = xtestutils::genDummyPointer<IKbNode>();

  // Expectations
  EXPECT_CALL(node_manager_, incRef(kbnode));

  // Exercise system
  kbnode_provider_->incRef(kbnode);
}

TEST_F(KbNodeProviderTest,
       should_findKbNodeByName_call_node_manager_findKbNode) { // NOLINT
  // Setup fixture
  auto name = xtestutils::genRandomString();
  std::vector<IKbNode*> expect_kbnodes;
  expect_kbnodes.push_back(xtestutils::genDummyPointer<IKbNode>());
  expect_kbnodes.push_back(xtestutils::genDummyPointer<IKbNode>());
  expect_kbnodes.push_back(xtestutils::genDummyPointer<IKbNode>());

  // Expectations
  EXPECT_CALL(node_manager_, findKbNode(name, &root_kbnode_))
      .WillOnce(Return(expect_kbnodes));

  // Exercise system
  auto actual_kbnodes = kbnode_provider_->findKbNodeByName(name);

  // Verify results
  ASSERT_EQ(expect_kbnodes, actual_kbnodes);
}

TEST_F(KbNodeProviderTest,
       should_findKbNodeByName_do_not_call_node_manager_findKbNode_when_name_equals_current_filter_pattern) { // NOLINT
  // Setup fixture
  auto filter_pattern = xtestutils::genRandomString();
  std::vector<IKbNode*> expect_kbnodes;
  expect_kbnodes.push_back(xtestutils::genDummyPointer<IKbNode>());
  expect_kbnodes.push_back(xtestutils::genDummyPointer<IKbNode>());
  expect_kbnodes.push_back(xtestutils::genDummyPointer<IKbNode>());

  EXPECT_CALL(node_manager_, findKbNode(filter_pattern, &root_kbnode_))
      .WillOnce(Return(expect_kbnodes));
  kbnode_provider_->setFilterPattern(filter_pattern);

  Mock::VerifyAndClearExpectations(&node_manager_);

  // Expectations
  EXPECT_CALL(node_manager_, findKbNode(_, _)).Times(0);

  // Exercise system
  auto actual_kbnodes = kbnode_provider_->findKbNodeByName(filter_pattern);

  // Verify results
  ASSERT_EQ(expect_kbnodes, actual_kbnodes);
}

TEST_F(KbNodeProviderTest,
       should_findKbNodeByName_with_empty_name_return_empty_results) { // NOLINT
  // Setup fixture
  std::vector<IKbNode*> dummy_kbnodes;
  dummy_kbnodes.push_back(xtestutils::genDummyPointer<IKbNode>());
  dummy_kbnodes.push_back(xtestutils::genDummyPointer<IKbNode>());
  dummy_kbnodes.push_back(xtestutils::genDummyPointer<IKbNode>());

  EXPECT_CALL(node_manager_, findKbNode(_, _))
      .WillRepeatedly(Return(dummy_kbnodes));

  kbnode_provider_->setFilterPattern(xtestutils::genRandomString());

  // Exercise system
  auto actual_kbnodes = kbnode_provider_->findKbNodeByName("");

  // Verify results
  std::vector<IKbNode*> expect_empty_kbnodes;
  ASSERT_EQ(expect_empty_kbnodes, actual_kbnodes);
}

}  // namespace tests
}  // namespace snailcore
