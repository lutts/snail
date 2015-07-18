//-*- TestCaseName: PfTriadManagerTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/pfmvp/pf_triad_manager_test_base.h"

namespace pfmvp {
namespace tests {

class PfTriadManagerTest : public PfTriadManagerTestBase
                         , public ::testing::Test {
 protected:
  PfTriadManagerTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }

  virtual void SetUp() {
    initialize();
  }

  // virtual void TearDown() { }
};

TEST_F(PfTriadManagerTest,
       should_PfCreateViewArgs_setter_getters_working) { // NOLINT
  // Setup fixture
  PfCreateViewArgs args;

  // default values
  ASSERT_EQ(INVALID_PF_VIEW_FACTORY_ID, args.view_factory_id());

  // Verify results
  args.set_view_factory_id(MockXXXViewFactory::viewFactoryId());
  ASSERT_EQ(MockXXXViewFactory::viewFactoryId(), args.view_factory_id());
}

TEST_F(PfTriadManagerTest, should_be_able_to_create_view_for_model_and_hold_triad_reference) { // NOLINT
  // Setup fixture
  auto xxx_model = std::make_shared<MockXXXModel>();
  auto xxx_view = std::make_shared<MockXXXView>();

  auto yyy_model = std::make_shared<MockYYYModel>();
  auto yyy_view = std::make_shared<MockYYYView>();

  // Exercise system
  CUSTOM_ASSERT(createTestXXXTriad(xxx_model, xxx_view));
  CUSTOM_ASSERT(createTestYYYTriad(yyy_model, yyy_view));
}

TEST_F(PfTriadManagerTest,
       should_args_passed_as_is_to_ViewFactory_createView) { // NOLINT
  // Setup fixture
  view_factory_t<MockXXXModel,
                 MockPfViewFactory> view_factory_wrapper;
  auto& factory = view_factory_wrapper.FTO_getFactory();
  std::shared_ptr<IPfModel> model = std::make_shared<MockXXXModel>();

  PfCreateViewArgs args;

  // Expectations
  EXPECT_CALL(factory, createView(model, &args)).WillOnce(Return(nullptr));

  // Exercise system
  triad_manager->createViewFor(model, nullptr, true, &args);

  // Verify result
  ::Mock::VerifyAndClearExpectations(&factory);

  // pass nullptr will got nullptr
  EXPECT_CALL(factory, createView(model, nullptr)).WillOnce(Return(nullptr));
  triad_manager->createViewFor(model, nullptr, true, nullptr);
  ::Mock::VerifyAndClearExpectations(&factory);
}

TEST_F(PfTriadManagerTest, should_return_null_when_view_factory_failed_create_view) { // NOLINT
  // Setup fixture
  view_factory_t<MockXXXModel,
                 MockPfViewFactory> view_factory_wrapper;
  auto& factory = view_factory_wrapper.FTO_getFactory();

  std::shared_ptr<IPfModel> model = std::make_shared<MockXXXModel>();
  ON_CALL(factory, createView(model, _))
      .WillByDefault(Return(nullptr));

  // Exercise system
  auto view = triad_manager->createViewFor(model);

  // Verify results
  ASSERT_EQ(nullptr, view);
}

TEST_F(PfTriadManagerTest, should_destroy_triads_in_FILO_order_when_triad_manager_destroyed) { // NOLINT
  // Setup fixture
  TestXXX_MVP_Triad mvp_triad;
  std::vector<TestXXX_MVP_Triad> all_mvp_triad;
  CUSTOM_ASSERT(createTestXXXTriads(&mvp_triad, &all_mvp_triad));

  // Expectations
  {
    InSequence seq;

    for (auto iter = all_mvp_triad.rbegin();
         iter != all_mvp_triad.rend();
         ++iter) {
      auto model = std::get<0>(*iter);
      auto view = std::get<1>(*iter);

      EXPECT_CALL(*view, destruct());
      EXPECT_CALL(*model, destruct());
    }
  }

  // Exercise system
  triad_manager.reset();
}

TEST_F(PfTriadManagerTest,
       should_be_able_to_destroy_triad_by_model) { // NOLINT
  // Setup fixture
  TestXXX_MVP_Triad mvp_triad;
  CUSTOM_ASSERT(createTestXXXTriads(&mvp_triad));
  auto model = std::get<0>(mvp_triad);
  auto view = get<1>(mvp_triad);

  // Expectations
  auto mockListener = MockListener::attachTo(triad_manager.get(), model, view);
  CheckPointType check;

  expectationsOnSingleTriadDestroy(mvp_triad, mockListener.get(),
                                   &check);

  // Exercise system
  triad_manager->removeTriadBy(model);
  check.Call("barrier");
}

TEST_F(PfTriadManagerTest,
       should_be_able_to_destroy_triad_by_view) { // NOLINT
  // Setup fixture
  TestXXX_MVP_Triad mvp_triad;
  CUSTOM_ASSERT(createTestXXXTriads(&mvp_triad));
  auto model = std::get<0>(mvp_triad);
  auto view = get<1>(mvp_triad);

  // Expectations
  auto mockListener = MockListener::attachTo(triad_manager.get(), model, view);
  CheckPointType check;

  expectationsOnSingleTriadDestroy(mvp_triad,
                                   mockListener.get(),
                                   &check);

  // Exercise system
  triad_manager->removeTriadBy(view);
  check.Call("barrier");
}

TEST_F(PfTriadManagerTest, should_be_able_to_monitor_managed_model_and_view) { // NOLINT
  // Setup fixture
  auto model = std::make_shared<MockXXXModel>();
  auto view = std::make_shared<MockXXXView>();

  createTestXXXTriad(model, view);

  // Exercise system
  auto mockListener = MockListener::attachTo(triad_manager.get(),
                                             model.get(), view.get());

  // Verify results
  ASSERT_TRUE(mockListener->requestRemoveModelBinded);
  ASSERT_TRUE(mockListener->aboutToDestroyModelBinded);
  ASSERT_TRUE(mockListener->aboutToDestroyViewBinded);
}

TEST_F(PfTriadManagerTest, should_failed_to_monitor_not_managed_model_and_view) { // NOLINT
  // Setup fixture
  auto model = std::make_shared<MockXXXModel>();
  auto view = std::make_shared<MockXXXView>();

  // Expectations
  auto mockListener = MockListener::attachTo(triad_manager.get(),
                                             model.get(), view.get());

  // Verify results
  ASSERT_FALSE(mockListener->requestRemoveModelBinded);
  ASSERT_FALSE(mockListener->aboutToDestroyModelBinded);
  ASSERT_FALSE(mockListener->aboutToDestroyViewBinded);
}

TEST_F(PfTriadManagerTest, should_destruct_triad_if_no_one_insterest_in_request_remove_signal) { // NOLINT
  // Setup fixture
  TestXXX_MVP_Triad mvp_triad;
  CUSTOM_ASSERT(createTestXXXTriads(&mvp_triad));
  auto model = std::get<0>(mvp_triad);
  auto view = get<1>(mvp_triad);

  // Expectations
  #define dont_monitor_request_remove false
  auto mockListener = MockListener::attachTo(triad_manager.get(), model, view,
                                             dont_monitor_request_remove);
  CheckPointType check;
  expectationsOnSingleTriadDestroy(mvp_triad,
                                   mockListener.get(),
                                   &check);

  // Exercise system
  ASSERT_TRUE(triad_manager->requestRemoveTriadByView(view));
  check.Call("barrier");
}

TEST_F(PfTriadManagerTest, should_destruct_triad_if_request_remove_by_view_return_true) { // NOLINT
  // Setup fixture
  TestXXX_MVP_Triad mvp_triad;
  CUSTOM_ASSERT(createTestXXXTriads(&mvp_triad));
  auto model = std::get<0>(mvp_triad);
  auto view = get<1>(mvp_triad);

  bool expect_result = true;

  // Expectations
  auto mockListener = MockListener::attachTo(triad_manager.get(), model, view);
  CheckPointType check;
  {
    Sequence s1;

    EXPECT_CALL(*mockListener, RequestRemoveModel(model))
        .InSequence(s1)
        .WillOnce(Return(expect_result));

    expectationsOnSingleTriadDestroy(mvp_triad,
                                     mockListener.get(),
                                     &check,
                                     &s1);
  }

  // Exercise system
  auto actual_result = triad_manager->requestRemoveTriadByView(view);
  check.Call("barrier");

  // Verify results
  ASSERT_EQ(expect_result, actual_result);
}

TEST_F(PfTriadManagerTest, should_not_destruct_triad_if_request_remove_by_view_return_false) { // NOLINT
  // Setup fixture
  TestXXX_MVP_Triad mvp_triad;
  CUSTOM_ASSERT(createTestXXXTriads(&mvp_triad));
  auto model = std::get<0>(mvp_triad);
  auto view = get<1>(mvp_triad);
  auto presenter = get<2>(mvp_triad);

  bool expect_result = false;

  // Expectations
  auto mockListener = MockListener::attachStrictTo(triad_manager.get(),
                                                   model, view);

  EXPECT_CALL(*mockListener, RequestRemoveModel(model))
      .WillOnce(Return(expect_result));

  expectationsOnNotDestroyTriad(mvp_triad,
                                mockListener.get());

  // Exercise system
  auto actual_result = triad_manager->requestRemoveTriadByView(view);

  // Verify results
  ASSERT_EQ(expect_result, actual_result);

  ::Mock::VerifyAndClear(model);
  ::Mock::VerifyAndClear(view);
  ::Mock::VerifyAndClear(presenter);
}

TEST_F(PfTriadManagerTest, should_remove_model_only_when_all_views_are_removed) { // NOLINT
  // Setup fixture
  MockXXXModel* model = nullptr;

  MockXXXPresenter* presenter1 = nullptr;
  MockXXXView* view1 = nullptr;

  MockXXXPresenter* presenter2 = nullptr;
  MockXXXView* view2 = nullptr;

  {
    auto m = std::make_shared<MockXXXModel>();
    auto v1 = std::make_shared<MockXXXView>();
    auto v2 = std::make_shared<MockXXXView>();

    createTestXXXTriad(m, v1, &presenter1);
    createTestXXXTriad(m, v2, &presenter2);

    model = m.get();
    view1 = v1.get();
    view2 = v2.get();
  }

  auto mockListener1 = MockListener::attachStrictTo(triad_manager.get(),
                                              model, view1);
  auto mockListener2 = MockListener::attachStrictTo(triad_manager.get(),
                                              model, view2);

  // Expectations on remove view1
  expectationsOnSingleTriadDestroy(
      make_xxx_triad(nullptr, view1, presenter1),
      mockListener1.get());

  expectationsOnNotDestroyTriad(
      make_xxx_triad(model, view2, presenter2),
      mockListener2.get());

  // Exercise system: remove view1
  triad_manager->removeTriadBy(view1);

  // Verify result
  ::Mock::VerifyAndClear(model);
  ::Mock::VerifyAndClear(view1);
  ::Mock::VerifyAndClear(presenter1);
  ::Mock::VerifyAndClear(view2);
  ::Mock::VerifyAndClear(presenter2);
  ::Mock::VerifyAndClear(mockListener1.get());
  ::Mock::VerifyAndClear(mockListener2.get());

  CheckPointType check;
  // Expectations on remove view2
  expectationsOnSingleTriadDestroy(
      make_xxx_triad(model, view2, presenter2),
      mockListener2.get(),
      &check);

  EXPECT_CALL(*mockListener1, AboutToDestroyModel(model));

  // Excercise system:: remove view1
  triad_manager->removeTriadBy(view2);
  check.Call("barrier");
}

TEST_F(PfTriadManagerTest, should_remove_all_triads_of_a_model_when_the_model_removed) { // NOLINT
  // Setup fixture
  MockXXXModel* model = nullptr;

  MockXXXPresenter* presenter1 = nullptr;
  MockXXXView* view1 = nullptr;

  MockXXXPresenter* presenter2 = nullptr;
  MockXXXView* view2 = nullptr;

  {
    auto m = std::make_shared<MockXXXModel>();
    auto v1 = std::make_shared<MockXXXView>();
    auto v2 = std::make_shared<MockXXXView>();

    createTestXXXTriad(m, v1, &presenter1);
    createTestXXXTriad(m, v2, &presenter2);

    model = m.get();
    view1 = v1.get();
    view2 = v2.get();
  }

  auto mockListener1 = MockListener::attachStrictTo(triad_manager.get(),
                                              model, view1);
  auto mockListener2 = MockListener::attachStrictTo(triad_manager.get(),
                                              model, view2);

  CheckPointType check;
  // Expectations
  expectationsOnTwoTriadDestroy(
      make_xxx_triad(model, view1, presenter1),
      mockListener1.get(),
      make_xxx_triad(model, view2, presenter2),
      mockListener2.get(),
      &check);

  // Exercise system
  triad_manager->removeTriadBy(model);
  check.Call("barrier");
}

TEST_F(PfTriadManagerTest, should_remove_subscriber_only_when_subject_removed) { // NOLINT
  // Setup fixture
  MockXXXModel* model = nullptr;
  MockXXXView* view1 = nullptr;
  MockXXXView* view2 = nullptr;

  auto m = std::make_shared<MockXXXModel>();
  auto v1 = std::make_shared<MockXXXView>();
  auto v2 = std::make_shared<MockXXXView>();

  createTestXXXTriad(m, v1);
  createTestXXXTriad(m, v2);

  model = m.get();
  view1 = v1.get();
  view2 = v2.get();

  auto mockListener1 = MockListener::attachStrictTo(triad_manager.get(),
                                              model, view1);
  auto mockListener2 = MockListener::attachStrictTo(triad_manager.get(),
                                                    model, view2);

  EXPECT_CALL(*mockListener1, AboutToDestroyView(view1));

  // only remove one view
  triad_manager->removeTriadBy(view1);

  ::Mock::VerifyAndClear(mockListener1.get());

  // Expectations: these methods will still be called
  EXPECT_CALL(*mockListener1, RequestRemoveModel(model))
      .WillOnce(Return(true));
  EXPECT_CALL(*mockListener2, RequestRemoveModel(model))
      .WillOnce(Return(true));
  EXPECT_CALL(*mockListener1, AboutToDestroyModel(model));
  EXPECT_CALL(*mockListener2, AboutToDestroyModel(model));
  EXPECT_CALL(*mockListener2, AboutToDestroyView(view2));

  // except this one!
  EXPECT_CALL(*mockListener1, AboutToDestroyView(view1)).Times(0);

  // Exercise system: remove the last view
  ASSERT_TRUE(triad_manager->requestRemoveTriadByView(view2));

  // Verify result
  ::Mock::VerifyAndClear(mockListener1.get());
  ::Mock::VerifyAndClear(mockListener2.get());

  // re-create a triad with the same model
  createTestXXXTriad(m, v2);

  // Expectations: these methods will not be called
  EXPECT_CALL(*mockListener1, RequestRemoveModel(model)).Times(0);
  EXPECT_CALL(*mockListener2, RequestRemoveModel(model)).Times(0);
  EXPECT_CALL(*mockListener1, AboutToDestroyModel(model)).Times(0);
  EXPECT_CALL(*mockListener1, AboutToDestroyView(view1)).Times(0);
  EXPECT_CALL(*mockListener2, AboutToDestroyModel(model)).Times(0);
  EXPECT_CALL(*mockListener2, AboutToDestroyView(view2)).Times(0);

  // Excercise system
  ASSERT_TRUE(triad_manager->requestRemoveTriadByView(view2));
}

TEST_F(PfTriadManagerTest, should_be_able_to_find_view_by_model) { // NOLINT
  // Setup fixture
  auto model = std::make_shared<MockXXXModel>();
  auto view1 = std::make_shared<MockXXXView>();
  auto view2 = std::make_shared<MockXXXView>();

  createTestXXXTriad(model, view1);
  createTestXXXTriad(model, view2);

  // create some jam triads
  TestXXX_MVP_Triad mvp_triad;
  CUSTOM_ASSERT(createTestXXXTriads(&mvp_triad));

  std::vector<IPfView*> expect_views;
  expect_views.push_back(view1.get());
  expect_views.push_back(view2.get());

  std::sort(expect_views.begin(), expect_views.end());

  // Verify results
  auto actual_views = triad_manager->findViewByModel(model.get());
  std::sort(actual_views.begin(), actual_views.end());

  ASSERT_EQ(expect_views, actual_views);
}

TEST_F(PfTriadManagerTest, should_be_able_to_find_model_by_view) { // NOLINT
  // Setup fixture
  TestXXX_MVP_Triad mvp_triad;
  CUSTOM_ASSERT(createTestXXXTriads(&mvp_triad));
  auto model = std::get<0>(mvp_triad);
  auto view = get<1>(mvp_triad);

  // Verify results
  ASSERT_EQ(model, triad_manager->findModelByView(view));
}

TEST_F(PfTriadManagerTest, should_be_able_to_delete_by_triad_self) { // NOLINT
  // Setup fixture
  TestXXX_MVPL_Tuple mvpl_tuple;
  createTestTriadAndListener<MockXXXViewFactory>(&mvpl_tuple);

  // Expectations
  CheckPointType check;
  expectationsOnSingleTriadDestroy(&mvpl_tuple, &check);

  // Exercise system
  auto presenter = std::get<2>(mvpl_tuple);
  presenter->destroySelfTriad();
  check.Call("barrier");
}

}  // namespace tests
}  // namespace pfmvp
