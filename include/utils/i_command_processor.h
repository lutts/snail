// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_SNAIL_I_COMMAND_PROCESSOR_H_
#define INCLUDE_SNAIL_I_COMMAND_PROCESSOR_H_

#include "utils/signal_slot.h"

namespace utils {

class Command;

class ICommandProcessor {
 public:
  virtual ~ICommandProcessor() { }

  SNAIL_SIGSLOT2(CanUndoChanged, void(bool canUndo));
  SNAIL_SIGSLOT2(CanRedoChanged, void(bool canRedo));

  virtual bool canUndo() const = 0;
  virtual bool canRedo() const = 0;

  virtual void undo() = 0;
  virtual void redo() = 0;

  virtual void do_cmd(Command *cmd) = 0;
};

}  // namespace utils

#endif  // INCLUDE_SNAIL_I_COMMAND_PROCESSOR_H_
