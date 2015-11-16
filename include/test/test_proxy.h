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
#define TEST_PROXY_BASE(RealClass)                              \
 public:                                                        \
  virtual ~RealClass##TestProxy() {                             \
    if (owned_)                                                 \
      delete self_;                                             \
  }                                                             \
                                                                \
  /* Test proxy has no "Real Self" */                           \
  RealClass* self() const {                                     \
    return self_;                                               \
  }                                                             \
                                                                \
  void setSelf(RealClass* self_ptr, bool owned = true) {        \
    self_ = self_ptr;                                           \
    owned_ = owned;                                             \
  }                                                             \
                                                                \
 private:                                                       \
  RealClass* self_ { nullptr };                                 \
  bool owned_ { true };

#define TEST_PROXY_WITHOUT_DEFAULT_CONSTRUCTOR(RealClass)       \
  TEST_PROXY_BASE(RealClass)                                    \
 private:                                                       \
  RealClass##TestProxy() = delete;

#define TEST_PROXY_ENABLE_FACTORY_SUPPORT(RealClass)                    \
  public:                                                               \
  static void setFactory(const RealClass##Factory* factory) {           \
    *getFactory_() = factory;                                           \
  }                                                                     \
                                                                        \
 private:                                                               \
  static const RealClass##Factory** getFactory_() {                     \
    static const RealClass##Factory* factory_ = nullptr;                \
        return &factory_;                                               \
  }                                                                     \
                                                                        \
  static const RealClass##Factory* getFactory() {                       \
    return *getFactory_();                                              \
  }                                                                     \
                                                                        \
  template<class ... Args>                                              \
  void createInstance(Args && ... args) {                               \
    if (getFactory()) {                                                 \
      setSelf(getFactory()->createInstance(std::forward<Args> (args) ...)); \
    } else {                                                            \
      ALOGE << #RealClass " factory not set";                           \
    }                                                                   \
  }

#define TEST_PROXY_ENABLE_COPY(RealClass)                               \
 public:                                                                \
  RealClass##TestProxy(const RealClass##TestProxy& rhs) {               \
    cloneObj(*(rhs.self_));                                             \
  }                                                                     \
                                                                        \
  RealClass##TestProxy(RealClass##TestProxy && rhs) {                   \
    std::swap(self_, rhs.self_);                                        \
    std::swap(owned_, rhs.owned_);                                      \
  }                                                                     \
                                                                        \
  RealClass##TestProxy& operator=(const RealClass##TestProxy& rhs) {    \
    cloneObj(*(rhs.self_));                                             \
    return *this;                                                       \
  }                                                                     \
                                                                        \
  RealClass##TestProxy& operator=(RealClass##TestProxy && rhs) {        \
    std::swap(self_, rhs.self_);                                        \
    std::swap(owned_, rhs.owned_);                                      \
    return *this;                                                       \
  }                                                                     \
                                                                        \
  RealClass##TestProxy(const RealClass& rhs) {                          \
    cloneObj(rhs);                                                      \
  }                                                                     \
                                                                        \
  RealClass##TestProxy& operator=(const RealClass& rhs) {               \
    cloneObj(rhs);                                                      \
    return *this;                                                       \
  }                                                                     \
                                                                        \
  RealClass##TestProxy* clone() const {                                 \
    return new RealClass##TestProxy(*this);                             \
  }                                                                     \
                                                                        \
 private:                                                               \
  void cloneObj(const RealClass& obj) {                                 \
    if (owned_) {                                                       \
      delete self_;                                                     \
    }
                                                                        \
    self_ = obj.clone();                                                \
    owned_ = true;                                                      \
  }                                                                     \

#define TEST_PROXY_DISABLE_COPY(RealClass)                              \
 private:                                                               \
  RealClass##TestProxy(const RealClass##TestProxy& rhs) = delete;       \
  RealClass##TestProxy(RealClass##TestProxy && rhs)= delete;            \
  RealClass##TestProxy& operator=(const RealClass##TestProxy& rhs) = delete; \
  RealClass##TestProxy& operator=(RealClass##TestProxy && rhs) = delete; \
  RealClass##TestProxy(const RealClass& rhs);                           \
  RealClass##TestProxy& operator=(const RealClass& rhs);                \

// if the default constructor of real object is allowed, use this macro instead
#define TEST_PROXY_WITH_DEFAULT_CONSTRUCTOR(RealClass)  \
  public:                                               \
  RealClass##TestProxy() = default;                     \
  TEST_PROXY_BASE(RealClass)

#endif  // INCLUDE_TEST_TEST_PROXY_H_
