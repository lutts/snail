//-*- TestCaseName: PfTriadManagerAutoRemoveChildTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/pfmvp/pf_triad_manager_test_base.h"

#include "src/utils/log/log.h"

namespace pfmvp {
namespace tests {

using DestroyMethodFunc =
    std::function<void(IPfTriadManager*, TestXXX_MVPL_Tuple*)>;

class PfTriadManagerAutoRemoveChildTest
    : public PfTriadManagerTestBase,
      public TestWithParam<DestroyMethodFunc> {
 protected:
  PfTriadManagerAutoRemoveChildTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }

  virtual void SetUp() {
    initLog();
    initialize();
  }

  std::vector<TestXXX_MVPL_Tuple> createTestTriadHierachy(
      bool root_enable_auto_remove_child = true, bool use_parent = true);

  virtual void TearDown() { cleanupLog(); }
};

static DestroyMethodFunc remove_by_model = [](IPfTriadManager* triad_manager,
                                              TestXXX_MVPL_Tuple* mvpl_tuple) {
  triad_manager->removeTriadBy(std::get<0>(*mvpl_tuple));
};

static DestroyMethodFunc remove_by_view = [](IPfTriadManager* triad_manager,
                                             TestXXX_MVPL_Tuple* mvpl_tuple) {
  triad_manager->removeTriadBy(std::get<1>(*mvpl_tuple));
};

static DestroyMethodFunc remove_by_request = [](
    IPfTriadManager* triad_manager, TestXXX_MVPL_Tuple* mvpl_tuple) {
  triad_manager->requestRemoveTriadByView(std::get<1>(*mvpl_tuple));
};

INSTANTIATE_TEST_CASE_P(VariousTriadCreateDestroyMethod,
                        PfTriadManagerAutoRemoveChildTest,
                        ::testing::Values(remove_by_model, remove_by_view,
                                          remove_by_request));

std::vector<TestXXX_MVPL_Tuple>
PfTriadManagerAutoRemoveChildTest::createTestTriadHierachy(
    bool root_enable_auto_remove_child, bool use_parent) {
  /*               __________
   *                |    1   | (root)           level0
   *                ----------
   *               /          \.
   *        ______/            \_______
   *        |  2 |              |  3  |         level1
   *        ------              -------
   *        /    \              /     \.
   *    ______   ______     ______    _______
   *    | 5  |   | 6  |     |  4 |    |  7  |   level2
   *    ------   ------     ------    -------
   *
   *    number in the box is the insertion order
   */

  std::vector<TestXXX_MVPL_Tuple> triad_vec;

  TestXXX_MVPL_Tuple root_tuple;
  createTestTriadAndListener<MockXXXViewFactory>(&root_tuple, nullptr,
                                                 root_enable_auto_remove_child);

  triad_vec.push_back(root_tuple);

#define ADD_TEST_TRIAD(level, order, parent)                                 \
  TestXXX_MVPL_Tuple level##_##order##_tuple;                                \
  createTestTriadAndListener<MockXXXViewFactory>(                            \
      &level##_##order##_tuple, use_parent ? std::get<2>(parent) : nullptr); \
  triad_vec.push_back(level##_##order##_tuple)

  ADD_TEST_TRIAD(level1, order2, root_tuple);
  ADD_TEST_TRIAD(level1, order3, root_tuple);
  ADD_TEST_TRIAD(level2, order4, level1_order3_tuple);
  ADD_TEST_TRIAD(level2, order5, level1_order2_tuple);
  ADD_TEST_TRIAD(level2, order6, level1_order2_tuple);
  ADD_TEST_TRIAD(level2, order7, level1_order3_tuple);

#undef ADD_TEST_TRIAD

  return triad_vec;
}

TEST_P(
    PfTriadManagerAutoRemoveChildTest,
    should_be_able_to_destroy_child_triad_first_in_FILO_order_when_destroy_parent_triad) {  // NOLINT
  // Setup fixture
  auto remove_method = GetParam();

  auto triad_vec = createTestTriadHierachy();
  auto& root_tuple = triad_vec[0];

  // Expectations
  xtestutils::CheckPointType check;
  Sequence s1;

  for (auto iter = triad_vec.rbegin(); iter != triad_vec.rend(); ++iter) {
    auto& mvpl_tuple = *iter;
    expectationsOnSingleTriadDestroy(&mvpl_tuple, nullptr, &s1);
  }
  EXPECT_CALL(check, Call("barrier")).InSequence(s1);

  // Exercise system
  //  triad_manager->removeTriadBy(std::get<0>(root_tuple));
  remove_method(triad_manager.get(), &root_tuple);
  check.Call("barrier");
}

TEST_P(
    PfTriadManagerAutoRemoveChildTest,
    should_not_destroy_chill_triad_if_parent_auto_remove_child_is_false) {  // NOLINT
  auto tester = [this](bool auto_remove_child, bool use_parent) {
    // Setup fixture
    auto remove_method = GetParam();

    auto triad_vec = createTestTriadHierachy(auto_remove_child, use_parent);
    auto& root_tuple = triad_vec[0];

    // Expectations
    xtestutils::CheckPointType check;
    Sequence s1;

    expectationsOnSingleTriadDestroy(&root_tuple, &check, &s1);

    // childs will not destroyed
    for (size_t idx = 1; idx < triad_vec.size(); ++idx) {
      auto& mvpl_tuple = triad_vec[idx];
      expectationsOnNotDestroyTriad(&mvpl_tuple);
    }

    // Exercise system
    remove_method(triad_manager.get(), &root_tuple);
    check.Call("barrier");

    for (auto& mvpl_tuple : triad_vec) {
      ::Mock::VerifyAndClear(std::get<0>(mvpl_tuple));
      ::Mock::VerifyAndClear(std::get<1>(mvpl_tuple));
      ::Mock::VerifyAndClear(std::get<2>(mvpl_tuple));
      ::Mock::VerifyAndClear(std::get<3>(mvpl_tuple).get());
    }
  };

  tester(false, true);
  tester(true, false);
}

TEST_F(PfTriadManagerAutoRemoveChildTest,
       should_failed_to_create_triad_with_unmanaged_parent) {  // NOLINT
  // Setup fixture
  MockXXXPresenter un_managed_presenter(nullptr, nullptr);
  auto model = std::make_shared<MockYYYModel>();

  // Verify results
  ASSERT_EQ(nullptr,
            triad_manager->createViewFor(model, &un_managed_presenter));
}

TEST_F(
    PfTriadManagerAutoRemoveChildTest,
    should_createViewFor_default_to_null_parent_and_true_auto_remove_child) {  // NOLINT
  // NOTE:
  //   * true auto_remove_child means child will be destroyed
  //     because parent is destroyed
  //   * null parent means a triad will not destroyed
  //     because another triad is destroyed
  // Setup fixture
  MockXXXView* view1 = nullptr;
  MockXXXView* view2 = nullptr;
  MockXXXView* view3 = nullptr;

  {  // prepare scope
    view_factory_t<MockXXXModel, MockXXXViewFactory> view_factory_wrapper;
    auto& factory = view_factory_wrapper.FTO_getFactory();

    auto model = std::make_shared<MockXXXModel>();
    std::shared_ptr<IPfModel> pfmodel = model;

    auto expect_view1 = std::make_shared<MockXXXView>();
    auto expect_view2 = std::make_shared<MockXXXView>();
    auto expect_view3 = std::make_shared<MockXXXView>();

    PfPresenter* presenter2 = nullptr;

    EXPECT_CALL(factory, createTestView())
        .WillOnce(Return(expect_view1))
        .WillOnce(Return(expect_view2))
        .WillOnce(Return(expect_view3));

    auto actual_view1 = triad_manager->createViewFor(model);
    auto actual_view2 = triad_manager->createViewFor(model);
    presenter2 = factory.last_presenter;
    ASSERT_NE(nullptr, presenter2);
    auto actual_view3 = triad_manager->createViewFor(model, presenter2);

    ASSERT_EQ(expect_view1, actual_view1);
    ASSERT_EQ(expect_view2, actual_view2);
    ASSERT_EQ(expect_view3, actual_view3);

    view1 = expect_view1.get();
    view2 = expect_view2.get();
    view3 = expect_view3.get();
  }

  // 1. destroy view1 will not destroy view2 and view3
  EXPECT_CALL(*view1, destruct());
  EXPECT_CALL(*view2, destruct()).Times(0);
  EXPECT_CALL(*view3, destruct()).Times(0);

  triad_manager->removeTriadBy(view1);

  ::Mock::VerifyAndClearExpectations(view1);
  ::Mock::VerifyAndClearExpectations(view2);
  ::Mock::VerifyAndClearExpectations(view3);

  // 2. destroy view2 will destroy view3 first
  {
    InSequence seq;

    EXPECT_CALL(*view3, destruct());
    EXPECT_CALL(*view2, destruct());
  }

  triad_manager->removeTriadBy(view2);

  ::Mock::VerifyAndClearExpectations(view2);
  ::Mock::VerifyAndClearExpectations(view3);
}

TEST_F(PfTriadManagerAutoRemoveChildTest,
       should_be_able_to_remove_child_before_remove_parent) {  // NOLINT
  // Setup fixture
  auto triad_vec = createTestTriadHierachy();
  auto& root_mvpl_tuple = triad_vec[0];
  auto& child_mvpl_tuple = triad_vec[1];

  // Exercise system
  triad_manager->removeTriadBy(std::get<0>(child_mvpl_tuple));
  triad_manager->removeTriadBy(std::get<0>(root_mvpl_tuple));

  // Verify results
  // no segment fault means ok
}

}  // namespace tests
}  // namespace pfmvp
