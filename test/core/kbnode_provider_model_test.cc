//-*- TestCaseName: KbNodeProviderModelTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"
#include "src/core/kbnode_provider_model.h"
#include "snail/mock_kbnode_provider.h"

namespace snailcore {
namespace tests {

class KbNodeProviderModelTest : public ::testing::Test {
 protected:
  KbNodeProviderModelTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~KbNodeProviderModelTest() { }
  virtual void SetUp() {
    model = utils::make_unique<KbNodeProviderModel>(&kbnode_provider);
  }
  // virtual void TearDown() { }

  // region: objects test subject depends on
  MockKbNodeProvider kbnode_provider;
  // endregion

  // region: test subject
  std::unique_ptr<KbNodeProviderModel> model;
  // endregion

  // region: object depends on test subject
  // endregion
};

TEST_F(KbNodeProviderModelTest,
       should_be_able_to_get_the_kbnode_provider) { // NOLINT
  ASSERT_EQ(&kbnode_provider, model->getKbNodeProvider());
}

TEST_F(KbNodeProviderModelTest,
       should_filter_pattern_be_provider_filter_pattern) { // NOLINT
  // Setup fixture
  auto expect_pattern = xtestutils::genRandomString();

  // Expectations
  EXPECT_CALL(kbnode_provider, getFilterPattern())
      .WillOnce(Return(expect_pattern));

  // Exercise system
  auto actual_pattern = model->getFilterPattern();

  // Verify results
  ASSERT_EQ(expect_pattern, actual_pattern);
}

TEST_F(KbNodeProviderModelTest,
       should_setFilterPattern_set_to_provider) { // NOLINT
  // Setup fixture
  auto filter_pattern = xtestutils::genRandomString();

  // Expectations
  EXPECT_CALL(kbnode_provider, setFilterPattern(filter_pattern));

  // Exercise system
  model->setFilterPattern(filter_pattern);
}

TEST_F(KbNodeProviderModelTest,
       should_be_able_to_set_and_get_new_kbnode_parent) { // NOLINT
  // Setup fixture
  auto expect_kbnode = xtestutils::genDummyPointer<IKbNode>();

  // initially nullptr
  ASSERT_EQ(nullptr, model->getNewKbNodeParent());

  // Exercise system
  model->setNewKbNodeParent(expect_kbnode);

  // Verify results
  ASSERT_EQ(expect_kbnode, model->getNewKbNodeParent());
}

TEST_F(KbNodeProviderModelTest,
       should_validate_false_when_set_an_empty_name) { // NOLINT
  // initially false
  ASSERT_EQ(false, model->isNewKbNodeNameValid());

  // Exercise system
  model->setNewKbNodeName("");

  // Verify results
  ASSERT_EQ(false, model->isNewKbNodeNameValid());
}

TEST_F(KbNodeProviderModelTest,
       should_validate_true_when_set_an_non_empty_name) { // NOLINT
  // Setup fixture
  auto name = xtestutils::genRandomString();

  // initially false
  ASSERT_EQ(false, model->isNewKbNodeNameValid());

  // Exercise system
  model->setNewKbNodeName(name);

  // Verify results
  ASSERT_EQ(true, model->isNewKbNodeNameValid());
}

BEGIN_MOCK_LISTENER_DEF(MockListener, IKbNodeProviderModel)

MOCK_METHOD2(KbNodeAdded,
               void(IKbNode* new_kbnode, IKbNode* parent_kbnode));

BEGIN_BIND_SIGNAL(IKbNodeProviderModel)

BIND_SIGNAL2(KbNodeAdded,
             void, IKbNode*, new_kbnode, IKbNode*, parent_kbnode);

END_BIND_SIGNAL()
END_MOCK_LISTENER_DEF()


TEST_F(KbNodeProviderModelTest,
       should_add_kbnode_with_setted_new_name_and_parent_and_emit_KbNodeAdded_when_add_successful) { // NOLINT
  // Setup fixture
  auto expect_parent_kbnode = xtestutils::genDummyPointer<IKbNode>();
  auto expect_new_kbnode = xtestutils::genDummyPointer<IKbNode>();
  auto expect_new_name = xtestutils::genRandomString();

  model->setNewKbNodeParent(expect_parent_kbnode);
  model->setNewKbNodeName(expect_new_name);

  // Expectations
  EXPECT_CALL(kbnode_provider, addKbNode(expect_new_name, expect_parent_kbnode))
      .WillOnce(Return(expect_new_kbnode));
  auto mock_listener = MockListener::attachTo(model.get());
  EXPECT_CALL(*mock_listener,
              KbNodeAdded(expect_new_kbnode, expect_parent_kbnode));

  // Exercise system
  model->addKbNode();
}

TEST_F(KbNodeProviderModelTest,
       should_not_add_kbnode_to_provider_when_name_is_invalid) { // NOLINT
  // Expectations
  EXPECT_CALL(kbnode_provider, addKbNode(_, _)).Times(0);

  auto mock_listener = MockListener::attachTo(model.get());
  EXPECT_CALL(*mock_listener, KbNodeAdded(_, _)).Times(0);

  // Exercise system
  model->addKbNode();
}

TEST_F(KbNodeProviderModelTest,
       should_not_emit_KbNodeAdded_when_add_failed) { // NOLINT
  // Setup fixture
  auto expect_parent_kbnode = xtestutils::genDummyPointer<IKbNode>();
  auto expect_new_name = xtestutils::genRandomString();

  model->setNewKbNodeParent(expect_parent_kbnode);
  model->setNewKbNodeName(expect_new_name);

  // Expectations
  EXPECT_CALL(kbnode_provider, addKbNode(expect_new_name, expect_parent_kbnode))
      .WillOnce(Return(nullptr));

  auto mock_listener = MockListener::attachTo(model.get());
  EXPECT_CALL(*mock_listener, KbNodeAdded(_, _)).Times(0);

  // Exercise system
  model->addKbNode();
}

}  // namespace tests
}  // namespace snailcore
