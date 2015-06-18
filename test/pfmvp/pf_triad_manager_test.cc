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
#include "pfmvp/pf_view_factory_manager.h"

namespace pfmvp {
namespace tests {

class MockPfViewFactory : public IPfViewFactory {
 public:
  DEF_VIEW_FACTORY_ID(MockPfViewFactory)

  MOCK_METHOD1(createView,
               std::shared_ptr<PfPresenter>(std::shared_ptr<IPfModel> model));
};

#define DEFINE_TEST_CLASSES(name)                                       \
  class Test##name##Model : public IPfModel {                           \
   public:                                                              \
     virtual ~Test##name##Model() = default;                              \
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
            std::shared_ptr<ITest##name##View> view) {                  \
       auto presenter = std::make_shared<Test##name##Presenter>(model, view); \
       return presenter;                                                \
     }                                                                  \
                                                                        \
     Test##name##Presenter(std::shared_ptr<Test##name##Model> model,    \
                           std::shared_ptr<ITest##name##View> view)     \
     : PfPresenterT<Test##name##Model, ITest##name##View>(model, view) { \
     }                                                                  \
                                                                        \
     bool initialized_ok { false };                                        \
                                                                        \
   private:                                                             \
     Test##name##Presenter(const Test##name##Presenter&) = delete;        \
     Test##name##Presenter& operator=(const Test##name##Presenter&) = delete; \
                                                                        \
     void initialize() override {                                       \
       if (triad_manager()) {                                           \
         /* check ourself is in triad manager when initialize() called */ \
         auto model_exist = findModelByView(view());                    \
         if (model_exist)                                               \
           initialized_ok = true;                                       \
       }                                                                \
     }                                                                  \
  };                                                                    \
                                                                        \
  class Test##name##ViewFactory : public IPfViewFactory {               \
   public:                                                              \
     Test##name##ViewFactory() = default;                                 \
     virtual ~Test##name##ViewFactory() = default;                      \
                                                                        \
     DEF_VIEW_FACTORY_ID(Test##name##ViewFactory)                       \
                                                                        \
     std::shared_ptr<PfPresenter>                                       \
     createView(std::shared_ptr<IPfModel> model) override {             \
       auto my_model = std::dynamic_pointer_cast<Test##name##Model>(model); \
       if (my_model) {                                                  \
         auto view = createTestView();                                  \
         auto presenter = Test##name##Presenter::create(my_model, view); \
         last_presenter = presenter.get();                              \
                                                                        \
         return presenter;                                              \
       }                                                                \
                                                                        \
       return nullptr;                                                  \
     }                                                                  \
                                                                        \
     virtual std::shared_ptr<ITest##name##View> createTestView() const { \
       return std::make_shared<Test##name##View>();                     \
     }                                                                  \
                                                                        \
     Test##name##Presenter* last_presenter;                             \
                                                                        \
   private:                                                             \
     Test##name##ViewFactory(const Test##name##ViewFactory&) = delete;    \
     Test##name##ViewFactory& operator=(                                \
         const Test##name##ViewFactory&) = delete;                      \
  };                                                                    \
                                                                        \
  class MockTest##name##ViewFactory : public Test##name##ViewFactory {  \
   public:                                                              \
     MOCK_CONST_METHOD0(createTestView,                                   \
                        std::shared_ptr<ITest##name##View>());          \
  };                                                                    \
                                                                        \
  class Test##name##ViewFactory2 : public IPfViewFactory {              \
   public:                                                              \
     Test##name##ViewFactory2() = default;                                 \
     virtual ~Test##name##ViewFactory2() = default;                     \
                                                                        \
     DEF_VIEW_FACTORY_ID(Test##name##ViewFactory2)                      \
                                                                        \
     std::shared_ptr<PfPresenter>                                       \
     createView(std::shared_ptr<IPfModel> model) override {             \
       auto my_model = std::dynamic_pointer_cast<Test##name##Model>(model); \
       if (my_model) {                                                  \
         auto view = createTestView();                                  \
         auto presenter = Test##name##Presenter::create(my_model, view); \
         last_presenter = presenter.get();                              \
                                                                        \
         return presenter;                                              \
       }                                                                \
                                                                        \
       return nullptr;                                                  \
     }                                                                  \
                                                                        \
     virtual std::shared_ptr<ITest##name##View> createTestView() const { \
       return std::make_shared<Test##name##View>();                     \
     }                                                                  \
                                                                        \
     Test##name##Presenter* last_presenter;                             \
                                                                        \
   private:                                                             \
     Test##name##ViewFactory2(const Test##name##ViewFactory2&) = delete;    \
     Test##name##ViewFactory2& operator=(                               \
         const Test##name##ViewFactory2&) = delete;                     \
  };                                                                    \
                                                                        \
  class MockTest##name##ViewFactory2 : public Test##name##ViewFactory2 { \
   public:                                                              \
     MOCK_CONST_METHOD0(createTestView,                                   \
                        std::shared_ptr<ITest##name##View>());          \
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
        PfViewFactoryManager::getInstance());

    ASSERT_EQ(
        0,
        triad_manager->findViewsByModelId(TestXXXModel::modelId()).size());
    ASSERT_EQ(
        0,
        triad_manager->findViewsByModelId(TestYYYModel::modelId()).size());

    auto dummy_model = std::make_shared<TestXXXModel>();
    ASSERT_EQ(0, triad_manager->findViewByModel(dummy_model.get()).size());

    TestXXXView dummy_view;
    ASSERT_EQ(nullptr, triad_manager->findModelByView(&dummy_view));

    // after init and no view factory registered, cannot create view from model
    ASSERT_EQ(nullptr, triad_manager->createViewFor(dummy_model));
    ASSERT_EQ(nullptr,
              triad_manager->createViewFor(
                  dummy_model,
                  TestXXXViewFactory::viewFactoryId()));
    ASSERT_EQ(nullptr,
              triad_manager->createViewFor(
                  dummy_model,
                  TestXXXViewFactory2::viewFactoryId()));
  }
  virtual void TearDown() {
    PfViewFactoryManager::resetInstance();
  }

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

class MockListener : public GenericMockListener<MockListener,
                                                IPfTriadManager> {
 public:
  MockListener(IPfModel* model, IPfView* view,
               bool monitor_request_remove = true)
      : model_(model)
      , view_(view)
      , monitor_request_remove_(monitor_request_remove) { }

  MOCK_METHOD1(RequestRemoveModel, bool(IPfModel* model));
  MOCK_METHOD1(AboutToDestroyModel, void(IPfModel* model));
  MOCK_METHOD1(AboutToDestroyView, void(IPfView* view));

  void bindListenerMethods(std::shared_ptr<utils::ITrackable> trackObject,
                           IPfTriadManager* triad_manager) {
    if (model_) {
      if (monitor_request_remove_) {
        requestRemoveModelBinded =
            triad_manager->whenRequestRemoveModel(
                model_,
                [this](IPfModel* model) -> bool {
                  return RequestRemoveModel(model);
                },
                trackObject);
      }

      aboutToDestroyModelBinded =
          triad_manager->whenAboutToDestroyModel(
              model_,
              [this](IPfModel* model) {
                AboutToDestroyModel(model);
              },
              trackObject);
    }

    if (view_) {
      aboutToDestroyViewBinded =
          triad_manager->whenAboutToDestroyView(
              view_,
              [this](IPfView* view) {
                AboutToDestroyView(view);
              },
              trackObject);
    }
  }

 public:
  bool requestRemoveModelBinded { false };
  bool aboutToDestroyModelBinded { false };
  bool aboutToDestroyViewBinded { false };

 private:
  IPfModel* model_ { nullptr };
  IPfView* view_ { nullptr };
  bool monitor_request_remove_;
};

template <typename VF, typename M, typename V>
void PfTriadManagerTest::createTestTriad(
    std::shared_ptr<M> model,
    std::shared_ptr<V> view) {
  auto old_model_use_count = model.use_count();
  auto old_view_use_count = view.use_count();

  {  // working scope
    view_factory_t<M, VF> view_factory_wrapper;
    auto& view_factory = view_factory_wrapper.FTO_getFactory();

    ON_CALL(view_factory, createTestView())
        .WillByDefault(Return(view));

    auto actual_view = triad_manager->createViewFor(model);
    ASSERT_EQ(view, actual_view);

    // for convenience, we will store the triad manager in presenter
    ASSERT_EQ(triad_manager.get(),
              view_factory.last_presenter->triad_manager());
    ASSERT_TRUE(view_factory.last_presenter->initialized_ok);
  }

  ASSERT_EQ(old_model_use_count + 1, model.use_count());
  ASSERT_EQ(old_view_use_count + 1, view.use_count());
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

  auto yyy_model = std::make_shared<MockTestYYYModel>();
  auto yyy_view = std::make_shared<MockTestYYYView>();

  // Exercise system
  CUSTOM_ASSERT(createTestXXXTriad(xxx_model, xxx_view));
  CUSTOM_ASSERT(createTestYYYTriad(yyy_model, yyy_view));
}

// TODO(lutts): checking not complete!!! see createTestTriad()
TEST_F(PfTriadManagerTest,
       should_be_able_to_create_view_for_model_with_specified_view_factory_id) { // NOLINT
  // Setup fixture
  auto model = std::make_shared<MockTestXXXModel>();
  auto view1 = std::make_shared<MockTestXXXView>();
  auto view2 = std::make_shared<MockTestXXXView>();

  view_factory_t<TestXXXModel,
                 MockTestXXXViewFactory> view_factory_wrapper1;
  view_factory_t<TestXXXModel,
                 MockTestXXXViewFactory2> view_factory_wrapper2;

  auto& factory1 = view_factory_wrapper1.FTO_getFactory();
  auto& factory2 = view_factory_wrapper2.FTO_getFactory();

  ON_CALL(factory1, createTestView())
      .WillByDefault(Return(view1));
  ON_CALL(factory2, createTestView())
      .WillByDefault(Return(view2));

  // Exercise system
  auto actual_view1 =
      triad_manager->createViewFor(model,
                                   factory1.getViewFactoryId());
  auto actual_view2 =
      triad_manager->createViewFor(model,
                                   factory2.getViewFactoryId());

  // Verify results
  // TODO(lutts): refractor, code duplicated with createTestTriad()
  ASSERT_EQ(view1, actual_view1);
  ASSERT_EQ(triad_manager.get(),
            factory1.last_presenter->triad_manager());
  ASSERT_TRUE(factory1.last_presenter->initialized_ok);

  ASSERT_EQ(view2, actual_view2);
  ASSERT_EQ(triad_manager.get(),
            factory2.last_presenter->triad_manager());
  ASSERT_TRUE(factory2.last_presenter->initialized_ok);
}

TEST_F(PfTriadManagerTest, should_return_null_when_view_factory_failed_create_view) { // NOLINT
  // Setup fixture
  view_factory_t<TestXXXModel,
                 MockPfViewFactory> view_factory_wrapper;
  auto& factory = view_factory_wrapper.FTO_getFactory();

  std::shared_ptr<IPfModel> model = std::make_shared<TestXXXModel>();
  ON_CALL(factory, createView(model))
      .WillByDefault(Return(nullptr));

  // Exercise system
  auto view = triad_manager->createViewFor(model);

  // Verify results
  ASSERT_EQ(nullptr, view);
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

// TODO(lutts)
TEST_F(PfTriadManagerTest, should_destroy_triads_when_triad_manager_destroyed) { // NOLINT
  // Setup fixture
  TestXXX_MVPair mvpair;
  std::vector<TestXXX_MVPair> all_mvpair;
  CUSTOM_ASSERT(createTestXXXTriads(&mvpair, &all_mvpair));

  // Expectations
  for (auto mv : all_mvpair) {
    EXPECT_CALL(*mv.first, destruct());
    EXPECT_CALL(*mv.second, destruct());
  }

  // Exercise system
  triad_manager.reset();
}

using CheckPointType = MockFunction<void(string check_point_name)>;

static void expectationsOnModelViewDestroy(
    MockTestXXXModel* model,
    MockTestXXXView* view1,
    MockListener* listener1,
    CheckPointType* checker = nullptr,
    Sequence* seq = nullptr,
    int times = 1,
    MockTestXXXView* view2 = nullptr,
    MockListener* listener2 = nullptr) {
  if (!seq) {
    seq = new Sequence();
  }

  if (view2) {
    if (listener2) {
      EXPECT_CALL(*listener2, AboutToDestroyView(view2))
          .InSequence(*seq);
    }

    EXPECT_CALL(*view2, destruct())
        .InSequence(*seq);
  }

  if (view1) {
    if (listener1) {
      EXPECT_CALL(*listener1, AboutToDestroyView(view1))
          .Times(times)
          .InSequence(*seq);
    }
  }

  if (model) {
    if (listener1) {
      EXPECT_CALL(*listener1, AboutToDestroyModel(model))
          .Times(times)
          .InSequence(*seq);
    }

    if (listener2) {
      EXPECT_CALL(*listener2, AboutToDestroyModel(model))
          .Times(times)
          .InSequence(*seq);
    }
  }

  if (view1) {
    EXPECT_CALL(*view1, destruct())
        .Times(times)
        .InSequence(*seq);
  }

  if (model){
    EXPECT_CALL(*model, destruct())
        .Times(times)
        .InSequence(*seq);
  }

  if (checker) {
    EXPECT_CALL(*checker, Call("barrier"))
        .Times(times)
        .InSequence(*seq);
  }
}

static void expectationsOnModelView1View2Destroy(
    MockTestXXXModel* model,
    MockTestXXXView* view1,
    MockListener* listener1,
    MockTestXXXView* view2,
    MockListener* listener2,
    CheckPointType* checker = nullptr) {
  expectationsOnModelViewDestroy(model,
                                 view1,
                                 listener1,
                                 checker,
                                 nullptr /* seq */,
                                 1 /* times */,
                                 view2,
                                 listener2);
}

static void expectationsOnNotDestroyModelView(
    MockTestXXXModel* model,
    MockTestXXXView* view,
    MockListener* listener) {
  expectationsOnModelViewDestroy(model,
                                 view,
                                 listener,
                                 nullptr /* checker */,
                                 nullptr /* seq */,
                                 0 /* times */);
}

TEST_F(PfTriadManagerTest,
       should_be_able_to_destroy_triad_by_model_and_fire_AboutToDestory_signals) { // NOLINT
  // Setup fixture
  TestXXX_MVPair mvpair;
  CUSTOM_ASSERT(createTestXXXTriads(&mvpair));
  auto model = mvpair.first;
  auto view = mvpair.second;

  // Expectations
  auto mockListener = MockListener::attachTo(triad_manager.get(), model, view);
  CheckPointType check;

  expectationsOnModelViewDestroy(model, view,
                                 mockListener.get(),
                                 &check);

  // Exercise system
  triad_manager->removeTriadBy(model);
  check.Call("barrier");
}

TEST_F(PfTriadManagerTest,
       should_be_able_to_destroy_triad_by_view_and_fire_AboutToDestory_signals) { // NOLINT
  // Setup fixture
  TestXXX_MVPair mvpair;
  CUSTOM_ASSERT(createTestXXXTriads(&mvpair));
  auto model = mvpair.first;
  auto view = mvpair.second;

  // Expectations
  auto mockListener = MockListener::attachTo(triad_manager.get(), model, view);
  CheckPointType check;

  expectationsOnModelViewDestroy(model,
                                 view,
                                 mockListener.get(),
                                 &check);

  // Exercise system
  triad_manager->removeTriadBy(view);
  check.Call("barrier");
}

TEST_F(PfTriadManagerTest, should_be_able_to_monitor_managed_model_and_view) { // NOLINT
  // Setup fixture
  auto model = std::make_shared<MockTestXXXModel>();
  auto view = std::make_shared<MockTestXXXView>();

  createTestXXXTriad(model, view);

  // Exercise system
  auto mockListener = MockListener::attachTo(triad_manager.get(),
                                             model.get(), view.get());

  // Verify results
  ASSERT_TRUE(mockListener->requestRemoveModelBinded);
  ASSERT_TRUE(mockListener->aboutToDestroyModelBinded);
  ASSERT_TRUE(mockListener->aboutToDestroyViewBinded);
}

TEST_F(PfTriadManagerTest, should_not_monitor_not_managed_model_and_view) { // NOLINT
  // Setup fixture
  auto model = std::make_shared<TestXXXModel>();
  auto view = std::make_shared<TestXXXView>();

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
  TestXXX_MVPair mvpair;
  CUSTOM_ASSERT(createTestXXXTriads(&mvpair));
  auto model = mvpair.first;
  auto view = mvpair.second;

  // Expectations
  #define dont_monitor_request_remove false
  auto mockListener = MockListener::attachTo(triad_manager.get(), model, view,
                                             dont_monitor_request_remove);
  CheckPointType check;
  expectationsOnModelViewDestroy(model,
                                 view,
                                 mockListener.get(),
                                 &check);

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
  CheckPointType check;
  {
    Sequence s1;

    EXPECT_CALL(*mockListener, RequestRemoveModel(model))
        .InSequence(s1)
        .WillOnce(Return(expect_result));

    expectationsOnModelViewDestroy(model,
                                   view,
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
  TestXXX_MVPair mvpair;
  CUSTOM_ASSERT(createTestXXXTriads(&mvpair));
  auto model = mvpair.first;
  auto view = mvpair.second;

  bool expect_result = false;

  // Expectations
  auto mockListener = MockListener::attachStrictTo(triad_manager.get(), model, view);

  EXPECT_CALL(*mockListener, RequestRemoveModel(model))
      .WillOnce(Return(expect_result));

  expectationsOnNotDestroyModelView(model,
                                    view,
                                    mockListener.get());

  // Exercise system
  auto actual_result = triad_manager->requestRemoveTriadByView(view);

  // Verify results
  ASSERT_EQ(expect_result, actual_result);

  ::Mock::VerifyAndClear(model);
  ::Mock::VerifyAndClear(view);
}

TEST_F(PfTriadManagerTest, should_remove_model_only_when_all_views_are_removed) { // NOLINT
  // Setup fixture
  MockTestXXXModel* model = nullptr;
  MockTestXXXView* view1 = nullptr;
  MockTestXXXView* view2 = nullptr;

  {
    auto m = std::make_shared<MockTestXXXModel>();
    auto v1 = std::make_shared<MockTestXXXView>();
    auto v2 = std::make_shared<MockTestXXXView>();

    createTestXXXTriad(m, v1);
    createTestXXXTriad(m, v2);

    model = m.get();
    view1 = v1.get();
    view2 = v2.get();
  }


  auto mockListener1 = MockListener::attachStrictTo(triad_manager.get(),
                                              model, view1);
  auto mockListener2 = MockListener::attachStrictTo(triad_manager.get(),
                                              model, view2);

  // Expectations on remove view1
  expectationsOnModelViewDestroy(nullptr /* model */,
                                 view1,
                                 mockListener1.get());

  expectationsOnNotDestroyModelView(model,
                                    view2,
                                    mockListener2.get());

  // Exercise system: remove view1
  triad_manager->removeTriadBy(view1);

  // Verify result
  ::Mock::VerifyAndClear(model);
  ::Mock::VerifyAndClear(view1);
  ::Mock::VerifyAndClear(view2);
  ::Mock::VerifyAndClear(mockListener1.get());
  ::Mock::VerifyAndClear(mockListener2.get());

  CheckPointType check;
  // Expectations on remove view2
  expectationsOnModelViewDestroy(model,
                                 view2,
                                 mockListener2.get(),
                                 &check);

  EXPECT_CALL(*mockListener1, AboutToDestroyModel(model));

  // Excercise system:: remove view1
  triad_manager->removeTriadBy(view2);
  check.Call("barrier");
}

TEST_F(PfTriadManagerTest, should_remove_all_triads_of_a_model_when_the_model_removed) { // NOLINT
  // Setup fixture
  MockTestXXXModel* model = nullptr;
  MockTestXXXView* view1 = nullptr;
  MockTestXXXView* view2 = nullptr;

  {
    auto m = std::make_shared<MockTestXXXModel>();
    auto v1 = std::make_shared<MockTestXXXView>();
    auto v2 = std::make_shared<MockTestXXXView>();

    createTestXXXTriad(m, v1);
    createTestXXXTriad(m, v2);

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
  expectationsOnModelView1View2Destroy(model,
                                       view1,
                                       mockListener1.get(),
                                       view2,
                                       mockListener2.get(),
                                       &check);

  // Exercise system
  triad_manager->removeTriadBy(model);
  check.Call("barrier");
}

TEST_F(PfTriadManagerTest, should_remove_subscriber_only_when_subject_removed) { // NOLINT
  // Setup fixture
  MockTestXXXModel* model = nullptr;
  MockTestXXXView* view1 = nullptr;
  MockTestXXXView* view2 = nullptr;

  auto m = std::make_shared<MockTestXXXModel>();
  auto v1 = std::make_shared<MockTestXXXView>();
  auto v2 = std::make_shared<MockTestXXXView>();

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
  auto model = std::make_shared<MockTestXXXModel>();
  auto view1 = std::make_shared<MockTestXXXView>();
  auto view2 = std::make_shared<MockTestXXXView>();

  createTestXXXTriad(model, view1);
  createTestXXXTriad(model, view2);

  // create some "background" triads
  TestXXX_MVPair mvpair;
  CUSTOM_ASSERT(createTestXXXTriads(&mvpair));

  std::vector<IPfView*> expect_views;
  expect_views.push_back(view1.get());
  expect_views.push_back(view2.get());

  std::sort(expect_views.begin(), expect_views.end());

  // Verify results
  auto actual_views = triad_manager->findViewByModel(model.get());
  std::sort(actual_views.begin(), actual_views.end());

  ASSERT_EQ(expect_views, actual_views);
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
