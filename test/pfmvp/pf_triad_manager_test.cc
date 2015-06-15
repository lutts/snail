//-*- TestCaseName: PfTriadManagerTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <cstdlib>
#include <ctime>

#include "test/testutils/gmock_common.h"

#include "utils/basic_utils.h"  // make_unique, <memory>
#include "test/testutils/utils.h"
#include "test/testutils/generic_mock_listener.h"
#include "test/testutils/slot_catcher.h"

#include "utils/i_trackable.h"
#include "src/pfmvp/pf_triad_manager.h"
#include "pfmvp/i_pf_model.h"
#include "pfmvp/pf_presenter.h"
#include "pfmvp/i_pf_view.h"
#include "pfmvp/i_pf_view_factory.h"

namespace pfmvp {
namespace tests {

class TestXXXModel : public IPfModel {
 public:
  virtual ~TestXXXModel() = default;

  DEF_MODEL_ID(TestXXXModel);
};

class MockTestXXXModel : public TestXXXModel {
 public:
  virtual ~MockTestXXXModel() { destruct(); }

  MOCK_METHOD0(destruct, void());
};

class ITestXXXView : public IPfView {
 public:
  virtual ~ITestXXXView() = default;
};

class TestXXXView : public ITestXXXView {
 public:
  virtual ~TestXXXView() = default;
};

class MockTestXXXView : public TestXXXView {
 public:
  virtual ~MockTestXXXView() { destruct(); }

  MOCK_METHOD0(destruct, void());
};

class TestXXXPresenter : public PfPresenterT<TestXXXModel, ITestXXXView> {
 public:
  static std::shared_ptr<TestXXXPresenter>
  create(std::shared_ptr<TestXXXModel> model,
         std::shared_ptr<ITestXXXView> view) {
    auto presenter = std::make_shared<TestXXXPresenter>(model, view);
    return presenter;
  }

  TestXXXPresenter(std::shared_ptr<TestXXXModel> model,
                   std::shared_ptr<ITestXXXView> view)
      : PfPresenterT<TestXXXModel, ITestXXXView>(model, view) {
  }

 private:
  TestXXXPresenter(const TestXXXPresenter& other) = delete;
  TestXXXPresenter& operator=(const TestXXXPresenter& other) = delete;
};

class TestXXXViewFactory : public IPfViewFactory {
 public:
  TestXXXViewFactory() = default;
  virtual ~TestXXXViewFactory() = default;

  std::shared_ptr<PfPresenter>
  createView(std::shared_ptr<IPfModel> model) override {
    auto my_model = std::dynamic_pointer_cast<TestXXXModel>(model);
    if (my_model) {
      auto view = createTestXXXView();
      auto presenter = TestXXXPresenter::create(my_model, view);
      last_presenter = presenter.get();

      return presenter;
    }

    return nullptr;
  }

  virtual std::shared_ptr<ITestXXXView> createTestXXXView() const {
    return std::make_shared<TestXXXView>();
  }

  TestXXXPresenter* last_presenter;

 private:
  TestXXXViewFactory(const TestXXXViewFactory& other) = delete;
  TestXXXViewFactory& operator=(const TestXXXViewFactory& other) = delete;
};

class MockTestXXXViewFactory : public TestXXXViewFactory {
 public:
  MOCK_CONST_METHOD0(createTestXXXView, std::shared_ptr<ITestXXXView>());
};

class PfTriadManagerTest : public ::testing::Test {
 protected:
  PfTriadManagerTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~PfTriadManagerTest() { }
  virtual void SetUp() {
    triad_manager = utils::make_unique<PfTriadManager>(
        PfViewFactoryManager::getInstance());
  }
  // virtual void TearDown() { }

  using TestXXX_MVPair = std::pair<MockTestXXXModel*, MockTestXXXView*>;
  void createTestXXXTriads(TestXXX_MVPair* mvpair,
                           std::vector<TestXXX_MVPair>* all_mvpair);
  void createTestXXXTriad(std::shared_ptr<MockTestXXXModel> model,
                          std::shared_ptr<MockTestXXXView> view);


  // region: objects test subject depends on
  // endregion

  // region: test subject
  std::unique_ptr<IPfTriadManager> triad_manager;
  // endregion

  // region: object depends on test subject
  // endregion
};

TEST_F(PfTriadManagerTest, should_be_able_to_create_view_for_model_and_hold_triad_reference) { // NOLINT
  // Setup fixture
  auto model = std::make_shared<TestXXXModel>();
  auto expect_view = std::make_shared<TestXXXView>();

  auto old_model_use_count = model.use_count();

  view_factory_t<TestXXXModel, MockTestXXXViewFactory> view_factory_wrapper;
  auto& view_factory = view_factory_wrapper.FTO_getFactory();
  ON_CALL(view_factory, createTestXXXView())
      .WillByDefault(Return(expect_view));

  auto old_view_use_count = expect_view.use_count();

  // Expectations

  // Exercise system
  auto actual_view = triad_manager->createViewFor(model);

  // Verify results
  ASSERT_EQ(expect_view, actual_view);
  // should hold reference
  ASSERT_EQ(old_model_use_count + 1, model.use_count());
  // NOTE: 2 = one by triad manager + one by actual_view
  ASSERT_EQ(old_view_use_count + 2, expect_view.use_count());

  // for convenience, we will store the triad in presenter
  ASSERT_EQ(triad_manager.get(), view_factory.last_presenter->triad_manager());
}

// TODO(lutts): use TestYYYModel to ensure it working

void PfTriadManagerTest::createTestXXXTriad(
    std::shared_ptr<MockTestXXXModel> model,
    std::shared_ptr<MockTestXXXView> view) {
  {
    view_factory_t<TestXXXModel, MockTestXXXViewFactory> view_factory_wrapper;
    auto& view_factory = view_factory_wrapper.FTO_getFactory();
    ON_CALL(view_factory, createTestXXXView())
        .WillByDefault(Return(view));

    triad_manager->createViewFor(model);
  }

  ASSERT_EQ(3, model.use_count());
  ASSERT_EQ(3, view.use_count());
}

void PfTriadManagerTest::createTestXXXTriads(
    TestXXX_MVPair* mvpair_to_test,
    std::vector<TestXXX_MVPair>* all_mvpair = nullptr) {
  std::vector<TestXXX_MVPair> all;

  const int TEST_TRIAD_COUNT = 10;

  std::srand(std::time(0));
  int triad_to_test = std::rand() % TEST_TRIAD_COUNT;

  for (int i = 0; i < TEST_TRIAD_COUNT; ++i) {
    auto model = std::make_shared<MockTestXXXModel>();
    auto view = std::make_shared<MockTestXXXView>();

    createTestXXXTriad(model, view);

    all.push_back(std::make_pair(model.get(), view.get()));
  }

  *mvpair_to_test = all[triad_to_test];
  if (all_mvpair)
    *all_mvpair = all;
}

TEST_F(PfTriadManagerTest, should_destroy_triads_when_triad_manager_destroyed) { // NOLINT
  // Setup fixture
  TestXXX_MVPair mvpair;
  std::vector<TestXXX_MVPair> all_mvpair;
  CUSTOM_ASSERT(createTestXXXTriads(&mvpair, &all_mvpair));

  // Expectations
  MockFunction<void(string check_point_name)> check;

  for (auto mv : all_mvpair) {
    EXPECT_CALL(*mv.first, destruct());
    EXPECT_CALL(*mv.second, destruct());
  }

  // Exercise system
  triad_manager.reset();
}

TEST_F(PfTriadManagerTest, should_be_able_to_destroy_triad_by_model) { // NOLINT
  // Setup fixture
  TestXXX_MVPair mvpair;
  CUSTOM_ASSERT(createTestXXXTriads(&mvpair));
  auto model = mvpair.first;
  auto view = mvpair.second;

  // Expectations
  MockFunction<void(string check_point_name)> check;
  {
    InSequence seq;

    EXPECT_CALL(*view, destruct());
    EXPECT_CALL(*model, destruct());
    EXPECT_CALL(check, Call("barrier"));
  }

  // Exercise system
  triad_manager->removeTriadBy(model);

  check.Call("barrier");
}

TEST_F(PfTriadManagerTest, should_be_able_to_destroy_triad_by_view) { // NOLINT
  // Setup fixture
  TestXXX_MVPair mvpair;
  CUSTOM_ASSERT(createTestXXXTriads(&mvpair));
  auto model = mvpair.first;
  auto view = mvpair.second;

  // Expectations
  MockFunction<void(string check_point_name)> check;
  {
    InSequence seq;

    EXPECT_CALL(*view, destruct());
    EXPECT_CALL(*model, destruct());
    EXPECT_CALL(check, Call("barrier"));
  }

  // Exercise system
  triad_manager->removeTriadBy(view);

  check.Call("barrier");
}

class MockListener : public GenericMockListener<MockListener,
                                                IPfTriadManager> {
 public:
  MockListener(IPfModel* model, IPfView* view)
      : model_(model)
      , view_(view) { }

  MOCK_METHOD1(RequestRemoveModel, bool(IPfModel* model));
  MOCK_METHOD1(AboutToDestroyModel, void(IPfModel* model));
  MOCK_METHOD1(AboutToDestroyView, void(IPfView* view));

  void bindListenerMethods(std::shared_ptr<utils::ITrackable> trackObject,
                           IPfTriadManager* triad_manager) {
    if (model_) {
      triad_manager->whenRequestRemoveModel(
          model_,
          [this](IPfModel* model) -> bool {
            return RequestRemoveModel(model);
          },
          trackObject);

      triad_manager->whenAboutToDestroyModel(
          model_,
          [this](IPfModel* model) {
            AboutToDestroyModel(model);
          },
          trackObject);
    }

    if (view_) {
      triad_manager->whenAboutToDestroyView(
          view_,
          [this](IPfView* view) {
            AboutToDestroyView(view);
          },
          trackObject);
    }
  }

 private:
  IPfModel* model_;
  IPfView* view_;
};

TEST_F(PfTriadManagerTest, should_destruct_triad_if_no_one_insterest_in_request_remove_signal) { // NOLINT
  // Setup fixture
  TestXXX_MVPair mvpair;
  CUSTOM_ASSERT(createTestXXXTriads(&mvpair));
  auto model = mvpair.first;
  auto view = mvpair.second;

  // Expectations
  MockFunction<void(string check_point_name)> check;
  {
    InSequence seq;

    EXPECT_CALL(*view, destruct());
    EXPECT_CALL(*model, destruct());
    EXPECT_CALL(check, Call("barrier"));
  }

  // Exercise system
  ASSERT_TRUE(triad_manager->requestRemoveTriadByView(view));
  check.Call("barrier");
}

TEST_F(PfTriadManagerTest, should_destruct_triad_if_request_remove_by_view_return_true) { // NOLINT
  // Setup fixture
  TestXXX_MVPair mvpair;
  CUSTOM_ASSERT(createTestXXXTriads(&mvpair));
  auto model = mvpair.first;
  auto view = mvpair.second;

  bool expect_result = true;

  // Expectations
  auto mockListener = MockListener::attachTo(triad_manager.get(), model, view);

  MockFunction<void(string check_point_name)> check;
  {
    InSequence seq;

    EXPECT_CALL(*mockListener, RequestRemoveModel(model))
        .WillOnce(Return(expect_result));
    EXPECT_CALL(*mockListener, AboutToDestroyView(view));
    EXPECT_CALL(*mockListener, AboutToDestroyModel(model));
    EXPECT_CALL(*view, destruct());
    EXPECT_CALL(*model, destruct());
    EXPECT_CALL(check, Call("barrier"));
  }

  // Exercise system
  auto actual_result = triad_manager->requestRemoveTriadByView(view);
  check.Call("barrier");

  // Verify results
  ASSERT_EQ(expect_result, actual_result);
}

TEST_F(PfTriadManagerTest, should_not_destruct_triad_if_request_remove_by_view_return_false) { // NOLINT
  // Setup fixture
  TestXXX_MVPair mvpair;
  CUSTOM_ASSERT(createTestXXXTriads(&mvpair));
  auto model = mvpair.first;
  auto view = mvpair.second;

  bool expect_result = false;

  // Expectations
  auto mockListener = MockListener::attachTo(triad_manager.get(), model, view);

  EXPECT_CALL(*mockListener, RequestRemoveModel(model))
      .WillOnce(Return(expect_result));
  EXPECT_CALL(*view, destruct()).Times(0);
  EXPECT_CALL(*model, destruct()).Times(0);

  // Exercise system
  auto actual_result = triad_manager->requestRemoveTriadByView(view);

  // Verify results
  ASSERT_EQ(expect_result, actual_result);

  ::Mock::VerifyAndClear(model);
  ::Mock::VerifyAndClear(view);
}

TEST_F(PfTriadManagerTest, should_fire_model_view_about_to_destroy_when_remove_by_model_or_view) { // NOLINT
  // Setup fixture
  TestXXX_MVPair mvpair;
  CUSTOM_ASSERT(createTestXXXTriads(&mvpair));
  auto model = mvpair.first;
  auto view = mvpair.second;

  // Expectations
  auto mockListener = MockListener::attachTo(triad_manager.get(), model, view);

  MockFunction<void(string check_point_name)> check;
  {
    InSequence seq;

    EXPECT_CALL(*mockListener, AboutToDestroyView(view));
    EXPECT_CALL(*mockListener, AboutToDestroyModel(model));
    EXPECT_CALL(*view, destruct());
    EXPECT_CALL(*model, destruct());
    EXPECT_CALL(check, Call("barrier"));
  }

  // Exercise system
  triad_manager->removeTriadBy(model);
  check.Call("barrier");
}

TEST_F(PfTriadManagerTest, should_remove_subscriber_when_remove_triad) { // NOLINT
  // Setup fixture
  auto model = std::make_shared<MockTestXXXModel>();
  auto view = std::make_shared<MockTestXXXView>();

  createTestXXXTriad(model, view);

  auto mockListener = MockListener::attachTo(triad_manager.get(),
                                             model.get(), view.get());
  EXPECT_CALL(*mockListener, RequestRemoveModel(model.get()))
      .WillOnce(Return(true));

  triad_manager->requestRemoveTriadByView(view.get());

  ASSERT_EQ(1, model.use_count());
  ASSERT_EQ(1, view.use_count());

  ::Mock::VerifyAndClear(mockListener.get());

  // recreate the triad with the same model and view,
  // but the listener is not re-attached
  createTestXXXTriad(model, view);

  // Expectations
  EXPECT_CALL(*mockListener, RequestRemoveModel(_)).Times(0);
  EXPECT_CALL(*mockListener, AboutToDestroyModel(_)).Times(0);
  EXPECT_CALL(*mockListener, AboutToDestroyView(_)).Times(0);

  // Exercise system
  ASSERT_TRUE(triad_manager->requestRemoveTriadByView(view.get()));
}

TEST_F(PfTriadManagerTest, should_be_able_to_find_view_by_model) { // NOLINT
  // Setup fixture
  auto model = std::make_shared<MockTestXXXModel>();
  auto view = std::make_shared<MockTestXXXView>();

  createTestXXXTriad(model, view);

  // Verify results
  ASSERT_EQ(view.get(), triad_manager->findViewByModel(model.get()));
}

TEST_F(PfTriadManagerTest, should_be_able_to_find_model_by_view) { // NOLINT
  // Setup fixture
  auto model = std::make_shared<MockTestXXXModel>();
  auto view = std::make_shared<MockTestXXXView>();

  createTestXXXTriad(model, view);

  // Verify results
  ASSERT_EQ(model.get(), triad_manager->findModelByView(view.get()));
}

}  // namespace tests
}  // namespace pfmvp
