//-*- TestCaseName: MockObjectGeneratorTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"

#include "src/utils/basic_utils.h"  // make_unique, <memory>
#include "test/testutils/generic_mock_listener.h"
#include "test/testutils/slot_catcher.h"

#include "test/testutils/mock_object_generator.h"

class MockDummy {
 public:
  ~MockDummy() { destruct(); }
  MOCK_METHOD0(destruct, void());
};

class MockObjectGeneratorTest : public ::testing::Test {
 protected:
  MockObjectGeneratorTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~MockObjectGeneratorTest() { }
  virtual void SetUp() {
    generator = new MockObjectGenerator<MockDummy>();
    mock_obj1 = generator->generate();
    mock_obj2 = generator->generate();
  }
  virtual void TearDown() {
    delete generator;
  }

  MockObjectGenerator<MockDummy>* generator { nullptr };
  MockDummy* mock_obj1 { nullptr };
  MockDummy* mock_obj2 { nullptr };
};

TEST_F(MockObjectGeneratorTest, should_be_able_to_generate_different_mock_objects) { // NOLINT
  // Verify results
  ASSERT_NE(nullptr, mock_obj1);
  ASSERT_NE(nullptr, mock_obj2);
  ASSERT_NE(mock_obj1, mock_obj2);
}

TEST_F(MockObjectGeneratorTest, should_destruct_generated_mocks_when_destruct_generator) { // NOLINT
  // Expectations
  EXPECT_CALL(*mock_obj1, destruct());
  EXPECT_CALL(*mock_obj2, destruct());

  // Exercise system
  delete generator;
  generator = nullptr;
}

TEST_F(MockObjectGeneratorTest, should_be_able_to_clear_generated_mocks_and_destruct_them) { // NOLINT
  MockFunction<void(string check_point_name)> check;

  // Expectations
  {
    InSequence seq;

    EXPECT_CALL(*mock_obj1, destruct());
    EXPECT_CALL(*mock_obj2, destruct());
    EXPECT_CALL(check, Call("barrier"));
  }

  // Exercise system
  generator->clear();
  check.Call("barrier");
}
