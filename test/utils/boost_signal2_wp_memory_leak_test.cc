// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <iostream>
#include <memory>

#include "utils/signal_slot.h"
#include "utils/signal_slot_impl.h"

class A : public utils::ITrackable {
 public:
  A() { std::cout << "construct A" << std::endl; }
  ~A() { std::cout << "destruct A" << std::endl; }

  static void* operator new(std::size_t sz) {
    void* p = ::operator new(sz);
    std::cout << "custom new for size " << sz << ", got p " << p << std::endl;
    return p;
  }

  static void operator delete(void* p) {
    std::cout << "custom delete for p " << p << std::endl;
    return ::operator delete(p);
  }
  static void* operator new[](std::size_t sz) {
    std::cout << "custom new for size " << sz << std::endl;
    return ::operator new(sz);
  }
 private:
  char data[50];
};

template <class T>
struct custom_allocator {
  typedef T value_type;
  custom_allocator() noexcept {}
  template <class U> custom_allocator(const custom_allocator<U>&) noexcept {}
  T* allocate(std::size_t n) {
    std::cout << "allocate " << n
              << " objects, object size is " << sizeof(T) << std::endl;
    return static_cast<T*>(::operator new(n * sizeof(T)));
  }
  void deallocate(T* p, std::size_t n) {
    std::cout << "deallocate " << n << " objects" << std::endl;
    ::operator delete(p);
  }
};

class ISignal2Test {
 public:
  virtual ~ISignal2Test() = default;

  SNAIL_SIGSLOT2(TestSignal, void());

  virtual void trigger() = 0;
};

class Signal2Test : public ISignal2Test {
 public:
  Signal2Test() = default;
  virtual ~Signal2Test() = default;

  void trigger() override {
    TestSignal();
  }

  SNAIL_SIGSLOT_IMPL(TestSignal);
};

int main() {
  A* p1 = new A;
  delete p1;

  A* p2 = new A[10];
  delete[] p2;

  std::cout << "-------1 begin----------------" << std::endl;
  std::weak_ptr<A> wp;

  {
    auto p3 = std::make_shared<A>();
    wp = p3;
  }
  std::cout << "--------1 end-----------------" << std::endl;

  std::cout << "--------2 begin-----------" << std::endl;

  custom_allocator<A> alloc;
  {
    std::weak_ptr<A> wp4;

    {
      auto p4 = std::allocate_shared<A>(alloc);
      wp4 = p4;
    }
    std::cout << "---------2 end-----------" << std::endl;
  }

  std::cout << "----------3 begin ----------" << std::endl;
  {
    Signal2Test test_signal;
    {
      auto p5 = std::allocate_shared<A>(alloc);

      test_signal.whenTestSignal([]() {
          std::cout << "signal triggered" << std::endl;
        }, p5);
      test_signal.trigger();
    }
    std::cout << "--------------3 trigger again------------" << std::endl;
    test_signal.trigger();
    std::cout << "--------------3 end ---------------" << std::endl;
  }

  std::cout << "-----------4 begin ---------" << std::endl;
  {
    Signal2Test test_signal;
    {
      auto p6 = utils::make_trackable<A>();

      test_signal.whenTestSignal([](){
          std::cout << "signal triggered" << std::endl;
        }, p6);
      test_signal.trigger();
    }
    std::cout << "--------------3 trigger again------------" << std::endl;
    test_signal.trigger();
    std::cout << "--------------3 end ---------------" << std::endl;
  }
}
