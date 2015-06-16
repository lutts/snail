//-*- TestCaseName: PfTriadManagerTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <cstdlib>
#include <ctime>

#include <algorithm>  // std::sort

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
#include "pfmvp/pf_single_view_factory_manager.h"

namespace pfmvp {
namespace tests {

#define DEFINE_TEST_CLASSES(name)                                       \
  class Test##name##Model : public IPfModel {                           \
   public:                                                              \
   virtual ~Test##name##Model() = default;                              \
                                                                        \
   DEF_MODEL_ID(Test##name##Model);                                     \
  };                                                                    \
                                                                        \
  class MockTest##name##Model : public Test##name##Model {              \
   public:                                                              \
   virtual ~MockTest##name##Model() { destruct(); }                     \
                                                                        \
   MOCK_METHOD0(destruct, void());                                      \
  };                                                                    \
                                                                        \
  class ITest##name##View : public IPfView {                            \
   public:                                                              \
   virtual ~ITest##name##View() = default;                              \
  };                                                                    \
                                                                        \
  class Test##name##View : public ITest##name##View {                   \
   public:                                                              \
   virtual ~Test##name##View() = default;                               \
  };                                                                    \
                                                                        \
  class MockTest##name##View : public Test##name##View {                \
   public:                                                              \
   virtual ~MockTest##name##View() { destruct(); }                      \
                                                                        \
   MOCK_METHOD0(destruct, void());                                      \
  };                                                                    \
                                                                        \
  class Test##name##Presenter : public PfPresenterT<Test##name##Model,  \
                                                    ITest##name##View> { \
   public:                                                              \
   static std::shared_ptr<Test##name##Presenter>                        \
   create(std::shared_ptr<Test##name##Model> model,                     \
          std::shared_ptr<ITest##name##View> view) {                    \
     auto presenter = std::make_shared<Test##name##Presenter>(model, view); \
     return presenter;                                                  \
   }                                                                    \
                                                                        \
   Test##name##Presenter(std::shared_ptr<Test##name##Model> model,      \
                         std::shared_ptr<ITest##name##View> view)       \
   : PfPresenterT<Test##name##Model, ITest##name##View>(model, view) {  \
   }                                                                    \
                                                                        \
   private:                                                             \
   Test##name##Presenter(const Test##name##Presenter&) = delete;        \
   Test##name##Presenter& operator=(const Test##name##Presenter&) = delete; \
  };                                                                    \
                                                                        \
  class Test##name##ViewFactory : public IPfViewFactory {               \
   public:                                                              \
   Test##name##ViewFactory() = default;                                 \
   virtual ~Test##name##ViewFactory() = default;                        \
                                                                        \
   std::shared_ptr<PfPresenter>                                         \
   createView(std::shared_ptr<IPfModel> model) override {               \
     auto my_model = std::dynamic_pointer_cast<Test##name##Model>(model); \
     if (my_model) {                                                    \
       auto view = createTestView();                                    \
  auto presenter = Test##name##Presenter::create(my_model, view);       \
  last_presenter = presenter.get();                                     \
                                                                        \
  return presenter;                                                     \
     }                                                                  \
                                                                        \
     return nullptr;                                                    \
   }                                                                    \
                                                                        \
   virtual std::shared_ptr<ITest##name##View> createTestView() const {  \
     return std::make_shared<Test##name##View>();                       \
   }                                                                    \
                                                                        \
   Test##name##Presenter* last_presenter;                               \
                                                                        \
   private:                                                             \
   Test##name##ViewFactory(const Test##name##ViewFactory&) = delete;    \
   Test##name##ViewFactory& operator=(                                  \
       const Test##name##ViewFactory&) = delete;                        \
  };                                                                    \
                                                                        \
  class MockTest##name##ViewFactory : public Test##name##ViewFactory {  \
   public:                                                              \
   MOCK_CONST_METHOD0(createTestView,                                   \
                      std::shared_ptr<ITest##name##View>());            \
  };

DEFINE_TEST_CLASSES(XXX)
DEFINE_TEST_CLASSES(YYY)

class PfTriadManagerTest : public ::testing::Test {
 protected:
  PfTriadManagerTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~PfTriadManagerTest() { }
  virtual void SetUp() {
    triad_manager = utils::make_unique<PfTriadManager>(
        PfSingleViewFactoryManager::getInstance());
  }
  // virtual void TearDown() { }

  template <typename VF, typename M, typename V>
  void createTestTriad(std::shared_ptr<M> model,
                       std::shared_ptr<V> view);

  using TestXXX_MVPair = std::pair<MockTestXXXModel*, MockTestXXXView*>;
  void createTestXXXTriads(TestXXX_MVPair* mvpair,
                           std::vector<TestXXX_MVPair>* all_mvpair);
  void createTestXXXTriad(std::shared_ptr<MockTestXXXModel> model,
                          std::shared_ptr<MockTestXXXView> view);

  using TestYYY_MVPair = std::pair<MockTestYYYModel*, MockTestYYYView*>;
  void createTestYYYTriads(TestYYY_MVPair* mvpair,
                           std::vector<TestYYY_MVPair>* all_mvpair);
  void createTestYYYTriad(std::shared_ptr<MockTestYYYModel> model,
                          std::shared_ptr<MockTestYYYView> view);


  // region: objects test subject depends on
  // endregion

  // region: test subject
  std::unique_ptr<IPfTriadManager> triad_manager;
  // endregion

  // region: object depends on test subject
  // endregion
};

template <typename VF, typename M, typename V>
void PfTriadManagerTest::createTestTriad(
    std::shared_ptr<M> model,
    std::shared_ptr<V> view) {
  {  // working scope
    view_factory_single_t<M, VF> view_factory_wrapper;
    auto& view_factory = view_factory_wrapper.FTO_getFactory();
    ON_CALL(view_factory, createTestView())
        .WillByDefault(Return(view));

    auto actual_view = triad_manager->createViewFor(model);
    ASSERT_EQ(view, actual_view);

    // for convenience, we will store the triad in presenter
    ASSERT_EQ(triad_manager.get(), view_factory.last_presenter->triad_manager());
  }

  ASSERT_EQ(4, model.use_count());
  ASSERT_EQ(4, view.use_count());
}

void PfTriadManagerTest::createTestXXXTriad(
    std::shared_ptr<MockTestXXXModel> model,
    std::shared_ptr<MockTestXXXView> view) {
  createTestTriad<MockTestXXXViewFactory>(model, view);
}

void PfTriadManagerTest::createTestYYYTriad(
    std::shared_ptr<MockTestYYYModel> model,
    std::shared_ptr<MockTestYYYView> view) {
  createTestTriad<MockTestYYYViewFactory>(model, view);
}

TEST_F(PfTriadManagerTest, should_be_able_to_create_view_for_model_and_hold_triad_reference) { // NOLINT
  // Setup fixture
  auto xxx_model = std::make_shared<MockTestXXXModel>();
  auto xxx_view = std::make_shared<MockTestXXXView>();

  // Exercise system
  createTestXXXTriad(xxx_model, xxx_view);

  // Verify results: should hold reference
  ASSERT_EQ(2, xxx_model.use_count());
  ASSERT_EQ(2, xxx_view.use_count());

  // Trianglation test
  auto yyy_model = std::make_shared<MockTestYYYModel>();
  auto yyy_view = std::make_shared<MockTestYYYView>();
  createTestYYYTriad(yyy_model, yyy_view);
  ASSERT_EQ(2, yyy_model.use_count());
  ASSERT_EQ(2, yyy_view.use_count());
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

void PfTriadManagerTest::createTestYYYTriads(
    TestYYY_MVPair* mvpair_to_test,
    std::vector<TestYYY_MVPair>* all_mvpair = nullptr) {
  std::vector<TestYYY_MVPair> all;

  const int TEST_TRIAD_COUNT = 10;

  std::srand(std::time(0));
  int triad_to_test = std::rand() % TEST_TRIAD_COUNT;

  for (int i = 0; i < TEST_TRIAD_COUNT; ++i) {
    auto model = std::make_shared<MockTestYYYModel>();
    auto view = std::make_shared<MockTestYYYView>();

    createTestYYYTriad(model, view);

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
  TestXXX_MVPair mvpair;
  CUSTOM_ASSERT(createTestXXXTriads(&mvpair));
  auto model = mvpair.first;
  auto view = mvpair.second;

  // Verify results
  ASSERT_EQ(view, triad_manager->findViewByModel(model));
}

TEST_F(PfTriadManagerTest, should_be_able_to_find_view_by_model_id) { // NOLINT
  // Setup fixture
  TestXXX_MVPair xxx_mvpair;
  std::vector<TestXXX_MVPair> all_xxx_mvpair;
  CUSTOM_ASSERT(createTestXXXTriads(&xxx_mvpair, &all_xxx_mvpair));

  TestYYY_MVPair yyy_mvpair;
  std::vector<TestYYY_MVPair> all_yyy_mvpair;
  CUSTOM_ASSERT(createTestYYYTriads(&yyy_mvpair, &all_yyy_mvpair));

  // Expectations
  std::vector<IPfView*> expect_views;
  for (auto mvpair : all_xxx_mvpair) {
    expect_views.push_back(mvpair.second);
  }

  std::sort(expect_views.begin(), expect_views.end());

  // Exercise system
  auto actual_views =
      triad_manager->findViewsByModelId(TestXXXModel::modelId());

  // Verify results
  std::sort(actual_views.begin(), actual_views.end());
  ASSERT_EQ(expect_views, actual_views);
}

TEST_F(PfTriadManagerTest, should_be_able_to_find_model_by_view) { // NOLINT
  // Setup fixture
  TestXXX_MVPair mvpair;
  CUSTOM_ASSERT(createTestXXXTriads(&mvpair));
  auto model = mvpair.first;
  auto view = mvpair.second;

  // Verify results
  ASSERT_EQ(model, triad_manager->findModelByView(view));
}

}  // namespace tests
}  // namespace pfmvp
