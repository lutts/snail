//-*- TestCaseName: WorkBasicInfoQModelTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#include <QSignalSpy>

#include "test/testutils/gmock_common.h"

#include "utils/basic_utils.h"  // make_unique, <memory>
#include "test/testutils/utils.h"
#include "test/testutils/random_string.h"
#include "test/testutils/generic_mock_listener.h"
#include "test/testutils/slot_catcher.h"

#include "snail/mock_work_model.h"
#include "test/qtui/work_test_utils.h"
#include "src/qtui/work_basic_info_qmodel.h"

using namespace snailcore::tests;  // NOLINT

class WorkBasicInfoQModelTest : public ::testing::Test {
 protected:
  WorkBasicInfoQModelTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~WorkBasicInfoQModelTest() { }
  virtual void SetUp() {
    work = createNamedWork();
    qmodel = utils::make_unique<WorkBasicInfoQModel>(work.get());
  }
  // virtual void TearDown() { }

  QModelIndex nameFieldIndex();
  utils::U8String nameFieldContent(int role);

  std::shared_ptr<MockWorkModel> work;
  std::unique_ptr<WorkBasicInfoQModel> qmodel;
};

QModelIndex WorkBasicInfoQModelTest::nameFieldIndex() {
  return qmodel->index(0, 0);
}

utils::U8String WorkBasicInfoQModelTest::nameFieldContent(int role) {
  auto cell_content = qmodel->data(nameFieldIndex(), role);
  return QStringToU8String(cell_content.toString());
}

TEST_F(WorkBasicInfoQModelTest, should_work_name_field_DisplayRole_be_work_name) { // NOLINT
  // Verify results
  auto actual_display_content = nameFieldContent(Qt::DisplayRole);
  ASSERT_EQ(work->name(), actual_display_content);

  auto actual_edit_content = nameFieldContent(Qt::EditRole);
  ASSERT_EQ(work->name(), actual_edit_content);
}

TEST_F(WorkBasicInfoQModelTest, should_write_new_name_to_work_when_setData_for_name_field) { // NOLINT
  // Expectations
  xtestutils::RandomString workName;
  EXPECT_CALL(*work, set_name(workName.ustr()));

  // Exercise system
  qmodel->setData(nameFieldIndex(), workName.qstr());
}

TEST_F(WorkBasicInfoQModelTest, should_fire_dataChanged_signal_if_set_new_name_successful) { // NOLINT
  // Setup fixture
  ON_CALL(*work, set_name(_)).WillByDefault(Return(true));

  QSignalSpy sigspy(qmodel.get(), SIGNAL(dataChanged(const QModelIndex&,
                                                     const QModelIndex&,
                                                     const QVector<int>&)));

  // Exercise system
  xtestutils::RandomString dummyStr;
  ASSERT_TRUE(qmodel->setData(nameFieldIndex(), dummyStr.qstr()));

  // Verify results
  ASSERT_EQ(1, sigspy.count());
  QList<QVariant> arguments = sigspy.takeFirst();
  ASSERT_EQ(arguments.at(0), nameFieldIndex());
  ASSERT_EQ(arguments.at(1), nameFieldIndex());
}

TEST_F(WorkBasicInfoQModelTest, should_not_fire_dataChanged_if_set_new_name_failed) { // NOLINT
  // Setup fixture
  ON_CALL(*work, set_name(_)).WillByDefault(Return(false));

  QSignalSpy sigspy(qmodel.get(), SIGNAL(dataChanged(const QModelIndex&,
                                                     const QModelIndex&,
                                                     const QVector<int>&)));

  // Exercise system
  xtestutils::RandomString dummyStr;
  ASSERT_FALSE(qmodel->setData(nameFieldIndex(), dummyStr.qstr()));

  // Verify results
  ASSERT_EQ(0, sigspy.count());
}
