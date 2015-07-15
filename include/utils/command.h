// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_UTILS_COMMAND_COMMAND_H_
#define SRC_UTILS_COMMAND_COMMAND_H_

#include "utils/u8string.h"

namespace utils {

class Command {
 public:
  enum CmdType {
    NORMAL,
    NO_CHANGE
  };
  virtual ~Command() { }
  virtual void redo() = 0;
  virtual void undo() { }

  virtual utils::U8String display_text() { return utils::U8String(""); }
  virtual utils::U8String help_text() { return utils::U8String(""); };

  virtual CmdType getType() { return NORMAL; }
};

}  // namespace utils

#endif  // SRC_UTILS_COMMAND_COMMAND_H_
