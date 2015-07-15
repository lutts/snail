// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// currently we use Limited Linear Undo facility,
// but we may change some later time.

#include "src/utils/command/command_processor.h"
#include "utils/command.h"

namespace utils {

constexpr int CommandProcessor::UNDO_UNLIMITED;

CommandProcessor::CommandProcessor() { }

CommandProcessor::~CommandProcessor() {
  clear();
}

void CommandProcessor::do_cmd(Command *cmd) {
  cmd->redo();

  if (cmd->getType() == Command::NO_CHANGE) {
    delete cmd;
    return;
  }

  std::shared_ptr<Command> sp_cmd(cmd);
  bool old_canUndo = canUndo();
  bool old_canRedo = canRedo();
  done_stack.push_back(sp_cmd);

  while (!undo_stack.empty()) {
    undo_stack.pop_back();
  }

  if (undo_limit_ && (count() > undo_limit_)) {
    done_stack.pop_front();
  }

  notifyUndoChanged(old_canUndo);
  notifyRedoChanged(old_canRedo);
}

bool CommandProcessor::canRedo() const {
  return !undo_stack.empty();
}

void CommandProcessor::redo() {
  if (canRedo()) {
    bool old_canUndo = canUndo();

    std::shared_ptr<Command> last_undo_cmd = undo_stack.back();
    last_undo_cmd->redo();
    undo_stack.pop_back();
    done_stack.push_back(last_undo_cmd);

    notifyUndoChanged(old_canUndo);
    notifyRedoChanged(true);
  }
}

bool CommandProcessor::canUndo() const {
  return !done_stack.empty();
}

void CommandProcessor::undo() {
  if (canUndo()) {
    bool old_canRedo = canRedo();

    std::shared_ptr<Command> last_done_cmd = done_stack.back();
    last_done_cmd->undo();
    done_stack.pop_back();
    undo_stack.push_back(last_done_cmd);

    notifyUndoChanged(true);
    notifyRedoChanged(old_canRedo);
  }
}

void CommandProcessor::clear() {
  bool old_canUndo = canUndo();
  bool old_canRedo = canRedo();

  done_stack.clear();

  // although we use too lists for implementation convenience,
  // we must simulate the behavior of the "classical"
  // Limited Linear Undo facility as if we have only one list!
  while (!undo_stack.empty()) {
    undo_stack.pop_back();
  }

  notifyUndoChanged(old_canUndo);
  notifyRedoChanged(old_canRedo);
}

int CommandProcessor::count() const {
  return done_stack.size() + undo_stack.size();
}

void CommandProcessor::set_undo_limit(int limit) {
  if (limit == 0) return;

  undo_limit_ = limit;

  if (limit != UNDO_UNLIMITED) {
    int del_count = count() - limit;

    if (del_count > 0) {
      bool old_canUndo = canUndo();
      for (; !done_stack.empty() && del_count; --del_count) {
        done_stack.pop_front();
      }
      notifyUndoChanged(old_canUndo);

      for (; !undo_stack.empty() && del_count; --del_count) {
        undo_stack.pop_back();
      }
      // because new limit will not be 0, so canRedo will not change
    }
  }
}

int CommandProcessor::undo_limit() const {
  return undo_limit_;
}

void CommandProcessor::notifyRedoChanged(bool old_canRedo) {
  bool new_canRedo = canRedo();

  if (new_canRedo != old_canRedo) {
    CanRedoChanged(new_canRedo);
  }
}

void CommandProcessor::notifyUndoChanged(bool old_canUndo) {
  bool new_canUndo = canUndo();

  if (new_canUndo != old_canUndo) {
    CanUndoChanged(new_canUndo);
  }
}

}  // namespace utils
