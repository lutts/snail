//-*- TestCaseName: MoveTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "test/testutils/gmock_common.h"
#include "include/config.h"
#include "test/test_proxy.h"

class IData {
 public:
  virtual ~IData() = default;

  virtual void setData(int data) = 0;
  virtual int getData() const = 0;
  virtual IData* clone() const = 0;
  virtual IData& operator=(IData&& rhs) = 0;
};

class MockData : public IData {
 public:
  MOCK_METHOD1(setData, void(int data));
  MOCK_CONST_METHOD0(getData, int());
  MOCK_CONST_METHOD0(clone, IData*());

  IData& operator=(IData&& rhs) override {
    moveFrom(rhs);
    return *this;
  }
  MOCK_METHOD1(moveFrom, void(IData& rhs));  // NOLINT
};

class IDataTestProxy {
  TEST_PROXY_WITHOUT_DEFAULT_CONSTRUCTOR(IData);
  TEST_PROXY_ENABLE_COPY(IData);

 public:
  void setData(int data) { self_->setData(data); }

  int getData() const { return self_->getData(); }
};

class Data final : public IData {
 public:
  Data() : Data(0) {}
  explicit Data(int data) : data_(new int(data)) {
    std::cout << "constructor called, data_ = " << data_ << std::endl;
  }
  virtual ~Data() { delete data_; }

  Data(const Data& rhs) : data_(new int(*rhs.data_)) {
    std::cout << "copy constructor called" << std::endl;
  }

  Data(Data&& rhs) : data_(rhs.data_) {
    rhs.data_ = nullptr;
    std::cout << "move constructor called" << std::endl;
  }

  Data& operator=(Data rhs) {
    std::cout << "unified move assignment swaping..." << std::endl;
    swap(rhs);
    std::cout << "unified move assignment called" << std::endl;
    return *this;
  }

  IData* clone() const override { return new Data(*this); }

  // so this method becomes "for test only"
  IData& operator=(IData&& rhs) {
    std::cout << "fto move assignment called" << std::endl;
    // NOTE: this cast is safe only when Data is the only subclass of IData
    // and Data itself Does not have any subclasses
    Data& data = static_cast<Data&>(rhs);
    swap(data);
    return *this;
  }

  void setData(int data) override { *data_ = data; }

  int getData() const override { return *data_; }

  int* getDataPtr() { return data_; }

 private:
  void swap(Data& rhs) { std::swap(data_, rhs.data_); }

  int* data_{nullptr};
};

class IDataHolder {
 public:
  virtual ~IDataHolder() = default;

  virtual IData* getData() = 0;
  virtual void setData(IData&& data) = 0;
};

class MockDataHolder : public IDataHolder {
 public:
  MOCK_METHOD0(getData, IData*());

  void setData(IData&& data) override { setData_(data); }
  MOCK_METHOD1(setData_, void(IData& data));  // NOLINT
};

class DataHolder : public IDataHolder {
 public:
  explicit DataHolder(IData* data) : data_(data) {}

  IData* getData() override { return data_; }

  void setData(IData&& data) override {
    *data_ = std::move(data);
    // data_->moveFrom(std::move(data));
  }

 private:
  IData* data_;
};

class DataDisplayer {
 public:
  explicit DataDisplayer(IDataHolder* data_holder)
      : data_holder_(data_holder), data_copy_(*(data_holder->getData())) {}

  void printData() const {
    std::cout << "Display: data = " << data_copy_.getData() << std::endl;
  }

  void setData(int data) { data_copy_.setData(data); }

  void saveData() { data_holder_->setData(std::move(*data_copy_.self())); }

 private:
  IDataHolder* data_holder_{nullptr};
  IDataTestProxy data_copy_;
};

class MoveTest : public ::testing::Test {
 protected:
  MoveTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~MoveTest() { }
  void SetUp() override {
    data_copy = new MockData();
    EXPECT_CALL(data_orig, clone()).WillOnce(Return(data_copy));
    EXPECT_CALL(data_holder, getData()).WillOnce(Return(&data_orig));

    data_displayer = utils::make_unique<DataDisplayer>(&data_holder);
  }
  // void TearDown() override { }

  // region: objects test subject depends on
  MockData data_orig;
  MockData* data_copy;
  MockDataHolder data_holder;
  // endregion

  // region: test subject
  std::unique_ptr<DataDisplayer> data_displayer;
  // endregion

  // region: object depends on test subject
  // endregion
};

TEST_F(MoveTest, should_construct_properly) {  // NOLINT
  // See SetUp()
  SUCCEED();
}

TEST_F(MoveTest, test_setData) {  // NOLINT
  // Setup fixture
  int data = std::rand();

  // Expectations
  EXPECT_CALL(*data_copy, setData(data));

  // Exercise system
  data_displayer->setData(data);
}

TEST_F(MoveTest, test_saveData) {  // NOLINT
  // Expectations
  EXPECT_CALL(data_holder, setData_(Ref(*data_copy)));

  // Exercise system
  data_displayer->saveData();
}

class DataHolderTest : public ::testing::Test {
 protected:
  DataHolderTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~DataHolderTest() { }
  void SetUp() override {
    data_holder = utils::make_unique<DataHolder>(&data);

    ASSERT_EQ(&data, data_holder->getData());
  }
  // void TearDown() override { }

  // region: objects test subject depends on
  MockData data;
  // endregion

  // region: test subject
  std::unique_ptr<DataHolder> data_holder;
  // endregion

  // region: object depends on test subject
  // endregion
};

TEST_F(DataHolderTest, test_setData) {  // NOLINT
  // Setup fixture
  MockData another_data;

  // Expectations
  EXPECT_CALL(data, moveFrom(Ref(another_data)));

  // Exercise system
  data_holder->setData(std::move(another_data));
}

class DataTest : public ::testing::Test {
 protected:
  DataTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~DataTest() { }
  void SetUp() override {
    expect_i = std::rand();

    data = utils::make_unique<Data>(expect_i);

    ASSERT_EQ(expect_i, data->getData());
  }
  // void TearDown() override { }

  // region: objects test subject depends on
  int expect_i;
  // endregion

  // region: test subject
  std::unique_ptr<Data> data;
  // endregion

  // region: object depends on test subject
  // endregion
};

TEST_F(DataTest, test_setData) {  // NOLINT
  // Setup fixture
  auto expect_data = std::rand();

  // Exercise system
  data->setData(expect_data);

  // Verify results
  auto actual_data = data->getData();
  ASSERT_EQ(expect_data, actual_data);
}

TEST_F(DataTest, test_moveFrom) {  // NOLINT
  // Setup fixture
  int another_i = expect_i + std::rand();
  Data another_data{another_i};
  ASSERT_EQ(another_i, another_data.getData());

  // Exercise system
  *data = std::move(another_data);

  // Verify results
  ASSERT_EQ(nullptr, another_data.getDataPtr());
  ASSERT_EQ(another_i, data->getData());
}
