#include "gtest/gtest.h"
#include "../Guarded.h"

TEST(Guarded, Vector) {

    Guarded<std::vector<int>> vector;

    EXPECT_TRUE(vector->empty());

    vector->push_back(42);
    EXPECT_EQ(vector->front(), 42);
    EXPECT_EQ(vector->size(), 1);

    vector->push_back(99);
    EXPECT_EQ(vector->size(), 2);
}

TEST(Guarded, Set) {
    Guarded<std::set<std::string>> strings;

    strings->insert("Hello");
    strings->insert("World");
    strings->insert("!");

    EXPECT_EQ(strings->size(), 3);
}

TEST(Guarded, Counter) {
    struct Counter {
        int value = 0;

        void SlowIncrement() {
            using namespace std;
            int old = value;
            //std::this_thread::sleep_for(50ms);
            value = old + 1;
        }
    };

    Guarded<Counter> counter;

    std::atomic<bool> flag(false);

    auto call_increment = [&]() {
        flag.wait(false);
        counter->SlowIncrement();
        };

    std::thread thread1(call_increment);
    std::thread thread2(call_increment);
    std::thread thread3(call_increment);
    std::thread thread4(call_increment);

    flag.store(true, std::memory_order_release);
    flag.notify_all();

    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();

    EXPECT_EQ(counter->value, 4);
}
