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
    delete real_obj_;                                                   \
  }                                                                     \
                                                                        \
  void removeOldObj() {                                                 \
    if (real_obj_)                                                      \
      delete real_obj_;                                                 \
  }                                                                     \
                                                                        \
  RealClass##TestProxy(const RealClass##TestProxy& rhs) {               \
    removeOldObj();                                                     \
    real_obj_ = rhs.real_obj_->clone();                                 \
  }                                                                     \
                                                                        \
  RealClass##TestProxy& operator=(const RealClass##TestProxy& rhs) {    \
    removeOldObj();                                                     \
    real_obj_ = rhs.real_obj_->clone();                                 \
    return *this;                                                       \
  }                                                                     \
                                                                        \
  RealClass* clone() const override {                                   \
    return new RealClass##TestProxy(*this);                             \
  }                                                                     \
                                                                        \
  RealClass##TestProxy(const RealClass& rhs) {                          \
    removeOldObj();                                                     \
    real_obj_ = rhs.clone();                                            \
  }                                                                     \
                                                                        \
  RealClass##TestProxy& operator=(const RealClass& rhs) {               \
    removeOldObj();                                                     \
    real_obj_ = rhs.clone();                                            \
    return *this;                                                       \
  }                                                                     \
                                                                        \
  RealClass& operator*() const noexcept {                               \
    return *real_obj_;                                                  \
  }                                                                     \
                                                                        \
  RealClass* operator->() const noexcept {                              \
    return real_obj_;                                                   \
  }                                                                     \
                                                                        \
  RealClass* operator&() const noexcept {   /* NOLINT */                \
    return real_obj_;                                                   \
  }                                                                     \
                                                                        \
  RealClass* getRealObject() const {                                    \
    return real_obj_;                                                   \
  }                                                                     \
                                                                        \
  void setRealObject(RealClass* real_obj) {                             \
    real_obj_ = real_obj;                                               \
  }                                                                     \
                                                                        \
 private:                                                               \
  RealClass* real_obj_ { nullptr };

// if the default constructor of real object is allowed, use this macro instead
#define TEST_PROXY_WITH_DEFAULT_CONSTRUCTOR(RealClass)  \
 public:                                                \
  RealClass##TestProxy() = default;                     \
  TEST_PROXY_WITHOUT_DEFAULT_CONSTRUCTOR(RealClass)

#endif  // INCLUDE_TEST_TEST_PROXY_H_
