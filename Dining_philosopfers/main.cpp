#include <iostream>
#include <vector>
#include <thread>
#include <cassert>
#include "philosopher.h"

const size_t iterations = 100000;
const size_t seats = 7;
dining::Table table_(seats);
std::vector<dining::Philosopher> philosophers_;


void RunPhilosopherThread(size_t seat) {
    auto& philosopher = philosophers_[seat];

    for (size_t i = 0; i < iterations; ++i) {
        philosopher.EatOneMoreTime();
    }
}

void CheckStarvation() {
    for (const auto& philosopher : philosophers_) {
        //assert(philosopher.EatCount() > iterations / 5, "Starvation");
        std::cout << philosopher.EatCount() << std::endl;
    }
}



int main() {

    for (size_t seat = 0; seat < seats; ++seat) {
        philosophers_.emplace_back(table_, seat);
    }

    std::vector<std::thread> threads;


    
    for (size_t seat = 0; seat < seats; ++seat) {
        threads.emplace_back(RunPhilosopherThread, seat);
    }

    
    //for (int i = 0; i < 100; i++) {
    //    CheckStarvation();
    //    std::cout << "----------" << std::endl;
    //}


    for (size_t seat = 0; seat < seats; ++seat) {
        threads[seat].join();
    }

    CheckStarvation();


	return 0;
}