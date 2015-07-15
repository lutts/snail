//-*- TestCaseName: AttrCreateViewArgsTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"

#include "src/qtui/attr_create_view_args.h"

class AttrCreateViewArgsTest : public ::testing::Test {
 protected:
  AttrCreateViewArgsTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~AttrCreateViewArgsTest() { }
  virtual void SetUp() {
  }
  // virtual void TearDown() { }
};

TEST_F(AttrCreateViewArgsTest,
       should_be_able_to_create_edit_mode_args) { // NOLINT
  // Exercise system
  auto args = AttrCreateViewArgs::getArgs(true);

  // Verify results
  ASSERT_TRUE(args->isEditMode());
}

TEST_F(AttrCreateViewArgsTest,
       should_return_the_same_edit_mode_args_on_every_calls) { // NOLINT
  // Setup fixture

  // Expectations

  // Exercise system
  auto args1 = AttrCreateViewArgs::getArgs(true);
  auto jam = utils::make_unique<AttrCreateViewArgs>(true);
  auto args2 = AttrCreateViewArgs::getArgs(true);

  // Verify results
  ASSERT_EQ(args1, args2);
}

TEST_F(AttrCreateViewArgsTest,
       should_be_able_to_create_non_edit_mode_args) { // NOLINT
  auto args = AttrCreateViewArgs::getArgs(false);

  // Verify results
  ASSERT_FALSE(args->isEditMode());
}

TEST_F(AttrCreateViewArgsTest,
       should_return_the_same_non_edit_mode_args_on_every_calls) { // NOLINT
  // Exercise system
  auto args1 = AttrCreateViewArgs::getArgs(false);
  auto jam = utils::make_unique<AttrCreateViewArgs>(false);
  auto args2 = AttrCreateViewArgs::getArgs(false);

  // Verify results
  ASSERT_EQ(args1, args2);
}

TEST_F(AttrCreateViewArgsTest,
       should_be_able_to_create_memento_and_do_equal_check) { // NOLINT
  // Setup fixture
  auto edit_mode_args = AttrCreateViewArgs::getArgs(true);
  auto non_edit_mode_args = AttrCreateViewArgs::getArgs(false);

  // Exercise system
  auto edit_mode_memento = edit_mode_args->getMemento();
  auto non_edit_mode_memento = non_edit_mode_args->getMemento();

  // Verify results
  ASSERT_NE(nullptr, edit_mode_memento);
  ASSERT_NE(nullptr, non_edit_mode_memento);

  ASSERT_TRUE(edit_mode_args->memento_equals(*edit_mode_memento));
  ASSERT_TRUE(non_edit_mode_args->memento_equals(*non_edit_mode_memento));

  ASSERT_FALSE(edit_mode_args->memento_equals(*non_edit_mode_memento));
  ASSERT_FALSE(non_edit_mode_args->memento_equals(*edit_mode_memento));
}
