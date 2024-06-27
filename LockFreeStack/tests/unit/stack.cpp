#include "gtest/gtest.h"
#include "../../src/lock_free_stack.hpp"
#include <thread>


TEST(LockFreeStack, JustWorks) {
  LockFreeStack<std::string> stack;

  stack.Push("Hello");
  auto value = stack.TryPop();
  EXPECT_TRUE(value);
  EXPECT_EQ(*value, "Hello");

  auto empty = stack.TryPop();
  ASSERT_FALSE(empty);
}

TEST(LockFreeStack, LIFO) {
  LockFreeStack<int> stack;

  stack.Push(1);
  stack.Push(2);
  stack.Push(3);

  ASSERT_EQ(*stack.TryPop(), 3);
  ASSERT_EQ(*stack.TryPop(), 2);
  ASSERT_EQ(*stack.TryPop(), 1);

  ASSERT_FALSE(stack.TryPop());
}

TEST(LockFreeStack, Dtor) {
  LockFreeStack<std::string> stack;
  stack.Push("Hello");
  stack.Push("World");
}

struct MoveOnly {
  explicit MoveOnly(int _value) : value(_value) {
  }

  // Non-copyable
  MoveOnly(const MoveOnly&) = delete;
  MoveOnly& operator=(const MoveOnly&) = delete;

  // Movable
  MoveOnly(MoveOnly&&) = default;
  MoveOnly& operator=(MoveOnly&&) = default;

  int value;
};

TEST(LockFreeStack, MoveOnly) {
  LockFreeStack<MoveOnly> stack;
  stack.Push(MoveOnly(1));
  ASSERT_TRUE(stack.TryPop());
}

TEST(LockFreeStack, TwoStacks) {
  LockFreeStack<int> stack_1;
  LockFreeStack<int> stack_2;

  stack_1.Push(3);
  stack_2.Push(11);
  ASSERT_EQ(*stack_1.TryPop(), 3);
  ASSERT_EQ(*stack_2.TryPop(), 11);
}

void StressTest(size_t threads) {
  LockFreeStack<int> stack;

  std::atomic<size_t> ops{0};
  std::atomic<size_t> pushed{0};
  std::atomic<size_t> popped{0};
  std::atomic_bool flag = true;

  std::vector<std::thread> vt;

  for (size_t i = 0; i < threads; ++i) {
    vt.emplace_back([&]() {
      while(flag.load());

      // Push     
      for (size_t j = 0; j < 1000; ++j) {
        stack.Push(j);
        pushed.fetch_add(j);
        ++ops;
      }

      // Pop
      for (size_t j = 0; j < 1000; ++j) {
        auto obj = stack.TryPop();
        ASSERT_TRUE(obj);
        popped.fetch_add(*obj);
      }
    });
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(300));
  flag.store(false);

  for(auto& x: vt) {
    x.join();
  }

  std::cout << "Operations #: " << ops.load() << std::endl;
  std::cout << "Pushed: " << pushed.load() << std::endl;
  std::cout << "Popped: " << popped.load() << std::endl;

  ASSERT_EQ(pushed.load(), popped.load());

  // Stack is empty
  ASSERT_FALSE(stack.TryPop());
}

TEST(LockFreeStack, StressTest1) {
  StressTest(1);
}
TEST(LockFreeStack, StressTest2) {
  StressTest(2);
}
TEST(LockFreeStack, StressTest3) {
  StressTest(3);
}
TEST(LockFreeStack, StressTest4) {
  StressTest(4);
}