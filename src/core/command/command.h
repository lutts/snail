// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_COMMAND_COMMAND_H_
#define SRC_CORE_COMMAND_COMMAND_H_

namespace snailcore {

class Command {
 public:
  enum CmdType {
    NORMAL,
    NO_CHANGE
  };
  virtual ~Command() { }
  virtual void redo() = 0;
  virtual void undo() = 0;
  virtual CmdType getType() { return NORMAL; }
};

}  // namespace snailcore

#endif  // SRC_CORE_COMMAND_COMMAND_H_
