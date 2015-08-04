// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_UTILS_MOCK_RELAY_COMMAND_FACTORY_H_
#define INCLUDE_UTILS_MOCK_RELAY_COMMAND_FACTORY_H_

#include "utils/i_relay_command_factory.h"

namespace utils {
namespace tests {

class MockRelayCommandFactory : public IRelayCommandFactory {
 public:
  MOCK_CONST_METHOD1(createCommand,
               std::shared_ptr<Command>(std::function<void()> callback));
};

}  // namespace tests
}  // namespace utils

#endif  // INCLUDE_UTILS_MOCK_RELAY_COMMAND_FACTORY_H_
