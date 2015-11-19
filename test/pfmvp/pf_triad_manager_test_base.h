// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef TEST_PFMVP_PF_TRIAD_MANAGER_TEST_BASE_H_
#define TEST_PFMVP_PF_TRIAD_MANAGER_TEST_BASE_H_

#include <cstdlib>
#include <ctime>
#include <algorithm>  // std::sort
#include <tuple>
#include <vector>

#include "test/testutils/gmock_common.h"

#include "utils/basic_utils.h"  // make_unique, <memory>
#include "test/testutils/utils.h"
#include "test/testutils/shared_mock_listener.h"
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

  MOCK_METHOD2(createView,
               std::shared_ptr<PfPresenter>(std::shared_ptr<IPfModel> model,
                                            PfCreateViewArgs* args));
};

#define DEFINE_TEST_CLASSES(name)                                              \
  class Mock##name##Model : public IPfModel {                                  \
   public:                                                                     \
    virtual ~Mock##name##Model() { destruct(); }                               \
    DEF_MODEL_ID(Mock##name##Model);                                           \
                                                                               \
    MOCK_METHOD0(onDestroy, void());                                           \
    MOCK_METHOD0(destruct, void());                                            \
                                                                               \
    void set_triad_manager(IPfTriadManager* triad_manager) override {          \
      triad_manager_ = triad_manager;                                          \
    }                                                                          \
                                                                               \
    IPfTriadManager* triad_manager() { return triad_manager_; }                \
                                                                               \
   private:                                                                    \
    IPfTriadManager* triad_manager_{nullptr};                                  \
  };                                                                           \
                                                                               \
  class ITest##name##View : public IPfView {                                   \
   public:                                                                     \
    virtual ~ITest##name##View() = default;                                    \
  };                                                                           \
                                                                               \
  class Mock##name##View : public ITest##name##View {                          \
   public:                                                                     \
    virtual ~Mock##name##View() { destruct(); }                                \
                                                                               \
    MOCK_METHOD0(onDestroy, void());                                           \
    MOCK_METHOD0(destruct, void());                                            \
  };                                                                           \
                                                                               \
  class Mock##name##Presenter                                                  \
      : public PfPresenterT<Mock##name##Model, ITest##name##View> {            \
   public:                                                                     \
    static std::shared_ptr<Mock##name##Presenter> create(                      \
        std::shared_ptr<model_type> model, std::shared_ptr<view_type> view) {  \
      auto presenter = std::make_shared<Mock##name##Presenter>(model, view);   \
      return presenter;                                                        \
    }                                                                          \
                                                                               \
    Mock##name##Presenter(std::shared_ptr<model_type> model,                   \
                          std::shared_ptr<view_type> view)                     \
        : PfPresenterT<Mock##name##Model, ITest##name##View>(model, view) {}   \
                                                                               \
    ~Mock##name##Presenter() { destruct(); }                                   \
                                                                               \
    void destroySelfTriad() { triad_manager()->removeTriadBy(model()); }       \
                                                                               \
    MOCK_METHOD0(onDestroy, void());                                           \
    MOCK_METHOD0(destruct, void());                                            \
                                                                               \
    bool initialized_ok{false};                                                \
                                                                               \
   private:                                                                    \
    Mock##name##Presenter(const Mock##name##Presenter&) = delete;              \
    Mock##name##Presenter& operator=(const Mock##name##Presenter&) = delete;   \
                                                                               \
    void initialize() override {                                               \
      if (triad_manager()) {                                                   \
        /* check ourself is in triad manager when initialize() called */       \
        auto model_exist = findModelByView(view());                            \
        if (model_exist) initialized_ok = true;                                \
      }                                                                        \
    }                                                                          \
  };                                                                           \
                                                                               \
  class Mock##name##ViewFactory : public IPfViewFactory {                      \
   public:                                                                     \
    Mock##name##ViewFactory() = default;                                       \
    virtual ~Mock##name##ViewFactory() = default;                              \
                                                                               \
    DEF_VIEW_FACTORY_ID(Mock##name##ViewFactory)                               \
                                                                               \
    std::shared_ptr<PfPresenter> createView(std::shared_ptr<IPfModel> model,   \
                                            PfCreateViewArgs* args) override { \
      V_UNUSED(args);                                                          \
      auto my_model = std::dynamic_pointer_cast<Mock##name##Model>(model);     \
      if (my_model) {                                                          \
        auto view = createTestView();                                          \
        auto presenter = Mock##name##Presenter::create(my_model, view);        \
        last_presenter = presenter.get();                                      \
                                                                               \
        return presenter;                                                      \
      }                                                                        \
                                                                               \
      return nullptr;                                                          \
    }                                                                          \
                                                                               \
    MOCK_CONST_METHOD0(createTestView, std::shared_ptr<ITest##name##View>());  \
                                                                               \
    Mock##name##Presenter* last_presenter;                                     \
                                                                               \
   private:                                                                    \
    Mock##name##ViewFactory(const Mock##name##ViewFactory&) = delete;          \
    Mock##name##ViewFactory& operator=(const Mock##name##ViewFactory&) =       \
        delete;                                                                \
  };                                                                           \
                                                                               \
  class Mock##name##ViewFactory2 : public IPfViewFactory {                     \
   public:                                                                     \
    Mock##name##ViewFactory2() = default;                                      \
    virtual ~Mock##name##ViewFactory2() = default;                             \
                                                                               \
    DEF_VIEW_FACTORY_ID(Mock##name##ViewFactory2)                              \
                                                                               \
    std::shared_ptr<PfPresenter> createView(std::shared_ptr<IPfModel> model,   \
                                            PfCreateViewArgs* args) override { \
      V_UNUSED(args);                                                          \
      auto my_model = std::dynamic_pointer_cast<Mock##name##Model>(model);     \
      if (my_model) {                                                          \
        auto view = createTestView();                                          \
        auto presenter = Mock##name##Presenter::create(my_model, view);        \
        last_presenter = presenter.get();                                      \
                                                                               \
        return presenter;                                                      \
      }                                                                        \
                                                                               \
      return nullptr;                                                          \
    }                                                                          \
                                                                               \
    MOCK_CONST_METHOD0(createTestView, std::shared_ptr<ITest##name##View>());  \
                                                                               \
    Mock##name##Presenter* last_presenter;                                     \
                                                                               \
   private:                                                                    \
    Mock##name##ViewFactory2(const Mock##name##ViewFactory2&) = delete;        \
    Mock##name##ViewFactory2& operator=(const Mock##name##ViewFactory2&) =     \
        delete;                                                                \
  };

DEFINE_TEST_CLASSES(XXX)
DEFINE_TEST_CLASSES(YYY)

class MockListener : public SharedMockListener<MockListener, IPfTriadManager> {
 public:
  MockListener(IPfModel* model, IPfView* view,
               bool monitor_request_remove = true)
      : model_(model),
        view_(view),
        monitor_request_remove_(monitor_request_remove) {}

  MOCK_METHOD1(RequestRemoveModel, bool(IPfModel* model));
  MOCK_METHOD1(AboutToDestroyModel, void(IPfModel* model));
  MOCK_METHOD1(AboutToDestroyView, void(IPfView* view));

  void bindListenerMethods(std::shared_ptr<utils::ITrackable> trackObject,
                           IPfTriadManager* triad_manager) {
    if (model_) {
      if (monitor_request_remove_) {
        auto conn = triad_manager->whenRequestRemoveModel(
            model_, [this](IPfModel* model) -> bool {
              return RequestRemoveModel(model);
            }, trackObject);
        requestRemoveModelBinded = conn.connected();
      }

      auto conn = triad_manager->whenAboutToDestroyModel(
          model_, [this](IPfModel* model) { AboutToDestroyModel(model); },
          trackObject);
      aboutToDestroyModelBinded = conn.connected();
    }

    if (view_) {
      auto conn = triad_manager->whenAboutToDestroyView(
          view_, [this](IPfView* view) { AboutToDestroyView(view); },
          trackObject);
      aboutToDestroyViewBinded = conn.connected();
    }
  }

 public:
  bool requestRemoveModelBinded{false};
  bool aboutToDestroyModelBinded{false};
  bool aboutToDestroyViewBinded{false};

  IPfModel* model_{nullptr};
  IPfView* view_{nullptr};
  bool monitor_request_remove_;
};

using TestXXX_MVP_Triad =
    std::tuple<MockXXXModel*, MockXXXView*, MockXXXPresenter*>;

using TestXXX_MVPL_Tuple =
    std::tuple<MockXXXModel*, MockXXXView*, MockXXXPresenter*,
               std::shared_ptr<MockListener>>;

using TestYYY_MVP_Triad =
    std::tuple<MockYYYModel*, MockYYYView*, MockYYYPresenter*>;

using TestYYY_MVPL_Tuple =
    std::tuple<MockYYYModel*, MockYYYView*, MockYYYPresenter*,
               std::shared_ptr<MockListener>>;

class PfTriadManagerTestBase {
 protected:
  virtual ~PfTriadManagerTestBase() { PfViewFactoryManager::resetInstance(); }

  void initialize() {
    triad_manager =
        utils::make_unique<PfTriadManager>(PfViewFactoryManager::getInstance());

    verifyTriadManagerInitialState();
  }

  void verifyTriadManagerInitialState() {
    auto dummy_model = std::make_shared<MockXXXModel>();
    ASSERT_EQ(0, triad_manager->findViewByModel(dummy_model.get()).size());

    MockXXXView dummy_view;
    ASSERT_EQ(nullptr, triad_manager->findModelByView(&dummy_view));

    // after init and no view factory registered, cannot create view from model
    ASSERT_EQ(nullptr, triad_manager->createViewFor(dummy_model));

    PfCreateViewArgs args;

    args.set_view_factory_id(MockXXXViewFactory::viewFactoryId());
    ASSERT_EQ(nullptr,
              triad_manager->createViewFor(dummy_model, nullptr, true, &args));

    args.set_view_factory_id(MockXXXViewFactory2::viewFactoryId());
    ASSERT_EQ(nullptr,
              triad_manager->createViewFor(dummy_model, nullptr, true, &args));
  }

  static TestXXX_MVP_Triad make_xxx_triad(MockXXXModel* model,
                                          MockXXXView* view,
                                          MockXXXPresenter* presenter) {
    return std::make_tuple(model, view, presenter);
  }

  static TestYYY_MVP_Triad make_yyy_triad(MockYYYModel* model,
                                          MockYYYView* view,
                                          MockYYYPresenter* presenter) {
    return std::make_tuple(model, view, presenter);
  }

  template <typename VF, typename M, typename V, typename P>
  void createTestTriad(std::shared_ptr<M> model, std::shared_ptr<V> view,
                       P** presenter_ret, PfPresenter* parent = nullptr,
                       bool auto_remove_child = true,
                       PfCreateViewArgs* args = nullptr);

  template <typename VF, typename MVPLTuple>
  void createTestTriadAndListener(MVPLTuple* tuple,
                                  PfPresenter* parent = nullptr,
                                  bool auto_remove_child = true,
                                  PfCreateViewArgs* args = nullptr);

  template <typename VF, typename TriadT>
  void createTestTriads(TriadT* mvp_triad_to_test,
                        std::vector<TriadT>* all_mvp_triad = nullptr);

  template <typename M, typename V, typename P>
  void verifyTriad(M* model, V* view, P* presenter);

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

  template <typename TriadT>
  void expectationsOnSingleTriadDestroy(TriadT triad, MockListener* listener,
                                        CheckPointType* checker = nullptr,
                                        Sequence* seq = nullptr, int times = 1);
  template <typename MVPLTuple>
  void expectationsOnSingleTriadDestroy(MVPLTuple* triad_and_listener,
                                        CheckPointType* checker = nullptr,
                                        Sequence* seq = nullptr, int times = 1);
  void expectationsOnTwoTriadDestroy(TestXXX_MVP_Triad triad1,
                                     MockListener* listener1,
                                     TestXXX_MVP_Triad triad2,
                                     MockListener* listener2,
                                     CheckPointType* checker = nullptr,
                                     Sequence* seq = nullptr, int times = 1);

  void expectationsOnNotDestroyTriad(TestXXX_MVP_Triad triad,
                                     MockListener* listener);

  template <typename MVPLTuple>
  void expectationsOnNotDestroyTriad(MVPLTuple* triad_and_listener);

  // region: objects test subject depends on
  // endregion

  // region: test subject
  std::unique_ptr<IPfTriadManager> triad_manager;
  // endregion
};

template <typename M, typename V, typename P>
void PfTriadManagerTestBase::verifyTriad(M* model, V* view, P* presenter) {
  ASSERT_NE(nullptr, presenter);
  ASSERT_EQ(model, presenter->getModel().get());
  ASSERT_EQ(triad_manager.get(), model->triad_manager());
  ASSERT_EQ(view, presenter->getView().get());
  ASSERT_EQ(triad_manager.get(), presenter->triad_manager());
  ASSERT_TRUE(presenter->initialized_ok);
}

template <typename VF, typename M, typename V, typename P>
void PfTriadManagerTestBase::createTestTriad(
    std::shared_ptr<M> model, std::shared_ptr<V> view, P** presenter_ret,
    PfPresenter* parent, bool auto_remove_child, PfCreateViewArgs* args) {
  auto old_model_use_count = model.use_count();
  auto old_view_use_count = view.use_count();

  {  // working scope
    view_factory_t<M, VF> view_factory_wrapper;
    auto& view_factory = view_factory_wrapper.FTO_getFactory();

    ON_CALL(view_factory, createTestView()).WillByDefault(Return(view));

    if (args) args->set_view_factory_id(view_factory.getViewFactoryId());

    std::shared_ptr<IPfView> actual_view;
    actual_view =
        triad_manager->createViewFor(model, parent, auto_remove_child, args);
    ASSERT_EQ(view, actual_view);

    // for convenience, we will store the triad manager in presenter
    auto presenter = view_factory.last_presenter;
    verifyTriad(model.get(), view.get(), presenter);

    if (presenter_ret) *presenter_ret = presenter;
  }

  ASSERT_EQ(old_model_use_count + 1, model.use_count());
  ASSERT_EQ(old_view_use_count + 1, view.use_count());
}

void PfTriadManagerTestBase::createTestXXXTriad(
    std::shared_ptr<MockXXXModel> model, std::shared_ptr<MockXXXView> view,
    MockXXXPresenter** presenter) {
  createTestTriad<MockXXXViewFactory>(model, view, presenter);
}

void PfTriadManagerTestBase::createTestYYYTriad(
    std::shared_ptr<MockYYYModel> model, std::shared_ptr<MockYYYView> view,
    MockYYYPresenter** presenter) {
  createTestTriad<MockYYYViewFactory>(model, view, presenter);
}

template <typename VF, typename MVPLTuple>
void PfTriadManagerTestBase::createTestTriadAndListener(
    MVPLTuple* tuple, PfPresenter* parent, bool auto_remove_child,
    PfCreateViewArgs* args) {
  using MT = typename std::remove_pointer<
      typename std::tuple_element<0, MVPLTuple>::type>::type;
  using VT = typename std::remove_pointer<
      typename std::tuple_element<1, MVPLTuple>::type>::type;
  using PT = typename std::remove_pointer<
      typename std::tuple_element<2, MVPLTuple>::type>::type;

  auto model = std::make_shared<MT>();
  auto view = std::make_shared<VT>();
  PT* presenter = nullptr;

  createTestTriad<VF>(model, view, &presenter, parent, auto_remove_child, args);

  auto listener = MockListener::attachTo(triad_manager.get(), model.get(),
                                         view.get(), false);

  *tuple = std::make_tuple(model.get(), view.get(), presenter, listener);
}

template <typename VF, typename TriadT>
void PfTriadManagerTestBase::createTestTriads(
    TriadT* mvp_triad_to_test, std::vector<TriadT>* all_mvp_triad) {
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
  if (all_mvp_triad) *all_mvp_triad = all;
}

void PfTriadManagerTestBase::createTestXXXTriads(
    TestXXX_MVP_Triad* mvp_triad_to_test,
    std::vector<TestXXX_MVP_Triad>* all_mvp_triad) {
  createTestTriads<MockXXXViewFactory>(mvp_triad_to_test, all_mvp_triad);
}

void PfTriadManagerTestBase::createTestYYYTriads(
    TestYYY_MVP_Triad* mvp_triad_to_test,
    std::vector<TestYYY_MVP_Triad>* all_mvp_triad) {
  createTestTriads<MockYYYViewFactory>(mvp_triad_to_test, all_mvp_triad);
}

template <typename TriadT>
void PfTriadManagerTestBase::expectationsOnSingleTriadDestroy(
    TriadT triad, MockListener* listener, CheckPointType* checker,
    Sequence* seq, int times) {
  auto model = std::get<0>(triad);
  auto view = std::get<1>(triad);
  auto presenter = std::get<2>(triad);

  if (model) ASSERT_EQ(model, listener->model_);

  if (view) ASSERT_EQ(view, listener->view_);

  std::unique_ptr<Sequence> lSeq;
  if (seq == nullptr) {
    lSeq = utils::make_unique<Sequence>();
    seq = lSeq.get();
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

  if (presenter) {
    EXPECT_CALL(*presenter, onDestroy()).Times(times).InSequence(*seq);
  }

  if (view) {
    EXPECT_CALL(*view, onDestroy()).Times(times).InSequence(*seq);
  }

  if (model) {
    EXPECT_CALL(*model, onDestroy()).Times(times).InSequence(*seq);
  }

  if (presenter) {
    EXPECT_CALL(*presenter, destruct()).Times(times).InSequence(*seq);
  }

  if (view) {
    EXPECT_CALL(*view, destruct()).Times(times).InSequence(*seq);
  }

  if (model) {
    EXPECT_CALL(*model, destruct()).Times(times).InSequence(*seq);
  }

  if (checker) {
    EXPECT_CALL(*checker, Call("barrier")).Times(times).InSequence(*seq);
  }
}

template <typename MVPLTuple>
void PfTriadManagerTestBase::expectationsOnSingleTriadDestroy(
    MVPLTuple* triad_and_listener, CheckPointType* checker, Sequence* seq,
    int times) {
  auto triad = std::make_tuple(std::get<0>(*triad_and_listener),
                               std::get<1>(*triad_and_listener),
                               std::get<2>(*triad_and_listener));
  auto listener = std::get<3>(*triad_and_listener);
  expectationsOnSingleTriadDestroy(triad, listener.get(), checker, seq, times);
}

void PfTriadManagerTestBase::expectationsOnTwoTriadDestroy(
    TestXXX_MVP_Triad triad1, MockListener* listener1, TestXXX_MVP_Triad triad2,
    MockListener* listener2, CheckPointType* checker, Sequence* seq,
    int times) {
  auto model1 = std::get<0>(triad1);
  auto view1 = std::get<1>(triad1);
  //  auto presenter1 = std::get<2>(triad1);

  auto model2 = std::get<0>(triad2);
  auto view2 = std::get<1>(triad2);
  //  auto presenter2 = std::get<2>(triad2);

  std::unique_ptr<Sequence> lSeq;
  if (!seq) {
    lSeq = utils::make_unique<Sequence>();
    seq = lSeq.get();
  }

  if (model1 != model2) {
    expectationsOnSingleTriadDestroy(triad1, listener1, nullptr /* checker */,
                                     seq, times);
    expectationsOnSingleTriadDestroy(triad2, listener2, nullptr /* checker */,
                                     seq, times);
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

      EXPECT_CALL(*view2, destruct()).Times(times).InSequence(*seq);
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
      EXPECT_CALL(*view1, destruct()).Times(times).InSequence(*seq);
    }

    if (model) {
      EXPECT_CALL(*model, destruct()).Times(times).InSequence(*seq);
    }
  }

  if (checker) {
    EXPECT_CALL(*checker, Call("barrier")).Times(times).InSequence(*seq);
  }
}

void PfTriadManagerTestBase::expectationsOnNotDestroyTriad(
    TestXXX_MVP_Triad triad, MockListener* listener) {
  expectationsOnSingleTriadDestroy(triad, listener, nullptr /* checker */,
                                   nullptr /* seq */, 0 /* times */);
}

template <typename MVPLTuple>
void PfTriadManagerTestBase::expectationsOnNotDestroyTriad(
    MVPLTuple* triad_and_listener) {
  expectationsOnSingleTriadDestroy(triad_and_listener, nullptr /* checker */,
                                   nullptr /* seq */, 0 /* times */);
}

}  // namespace tests
}  // namespace pfmvp

#endif  // TEST_PFMVP_PF_TRIAD_MANAGER_TEST_BASE_H_
