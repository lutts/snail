// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_CORE_APPLICATION_FACTORY_H_
#define SRC_CORE_APPLICATION_FACTORY_H_

#include <memory>

namespace snailcore {

class ApplicationFactoryImpl;
class IMainWindowModel;

class ApplicationFactory {
 public:
  ApplicationFactory();
  virtual ~ApplicationFactory();

  static std::shared_ptr<ApplicationFactory> create();

  std::shared_ptr<IMainWindowModel> makeMainWindowModel();

 private:
  ApplicationFactory(const ApplicationFactory& other) = delete;
  ApplicationFactory& operator=(const ApplicationFactory& other) = delete;

  std::unique_ptr<ApplicationFactoryImpl> impl_;
  friend class ApplicationFactoryImpl;
};

}  // namespace snailcore

#endif  // SRC_CORE_APPLICATION_FACTORY_H_
