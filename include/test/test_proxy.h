// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_TEST_TEST_PROXY_H_
#define INCLUDE_TEST_TEST_PROXY_H_

#include <algorithm>
#include <iostream>  // NOLINT

#include "src/utils/log/log.h"
#include "utils/stacktrace.h"

class TestProxyLogger {
 public:
  TestProxyLogger(bool en) {
    saved_enabled_ = enabled_();
    enabled_() = en;
  }

  ~TestProxyLogger() { enabled_() = saved_enabled_; }

 private:
  bool saved_enabled_;

 public:
  static void enable() { enabled_() = true; }

  static void disable() { enabled_() = false; }

  static bool enabled() { return enabled_(); }

 private:
  static bool& enabled_() {
    static bool var = false;
    return var;
  }
};

/** The Most Important Thing to use TestProxy:
 *    * remember mock objects SHOULD NOT COPIED, NOT MOVED
 *    * self_ is BINDed to a proxy, aka, is a self_ pointer is set to a
 *      TestProxy, it will not transfer to another TestProxy
 */

// NOTE: if default construct for real object is forbided, default constructor
// in test proxy should also removed
#define TEST_PROXY_BASE(RealClass)                       \
 public:                                                 \
  virtual ~RealClass##TestProxy() {                      \
    if (owned_) delete self_;                            \
  }                                                      \
                                                         \
  /* Test proxy has no "Real Self" */                    \
  RealClass* self() const {                              \
    if (!self_) {                                        \
      Stacktrace::print_stacktrace();                    \
    }                                                    \
    return self_;                                        \
  }                                                      \
                                                         \
  void setSelf(RealClass* self_ptr, bool owned = true) { \
    self_ = self_ptr;                                    \
    owned_ = owned;                                      \
  }                                                      \
                                                         \
 private:                                                \
  RealClass* self_{nullptr};                             \
  bool owned_{true};

#define TEST_PROXY_ENABLE_SWAP(RealClass)                                \
 public:                                                                 \
  friend inline void swap(RealClass##TestProxy& v1,                      \
                          RealClass##TestProxy& v2) {                    \
    if (TestProxyLogger::enabled()) {                                    \
      std::cout << #RealClass "TestProxy: swap, v1.self_ = " << v1.self_ \
                << ", v2.self_ = " << v2.self_ << std::endl;             \
    }                                                                    \
    /* Note that v1.self_ can not be nullptr */                          \
    if (v2.self_) v1.self()->swap_with(*v2.self());                      \
  }

// swap declared, but not defined
#define TEST_PROXY_DISABLE_SWAP(RealClass) \
  friend void swap(RealClass##TestProxy& v1, RealClass##TestProxy& v2);

#define TEST_PROXY_WITHOUT_DEFAULT_CONSTRUCTOR(RealClass) \
  TEST_PROXY_BASE(RealClass)                              \
 private:                                                 \
  RealClass##TestProxy() = delete;

#define TEST_PROXY_ENABLE_FACTORY_SUPPORT(RealClass)                        \
 public:                                                                    \
  static void setFactory(const RealClass##Factory* factory) {               \
    *getFactory_() = factory;                                               \
  }                                                                         \
                                                                            \
 private:                                                                   \
  static const RealClass##Factory** getFactory_() {                         \
    static const RealClass##Factory* factory_ = nullptr;                    \
    return &factory_;                                                       \
  }                                                                         \
                                                                            \
  static const RealClass##Factory* getFactory() { return *getFactory_(); }  \
                                                                            \
  template <class... Args>                                                  \
  void createInstance(Args&&... args) {                                     \
    if (getFactory()) {                                                     \
      auto obj = getFactory()->createInstance(std::forward<Args>(args)...); \
      if (TestProxyLogger::enabled()) {                                     \
        std::cout << #RealClass "TestProxy: createInstance: obj = " << obj  \
                  << std::endl;                                             \
      }                                                                     \
      setSelf(obj);                                                         \
    } else {                                                                \
      if (TestProxyLogger::enabled()) {                                     \
        std::cout << #RealClass "TestProxy: " #RealClass " factory not set" \
                  << std::endl;                                             \
      }                                                                     \
      ALOGE << #RealClass " factory not set";                               \
    }                                                                       \
  }

// NOTE: clone()/moveClone() return nullptr is legal, this can happen when use
// unified assignment idom.  when use unified assignment idom, a temporary
// intermediary will created, which will cause clone() or moveClone() be
// called. Under test environment, we do not care this intermediary object,
// so we do not care clone() or moveClone() return nullptr at this time.
//
// see also swap();
//
#define TEST_PROXY_ENABLE_COPY_CONSTRUCT(RealClass)                            \
 public:                                                                       \
  RealClass##TestProxy(const RealClass##TestProxy& rhs) {                      \
    if (TestProxyLogger::enabled()) {                                          \
      std::cout << #RealClass "TestProxy: copy construct, rhs.self_ = "        \
                << rhs.self_ << std::endl;                                     \
    }                                                                          \
                                                                               \
    auto obj = rhs.self()->clone();                                            \
    setSelf(obj);                                                              \
                                                                               \
    if (TestProxyLogger::enabled()) {                                          \
      std::cout << #RealClass "TestProxy: clone() ret = " << obj << std::endl; \
    }                                                                          \
  }                                                                            \
                                                                               \
  RealClass##TestProxy(const RealClass& rhs) { setSelf(rhs.clone()); }         \
                                                                               \
  RealClass##TestProxy* clone() const {                                        \
    return new RealClass##TestProxy(*this);                                    \
  }

#define TEST_PROXY_DISABLE_COPY_CONSTRUCT(RealClass)              \
 private:                                                         \
  RealClass##TestProxy(const RealClass##TestProxy& rhs) = delete; \
  RealClass##TestProxy(const RealClass& rhs);

#define TEST_PROXY_ENABLE_COPY_ASSIGNMENT(RealClass)                           \
 public:                                                                       \
  RealClass##TestProxy& operator=(const RealClass##TestProxy& rhs) {           \
    if (TestProxyLogger::enabled()) {                                          \
      std::cout << #RealClass "TestProxy: copy assignment, myself = " << self_ \
                << ", rhs.self_ = " << rhs.self_ << std::endl;                 \
    }                                                                          \
                                                                               \
    self()->operator=(*rhs.self());                                            \
    return *this;                                                              \
  }                                                                            \
                                                                               \
  RealClass##TestProxy& operator=(const RealClass& rhs) {                      \
    self()->operator=(rhs);                                                    \
    return *this;                                                              \
  }

#define TEST_PROXY_DISABLE_COPY_ASSIGNMENT(RealClass)                        \
 private:                                                                    \
  RealClass##TestProxy& operator=(const RealClass##TestProxy& rhs) = delete; \
  RealClass##TestProxy& operator=(const RealClass& rhs);

// moveClone() return nullptr is legal, see copy construct comment above
#define TEST_PROXY_ENABLE_MOVE_CONSTRUCT(RealClass)                     \
 public:                                                                \
  RealClass##TestProxy(RealClass##TestProxy&& rhs) { /* NOLINT */       \
    if (TestProxyLogger::enabled()) {                                   \
      std::cout << #RealClass "TestProxy: move construct, rhs.self_ = " \
                << rhs.self_ << std::endl;                              \
    }                                                                   \
                                                                        \
    auto obj = rhs.self()->moveClone();                                 \
    setSelf(obj);                                                       \
                                                                        \
    if (TestProxyLogger::enabled()) {                                   \
      std::cout << #RealClass "TestProxy: moveClone() ret = " << obj    \
                << std::endl;                                           \
    }                                                                   \
  }                                                                     \
                                                                        \
  RealClass##TestProxy(RealClass&& rhs) { /* NOLINT */                  \
    self_ = rhs.moveClone();                                            \
  }                                                                     \
                                                                        \
  RealClass##TestProxy* moveClone() const {                             \
    return new RealClass##TestProxy(std::move(*this));                  \
  }

#define TEST_PROXY_DISABLE_MOVE_CONSTRUCT(RealClass)                      \
 private:                                                                 \
  RealClass##TestProxy(RealClass##TestProxy&& rhs) = delete; /* NOLINT */ \
  RealClass##TestProxy(RealClass&& rhs);                     /* NOLINT */

#define TEST_PROXY_ENABLE_MOVE_ASSIGNMENT(RealClass)                           \
  RealClass##TestProxy& operator=(RealClass##TestProxy&& rhs) { /* NOLINT */   \
    if (TestProxyLogger::enabled()) {                                          \
      std::cout << #RealClass "TestProxy: move assignment, myself = " << self_ \
                << ", rhs.self_ = " << rhs.self_ << std::endl;                 \
    }                                                                          \
    self()->operator=(std::move(*rhs.self()));                                 \
    return *this;                                                              \
  }                                                                            \
                                                                               \
  RealClass##TestProxy& operator=(RealClass&& rhs) {                           \
    self()->operator=(std::move(rhs));                                         \
    return *this;                                                              \
  }

#define TEST_PROXY_DISABLE_MOVE_ASSIGNMENT(RealClass)                        \
 private:                                                                    \
  RealClass##TestProxy& operator=(RealClass##TestProxy&& rhs) /* NOLINT */ = \
      delete;                                                                \
  RealClass##TestProxy& operator=(RealClass&& rhs);

// if the default constructor of real object is allowed, use this macro instead
#define TEST_PROXY_WITH_DEFAULT_CONSTRUCTOR(RealClass) \
 public:                                               \
  RealClass##TestProxy() = default;                    \
  TEST_PROXY_BASE(RealClass)

// impl helpers
#define TEST_PROXY_CLONE_INTERFACE(Cls) SNAIL_CONST_INTERFACE0(clone, Cls*())
#define TEST_PROXY_CLONE_MOCK(Cls) TEST_PROXY_CLONE_INTERFACE(Cls)

#define TEST_PROXY_COPY_ASSIGN_INTERFACE(Cls) \
  virtual Cls& operator=(const Cls& rhs) = 0

#define TEST_PROXY_COPY_ASSIGN_MOCK(Cls)    \
  Cls& operator=(const Cls& rhs) override { \
    copy_assignment(rhs);                   \
    return *this;                           \
  }                                         \
                                            \
  MOCK_METHOD1(copy_assignment, void(const Cls& rhs));

#define TEST_PROXY_MOVE_CLONE_INTERFACE(Cls) SNAIL_INTERFACE0(moveClone, Cls*())
#define TEST_PROXY_MOVE_CLONE_MOCK(Cls) TEST_PROXY_MOVE_CLONE_INTERFACE(Cls)

#define TEST_PROXY_MOVE_ASSIGN_INTERFACE(Cls) \
  virtual Cls& operator=(Cls&& rhs) = 0

#define TEST_PROXY_MOVE_ASSIGN_MOCK(Cls) \
  Cls& operator=(Cls&& rhs) override {   \
    move_assignment(rhs);                \
    return *this;                        \
  }                                      \
                                         \
  MOCK_METHOD1(move_assignment, void(Cls & rhs));

#define TEST_PROXY_SWAP_INTERFACE(Cls) \
  SNAIL_INTERFACE1(swap_with, void(Cls & rhs))

#define TEST_PROXY_SWAP_MOCK(Cls) TEST_PROXY_SWAP_INTERFACE(Cls)

#endif  // INCLUDE_TEST_TEST_PROXY_H_
