// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_TEST_TEST_PROXY_H_
#define INCLUDE_TEST_TEST_PROXY_H_

// NOTE: if default construct for real object is forbided, default constructor
// in test proxy should also removed
#define TEST_PROXY_WITHOUT_DEFAULT_CONSTRUCTOR(RealClass)               \
 public:                                                                \
  virtual ~RealClass##TestProxy() {                                     \
    delete self_;                                                       \
  }                                                                     \
                                                                        \
  RealClass##TestProxy(const RealClass##TestProxy& rhs) {               \
    removeOldObj();                                                     \
    cloneObj(*(rhs.self_));                                             \
  }                                                                     \
                                                                        \
  RealClass##TestProxy(RealClass##TestProxy&& rhs) {                    \
    std::swap(self_, rhs.self_);                                        \
    std::swap(owned_,rhs.owned_);                                       \
  }                                                                     \
                                                                        \
  RealClass##TestProxy& operator=(const RealClass##TestProxy& rhs) {    \
    removeOldObj();                                                     \
    cloneObj(*(rhs.self_));                                             \
    return *this;                                                       \
  }                                                                     \
                                                                        \
  RealClass##TestProxy& operator=(RealClass##TestProxy&& rhs) {         \
    std::swap(self_, rhs.self_);                                        \
    std::swap(owned_, rhs.owned_);                                      \
    return *this;                                                       \
  }                                                                     \
                                                                        \
  RealClass##TestProxy(const RealClass& rhs) {                          \
    removeOldObj();                                                     \
    cloneObj(rhs);                                                      \
  }                                                                     \
                                                                        \
  RealClass##TestProxy& operator=(const RealClass& rhs) {               \
    removeOldObj();                                                     \
    cloneObj(rhs);                                                      \
    return *this;                                                       \
  }                                                                     \
                                                                        \
  /* Test proxy has no "Real Self" */                                   \
  RealClass* self() const {                                             \
    return self_;                                                       \
  }                                                                     \
                                                                        \
  void setSelf(RealClass* self_ptr, bool owned = true) {                \
    self_ = self_ptr;                                                   \
    owned_ = owned;                                                     \
  }                                                                     \
                                                                        \
 private:                                                               \
  void removeOldObj() {                                                 \
    if (owned_)                                                         \
      delete self_;                                                     \
  }                                                                     \
                                                                        \
  void cloneObj(const RealClass& obj) {                                 \
    self_ = obj.clone();                                                \
    owned_ = true;                                                      \
  }                                                                     \
                                                                        \
  RealClass* self_ { nullptr };                                         \
  bool owned_ { true };

// if the default constructor of real object is allowed, use this macro instead
#define TEST_PROXY_WITH_DEFAULT_CONSTRUCTOR(RealClass)  \
  public:                                               \
  RealClass##TestProxy() = default;                     \
  TEST_PROXY_WITHOUT_DEFAULT_CONSTRUCTOR(RealClass)

#endif  // INCLUDE_TEST_TEST_PROXY_H_
