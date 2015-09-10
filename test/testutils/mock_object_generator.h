// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef TEST_TESTUTILS_MOCK_OBJECT_GENERATOR_H_
#define TEST_TESTUTILS_MOCK_OBJECT_GENERATOR_H_

#include <vector>

template <typename T>
class MockObjectGenerator {
 public:
  MockObjectGenerator() { }
  virtual ~MockObjectGenerator() {
    clear();
  }

  T* generate() {
    auto obj = new T();
    mock_objs.push_back(obj);
    return obj;
  }

  template <typename... Args>
  T* generate(Args&&... args) {
    auto obj =  new T(std::forward<Args>(args)...);
    mock_objs.push_back(obj);
    return obj;
  }

  void clear() {
    for (auto mock_obj : mock_objs) {
      delete mock_obj;
    }

    mock_objs.clear();
  }

 private:
  MockObjectGenerator(const MockObjectGenerator& other) = delete;
  MockObjectGenerator& operator=(const MockObjectGenerator& other) = delete;

  std::vector<T*> mock_objs;
};


#endif  // TEST_TESTUTILS_MOCK_OBJECT_GENERATOR_H_
