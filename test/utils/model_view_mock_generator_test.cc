//-*- TestCaseName: ModelViewMockGeneratorTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"

#include "utils/basic_utils.h"  // make_unique, <memory>
#include "test/testutils/model_view_mock_generator.h"

class IDummyView {
 public:
  virtual ~IDummyView() = default;
};

class MockDummyView : public IDummyView {
 public:
  ~MockDummyView() { destruct(); }
  MOCK_METHOD0(destruct, void());
};

class IDummyModel {
 public:
  virtual ~IDummyModel() = default;
};

class MockDummyModel : public IDummyModel {
 public:
  ~MockDummyModel() { destruct(); }
  MOCK_METHOD0(destruct, void());
};

class ModelViewMockGeneratorTest : public ::testing::Test {
 protected:
  ModelViewMockGeneratorTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~ModelViewMockGeneratorTest() { }
  virtual void SetUp() { }
  // virtual void TearDown() { }

  ModelViewMockGenerator<MockDummyModel, MockDummyView> mvmock_generator;
};

TEST_F(ModelViewMockGeneratorTest, should_be_able_to_generator_specificed_class_mock_model_and_views) { // NOLINT
  // Exercise system
  auto mvpair = mvmock_generator.make_mvpair();

  // Verify results
  MockDummyModel* model = mvpair->model();
  ASSERT_NE(nullptr, model);

  MockDummyView* view = mvpair->view();
  ASSERT_NE(nullptr, view);
}

TEST_F(ModelViewMockGeneratorTest, should_be_able_to_return_as_std_pair) { // NOLINT
  // Setup fixture
  auto mvpair = mvmock_generator.make_mvpair();
  auto expect_model = mvpair->model();
  auto expect_view = mvpair->view();

  // Verify results
  auto stdpair = mvpair->toStdPair();
  ASSERT_EQ(expect_model, stdpair.first);
  ASSERT_EQ(expect_view, stdpair.second);
}

TEST_F(ModelViewMockGeneratorTest, should_get_same_model_view_when_call_model_view_multiple_times) { // NOLINT
  // Setup fixture
  auto mvpair = mvmock_generator.make_mvpair();

  // Expectations
  auto expect_model = mvpair->model();
  auto expect_view = mvpair->view();

  // Exercise system
  auto model2 = mvpair->model();
  auto view2 = mvpair->view();

  // Verify results
  ASSERT_EQ(expect_model, model2);
  ASSERT_EQ(expect_view, view2);
}

TEST_F(ModelViewMockGeneratorTest, should_get_different_model_view_on_different_mvpair) { // NOLINT
  // Setup fixture
  auto mvpair1 = mvmock_generator.make_mvpair();
  auto mvpair2 = mvmock_generator.make_mvpair();

  // Expectations

  // Exercise system
  auto model1 = mvpair1->model();
  auto model2 = mvpair2->model();
  ASSERT_NE(model1, model2);

  auto view1 = mvpair1->view();
  auto view2 = mvpair2->view();
  ASSERT_NE(view1, view2);
}

TEST_F(ModelViewMockGeneratorTest, should_destruct_model_view_when_reset_mvpair) { // NOLINT
  // Setup fixture
  auto mvpair = mvmock_generator.make_mvpair();
  auto model = mvpair->model();
  auto view = mvpair->view();

  // Expectations
  MockFunction<void(string check_point_name)> check;
  {
    InSequence seq;

    EXPECT_CALL(*model, destruct());
    EXPECT_CALL(*view, destruct());
    EXPECT_CALL(check, Call("after reset"));
  }

  // Exercise system
  mvpair->reset();
  check.Call("after reset");
}

TEST_F(ModelViewMockGeneratorTest, should_get_different_model_view_after_reset_mvpair) { // NOLINT
  // Setup fixture
  auto mvpair = mvmock_generator.make_mvpair();
  auto model = mvpair->model();
  auto view = mvpair->view();

  // Exercise system
  mvpair->reset();

  // NOTE: if we call getXXX imediately after reset(), we may got the same
  // address, so we insert some dummy allocation here
  auto dummy_mvpair = mvmock_generator.make_mvpair();
  dummy_mvpair->model();
  dummy_mvpair->view();

  // Verify results
  auto new_view = mvpair->view();
  auto new_model = mvpair->model();
  EXPECT_NE(view, new_view);
  EXPECT_NE(model, new_model);
}

TEST_F(ModelViewMockGeneratorTest, should_destruct_model_view_when_mvpair_deleted) { // NOLINT
  // Setup fixture
  auto mvpair = mvmock_generator.make_mvpair();
  auto model = mvpair->model();
  auto view = mvpair->view();

  // Expectations
  EXPECT_CALL(*model, destruct());
  EXPECT_CALL(*view, destruct());

  // Exercise system
  mvpair.reset();
}

template <typename TModel, typename TView>
class MockMVPair : public IModelViewPair {
 public:
  ~MockMVPair() { destruct(); }

  MOCK_METHOD0(destruct, void());
};

TEST_F(ModelViewMockGeneratorTest, should_destruct_all_mvpair_when_mvmock_generator_destruct) { // NOLINT
  // Setup fixture
  ModelViewMockGenerator<MockDummyModel,
                         MockDummyView,
                         MockMVPair> l_generator;

  auto mvpair1 = l_generator.make_mvpair();
  auto mvpair2 = l_generator.make_mvpair();
  auto mvpair3 = l_generator.make_mvpair();

  // Expectations
  EXPECT_CALL(*mvpair1, destruct());
  EXPECT_CALL(*mvpair2, destruct());
  EXPECT_CALL(*mvpair3, destruct());
}
