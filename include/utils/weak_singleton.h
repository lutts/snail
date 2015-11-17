// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_UTILS_WEAK_SINGLETON_H_
#define INCLUDE_UTILS_WEAK_SINGLETON_H_

#include <memory>

template <typename Type>
class WeakSingleton {
 public:
  static std::shared_ptr<Type> getInstance() {
    // TODO(lutts): scope lock here

    auto instance = getWeakInstance().lock();
    if (!instance) {
      instance = std::make_shared<Type>();
      setWeakInstance(instance);
    }

    return instance;
  }

  static bool hasInstance() { return getWeakInstance().expired(); }

 protected:
  WeakSingleton() {}
  ~WeakSingleton() {}

 private:
  WeakSingleton(const WeakSingleton& other) = delete;
  WeakSingleton& operator=(const WeakSingleton& other) = delete;

  static std::weak_ptr<Type>& getWeakInstance() {
    static std::weak_ptr<Type> sInstance;
    return sInstance;
  }

  static void setWeakInstance(const std::shared_ptr<Type>& instance) {
    getWeakInstance() = instance;
  }
};

#endif  // INCLUDE_UTILS_WEAK_SINGLETON_H_
