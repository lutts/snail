// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef TEST_CORE_COMMAND_MOCK_COMMAND_H_
#define TEST_CORE_COMMAND_MOCK_COMMAND_H_

#include "src/utils/command/command.h"

namespace utils {
namespace tests {

class MockCommand : public Command {
 public:
  virtual ~MockCommand() { destruct(); }

  MOCK_METHOD0(redo, void());
  MOCK_METHOD0(undo, void());
  MOCK_METHOD0(destruct, void());
  MOCK_METHOD0(getType, CmdType());
};

class DummyCommand : public Command {
 public:
  DummyCommand() { }
  ~DummyCommand() { }

  void redo() override { }
  void undo() override { }
};

}  // namespace tests
}  // namespace utils

#endif  // TEST_CORE_COMMAND_MOCK_COMMAND_H_
