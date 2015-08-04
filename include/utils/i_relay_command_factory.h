// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_UTILS_I_RELAY_COMMAND_FACTORY_H_
#define INCLUDE_UTILS_I_RELAY_COMMAND_FACTORY_H_

#include <memory>
#include <functional>

namespace utils {

class Command;

class IRelayCommandFactory {
 public:
  virtual ~IRelayCommandFactory() = default;

  virtual
  std::shared_ptr<Command> createCommand(std::function<void()> callback) const = 0;
};

}  // namespace utils

#endif  // INCLUDE_UTILS_I_RELAY_COMMAND_FACTORY_H_
