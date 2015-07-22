// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_UTILS_COMMAND_RELAY_COMMAND_H_
#define SRC_UTILS_COMMAND_RELAY_COMMAND_H_

#include <functional>

#include "utils/command.h"
#include "utils/u8string.h"

namespace utils {

class RelayCommand : public Command {
 public:
  using CallbackFunctor = std::function<void()>;

  RelayCommand(const utils::U8String& display_text,
               CallbackFunctor callback)
      : display_text_(display_text)
      , callback_(callback) { }
  virtual ~RelayCommand() = default;


  void set_display_text(const utils::U8String& new_text) {
    display_text_ = new_text;
  }

  utils::U8String display_text() const override {
    return display_text_;
  }

  void redo() override {
    callback_();
  }

 private:
  utils::U8String display_text_;
  CallbackFunctor callback_;
};

}  // namespace utils

#endif  // SRC_UTILS_COMMAND_RELAY_COMMAND_H_
