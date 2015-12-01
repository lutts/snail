//-*- TestCaseName: CommandProcessorTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <vector>

#include "test/testutils/gmock_common.h"

#include "test/testutils/simple_mock_listener.h"
#include "src/utils/command/command_processor.h"
#include "utils/mock_command.h"

namespace utils {
namespace tests {

class MockListener : public xtestutils::SimpleMockListener<ICommandProcessor> {
 public:
  SNAIL_MOCK_LISTENER1(MockListener, CanUndoChanged, void(bool canUndo));
  SNAIL_MOCK_LISTENER1(MockListener, CanRedoChanged, void(bool canRedo));

 public:
  explicit MockListener(ICommandProcessor *commandProcessor)
      : SimpleMockListener(commandProcessor) {
    SNAIL_MOCK_LISTENER_REGISTER(CanUndoChanged, this);
    SNAIL_MOCK_LISTENER_REGISTER(CanRedoChanged, this);

    attach();
  }
};

class CommandProcessorTest : public ::testing::Test {
 protected:
  CommandProcessorTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  ~CommandProcessorTest() {}

  virtual void SetUp() {
    // DefaultValue<Command::CmdType>::Set(Command::NORMAL);

    commandProcessor = new CommandProcessor;
  }
  virtual void TearDown() { delete commandProcessor; }

  MockCommand *generateNormalCommand();

  void enterCanUndoTrue_canRedoFalse_withOneCmd();
  void enterCanUndoTrue_canRedoFalse_withOneCmd_plus_One();

  void enterCanUndoCanRedoBothTrueState_with_Two_Cmds();

  void enterCanRedoTrue_canUndoFalse_withOneCmd();
  void enterCanRedoTrue_canUndoFalse_withTwoCmds();

  int setTestUndoLimit();
  void resetUndoLimit();

  void fillWithDummyCommands(int count);
  void undoAllCommands();

  void build_oneDoneCmd_oneUndoneCmd_plus_other_undone_cmds(
      int total_count, MockCommand **done_cmd, MockCommand **undone_cmd);

  std::vector<StrictMock<MockCommand> *> fillWithStrictMockCommands(int count);

  CommandProcessor *commandProcessor;
};

inline MockCommand *CommandProcessorTest::generateNormalCommand() {
  MockCommand *cmd = new MockCommand;

  ON_CALL(*cmd, getType()).WillByDefault(Return(Command::NORMAL));

  return cmd;
}

//////// basic function test start //////
TEST_F(CommandProcessorTest, count_initially_zero) {
  // Verify results
  ASSERT_EQ(0, commandProcessor->count());
}

TEST_F(CommandProcessorTest, do_cmd_One_CauseCountToOne) {
  // Exercise system
  commandProcessor->do_cmd(new DummyCommand);

  // Verify results
  ASSERT_EQ(1, commandProcessor->count());
}

TEST_F(CommandProcessorTest, do_cmd_Two_CauseCountToTwo) {
  // Exercise system
  commandProcessor->do_cmd(new DummyCommand);
  commandProcessor->do_cmd(new DummyCommand);

  // Verify results
  ASSERT_EQ(2, commandProcessor->count());
}

TEST_F(CommandProcessorTest, clear_one_Cause_count_reset_to_zero) {
  // Setup fixture
  commandProcessor->do_cmd(new DummyCommand);

  // Exercise system
  commandProcessor->clear();

  // Verify results
  ASSERT_EQ(0, commandProcessor->count());
}

TEST_F(CommandProcessorTest, clear_two_Cause_count_reset_to_zero) {
  // Setup fixture
  commandProcessor->do_cmd(new DummyCommand);
  commandProcessor->do_cmd(new DummyCommand);

  // Exercise system
  commandProcessor->clear();

  // Verify results
  ASSERT_EQ(0, commandProcessor->count());
}

TEST_F(CommandProcessorTest, clear_willCauseCommandDestroy_one) {
  MockCommand *cmd = generateNormalCommand();

  // expectation
  EXPECT_CALL(*cmd, destruct());

  // Exercise system
  commandProcessor->do_cmd(cmd);
  commandProcessor->clear();

  // nothing to tear down
}

TEST_F(CommandProcessorTest, clear_willCauseCommandDestroy_two) {
  MockCommand *cmd1 = generateNormalCommand();
  MockCommand *cmd2 = generateNormalCommand();

  // expectation
  EXPECT_CALL(*cmd1, destruct());
  EXPECT_CALL(*cmd2, destruct());

  // Exercise system
  commandProcessor->do_cmd(cmd1);
  commandProcessor->do_cmd(cmd2);

  commandProcessor->clear();

  // nothing to tear down
}

TEST_F(CommandProcessorTest, do_cmd_CauseCommandRedo_one) {
  MockCommand *cmd = generateNormalCommand();

  // expectation
  EXPECT_CALL(*cmd, redo());

  // Exercise system
  commandProcessor->do_cmd(cmd);
}

TEST_F(CommandProcessorTest, do_cmd_CauseCommandRedo_two_in_order) {
  MockCommand *cmd1 = generateNormalCommand();
  MockCommand *cmd2 = generateNormalCommand();

  // expectation
  {
    InSequence seq;

    EXPECT_CALL(*cmd1, redo());
    EXPECT_CALL(*cmd2, redo());
  }

  // Exercise system
  commandProcessor->do_cmd(cmd1);
  commandProcessor->do_cmd(cmd2);
}

TEST_F(CommandProcessorTest, undo_CauseCommandUndo_one) {
  MockCommand *cmd = generateNormalCommand();

  // Setup fixture
  commandProcessor->do_cmd(cmd);

  // expectation
  EXPECT_CALL(*cmd, undo());

  // Exercise system
  commandProcessor->undo();
}

TEST_F(CommandProcessorTest, undo_CauseCommandUndo_two_in_reverse_order) {
  MockCommand *cmd1 = generateNormalCommand();
  MockCommand *cmd2 = generateNormalCommand();

  // Setup fixture
  commandProcessor->do_cmd(cmd1);
  commandProcessor->do_cmd(cmd2);

  // expectations
  {
    InSequence seq;

    EXPECT_CALL(*cmd2, undo());
    EXPECT_CALL(*cmd1, undo());
  }

  // Exercise system
  commandProcessor->undo();
  commandProcessor->undo();
}

TEST_F(CommandProcessorTest, undo_do_not_affect_command_count_one) {
  // Setup fixture
  commandProcessor->do_cmd(new DummyCommand);
  int count_before_undo = commandProcessor->count();

  // Exercise system
  commandProcessor->undo();

  // Verify results
  ASSERT_EQ(count_before_undo, commandProcessor->count());
}

TEST_F(CommandProcessorTest, undo_do_not_affect_command_count_two) {
  // Setup fixture
  commandProcessor->do_cmd(new DummyCommand);
  commandProcessor->do_cmd(new DummyCommand);
  int count_before_undo = commandProcessor->count();

  // Exercise system
  commandProcessor->undo();

  // Verify results
  ASSERT_EQ(count_before_undo, commandProcessor->count());
}

TEST_F(CommandProcessorTest, undo_do_not_affect_command_count_two_two) {
  // Setup fixture
  commandProcessor->do_cmd(new DummyCommand);
  commandProcessor->do_cmd(new DummyCommand);
  int count_before_undo = commandProcessor->count();

  // Exercise system
  commandProcessor->undo();
  commandProcessor->undo();

  // Verify results
  ASSERT_EQ(count_before_undo, commandProcessor->count());
}
//////// basic function test end

//////// undo/redo status test start ////////

/*
 *          (init)
 *            |
 *    ________V________________________________________
 *    |                 can not undo                  |
 *    +-----------------------------------------------+
 *      |          ^                |               ^
 *  do first cmd   | all cmds       | redo an       |
 *      |          | are undone     | undone cmd    | clear
 *      |          |                |               |
 *    __V__________|________________V_________________
 *    |                   can undo                   |
 *    +----------------------------------------------+
 *                  ^
 *            do cmd when canUndo is already true
 *           redo cmd when canUndo is already true
 *
 *
 *         (init)
 *           |
 *   ________V_____________________________
 *   |            can not redo            |
 *   +------------------------------------+
 *    ^             ^        |          ^
 *    | all cmds    |      undo a cmd   |
 *    | are redone  |        |          | clear
 *    |           do cmd     |          |
 *   _|_____________|________V__________|__
 *   |              can redo              |
 *   +------------------------------------+
 *                   ^
 *           undo cmd when canRedo is already true
 *
 * undo when canUndo false will do nothing
 *    * canUndo false & canRedo false
 *    * canUndo false & canRedo true
 *
 * redo when canRedo false will do nothing
 *    * canRedo false & canUndo false
 *    * canRedo false & canUndo true
 */

TEST_F(CommandProcessorTest, canUndo_initially_false) {
  // Verify results
  ASSERT_FALSE(commandProcessor->canUndo());
}

TEST_F(CommandProcessorTest,
       redo_when_canRedo_and_canUndo_false_remains_false) {  // NOLINT
  // Exercise system
  commandProcessor->redo();

  // Verify results
  ASSERT_FALSE(commandProcessor->canRedo());
  ASSERT_FALSE(commandProcessor->canUndo());
}

inline void CommandProcessorTest::enterCanUndoTrue_canRedoFalse_withOneCmd() {
  commandProcessor->do_cmd(new DummyCommand);
}

TEST_F(CommandProcessorTest, do_first_cmd_cause_canUndo_true) {
  // Exercise system
  enterCanUndoTrue_canRedoFalse_withOneCmd();

  // Verify results
  ASSERT_TRUE(commandProcessor->canUndo());
}

inline void CommandProcessorTest::
    enterCanUndoTrue_canRedoFalse_withOneCmd_plus_One() {  // NOLINT
  enterCanUndoTrue_canRedoFalse_withOneCmd();

  // do another command
  commandProcessor->do_cmd(new DummyCommand);
}

TEST_F(CommandProcessorTest, do_cmd_when_canUndo_already_true_remains_true) {
  // Exercise system
  enterCanUndoTrue_canRedoFalse_withOneCmd_plus_One();

  // Verify results
  ASSERT_TRUE(commandProcessor->canUndo());
}

TEST_F(CommandProcessorTest, clear_cause_canUndo_false_one) {
  // Setup fixture
  enterCanUndoTrue_canRedoFalse_withOneCmd();

  // Exercise system
  commandProcessor->clear();

  // Verify results
  ASSERT_FALSE(commandProcessor->canUndo());

  // nothing to teardown
}

TEST_F(CommandProcessorTest, clear_caue_canUndo_false_two) {
  // Setup fixture
  enterCanUndoTrue_canRedoFalse_withOneCmd_plus_One();

  // Exercise system
  commandProcessor->clear();

  // Verify results
  ASSERT_FALSE(commandProcessor->canUndo());

  // nothing to teardown
}

TEST_F(CommandProcessorTest, canRedo_initially_false) {
  // Verify results
  ASSERT_FALSE(commandProcessor->canRedo());
}

TEST_F(CommandProcessorTest, undo_when_canUndo_canRedo_false_remains_false) {
  // Exercise system
  commandProcessor->undo();

  // Verify results
  ASSERT_FALSE(commandProcessor->canRedo());
  ASSERT_FALSE(commandProcessor->canUndo());
}

inline void CommandProcessorTest::enterCanRedoTrue_canUndoFalse_withOneCmd() {
  // Setup fixture
  enterCanUndoTrue_canRedoFalse_withOneCmd();

  // Exercise system
  commandProcessor->undo();
}

TEST_F(CommandProcessorTest, undo_cmd_cause_canRedo_true) {
  enterCanRedoTrue_canUndoFalse_withOneCmd();

  // Verify results
  ASSERT_TRUE(commandProcessor->canRedo());
}

inline void
CommandProcessorTest::enterCanUndoCanRedoBothTrueState_with_Two_Cmds() {
  // Setup fixture
  enterCanUndoTrue_canRedoFalse_withOneCmd_plus_One();

  // Exercise system
  commandProcessor->undo();
}

TEST_F(CommandProcessorTest, undo_portion_cmds_canUndo_true_canRedo_true) {
  enterCanUndoCanRedoBothTrueState_with_Two_Cmds();

  // Verify results
  ASSERT_TRUE(commandProcessor->canRedo());
  ASSERT_TRUE(commandProcessor->canUndo());
}

TEST_F(CommandProcessorTest, undo_when_canRedo_remains_true) {
  // Setup fixture
  enterCanUndoCanRedoBothTrueState_with_Two_Cmds();

  // Exercise system
  commandProcessor->undo();

  // Verify results
  ASSERT_TRUE(commandProcessor->canRedo());
}

TEST_F(CommandProcessorTest, undo_all_cmds_cause_canUndo_false_one) {
  // Setup fixture
  enterCanUndoTrue_canRedoFalse_withOneCmd();

  // Exercise system
  commandProcessor->undo();

  // Verify results
  ASSERT_FALSE(commandProcessor->canUndo());
}

inline void CommandProcessorTest::enterCanRedoTrue_canUndoFalse_withTwoCmds() {
  // Setup fixture
  enterCanUndoCanRedoBothTrueState_with_Two_Cmds();

  // Exercise system
  commandProcessor->undo();
}

TEST_F(CommandProcessorTest, undo_all_cmds_cause_canUndo_false_two) {
  enterCanRedoTrue_canUndoFalse_withTwoCmds();

  // Verify results
  ASSERT_FALSE(commandProcessor->canUndo());
}

TEST_F(CommandProcessorTest, clear_cause_canRedo_false_one) {
  // Setup fixture
  enterCanRedoTrue_canUndoFalse_withOneCmd();

  // Exercise system
  commandProcessor->clear();

  // Verify results
  ASSERT_FALSE(commandProcessor->canRedo());
}

TEST_F(CommandProcessorTest, clear_cause_canRedo_false_two) {
  // Setup fixture
  enterCanRedoTrue_canUndoFalse_withTwoCmds();

  // Exercise system
  commandProcessor->clear();

  // Verify results
  ASSERT_FALSE(commandProcessor->canRedo());
}

TEST_F(CommandProcessorTest, clear_cause_both_canRedo_canUndo_false) {
  // Setup fixture
  enterCanUndoCanRedoBothTrueState_with_Two_Cmds();

  // Exercise system
  commandProcessor->clear();

  // Verify results
  ASSERT_FALSE(commandProcessor->canRedo());
  ASSERT_FALSE(commandProcessor->canUndo());
}

TEST_F(CommandProcessorTest,
       redo_undone_cmd_when_canUndo_false_changed_to_true) {  // NOLINT
  // Setup fixture
  enterCanRedoTrue_canUndoFalse_withTwoCmds();

  // Exercise system
  commandProcessor->redo();

  // Verify results
  ASSERT_TRUE(commandProcessor->canUndo());
}

TEST_F(CommandProcessorTest, redo_when_canUndo_true_remains_true) {
  // Setup fixture
  enterCanUndoCanRedoBothTrueState_with_Two_Cmds();

  // Exercise system
  commandProcessor->redo();

  // Verify results
  ASSERT_TRUE(commandProcessor->canUndo());
}

TEST_F(CommandProcessorTest, redo_all_cmds_cause_canRedo_false) {
  // Setup fixture
  enterCanUndoCanRedoBothTrueState_with_Two_Cmds();

  // Exercise system
  commandProcessor->redo();

  // Verify results
  ASSERT_FALSE(commandProcessor->canRedo());
}

TEST_F(CommandProcessorTest, do_cmd_discards_undo_stack) {
  // Setup fixture
  MockCommand *undoneCmd = generateNormalCommand();
  commandProcessor->do_cmd(new DummyCommand);
  commandProcessor->do_cmd(undoneCmd);
  commandProcessor->undo();

  int oldCmdCount = commandProcessor->count();

  // expectations
  EXPECT_CALL(*undoneCmd, destruct());

  // Exercise system
  commandProcessor->do_cmd(new DummyCommand());

  // Verify results
  const int newCmdCount = oldCmdCount + 1  // one more new command
                          - 1;             // one command discarded
  ASSERT_EQ(newCmdCount, commandProcessor->count());
  ASSERT_FALSE(commandProcessor->canRedo());
}

//////// undo/redo status test end ////////

//////// listener test begin ////////

TEST_F(CommandProcessorTest, fire_canUndoChanged_true_when_do_first_cmd) {
  // Setup fixture
  MockListener mockListener{commandProcessor};

  // expectations
  EXPECT_CALL(mockListener, CanUndoChanged(true));

  // Exercise system
  enterCanUndoTrue_canRedoFalse_withOneCmd();

  // Teardown fixture
  mockListener.detatch();
}

TEST_F(CommandProcessorTest,
       not_fire_canUndoChanged_when_canUndo_already_true) {  // NOLINT
  // Setup fixture
  enterCanUndoTrue_canRedoFalse_withOneCmd();

  // expectations
  StrictMock<MockListener> mockListener{commandProcessor};
  // expect strict listener's methods will not be called

  // Exercise system
  // do another command
  commandProcessor->do_cmd(new DummyCommand);

  // Teardown fixture
  mockListener.detatch();
}

TEST_F(CommandProcessorTest,
       not_fire_canUndoChanged_when_canUndo_already_true_two) {  // NOLINT
  // Setup fixture
  enterCanUndoCanRedoBothTrueState_with_Two_Cmds();

  // expectations
  StrictMock<MockListener> mockListener(commandProcessor);
  EXPECT_CALL(mockListener, CanRedoChanged(false));

  // Exercise system
  commandProcessor->redo();

  // Teardown fixture
  mockListener.detatch();
}

TEST_F(CommandProcessorTest,
       fire_canUndoChanged_false_when_clear_canUndo_true_one) {  // NOLINT
  // Setup fixture
  enterCanUndoTrue_canRedoFalse_withOneCmd();

  // expectations
  StrictMock<MockListener> mockListener{commandProcessor};
  EXPECT_CALL(mockListener, CanUndoChanged(false));

  // Exercise system
  commandProcessor->clear();

  // Teardown fixture
}

TEST_F(CommandProcessorTest,
       fire_canUndoChanged_false_when_clear_canUndo_true_two) {  // NOLINT
  // Setup fixture
  enterCanUndoTrue_canRedoFalse_withOneCmd_plus_One();

  // expectations
  StrictMock<MockListener> mockListener{commandProcessor};
  EXPECT_CALL(mockListener, CanUndoChanged(false));

  // Exercise system
  commandProcessor->clear();

  // Teardown fixture
}

TEST_F(
    CommandProcessorTest,
    not_fire_canUndoChanged_when_undo_when_canUndo_canRedo_false) {  // NOLINT
  // expectations
  StrictMock<MockListener> mockListener{commandProcessor};

  // Exercise system
  commandProcessor->undo();

  // Teardown fixture
}

TEST_F(CommandProcessorTest,
       fire_canRedoChanged_true_when_undo_cmd_cause_canRedo_true) {  // NOLINT
  // Setup fixture
  enterCanUndoTrue_canRedoFalse_withOneCmd_plus_One();

  // expectations
  StrictMock<MockListener> mockListener{commandProcessor};
  EXPECT_CALL(mockListener, CanRedoChanged(true));

  // Exercise system
  commandProcessor->undo();

  // Teardown fixture
  mockListener.detatch();
}

TEST_F(CommandProcessorTest,
       fire_canUndoChanged_false_when_undo_all_cmds_one) {  // NOLINT
  // Setup fixture
  enterCanUndoTrue_canRedoFalse_withOneCmd();

  // expectations
  StrictMock<MockListener> mockListener{commandProcessor};
  EXPECT_CALL(mockListener, CanUndoChanged(false));
  EXPECT_CALL(mockListener, CanRedoChanged(true));

  // Exercise system
  commandProcessor->undo();

  // Teardown fixture
  mockListener.detatch();
}

TEST_F(CommandProcessorTest,
       fire_canUndoChanged_false_when_undo_all_cmds_two) {  // NOLINT
  // Setup fixture
  enterCanUndoCanRedoBothTrueState_with_Two_Cmds();

  // expectations
  StrictMock<MockListener> mockListener{commandProcessor};
  // NOTE: because listener are strict, we also tested the
  // not_fire_canRedoChanged_when_canRedo_already_true case
  EXPECT_CALL(mockListener, CanUndoChanged(false));

  // Exercise system
  commandProcessor->undo();

  // Teardown fixture
  mockListener.detatch();
}

TEST_F(CommandProcessorTest,
       fire_canRedoChanged_false_when_clear_canRedo_true_one) {  // NOLINT
  // Setup fixture
  enterCanRedoTrue_canUndoFalse_withOneCmd();

  // expectations
  StrictMock<MockListener> mockListener{commandProcessor};
  EXPECT_CALL(mockListener, CanRedoChanged(false));

  // Exercise system
  commandProcessor->clear();

  // Teardown fixture
}

TEST_F(CommandProcessorTest,
       fire_canRedoChanged_false_when_clear_canRedo_true_two) {  // NOLINT
  // Setup fixture
  enterCanRedoTrue_canUndoFalse_withTwoCmds();

  // expectations
  StrictMock<MockListener> mockListener{commandProcessor};
  EXPECT_CALL(mockListener, CanRedoChanged(false));

  // Exercise system
  commandProcessor->clear();

  // Teardown fixture
}

TEST_F(
    CommandProcessorTest,
    fire_canRedoChanged_canUndoChanged_false_when_clear_when_both_canRedo_canUndo_true) {  // NOLINT
  // Setup fixture
  enterCanUndoCanRedoBothTrueState_with_Two_Cmds();

  // expectations
  StrictMock<MockListener> mockListener{commandProcessor};
  EXPECT_CALL(mockListener, CanUndoChanged(false));
  EXPECT_CALL(mockListener, CanRedoChanged(false));

  // Exercise system
  commandProcessor->clear();

  // Teardown fixture
}

TEST_F(
    CommandProcessorTest,
    fire_canUndoChanged_true_when_redo_undone_cmd_if_canUndo_false) {  // NOLINT
  // Setup fixture
  enterCanRedoTrue_canUndoFalse_withTwoCmds();

  // expectations
  StrictMock<MockListener> mockListener{commandProcessor};
  EXPECT_CALL(mockListener, CanUndoChanged(true));

  // Exercise system
  commandProcessor->redo();

  // Teardown fixture
  mockListener.detatch();
}

TEST_F(CommandProcessorTest,
       fire_canRedoChanged_false_when_do_cmd_discard_undo_stack) {  // NOLINT
  // Setup fixture
  enterCanUndoCanRedoBothTrueState_with_Two_Cmds();

  // expectations
  StrictMock<MockListener> mockListener{commandProcessor};
  EXPECT_CALL(mockListener, CanRedoChanged(false));

  // Exercise system
  commandProcessor->do_cmd(new DummyCommand);

  // Teardown fixture
  mockListener.detatch();
}
//////// listener test end ////////

//////// undo limit test start ////////

TEST_F(CommandProcessorTest, undo_limit_initially_unlimited) {  // NOLINT
  // Verify results
  ASSERT_EQ(CommandProcessor::UNDO_UNLIMITED, commandProcessor->undo_limit());
}

inline int CommandProcessorTest::setTestUndoLimit() {
  const int kNewUnLimited = 8;
  commandProcessor->set_undo_limit(kNewUnLimited);
  return kNewUnLimited;
}

inline void CommandProcessorTest::resetUndoLimit() {
  commandProcessor->set_undo_limit(CommandProcessor::UNDO_UNLIMITED);
}

TEST_F(CommandProcessorTest, set_undo_limit_get_setted_value) {  // NOLINT
  const int kNewUnLimited = 8;

  // Exercise system
  commandProcessor->set_undo_limit(kNewUnLimited);

  // Verify results
  ASSERT_EQ(kNewUnLimited, commandProcessor->undo_limit());

  // Teardown fixture
  resetUndoLimit();
}

TEST_F(CommandProcessorTest, set_undo_limit_to_zero_has_no_effect) {  // NOLINT
  // Setup fixture
  int old_limit = setTestUndoLimit();

  // Exercise system
  commandProcessor->set_undo_limit(0);

  // Verify results
  ASSERT_EQ(old_limit, commandProcessor->undo_limit());

  // Teardown fixture
  resetUndoLimit();
}

void CommandProcessorTest::fillWithDummyCommands(int count) {
  for (int i = 0; i < count; ++i) {
    commandProcessor->do_cmd(new DummyCommand);
  }
}

void CommandProcessorTest::undoAllCommands() {
  while (commandProcessor->canUndo()) {
    commandProcessor->undo();
  }
}

TEST_F(CommandProcessorTest,
       undo_limit_includes_done_and_undone_cmds) {  // NOLINT
  // Test Logic: undo will not cause oldest command be dropped

  // Setup fixture
  int limit = setTestUndoLimit();
  MockCommand *oldest_cmd = generateNormalCommand();
  commandProcessor->do_cmd(oldest_cmd);
  fillWithDummyCommands(limit - 1);

  // expectations
  EXPECT_CALL(*oldest_cmd, destruct()).Times(0);

  // Exercise system
  undoAllCommands();

  // Verify results
  ::testing::Mock::VerifyAndClearExpectations(oldest_cmd);

  // Teardown fixture
  resetUndoLimit();
}

TEST_F(CommandProcessorTest,
       do_cmd_when_exceed_limit_drop_oldest_cmd_one) {  // NOLINT
  // Setup fixture
  int limit = setTestUndoLimit();
  MockCommand *oldest_cmd = generateNormalCommand();
  commandProcessor->do_cmd(oldest_cmd);
  fillWithDummyCommands(limit - 1);

  // expectations
  EXPECT_CALL(*oldest_cmd, destruct());

  // Exercise system
  commandProcessor->do_cmd(new DummyCommand);

  // Teardown fixture
  resetUndoLimit();
}

TEST_F(CommandProcessorTest,
       do_cmd_when_exceed_limit_drop_oldest_cmd_two) {  // NOLINT
  // Setup fixture
  int limit = setTestUndoLimit();

  MockCommand *oldest_cmd = generateNormalCommand();
  MockCommand *sub_oldest_cmd = generateNormalCommand();

  commandProcessor->do_cmd(oldest_cmd);
  commandProcessor->do_cmd(sub_oldest_cmd);

  fillWithDummyCommands(limit - 2);

  // expectations
  {
    InSequence seq;

    EXPECT_CALL(*oldest_cmd, destruct());
    EXPECT_CALL(*sub_oldest_cmd, destruct());
  }

  // Exercise system
  fillWithDummyCommands(2);

  // Verify result
  ::testing::Mock::VerifyAndClearExpectations(oldest_cmd);
  ::testing::Mock::VerifyAndClearExpectations(sub_oldest_cmd);

  // Teardown fixture
  resetUndoLimit();
}

TEST_F(CommandProcessorTest,
       do_cmd_when_exceed_limit_drop_undone_cmds_before_done_cmds) {  // NOLINT
  // Setup fixture
  int limit = setTestUndoLimit();

  MockCommand *done_cmd = generateNormalCommand();
  MockCommand *undone_cmd = generateNormalCommand();

  commandProcessor->do_cmd(done_cmd);
  fillWithDummyCommands(limit - 2);
  commandProcessor->do_cmd(undone_cmd);
  commandProcessor->undo();

  // expectations
  {
    InSequence seq;

    EXPECT_CALL(*undone_cmd, destruct());
    EXPECT_CALL(*done_cmd, destruct());
  }

  // Exercise system
  fillWithDummyCommands(2);

  // Verify result
  ::testing::Mock::VerifyAndClearExpectations(undone_cmd);
  ::testing::Mock::VerifyAndClearExpectations(done_cmd);

  // Teardown fixture
  resetUndoLimit();
}

TEST_F(
    CommandProcessorTest,
    do_cmd_when_exceed_limit_drop_undone_cmds_fire_canRedoChanged_false) {  // NOLINT
  // Setup fixture
  fillWithDummyCommands(setTestUndoLimit());
  commandProcessor->undo();

  // expectations
  StrictMock<MockListener> mockListener{commandProcessor};
  EXPECT_CALL(mockListener, CanRedoChanged(false));

  // Exercise system
  fillWithDummyCommands(1);

  // Teardown fixture
  resetUndoLimit();
  mockListener.detatch();
}

TEST_F(CommandProcessorTest,
       set_smaller_undo_limit_shrink_history_list_one) {  // NOLINT
  // Setup fixture
  int limit = setTestUndoLimit();

  MockCommand *oldest_cmd = generateNormalCommand();
  commandProcessor->do_cmd(oldest_cmd);

  fillWithDummyCommands(limit - 1);

  // expectations
  EXPECT_CALL(*oldest_cmd, destruct());

  // Exercise system
  commandProcessor->set_undo_limit(limit - 1);

  // Teardown fixture
  resetUndoLimit();
}

TEST_F(CommandProcessorTest,
       set_smaller_undo_limit_shrink_history_list_two) {  // NOLINT
  // Setup fixture
  int limit = setTestUndoLimit();

  MockCommand *oldest_cmd = generateNormalCommand();
  MockCommand *sub_oldest_cmd = generateNormalCommand();
  commandProcessor->do_cmd(oldest_cmd);
  commandProcessor->do_cmd(sub_oldest_cmd);

  fillWithDummyCommands(limit - 2);

  // expectations
  {
    InSequence seq;

    EXPECT_CALL(*oldest_cmd, destruct());
    EXPECT_CALL(*sub_oldest_cmd, destruct());
  }

  // Exercise system
  commandProcessor->set_undo_limit(limit - 2);
  ::testing::Mock::VerifyAndClearExpectations(oldest_cmd);
  ::testing::Mock::VerifyAndClearExpectations(sub_oldest_cmd);

  // Teardown fixture
  resetUndoLimit();
}

void CommandProcessorTest::build_oneDoneCmd_oneUndoneCmd_plus_other_undone_cmds(
    int total_count, MockCommand **done_cmd, MockCommand **undone_cmd) {
  MockCommand *done_cmd_ = generateNormalCommand();
  MockCommand *undone_cmd_ = generateNormalCommand();
  commandProcessor->do_cmd(done_cmd_);
  commandProcessor->do_cmd(undone_cmd_);
  fillWithDummyCommands(total_count - 2);

  // undo until undone_cmd
  for (int i = 0; i < total_count - 1; ++i) {
    commandProcessor->undo();
  }

  if (done_cmd) *done_cmd = done_cmd_;

  if (undone_cmd) *undone_cmd = undone_cmd_;
}

TEST_F(
    CommandProcessorTest,
    set_smaller_undo_limit_shrink_oldest_done_cmds_befor_undone_cmds) {  // NOLINT
  // Setup fixture
  const int limit = 3;
  commandProcessor->set_undo_limit(3);

  MockCommand *done_cmd = nullptr;
  MockCommand *undone_cmd = nullptr;

  build_oneDoneCmd_oneUndoneCmd_plus_other_undone_cmds(limit, &done_cmd,
                                                       &undone_cmd);

  // expectations
  {
    InSequence seq;

    EXPECT_CALL(*done_cmd, destruct());
    EXPECT_CALL(*undone_cmd, destruct());
  }

  // Exercise system
  commandProcessor->set_undo_limit(limit - 2);

  // Verify results
  ::testing::Mock::VerifyAndClearExpectations(done_cmd);
  ::testing::Mock::VerifyAndClearExpectations(undone_cmd);

  // Teardown fixture
  resetUndoLimit();
}

TEST_F(
    CommandProcessorTest,
    set_smaller_undo_limit_shrink_history_list_may_fire_canUndoChanged_false) {  // NOLINT
  // Setup fixture
  build_oneDoneCmd_oneUndoneCmd_plus_other_undone_cmds(setTestUndoLimit(),
                                                       nullptr, nullptr);

  // expectations
  StrictMock<MockListener> mockListener{commandProcessor};
  EXPECT_CALL(mockListener, CanUndoChanged(false));

  // Exercise system
  commandProcessor->set_undo_limit(commandProcessor->undo_limit() - 1);

  // Teardown fixture
  resetUndoLimit();
  mockListener.detatch();
}

std::vector<StrictMock<MockCommand> *>
CommandProcessorTest::fillWithStrictMockCommands(int count) {
  std::vector<StrictMock<MockCommand> *> cmds;

  for (int i = 0; i < count; ++i) {
    StrictMock<MockCommand> *cmd = new StrictMock<MockCommand>;
    EXPECT_CALL(*cmd, getType()).WillOnce(Return(Command::NORMAL));
    EXPECT_CALL(*cmd, redo());
    commandProcessor->do_cmd(cmd);
    cmds.push_back(cmd);
  }

  for (int i = 0; i < count; ++i) {
    ::testing::Mock::VerifyAndClearExpectations(cmds[i]);
  }

  return cmds;
}

TEST_F(CommandProcessorTest,
       set_unlimit_will_not_affect_history_list) {  // NOLINT
  // Setup fixture
  fillWithStrictMockCommands(setTestUndoLimit());

  // expectations
  // NOTE: expect that no methods on cmds will be called
  StrictMock<MockListener> mockListener{commandProcessor};

  // Exercise system
  resetUndoLimit();

  // Teardown fixture
  mockListener.detatch();
}

TEST_F(CommandProcessorTest,
       set_larger_limit_will_not_affect_history_list) {  // NOLINT
  // Setup fixture
  fillWithStrictMockCommands(setTestUndoLimit());

  // expectations
  // NOTE: expect that no methods on cmds will be called
  StrictMock<MockListener> mockListener{commandProcessor};

  // Exercise system
  commandProcessor->set_undo_limit(commandProcessor->undo_limit() + 1);

  // Teardown fixture
  resetUndoLimit();
  mockListener.detatch();
}

//////// undo limit test end ////////

}  // namespace tests
}  // namespace utils
