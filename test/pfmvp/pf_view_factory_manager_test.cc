//-*- TestCaseName: PfViewFactoryManagerTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"

#include "utils/basic_utils.h"  // make_unique, <memory>
#include "test/testutils/generic_mock_listener.h"
#include "test/testutils/slot_catcher.h"

#include "pfmvp/pf_view_factory_manager.h"

namespace pfmvp {
namespace tests {

class TestBobModel : public IPfModel {
 public:
  virtual ~TestBobModel() = default;
  DEF_MODEL_ID(TestBobModel);
};

class TestAliceModel : public IPfModel {
 public:
  virtual ~TestAliceModel() = default;
  DEF_MODEL_ID(TestAliceModel);
};

class TestXXXViewFactory : public IPfViewFactory {
 public:
  TestXXXViewFactory() = default;
  virtual ~TestXXXViewFactory() = default;

  DEF_VIEW_FACTORY_ID(TestXXXViewFactory);

  std::shared_ptr<PfPresenter>
  createView(std::shared_ptr<IPfModel> model, PfCreateViewArgs* args) override {
    (void)model;
    (void)args;
    return nullptr;
  }

 private:
  TestXXXViewFactory(const TestXXXViewFactory& other) = delete;
  TestXXXViewFactory& operator=(const TestXXXViewFactory& other) = delete;
};

class TestYYYViewFactory : public IPfViewFactory {
 public:
  TestYYYViewFactory() = default;
  virtual ~TestYYYViewFactory() = default;

  DEF_VIEW_FACTORY_ID(TestYYYViewFactory);

  std::shared_ptr<PfPresenter>
  createView(std::shared_ptr<IPfModel> model,
             PfCreateViewArgs* args) override {
    (void)model;
    (void)args;
    return nullptr;
  }

 private:
  TestYYYViewFactory(const TestYYYViewFactory& other) = delete;
  TestYYYViewFactory& operator=(const TestYYYViewFactory& other) = delete;
};

class TestZZZViewFactory : public IPfViewFactory {
 public:
  TestZZZViewFactory() = default;
  virtual ~TestZZZViewFactory() = default;

  DEF_VIEW_FACTORY_ID(TestZZZViewFactory);

  std::shared_ptr<PfPresenter>
  createView(std::shared_ptr<IPfModel> model,
             PfCreateViewArgs* args) override {
    (void)model;
    (void)args;
    return nullptr;
  }

 private:
  TestZZZViewFactory(const TestZZZViewFactory& other) = delete;
  TestZZZViewFactory& operator=(const TestZZZViewFactory& other) = delete;
};

class PfViewFactoryManagerTest : public ::testing::Test {
 protected:
  PfViewFactoryManagerTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~PfViewFactoryManagerTest() { }
  virtual void SetUp() {
    vf_manager_ = new PfViewFactoryManager;

    {
      auto vfs = vf_manager_->getAllViewFactory(TestBobModel::modelId());
      ASSERT_EQ(0, vfs.size());
      vfs = vf_manager_->getAllViewFactory(TestAliceModel::modelId());
      ASSERT_EQ(0, vfs.size());

      ASSERT_EQ(nullptr, vf_manager_->getViewFactory(TestBobModel::modelId()));
      ASSERT_EQ(nullptr,
                vf_manager_->getViewFactory(
                    TestBobModel::modelId(),
                    TestXXXViewFactory::viewFactoryId()));
      ASSERT_EQ(nullptr,
                vf_manager_->getViewFactory(
                    TestBobModel::modelId(),
                    TestYYYViewFactory::viewFactoryId()));
      ASSERT_EQ(nullptr,
                vf_manager_->getViewFactory(
                    TestBobModel::modelId(),
                    TestZZZViewFactory::viewFactoryId()));
    }

    vf_manager_->addViewFactory(TestBobModel::modelId(),
                                &xxx_factory);
    vf_manager_->addViewFactory(TestBobModel::modelId(),
                                &yyy_factory);
    vf_manager_->addViewFactory(TestBobModel::modelId(),
                                &zzz_factory);

    factories.push_back(&xxx_factory);
    factories.push_back(&yyy_factory);
    factories.push_back(&zzz_factory);
    std::sort(factories.begin(), factories.end());
  }
  virtual void TearDown() {
    delete vf_manager_;
  }

  // region: objects test subject depends on
  // For Bob
  TestXXXViewFactory xxx_factory;
  TestYYYViewFactory yyy_factory;
  TestZZZViewFactory zzz_factory;
  std::vector<IPfViewFactory*> factories;
  // endregion

  // region: test subject
  IPfViewFactoryManager* vf_manager_ { nullptr };
  // endregion

  // region: object depends on test subject
  // endregion
};

TEST_F(PfViewFactoryManagerTest, should_be_able_to_add_view_factory_with_specified_model_id) { // NOLINT
  // Expectations
  std::vector<IPfViewFactory*> expect_factories = factories;

  // Verify results
  auto actual_factories =
      vf_manager_->getAllViewFactory(TestBobModel::modelId());
  std::sort(actual_factories.begin(), actual_factories.end());
  ASSERT_EQ(expect_factories, actual_factories);

  // Trianglation Test
  TestXXXViewFactory alice_xxx_factory;
  TestYYYViewFactory alice_yyy_factory;
  TestZZZViewFactory alice_zzz_factory;

  std::vector<IPfViewFactory*> expect_alice_factories;
  expect_alice_factories.push_back(&alice_xxx_factory);
  expect_alice_factories.push_back(&alice_yyy_factory);
  expect_alice_factories.push_back(&alice_zzz_factory);
  std::sort(expect_alice_factories.begin(), expect_alice_factories.end());

  vf_manager_->addViewFactory(TestAliceModel::modelId(), &alice_xxx_factory);
  vf_manager_->addViewFactory(TestAliceModel::modelId(), &alice_yyy_factory);
  vf_manager_->addViewFactory(TestAliceModel::modelId(), &alice_zzz_factory);

  auto actual_alice_factories =
      vf_manager_->getAllViewFactory(TestAliceModel::modelId());
  std::sort(actual_alice_factories.begin(), actual_alice_factories.end());
  ASSERT_EQ(expect_alice_factories, actual_alice_factories);
}

TEST_F(PfViewFactoryManagerTest, should_replace_existing_factory_when_factory_with_the_same_id) { // NOLINT
  // Setup fixture
  TestYYYViewFactory yyy_factory2;

  // Expectations
  std::vector<IPfViewFactory*> expect_factories;
  expect_factories.push_back(&xxx_factory);
  expect_factories.push_back(&yyy_factory2);
  expect_factories.push_back(&zzz_factory);
  std::sort(expect_factories.begin(), expect_factories.end());

  // Exercise system
  vf_manager_->addViewFactory(TestBobModel::modelId(), &yyy_factory2);

  // Verify results
  auto actual_factories =
      vf_manager_->getAllViewFactory(TestBobModel::modelId());
  std::sort(actual_factories.begin(), actual_factories.end());
  ASSERT_EQ(expect_factories, actual_factories);
}

TEST_F(PfViewFactoryManagerTest, should_getViewFactory_return_some_factory_we_added) { // NOLINT
  // Verify results
  auto vf = vf_manager_->getViewFactory(TestBobModel::modelId());
  ASSERT_NE(nullptr, vf);
  EXPECT_THAT(factories, Contains(vf));
}

TEST_F(PfViewFactoryManagerTest, should_getViewFactory_with_factory_id_return_exactly_the_factory_with_the_id) { // NOLINT
  // Setup fixture

  // Expectations

  // Exercise system

  // Verify results
  ASSERT_EQ(&xxx_factory,
            vf_manager_->getViewFactory(TestBobModel::modelId(),
                                        TestXXXViewFactory::viewFactoryId()));
  ASSERT_EQ(&yyy_factory,
            vf_manager_->getViewFactory(TestBobModel::modelId(),
                                        TestYYYViewFactory::viewFactoryId()));
  ASSERT_EQ(&zzz_factory,
            vf_manager_->getViewFactory(TestBobModel::modelId(),
                                        TestZZZViewFactory::viewFactoryId()));
}

}  // namespace tests
}  // namespace pfmvp
