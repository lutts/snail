//-*- TestCaseName: PfTriadManagerTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <cstdlib>
#include <ctime>

#include <tuple>
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
  class Mock##name##Model : public IPfModel {                       \
   public:                                                              \
   virtual ~Mock##name##Model() { destruct(); }                     \
   DEF_MODEL_ID(Mock##name##Model);                                 \
                                                                        \
     MOCK_METHOD0(destruct, void());                                    \
  };                                                                    \
                                                                        \
  class ITest##name##View : public IPfView {                            \
   public:                                                              \
     virtual ~ITest##name##View() = default;                              \
  };                                                                    \
                                                                        \
  class Mock##name##View : public ITest##name##View {                   \
   public:                                                              \
     virtual ~Mock##name##View() { destruct(); }                          \
                                                                        \
     MOCK_METHOD0(destruct, void());                                    \
  };                                                                    \
                                                                        \
  class Mock##name##Presenter : public PfPresenterT<Mock##name##Model, \
                                                    ITest##name##View> { \
   public:                                                              \
     static std::shared_ptr<Mock##name##Presenter>                        \
     create(std::shared_ptr<Mock##name##Model> model,                     \
            std::shared_ptr<ITest##name##View> view) {                  \
       auto presenter = std::make_shared<Mock##name##Presenter>(model, view); \
       return presenter;                                                \
     }                                                                  \
                                                                        \
     Mock##name##Presenter(std::shared_ptr<Mock##name##Model> model, \
                           std::shared_ptr<ITest##name##View> view)     \
     : PfPresenterT<Mock##name##Model, ITest##name##View>(model, view) { \
     }                                                                  \
                                                                        \
     bool initialized_ok { false };                                     \
                                                                        \
   private:                                                             \
     Mock##name##Presenter(const Mock##name##Presenter&) = delete;        \
     Mock##name##Presenter& operator=(const Mock##name##Presenter&) = delete; \
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
  class Mock##name##ViewFactory : public IPfViewFactory {               \
   public:                                                              \
     Mock##name##ViewFactory() = default;                                 \
     virtual ~Mock##name##ViewFactory() = default;                      \
                                                                        \
     DEF_VIEW_FACTORY_ID(Mock##name##ViewFactory)                       \
                                                                        \
     std::shared_ptr<PfPresenter>                                       \
     createView(std::shared_ptr<IPfModel> model) override {             \
       auto my_model = std::dynamic_pointer_cast<Mock##name##Model>(model); \
       if (my_model) {                                                  \
         auto view = createTestView();                                  \
         auto presenter = Mock##name##Presenter::create(my_model, view); \
         last_presenter = presenter.get();                              \
                                                                        \
         return presenter;                                              \
       }                                                                \
                                                                        \
       return nullptr;                                                  \
     }                                                                  \
                                                                        \
     MOCK_CONST_METHOD0(createTestView,                                 \
                        std::shared_ptr<ITest##name##View>());          \
                                                                        \
     Mock##name##Presenter* last_presenter;                             \
                                                                        \
   private:                                                             \
     Mock##name##ViewFactory(const Mock##name##ViewFactory&) = delete;    \
     Mock##name##ViewFactory& operator=(                                \
         const Mock##name##ViewFactory&) = delete;                      \
  };                                                                    \
                                                                        \
  class Mock##name##ViewFactory2 : public IPfViewFactory {              \
   public:                                                              \
     Mock##name##ViewFactory2() = default;                                 \
     virtual ~Mock##name##ViewFactory2() = default;                     \
                                                                        \
     DEF_VIEW_FACTORY_ID(Mock##name##ViewFactory2)                      \
                                                                        \
     std::shared_ptr<PfPresenter>                                       \
     createView(std::shared_ptr<IPfModel> model) override {             \
       auto my_model = std::dynamic_pointer_cast<Mock##name##Model>(model); \
       if (my_model) {                                                  \
         auto view = createTestView();                                  \
         auto presenter = Mock##name##Presenter::create(my_model, view); \
         last_presenter = presenter.get();                              \
                                                                        \
         return presenter;                                              \
       }                                                                \
                                                                        \
       return nullptr;                                                  \
     }                                                                  \
                                                                        \
     MOCK_CONST_METHOD0(createTestView,                                 \
                        std::shared_ptr<ITest##name##View>());          \
                                                                        \
     Mock##name##Presenter* last_presenter;                             \
                                                                        \
   private:                                                             \
     Mock##name##ViewFactory2(const Mock##name##ViewFactory2&) = delete;    \
     Mock##name##ViewFactory2& operator=(                               \
         const Mock##name##ViewFactory2&) = delete;                     \
  };

DEFINE_TEST_CLASSES(XXX)
DEFINE_TEST_CLASSES(YYY)

using TestXXX_MVP_Triad = std::tuple<MockXXXModel*,
                                     MockXXXView*,
                                     MockXXXPresenter*>;

using TestYYY_MVP_Triad = std::tuple<MockYYYModel*,
                                     MockYYYView*,
                                     MockYYYPresenter*>;

static TestXXX_MVP_Triad make_xxx_triad(MockXXXModel* model,
                                        MockXXXView* view,
                                        MockXXXPresenter* presenter) {
  return std::make_tuple(model, view, presenter);
}

#if 0
static TestYYY_MVP_Triad make_yyy_triad(MockYYYModel* model,
                                        MockYYYView* view,
                                        MockYYYPresenter* presenter) {
  return std::make_tuple(model, view, presenter);
}
#endif

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
        triad_manager->findViewsByModelId(MockXXXModel::modelId()).size());
    ASSERT_EQ(
        0,
        triad_manager->findViewsByModelId(MockYYYModel::modelId()).size());

    auto dummy_model = std::make_shared<MockXXXModel>();
    ASSERT_EQ(0, triad_manager->findViewByModel(dummy_model.get()).size());

    MockXXXView dummy_view;
    ASSERT_EQ(nullptr, triad_manager->findModelByView(&dummy_view));

    // after init and no view factory registered, cannot create view from model
    ASSERT_EQ(nullptr, triad_manager->createViewFor(dummy_model));
    ASSERT_EQ(nullptr,
              triad_manager->createViewFor(
                  dummy_model,
                  MockXXXViewFactory::viewFactoryId()));
    ASSERT_EQ(nullptr,
              triad_manager->createViewFor(
                  dummy_model,
                  MockXXXViewFactory2::viewFactoryId()));
  }
  virtual void TearDown() {
    PfViewFactoryManager::resetInstance();
  }

  template <typename VF, typename M, typename V, typename P>
  void createTestTriad(std::shared_ptr<M> model,
                       std::shared_ptr<V> view,
                       P** presenter_ret);

  template <typename VF, typename TriadT>
  void createTestTriads(
      TriadT* mvp_triad_to_test,
      std::vector<TriadT>* all_mvp_triad = nullptr);

  void createTestXXXTriad(std::shared_ptr<MockXXXModel> model,
                          std::shared_ptr<MockXXXView> view,
                          MockXXXPresenter** presenter = nullptr);
  void createTestXXXTriads(
      TestXXX_MVP_Triad* mvp_triad,
      std::vector<TestXXX_MVP_Triad>* all_mvp_triad = nullptr);

  void createTestYYYTriad(std::shared_ptr<MockYYYModel> model,
                          std::shared_ptr<MockYYYView> view,
                          MockYYYPresenter** presenter = nullptr);
  void createTestYYYTriads(
      TestYYY_MVP_Triad* mvp_triad,
      std::vector<TestYYY_MVP_Triad>* all_mvp_triad = nullptr);

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

template <typename VF, typename M, typename V, typename P>
void PfTriadManagerTest::createTestTriad(
    std::shared_ptr<M> model,
    std::shared_ptr<V> view,
    P** presenter_ret) {
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
    auto presenter = view_factory.last_presenter;
    ASSERT_NE(nullptr, presenter);
    ASSERT_EQ(model, presenter->getModel());
    ASSERT_EQ(view, presenter->getView());
    ASSERT_EQ(triad_manager.get(), presenter->triad_manager());
    ASSERT_TRUE(presenter->initialized_ok);

    if (presenter_ret)
      *presenter_ret = presenter;
  }

  ASSERT_EQ(old_model_use_count + 1, model.use_count());
  ASSERT_EQ(old_view_use_count + 1, view.use_count());
}

void PfTriadManagerTest::createTestXXXTriad(
    std::shared_ptr<MockXXXModel> model,
    std::shared_ptr<MockXXXView> view,
    MockXXXPresenter** presenter) {
  createTestTriad<MockXXXViewFactory>(model, view, presenter);
}

void PfTriadManagerTest::createTestYYYTriad(
    std::shared_ptr<MockYYYModel> model,
    std::shared_ptr<MockYYYView> view,
    MockYYYPresenter** presenter) {
  createTestTriad<MockYYYViewFactory>(model, view, presenter);
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
       should_be_able_to_create_view_for_model_with_specified_view_factory_id) { // NOLINT
  // Setup fixture
  auto model = std::make_shared<MockXXXModel>();
  auto view1 = std::make_shared<MockXXXView>();
  auto view2 = std::make_shared<MockXXXView>();

  view_factory_t<MockXXXModel,
                 MockXXXViewFactory> view_factory_wrapper1;
  view_factory_t<MockXXXModel,
                 MockXXXViewFactory2> view_factory_wrapper2;

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
  view_factory_t<MockXXXModel,
                 MockPfViewFactory> view_factory_wrapper;
  auto& factory = view_factory_wrapper.FTO_getFactory();

  std::shared_ptr<IPfModel> model = std::make_shared<MockXXXModel>();
  ON_CALL(factory, createView(model))
      .WillByDefault(Return(nullptr));

  // Exercise system
  auto view = triad_manager->createViewFor(model);

  // Verify results
  ASSERT_EQ(nullptr, view);
}

template <typename VF, typename TriadT>
void PfTriadManagerTest::createTestTriads(
    TriadT* mvp_triad_to_test,
    std::vector<TriadT>* all_mvp_triad) {
  using MT = typename std::remove_pointer<
    typename std::tuple_element<0, TriadT>::type>::type;
  using VT = typename std::remove_pointer<
    typename std::tuple_element<1, TriadT>::type>::type;
  using PT = typename std::remove_pointer<
    typename std::tuple_element<2, TriadT>::type>::type;

  std::vector<TriadT> all;

  const int TEST_TRIAD_COUNT = 10;

  std::srand(std::time(0));
  int triad_to_test = std::rand() % TEST_TRIAD_COUNT;

  PT* presenter = nullptr;
  for (int i = 0; i < TEST_TRIAD_COUNT; ++i) {
    auto model = std::make_shared<MT>();
    auto view = std::make_shared<VT>();

    createTestTriad<VF>(model, view, &presenter);

    all.push_back(std::make_tuple(model.get(), view.get(), presenter));
  }

  *mvp_triad_to_test = all[triad_to_test];
  if (all_mvp_triad)
    *all_mvp_triad = all;
}

void PfTriadManagerTest::createTestXXXTriads(
    TestXXX_MVP_Triad* mvp_triad_to_test,
    std::vector<TestXXX_MVP_Triad>* all_mvp_triad) {
  createTestTriads<MockXXXViewFactory>(mvp_triad_to_test, all_mvp_triad);
}

void PfTriadManagerTest::createTestYYYTriads(
    TestYYY_MVP_Triad* mvp_triad_to_test,
    std::vector<TestYYY_MVP_Triad>* all_mvp_triad) {
  createTestTriads<MockYYYViewFactory>(mvp_triad_to_test, all_mvp_triad);
}

// TODO(lutts): should enforce destruction order
TEST_F(PfTriadManagerTest, should_destroy_triads_when_triad_manager_destroyed) { // NOLINT
  // Setup fixture
  TestXXX_MVP_Triad mvp_triad;
  std::vector<TestXXX_MVP_Triad> all_mvp_triad;
  CUSTOM_ASSERT(createTestXXXTriads(&mvp_triad, &all_mvp_triad));

  // Expectations
  for (auto mvp : all_mvp_triad) {
    EXPECT_CALL(*std::get<0>(mvp), destruct());
    EXPECT_CALL(*std::get<1>(mvp), destruct());
  }

  // Exercise system
  triad_manager.reset();
}

using CheckPointType = MockFunction<void(string check_point_name)>;

static void expectationsOnSingleTriadDestroy(
    TestXXX_MVP_Triad triad, MockListener* listener,
    CheckPointType* checker = nullptr,
    Sequence* seq = nullptr,
    int times = 1) {
  auto model = std::get<0>(triad);
  auto view = std::get<1>(triad);
  //  auto presenter = std::get<2>(triad);

  if (seq == nullptr) {
    seq = new Sequence();
  }

  if (listener) {
    if (view) {
      EXPECT_CALL(*listener, AboutToDestroyView(view))
          .Times(times)
          .InSequence(*seq);
    }

    if (model) {
      EXPECT_CALL(*listener, AboutToDestroyModel(model))
          .Times(times)
          .InSequence(*seq);
    }
  }

  if (view) {
    EXPECT_CALL(*view, destruct())
        .Times(times)
        .InSequence(*seq);
  }

  if (model) {
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

static void expectationsOnTriadDestroy(
    TestXXX_MVP_Triad triad1, MockListener* listener1,
    TestXXX_MVP_Triad triad2, MockListener* listener2,
    CheckPointType* checker = nullptr,
    Sequence* seq = nullptr,
    int times = 1) {
  auto model1 = std::get<0>(triad1);
  auto view1 = std::get<1>(triad1);
  //  auto presenter1 = std::get<2>(triad1);

  auto model2 = std::get<0>(triad2);
  auto view2 = std::get<1>(triad2);
  //  auto presenter2 = std::get<2>(triad2);

  if (!seq) {
    seq = new Sequence();  // TODO(lutts): memory leak
  }

  if (model1 != model2) {
    expectationsOnSingleTriadDestroy(triad1, listener1,
                                        nullptr /* checker */,
                                        seq,
                                        times);
    expectationsOnSingleTriadDestroy(triad2, listener2,
                                        nullptr /* checker */,
                                        seq,
                                        times);
  } else {
    auto model = model1;

    // NOTE: view1 and view2's destroy order may be not important, but
    // we use FILO policy here, please see comments on createViewFor() in
    // PfTriadManager.cc
    if (view2) {
      if (listener2) {
        EXPECT_CALL(*listener2, AboutToDestroyView(view2))
            .Times(times)
            .InSequence(*seq);
      }

      EXPECT_CALL(*view2, destruct())
          .Times(times)
          .InSequence(*seq);
    }

    if (view1 && listener1) {
      EXPECT_CALL(*listener1, AboutToDestroyView(view1))
          .Times(times)
          .InSequence(*seq);
    }

    // NOTE: listener order is the bind order
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

    if (view1) {
      EXPECT_CALL(*view1, destruct())
          .Times(times)
          .InSequence(*seq);
    }

    if (model) {
      EXPECT_CALL(*model, destruct())
          .Times(times)
          .InSequence(*seq);
    }
  }

  if (checker) {
    EXPECT_CALL(*checker, Call("barrier"))
        .Times(times)
        .InSequence(*seq);
  }
}

static void expectationsOnModelView1View2Destroy(
    TestXXX_MVP_Triad triad1, MockListener* listener1,
    TestXXX_MVP_Triad triad2, MockListener* listener2,
    CheckPointType* checker = nullptr) {
  expectationsOnTriadDestroy(
      triad1, listener1,
      triad2, listener2,
      checker);
}

static void expectationsOnNotDestroyModelView(
    TestXXX_MVP_Triad triad, MockListener* listener) {
  expectationsOnSingleTriadDestroy(
      triad, listener,
      nullptr /* checker */,
      nullptr /* seq */,
      0 /* times */);
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

  bool expect_result = false;

  // Expectations
  auto mockListener = MockListener::attachStrictTo(triad_manager.get(), model, view);

  EXPECT_CALL(*mockListener, RequestRemoveModel(model))
      .WillOnce(Return(expect_result));

  expectationsOnNotDestroyModelView(mvp_triad,
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

  expectationsOnNotDestroyModelView(
      make_xxx_triad(model, view2, presenter2),
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
  expectationsOnModelView1View2Destroy(
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

  // create some "background" triads
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

TEST_F(PfTriadManagerTest, should_be_able_to_find_view_by_model_id) { // NOLINT
  // Setup fixture
  TestXXX_MVP_Triad xxx_mvp_triad;
  std::vector<TestXXX_MVP_Triad> all_xxx_mvp_triad;
  CUSTOM_ASSERT(createTestXXXTriads(&xxx_mvp_triad, &all_xxx_mvp_triad));

  TestYYY_MVP_Triad yyy_mvp_triad;
  std::vector<TestYYY_MVP_Triad> all_yyy_mvp_triad;
  CUSTOM_ASSERT(createTestYYYTriads(&yyy_mvp_triad, &all_yyy_mvp_triad));

  // Expectations
  std::vector<IPfView*> expect_views;
  for (auto mvp_triad : all_xxx_mvp_triad) {
    expect_views.push_back(get<1>(mvp_triad));
  }

  std::sort(expect_views.begin(), expect_views.end());

  // Exercise system
  auto actual_views =
      triad_manager->findViewsByModelId(MockXXXModel::modelId());

  // Verify results
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

}  // namespace tests
}  // namespace pfmvp
