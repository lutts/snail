//-*- TestCaseName: RelayCommandTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"

#include "src/utils/command/relay_command.h"

namespace utils {
namespace tests {

TEST(RelayCommandTest,
     should_be_able_to_construct_with_display_text_and_callback_function_and_redo_will_call_this_function) { // NOLINT
  bool called = false;
  auto expect_display_text = xtestutils::genRandomString();
  RelayCommand cmd { expect_display_text,
        [&called]() {
      called = true;
    }};

  cmd.redo();

  ASSERT_EQ(expect_display_text, cmd.display_text());
  ASSERT_TRUE(called);
}

TEST(RelayCommandTest, should_be_change_display_text) {
  RelayCommand cmd { xtestutils::genRandomString(), [](){}};

  auto new_display_text =
      xtestutils::genRandomDifferentString(cmd.display_text());

  cmd.set_display_text(new_display_text);

  ASSERT_EQ(new_display_text, cmd.display_text());
}

}  // namespace tests
}  // namespace utils
