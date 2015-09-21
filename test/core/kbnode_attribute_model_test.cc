//-*- TestCaseName: KbNodeAttributeModelTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"
#include "test/core/kbnode_id_generator.h"

#include "src/core/kbnode_attribute_model.h"
#include "core/mock_kbnode_attribute.h"
#include "core/mock_kbnode_attribute_supplier.h"
#include "core/mock_kbnode_manager.h"
#include "snail/mock_kbnode_provider.h"

namespace snailcore {
namespace tests {

class KbNodeAttributeModelTest : public ::testing::Test {
 protected:
  KbNodeAttributeModelTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~KbNodeAttributeModelTest() { }
  virtual void SetUp() {
    kbnode_provider = std::make_shared<MockKbNodeProvider>();

    // get kbnode provider mocks
    EXPECT_CALL(kbnode_attr, kbnode_supplier())
        .WillRepeatedly(Return(&kbnode_attr_supplier));

    auto root_kbnode_id = KbNodeIdGenerator::gen();
    EXPECT_CALL(kbnode_attr_supplier, getRootKbNodeId())
        .WillRepeatedly(Return(root_kbnode_id));
    // can create kbnode provider only once
    EXPECT_CALL(kbnode_manager, createKbNodeProvider(root_kbnode_id))
        .Times(AtMost(1))
        .WillOnce(Return(kbnode_provider));

    model = utils::make_unique<KbNodeAttributeModel>(&kbnode_attr,
                                                     &kbnode_manager);
  }
  // virtual void TearDown() { }


  // region: objects test subject depends on
  MockKbNodeAttributeSupplier kbnode_attr_supplier;
  MockKbNodeAttribute kbnode_attr;
  MockKbNodeManager kbnode_manager;
  std::shared_ptr<MockKbNodeProvider> kbnode_provider;
  // endregion

  // region: test subject
  std::unique_ptr<KbNodeAttributeModel> model;
  // endregion

  // region: object depends on test subject
  // endregion
};

TEST_F(KbNodeAttributeModelTest,
       should_be_able_to_return_a_kbnode_provider) { // NOLINT
  // Exercise system
  auto actual_provider = model->getKbNodeProvider();

  // Verify results
  ASSERT_EQ(kbnode_provider.get(), actual_provider);

  // get again will got the same
  auto actual_provider_again = model->getKbNodeProvider();
  ASSERT_EQ(kbnode_provider.get(), actual_provider_again);
}

TEST_F(KbNodeAttributeModelTest,
       should_getKbNodeName_return_the_kbnode_attr_value_text) { // NOLINT
  // Setup fixture
  auto expect_name = xtestutils::genRandomString();

  // Expectations
  EXPECT_CALL(kbnode_attr, valueText())
      .WillOnce(Return(expect_name));

  // Exercise system
  auto actual_name = model->getKbNodeName();

  // Verify results
  ASSERT_EQ(expect_name, actual_name);
}

BEGIN_MOCK_LISTENER_DEF(MockListener, IAttributeModel)

MOCK_METHOD1(ValidateComplete, void(bool result));

BEGIN_BIND_SIGNAL(IAttributeModel)

BIND_SIGNAL1(ValidateComplete, void, bool, result);

END_BIND_SIGNAL()
END_MOCK_LISTENER_DEF()

TEST_F(KbNodeAttributeModelTest,
       test_setKbNode) { // NOLINT
  // Setup fixture
  auto kbnode = xtestutils::genDummyPointer<IKbNode>();

  // Expectations
  EXPECT_CALL(kbnode_attr, setKbNode(kbnode));
  // quit filter mode
  EXPECT_CALL(*kbnode_provider, setFilterPattern(""));
  EXPECT_CALL(*kbnode_provider, incRef(kbnode));

  auto mock_listener = MockListener::attachTo(model.get());
  EXPECT_CALL(*mock_listener, ValidateComplete(true));

  // Exercise system
  model->setKbNode(kbnode);
}

TEST_F(KbNodeAttributeModelTest,
       test_setKbNodeByName_with_the_same_name_as_current_kbnode) { // NOLINT
  // Setup fixture
  auto expect_name = xtestutils::genRandomString();
  int expect_result = IKbNodeAttributeModel::kSetKbNodeSuccess;

  // Expectations
  EXPECT_CALL(kbnode_attr, valueText())
      .WillOnce(Return(expect_name));

  EXPECT_CALL(*kbnode_provider, setFilterPattern(""));

  EXPECT_CALL(*kbnode_provider, findKbNodeByName(expect_name)).Times(0);
  EXPECT_CALL(kbnode_attr, setKbNode(_)).Times(0);
  // unfortunately, FailUninterestingCalls is private
  // Mock::FailUninterestingCalls(kbnode_provider.get());

  auto mock_listener = MockListener::attachTo(model.get());
  EXPECT_CALL(*mock_listener, ValidateComplete(true));

  // Exercise system
  int actual_result = model->setKbNodeByName(expect_name);

  ASSERT_EQ(expect_result, actual_result);
}

TEST_F(KbNodeAttributeModelTest,
       test_setKbNodeByName_with_no_match_in_provider) { // NOLINT
  // Setup fixture
  auto name = xtestutils::genRandomString();
  int expect_result = IKbNodeAttributeModel::kSetKbNodeNotFound;

  std::vector<IKbNode*> kbnodes;

  // Expectations
  EXPECT_CALL(*kbnode_provider, findKbNodeByName(name))
      .WillOnce(Return(kbnodes));

  EXPECT_CALL(kbnode_attr, setKbNode(_)).Times(0);

  auto mock_listener = MockListener::attachTo(model.get());
  EXPECT_CALL(*mock_listener, ValidateComplete(false));

  // Exercise system
  int actual_result = model->setKbNodeByName(name);

  // Verify results
  ASSERT_EQ(expect_result, actual_result);
}

TEST_F(KbNodeAttributeModelTest,
       test_setKbNodeByName_with_only_one_match) { // NOLINT
  // Setup fixture
  auto name = xtestutils::genRandomString();
  int expect_result = IKbNodeAttributeModel::kSetKbNodeSuccess;

  std::vector<IKbNode*> kbnodes;
  auto kbnode = xtestutils::genDummyPointer<IKbNode>();
  kbnodes.push_back(kbnode);

  // Expectations
  EXPECT_CALL(*kbnode_provider, findKbNodeByName(name))
      .WillOnce(Return(kbnodes));

  EXPECT_CALL(kbnode_attr, setKbNode(kbnode));
  EXPECT_CALL(*kbnode_provider, setFilterPattern(""));
  EXPECT_CALL(*kbnode_provider, incRef(kbnode));

  auto mock_listener = MockListener::attachTo(model.get());
  EXPECT_CALL(*mock_listener, ValidateComplete(true));

  // Exercise system
  int actual_result = model->setKbNodeByName(name);

  // Verify results
  ASSERT_EQ(expect_result, actual_result);
}

TEST_F(KbNodeAttributeModelTest,
       test_setKbNodeByName_with_multi_match) { // NOLINT
  // Setup fixture
  auto name = xtestutils::genRandomString();
  int expect_result = IKbNodeAttributeModel::kSetKbNodeMultpicMatched;

  std::vector<IKbNode*> kbnodes;
  kbnodes.push_back(xtestutils::genDummyPointer<IKbNode>());
  kbnodes.push_back(xtestutils::genDummyPointer<IKbNode>());

  // Expectations
  EXPECT_CALL(*kbnode_provider, findKbNodeByName(name))
      .WillOnce(Return(kbnodes));

  EXPECT_CALL(kbnode_attr, setKbNode(_)).Times(0);

  auto mock_listener = MockListener::attachTo(model.get());
  EXPECT_CALL(*mock_listener, ValidateComplete(false));

  // Exercise system
  int actual_result = model->setKbNodeByName(name);

  // Verify results
  ASSERT_EQ(expect_result, actual_result);
}

}  // namespace tests
}  // namespace snailcore
