// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_UTILS_MOCK_COMMAND_H_
#define INCLUDE_UTILS_MOCK_COMMAND_H_

#include "utils/command.h"

namespace utils {
namespace tests {

class MockCommand : public Command {
 public:
  virtual ~MockCommand() { destruct(); }

  MOCK_METHOD0(redo, void());
  MOCK_METHOD0(undo, void());
  MOCK_METHOD0(destruct, void());
  MOCK_CONST_METHOD0(display_text, utils::U8String());
  MOCK_CONST_METHOD0(help_text, utils::U8String());
  MOCK_METHOD0(getType, CmdType());
};

#if 0
void PrintTo(const MockCommand& command, ::std::ostream* os) {
  *os << command.display_text();
}
#endif

class DummyCommand : public Command {
 public:
  DummyCommand() { }
  ~DummyCommand() { }

  void redo() override { }
  void undo() override { }
};

}  // namespace tests
}  // namespace utils

#endif  // INCLUDE_UTILS_MOCK_COMMAND_H_
