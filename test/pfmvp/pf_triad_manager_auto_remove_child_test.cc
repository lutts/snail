//-*- TestCaseName: PfTriadManagerAutoRemoveChildTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/pfmvp/pf_triad_manager_test_base.h"

namespace pfmvp {
namespace tests {

using DestroyMethodFunc =
    std::function<void(IPfTriadManager*, TestXXX_MVPL_Tuple*)>;
using TestParamType =
    std::tuple<DestroyMethodFunc,
               bool>;  // use_factory_id>;

class PfTriadManagerAutoRemoveChildTest
    : public PfTriadManagerTestBase
    , public TestWithParam<TestParamType> {
 protected:
  PfTriadManagerAutoRemoveChildTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }

  virtual void SetUp() {
    initialize();
  }

  template <typename VF, typename MVPLTuple>
  void createTestTriadAndListener(
      MVPLTuple* tuple,
      ViewCreatationFunction* viewCreatationFunc = nullptr,
      PfPresenter* parent_presenter = nullptr,
      bool auto_remove_child = true);

  std::vector<TestXXX_MVPL_Tuple> createTestTriadHierachy(
      bool use_view_factory_id = false,
      bool root_enable_auto_remove_child = true,
      bool use_parent = true);

  // virtual void TearDown() { }
};

template <typename VF, typename MVPLTuple>
void PfTriadManagerAutoRemoveChildTest::createTestTriadAndListener(
    MVPLTuple* tuple,
    ViewCreatationFunction* viewCreatationFunc,
    PfPresenter* parent_presenter,
    bool auto_remove_child) {
  using MT = typename std::remove_pointer<
    typename std::tuple_element<0, MVPLTuple>::type>::type;
  using VT = typename std::remove_pointer<
    typename std::tuple_element<1, MVPLTuple>::type>::type;
  using PT = typename std::remove_pointer<
    typename std::tuple_element<2, MVPLTuple>::type>::type;

  auto model = std::make_shared<MT>();
  auto view = std::make_shared<VT>();
  PT* presenter = nullptr;

  createTestTriad<VF>(model, view, &presenter,
                      viewCreatationFunc,
                      parent_presenter,
                      auto_remove_child);

  auto listener = MockListener::attachTo(triad_manager.get(),
                                         model.get(),
                                         view.get(),
                                         false);

  *tuple = std::make_tuple(model.get(), view.get(), presenter, listener);
}

static DestroyMethodFunc remove_by_model =
    [](IPfTriadManager* triad_manager,
       TestXXX_MVPL_Tuple* mvpl_tuple) {
  triad_manager->removeTriadBy(std::get<0>(*mvpl_tuple));
};

static DestroyMethodFunc remove_by_view =
  [](IPfTriadManager* triad_manager,
     TestXXX_MVPL_Tuple* mvpl_tuple) {
  triad_manager->removeTriadBy(std::get<1>(*mvpl_tuple));
};

static DestroyMethodFunc remove_by_request =
    [](IPfTriadManager* triad_manager,
       TestXXX_MVPL_Tuple* mvpl_tuple) {
  triad_manager->requestRemoveTriadByView(std::get<1>(*mvpl_tuple));
};

INSTANTIATE_TEST_CASE_P(
    VariousTriadCreateDestroyMethod,
    PfTriadManagerAutoRemoveChildTest,
    ::testing::Values(
         std::make_tuple(remove_by_model, false),
         std::make_tuple(remove_by_view, false),
         std::make_tuple(remove_by_request, false),

         std::make_tuple(remove_by_model, true),
         std::make_tuple(remove_by_view, true),
         std::make_tuple(remove_by_request, true)));

std::vector<TestXXX_MVPL_Tuple>
PfTriadManagerAutoRemoveChildTest::createTestTriadHierachy(
    bool use_view_factory_id,
    bool root_enable_auto_remove_child,
    bool use_parent) {
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

  ViewCreatationFunction createViewFunc =
      [this, use_view_factory_id](
          std::shared_ptr<IPfModel> model,
          const IPfViewFactory::ViewFactoryIdType& view_factory_id,
          PfPresenter* parent_presenter,
          bool auto_remove_child) -> std::shared_ptr<IPfView> {
    if (!use_view_factory_id) {
      return triad_manager->createViewFor(model,
                                          parent_presenter,
                                          auto_remove_child);
    } else {
      return triad_manager->createViewFor(model,
                                          view_factory_id,
                                          parent_presenter,
                                          auto_remove_child);
    }
  };

  TestXXX_MVPL_Tuple root_tuple;
  createTestTriadAndListener<MockXXXViewFactory>(
      &root_tuple,
      &createViewFunc,
      nullptr,
      root_enable_auto_remove_child);

  triad_vec.push_back(root_tuple);

#define ADD_TEST_TRIAD(level, order, parent)            \
  TestXXX_MVPL_Tuple level##_##order##_tuple;           \
      createTestTriadAndListener<MockXXXViewFactory>(   \
          &level##_##order##_tuple,                     \
          &createViewFunc,                              \
          use_parent ? std::get<2>(parent) : nullptr);  \
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

TEST_P(PfTriadManagerAutoRemoveChildTest,
       should_be_able_to_destroy_child_triad_first_in_FILO_order_when_destroy_parent_triad) { // NOLINT
  // Setup fixture
  auto remove_method = std::get<0>(GetParam());
  auto use_view_factory_id = std::get<1>(GetParam());

  auto triad_vec = createTestTriadHierachy(use_view_factory_id);
  auto& root_tuple = triad_vec[0];

  // Expectations
  CheckPointType check;
  Sequence s1;

  for (auto iter = triad_vec.rbegin();
       iter != triad_vec.rend();
       ++iter) {
    auto& mvpl_tuple = *iter;
    expectationsOnSingleTriadDestroy(&mvpl_tuple,
                                     nullptr,
                                     &s1);
  }
  EXPECT_CALL(check, Call("barrier"))
      .InSequence(s1);

  // Exercise system
  //  triad_manager->removeTriadBy(std::get<0>(root_tuple));
  remove_method(triad_manager.get(), &root_tuple);
  check.Call("barrier");
}

TEST_P(PfTriadManagerAutoRemoveChildTest,
       should_not_destroy_chill_triad_if_parent_auto_remove_child_is_false) { // NOLINT
  auto tester = [this](bool auto_remove_child,
                       bool use_parent) {
    // Setup fixture
    auto remove_method = std::get<0>(GetParam());
    auto use_view_factory_id = std::get<1>(GetParam());

    auto triad_vec = createTestTriadHierachy(use_view_factory_id,
                                             auto_remove_child,
                                             use_parent);
    auto& root_tuple = triad_vec[0];

    // Expectations
    CheckPointType check;
    Sequence s1;

    expectationsOnSingleTriadDestroy(&root_tuple,
                                     &check,
                                     &s1);

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
       should_failed_to_create_triad_with_unmanaged_parent) { // NOLINT
  // Setup fixture
  MockXXXPresenter un_managed_presenter(nullptr, nullptr);
  auto model = std::make_shared<MockYYYModel>();

  // Verify results
  ASSERT_EQ(nullptr,
            triad_manager->createViewFor(model, &un_managed_presenter));
}

}  // namespace tests
}  // namespace pfmvp
