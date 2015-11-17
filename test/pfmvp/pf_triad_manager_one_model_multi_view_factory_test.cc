//-*- TestCaseName: PfTriadManagerOneModelMultiViewFactoryTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#define BOOST_BIND_NO_PLACEHOLDERS

#include "test/pfmvp/pf_triad_manager_test_base.h"

using namespace std::placeholders;  // NOLINT

namespace pfmvp {
namespace tests {

class PfTriadManagerOneModelMultiViewFactoryTest
    : public PfTriadManagerTestBase,
      public ::testing::Test {
 protected:
  PfTriadManagerOneModelMultiViewFactoryTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  virtual void SetUp() {
    initialize();

    view_factory_t<MockXXXModel, MockXXXViewFactory> view_factory_wrapper1;
    view_factory_t<MockXXXModel, MockXXXViewFactory2> view_factory_wrapper2;

    auto& factory1 = view_factory_wrapper1.FTO_getFactory();
    auto& factory2 = view_factory_wrapper2.FTO_getFactory();

    model = std::make_shared<MockXXXModel>();

    const int NUM_TEST_VIEWS = 3;

    RECORD_USED_MOCK_OBJECTS_SETUP;

    for (int i = 0; i < NUM_TEST_VIEWS; ++i) {
      auto aView = std::make_shared<MockXXXView>();
      R_EXPECT_CALL(factory1, createTestView())
          .WillOnce(Return(aView))
          .RetiresOnSaturation();

      expect_factory1_views.push_back(aView.get());
    }

    std::vector<std::shared_ptr<MockXXXView>> factory2_views;
    for (int i = 0; i < NUM_TEST_VIEWS; ++i) {
      auto aView = std::make_shared<MockXXXView>();
      R_EXPECT_CALL(factory2, createTestView())
          .WillOnce(Return(aView))
          .RetiresOnSaturation();

      expect_factory2_views.push_back(aView.get());
    }

    // Exercise system
    PfCreateViewArgs args;
    args.set_view_factory_id(factory1.getViewFactoryId());

    for (int i = 0; i < NUM_TEST_VIEWS; ++i) {
      auto actual_view =
          triad_manager->createViewFor(model, nullptr, true, &args);

      ASSERT_EQ(expect_factory1_views[NUM_TEST_VIEWS - 1 - i],
                actual_view.get());
      verifyTriad(model.get(), actual_view.get(), factory1.last_presenter);
    }

    args.set_view_factory_id(factory2.getViewFactoryId());
    for (int i = 0; i < NUM_TEST_VIEWS; ++i) {
      auto actual_view =
          triad_manager->createViewFor(model, nullptr, true, &args);

      ASSERT_EQ(expect_factory2_views[NUM_TEST_VIEWS - 1 - i],
                actual_view.get());
      verifyTriad(model.get(), actual_view.get(), factory2.last_presenter);
    }

    VERIFY_RECORDED_MOCK_OBJECTS;

    std::sort(expect_factory1_views.begin(), expect_factory1_views.end());
    std::sort(expect_factory2_views.begin(), expect_factory2_views.end());
  }

  using MatcherFunc =
      std::function<int(const PfCreateViewArgsMemento& memento)>;
  void checkFindViewByModelWithMatcher(MatcherFunc matcher1,
                                       MatcherFunc matcher2);

  std::shared_ptr<MockXXXModel> model;
  std::vector<IPfView*> expect_factory1_views;
  std::vector<IPfView*> expect_factory2_views;
};

TEST_F(PfTriadManagerOneModelMultiViewFactoryTest,
       should_be_able_to_find_view_by_model_and_view_factory_id) {
  auto actual_factory1_views = triad_manager->findViewByModelAndViewFactory(
      model.get(), MockXXXViewFactory::viewFactoryId());
  auto actual_factory2_views = triad_manager->findViewByModelAndViewFactory(
      model.get(), MockXXXViewFactory2::viewFactoryId());

  // Verify results
  std::sort(actual_factory1_views.begin(), actual_factory1_views.end());
  std::sort(actual_factory2_views.begin(), actual_factory2_views.end());

  ASSERT_EQ(expect_factory1_views, actual_factory1_views);
  ASSERT_EQ(expect_factory2_views, actual_factory2_views);
}

TEST_F(
    PfTriadManagerOneModelMultiViewFactoryTest,
    should_build_in_PfCreateViewArgsMemento_stored_the_factory_id) {  // NOLINT
  // Setup fixture
  PfCreateViewArgs args;
  args.set_view_factory_id(MockXXXViewFactory::viewFactoryId());

  // Expectations
  auto memento = args.getMemento();

  // Exercise system
  ASSERT_EQ(MockXXXViewFactory::viewFactoryId(), memento->view_factory_id());
}

void PfTriadManagerOneModelMultiViewFactoryTest::
    checkFindViewByModelWithMatcher(MatcherFunc matcher1,
                                    MatcherFunc matcher2) {
  // create some jam triads
  TestXXX_MVP_Triad mvp_triad;
  CUSTOM_ASSERT(createTestXXXTriads(&mvp_triad));

  // Exercise system
  auto actual_factory1_views =
      triad_manager->findViewByModel_if(model.get(), matcher1);

  auto actual_factory2_views =
      triad_manager->findViewByModel_if(model.get(), matcher2);

  // Verify results
  std::sort(actual_factory1_views.begin(), actual_factory1_views.end());
  std::sort(actual_factory2_views.begin(), actual_factory2_views.end());

  ASSERT_EQ(expect_factory1_views, actual_factory1_views);
  ASSERT_EQ(expect_factory2_views, actual_factory2_views);
}

TEST_F(
    PfTriadManagerOneModelMultiViewFactoryTest,
    should_be_able_to_find_using_predicate_with_CreateViewArgsMemento) {  // NOLINT
  auto matcher = [](
      const PfCreateViewArgsMemento& memento,
      const IPfViewFactory::ViewFactoryIdType& view_factory_id) -> int {
    if (memento.view_factory_id() == view_factory_id) {
      return IPfTriadManager::kMatchedContinue;
    } else {
      return IPfTriadManager::kNotMatched;
    }
  };

  // Verify results
  CUSTOM_ASSERT(checkFindViewByModelWithMatcher(
      std::bind(matcher, _1, MockXXXViewFactory::viewFactoryId()),
      std::bind(matcher, _1, MockXXXViewFactory2::viewFactoryId())));
}

TEST_F(PfTriadManagerOneModelMultiViewFactoryTest,
       test_that_PfCreateViewArgs_can_do_memento_equal_check) {  // NOLINT
  // Setup fixture
  PfCreateViewArgs factory1_args;
  factory1_args.set_view_factory_id(MockXXXViewFactory::viewFactoryId());

  PfCreateViewArgs factory2_args;
  factory2_args.set_view_factory_id(MockXXXViewFactory2::viewFactoryId());

  auto matcher = [](const PfCreateViewArgsMemento& memento,
                    PfCreateViewArgs* args) -> int {
    if (args->memento_equals(memento)) {
      return IPfTriadManager::kMatchedContinue;
    } else {
      return IPfTriadManager::kNotMatched;
    }
  };

  // Verify results
  CUSTOM_ASSERT(
      checkFindViewByModelWithMatcher(std::bind(matcher, _1, &factory1_args),
                                      std::bind(matcher, _1, &factory2_args)));
}

TEST_F(
    PfTriadManagerOneModelMultiViewFactoryTest,
    should_triad_create_with_no_args_use_default_memento_when_use_predicate_find) {  // NOLINT
  // Setup fixture
  auto no_args_model = std::make_shared<MockXXXModel>();
  auto no_args_view = std::make_shared<MockXXXView>();

  CUSTOM_ASSERT(createTestXXXTriad(no_args_model, no_args_view));

  bool equals_default = false;

  // Verify results
  auto actual_views = triad_manager->findViewByModel_if(
      no_args_model.get(),
      [&equals_default](const PfCreateViewArgsMemento& memento) -> int {
        PfCreateViewArgsMemento default_memento;
        equals_default = default_memento.equals(memento);

        return IPfTriadManager::kMatchedContinue;
      });

  ASSERT_TRUE(equals_default);
  ASSERT_EQ(1, actual_views.size());
  ASSERT_EQ(no_args_view.get(), actual_views[0]);
}

TEST_F(PfTriadManagerOneModelMultiViewFactoryTest,
       should_findViewByModel_if_can_be_break_on_first_match) {  // NOLINT
  // Fixture setup
  auto matcher = [](const PfCreateViewArgsMemento& memento,
                    const IPfViewFactory::ViewFactoryIdType& view_factory_id) {
    if (memento.view_factory_id() == view_factory_id) {
      return IPfTriadManager::kMatchedBreak;
    } else {
      return IPfTriadManager::kNotMatched;
    }
  };

  // ensure there are multiple views for factory1
  ASSERT_GT(expect_factory1_views.size(), 1);

  // Exercise system
  auto actual_factory1_views = triad_manager->findViewByModel_if(
      model.get(), std::bind(matcher, _1, MockXXXViewFactory::viewFactoryId()));

  // Verify results
  ASSERT_EQ(1, actual_factory1_views.size());
}

}  // namespace tests
}  // namespace pfmvp
