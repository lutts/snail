// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_TEST_TEST_PROXY_H_
#define INCLUDE_TEST_TEST_PROXY_H_

#include <algorithm>

#include "src/utils/log/log.h"

// NOTE: if default construct for real object is forbided, default constructor
// in test proxy should also removed
#define TEST_PROXY_BASE(RealClass)                       \
 public:                                                 \
  virtual ~RealClass##TestProxy() {                      \
    if (owned_) delete self_;                            \
  }                                                      \
                                                         \
  /* Test proxy has no "Real Self" */                    \
  RealClass* self() const { return self_; }              \
                                                         \
  void setSelf(RealClass* self_ptr, bool owned = true) { \
    self_ = self_ptr;                                    \
    owned_ = owned;                                      \
  }                                                      \
                                                         \
  void swap(RealClass##TestProxy& rhs) {                 \
    std::swap(self_, rhs.self_);                         \
    std::swap(owned_, rhs.owned_);                       \
  }                                                      \
                                                         \
  friend inline void swap(RealClass##TestProxy& v1,      \
                          RealClass##TestProxy& v2) {    \
    v1.swap(v2);                                         \
  }                                                      \
                                                         \
 private:                                                \
  void cloneObj(const RealClass& obj) {                  \
    if (owned_) {                                        \
      delete self_;                                      \
    }                                                    \
                                                         \
    self_ = obj.clone();                                 \
    owned_ = true;                                       \
  }                                                      \
  RealClass* self_{nullptr};                             \
  bool owned_{true};

#define TEST_PROXY_WITHOUT_DEFAULT_CONSTRUCTOR(RealClass) \
  TEST_PROXY_BASE(RealClass)                              \
 private:                                                 \
  RealClass##TestProxy() = delete;

#define TEST_PROXY_ENABLE_FACTORY_SUPPORT(RealClass)                       \
 public:                                                                   \
  static void setFactory(const RealClass##Factory* factory) {              \
    *getFactory_() = factory;                                              \
  }                                                                        \
                                                                           \
 private:                                                                  \
  static const RealClass##Factory** getFactory_() {                        \
    static const RealClass##Factory* factory_ = nullptr;                   \
    return &factory_;                                                      \
  }                                                                        \
                                                                           \
  static const RealClass##Factory* getFactory() { return *getFactory_(); } \
                                                                           \
  template <class... Args>                                                 \
  void createInstance(Args&&... args) {                                    \
    if (getFactory()) {                                                    \
      setSelf(getFactory()->createInstance(std::forward<Args>(args)...));  \
    } else {                                                               \
      ALOGE << #RealClass " factory not set";                              \
    }                                                                      \
  }

#define TEST_PROXY_ENABLE_COPY_CONSTRUCT(RealClass)             \
 public:                                                        \
  RealClass##TestProxy(const RealClass##TestProxy& rhs) {       \
    cloneObj(*(rhs.self_));                                     \
  }                                                             \
  RealClass##TestProxy(const RealClass& rhs) { cloneObj(rhs); } \
                                                                \
  RealClass##TestProxy* clone() const {                         \
    return new RealClass##TestProxy(*this);                     \
  }

#define TEST_PROXY_DISABLE_COPY_CONSTRUCT(RealClass)              \
 private:                                                         \
  RealClass##TestProxy(const RealClass##TestProxy& rhs) = delete; \
  RealClass##TestProxy(const RealClass& rhs);

#define TEST_PROXY_ENABLE_COPY_ASSIGNMENT(RealClass)                 \
 public:                                                             \
  RealClass##TestProxy& operator=(const RealClass##TestProxy& rhs) { \
    cloneObj(*(rhs.self_));                                          \
    return *this;                                                    \
  }

#define TEST_PROXY_DISABLE_COPY_ASSIGNMENT(RealClass)                        \
 private:                                                                    \
  RealClass##TestProxy& operator=(const RealClass##TestProxy& rhs) = delete; \
  RealClass##TestProxy& operator=(const RealClass& rhs);

#define TEST_PROXY_ENABLE_MOVE_CONSTRUCT(RealClass)               \
 public:                                                          \
  RealClass##TestProxy(RealClass##TestProxy&& rhs) { /* NOLINT */ \
    swap(rhs);                                                    \
  }

#define TEST_PROXY_DISABLE_MOVE_CONSTRUCT(RealClass) \
 private:                                            \
  RealClass##TestProxy(RealClass##TestProxy&& rhs) = delete; /* NOLINT */

#define TEST_PROXY_ENABLE_MOVE_ASSIGNMENT(RealClass)                         \
  RealClass##TestProxy& operator=(RealClass##TestProxy&& rhs) { /* NOLINT */ \
    swap(rhs);                                                               \
    return *this;                                                            \
  }                                                                          \
                                                                             \
  RealClass##TestProxy& operator=(const RealClass& rhs) {                    \
    cloneObj(rhs);                                                           \
    return *this;                                                            \
  }

#define TEST_PROXY_DISABLE_MOVE_ASSIGNMENT(RealClass)                        \
 private:                                                                    \
  RealClass##TestProxy& operator=(RealClass##TestProxy&& rhs) /* NOLINT */ = \
      delete;

// if the default constructor of real object is allowed, use this macro instead
#define TEST_PROXY_WITH_DEFAULT_CONSTRUCTOR(RealClass) \
 public:                                               \
  RealClass##TestProxy() = default;                    \
  TEST_PROXY_BASE(RealClass)

#endif  // INCLUDE_TEST_TEST_PROXY_H_
