// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_COMMAND_COMMAND_PROCESSOR_H_
#define SRC_CORE_COMMAND_COMMAND_PROCESSOR_H_

#include <list>
#include <memory>

#include "utils/i_command_processor.h"
#include "utils/signal_slot_impl.h"

namespace utils {

// TODO(lutts): CommandProcessor&AbstractCommand may be in its own package
class Command;

// CommandProcessor of CommandProcessor Pattern
class CommandProcessor : public ICommandProcessor {
 public:
  CommandProcessor();
  ~CommandProcessor();

  // call cmd->redo(), according the cmd type, CommandProcessor
  // will take ownership of this \a cmd or not
  //   * NORMAL: take ownership
  //   * NO_CHANGE: will delete this cmd object
  void do_cmd(Command *cmd) override;

  bool canRedo() const override;
  void redo() override;  // redo the last undo command(s)

  bool canUndo() const override;
  void undo() override;  // undo the last command

  // Clears the command stack by deleting all commands on it, and
  // returns the stack to the clean state.
  //
  // Commands are not undone or redone;
  // the state of the edited object remains unchanged.
  //
  // This function is usually used when the contents of the
  // document are abandoned.
  void clear();

  // total command count managed by this processor,
  // include done and undone commands
  int count() const;

  void set_undo_limit(int limit);
  int undo_limit() const;

  static constexpr int UNDO_UNLIMITED = 0;

 private:
  CommandProcessor(const CommandProcessor& other) = delete;
  CommandProcessor& operator=(const CommandProcessor& other) = delete;

  void notifyRedoChanged(bool old_canRedo);
  void notifyUndoChanged(bool old_canUndo);

  // use two list to simulate Limited Linear Undo facility
  //
  //                             top
  //                              v
  // done begin  ----- done end  ---- undo end  ---- undo begin
  // oldest done ----- last done ---- last undo ---- oldest undo
  //
  std::list<std::shared_ptr<Command>> done_stack;
  std::list<std::shared_ptr<Command>> undo_stack;

  int undo_limit_ { UNDO_UNLIMITED };

 private:
  SNAIL_SIGSLOT_IMPL(CanUndoChanged)
  SNAIL_SIGSLOT_IMPL(CanRedoChanged)
};

}  // namespace utils

#endif  // SRC_CORE_COMMAND_COMMAND_PROCESSOR_H_
