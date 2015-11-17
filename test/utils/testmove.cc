// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#include <iostream>  // NOLINT
#include <utility>

// NOTE: if default construct for real object is forbided, default constructor
// in test proxy should also removed
#define TEST_PROXY_WITHOUT_DEFAULT_CONSTRUCTOR(RealClass)            \
 public:                                                             \
  virtual ~RealClass##TestProxy() { delete self_; }                  \
                                                                     \
  RealClass##TestProxy(const RealClass##TestProxy& rhs) {            \
    removeOldObj();                                                  \
    cloneObj(*(rhs.self_));                                          \
  }                                                                  \
                                                                     \
  RealClass##TestProxy& operator=(const RealClass##TestProxy& rhs) { \
    removeOldObj();                                                  \
    cloneObj(*(rhs.self_));                                          \
    return *this;                                                    \
  }                                                                  \
                                                                     \
  RealClass##TestProxy(const RealClass& rhs) {                       \
    removeOldObj();                                                  \
    cloneObj(rhs);                                                   \
  }                                                                  \
                                                                     \
  RealClass##TestProxy& operator=(const RealClass& rhs) {            \
    removeOldObj();                                                  \
    cloneObj(rhs);                                                   \
    return *this;                                                    \
  }                                                                  \
                                                                     \
  /* Test proxy has no "Real Self" */                                \
  RealClass* self() const { return self_; }                          \
                                                                     \
  void setSelf(RealClass* self_ptr, bool owned = true) {             \
    self_ = self_ptr;                                                \
    owned_ = owned;                                                  \
  }                                                                  \
                                                                     \
 private:                                                            \
  void removeOldObj() {                                              \
    if (owned_) delete self_;                                        \
  }                                                                  \
                                                                     \
  void cloneObj(const RealClass& obj) {                              \
    self_ = obj.clone();                                             \
    owned_ = true;                                                   \
  }                                                                  \
                                                                     \
  RealClass* self_{nullptr};                                         \
  bool owned_{true};

// if the default constructor of real object is allowed, use this macro instead
#define TEST_PROXY_WITH_DEFAULT_CONSTRUCTOR(RealClass) \
 public:                                               \
  RealClass##TestProxy() = default;                    \
  TEST_PROXY_WITHOUT_DEFAULT_CONSTRUCTOR(RealClass)

class IData {
 public:
  virtual ~IData() = default;

  virtual void setData(int data) = 0;
  virtual int getData() const = 0;
  virtual int* getDataPtr() = 0;
  virtual IData* clone() const = 0;
  virtual IData& operator=(IData&& rhs) = 0;
};

class IDataTestProxy {
  TEST_PROXY_WITHOUT_DEFAULT_CONSTRUCTOR(IData);

 public:
  void setData(int data) { self_->setData(data); }

  int getData() const { return self_->getData(); }
};

class Data : public IData {
 public:
  Data() : Data(0) {}
  explicit Data(int data) : data_(new int(data)) {}
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

// NOTE: this cast is safe only when Data is the only subclass of IData
// and Data itself Does not have any subclasses
#define TEST_ONLY_MOVE_ASSIGNMENT(Cls)           \
  I##Cls& operator=(I##Cls&& rhs) { /* NOLINT */ \
    Cls& data = static_cast<Cls&>(rhs);          \
    swap(data);                                  \
    return *this;                                \
  }

  TEST_ONLY_MOVE_ASSIGNMENT(Data);

#if 0
  IData& operator=(IData&& rhs) {
    std::cout << "fto move assignment called" << std::endl;
    // NOTE: this cast is safe only when Data is the only subclass of IData
    // and Data itself Does not have any subclasses
    Data& data = static_cast<Data&>(rhs);
    swap(data);
    return *this;
  }
#endif

  void setData(int data) override { *data_ = data; }

  int getData() const override { return *data_; }

  int* getDataPtr() override { return data_; }

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

int main() {
  std::cout << "prepare data......" << std::endl;
  Data data{100};
  DataHolder data_holder{&data};

  std::cout << "create data displayer......" << std::endl;
  DataDisplayer data_displayer(&data_holder);
  data_displayer.printData();
  std::cout << "modify data to 1000......" << std::endl;
  data_displayer.setData(1000);
  data_displayer.printData();
  std::cout << "save data......" << std::endl;
  data_displayer.saveData();

  std::cout << "saved data: " << data.getData() << std::endl;

  Data data1{101};
  Data data2{102};
  data2 = std::move(data1);
  std::cout << "data1 data ptr = " << data1.getDataPtr() << std::endl;
  std::cout << "data2 = " << data2.getData() << std::endl;

  IData* data3 = new Data{103};
  IData* data4 = new Data{104};
  *data4 = std::move(*data3);
  std::cout << "data3 data ptr = " << data3->getDataPtr() << std::endl;
  std::cout << "data4 = " << data4->getData() << std::endl;

#if 0  // C++11 does not have is_final, is_final is introduced in C++14
  std::cout << "Data class is final? "
            << std::is_final<Data>::value << std::endl;
#endif
}
