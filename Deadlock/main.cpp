#include <iostream>
#include <thread>
#include <future>

#include <mutex>



struct DeadlockDetected : std::exception {
    DeadlockDetected(char const* const _Message) : exception(_Message) {};
};

// Deadlock with one fiber and one mutex
void DeadlockOneThread() {
    std::mutex mutex;

    std::promise<void> p;
    auto f(p.get_future());

    auto fiber = [&]() {
        
        auto fun = [&]() {
            mutex.lock();
            throw DeadlockDetected("DeadlockException");
            mutex.unlock();
            };

        try
        {
            fun();
        }
        catch (const std::exception&)
        {
            p.set_exception(std::make_exception_ptr(DeadlockDetected("DeadlockDetected")));
            std::cout << "DeadlockDetected" << std::endl;
        }
        
        // код выполнится, так как мы обработали исключение. Иначе, код до сюда не доходит и мьютех на разблокирутеся
        
        // use mutex.Lock() / mutex.Unlock() to lock/unlock mutex
        };

    std::thread t(fiber);
        try {
        
            //f.get();   
        }
        catch (const DeadlockDetected& e) {
            std::cout << e.what() << std::endl;
        }
        catch (...) {

        }
    t.join();
    
    // RunScheduler(fiber) should throw DeadlockDetected exception
    // ASSERT_THROW(RunScheduler(fiber), DeadlockDetected);
}

 //Deadlock with two fibers
void DeadlockTwoThread() {
    // Declare some Mutex-es
    std::mutex mutex1;
    std::mutex mutex2;

    auto finn = [&]() {
        mutex1.lock();
        mutex2.lock();

        std::cout << "finn" << std::endl;
        mutex1.unlock();
        mutex2.unlock();
        // Use Yield() to reschedule current fiber
        };

    auto jake = [&]() {
        mutex2.lock();
        mutex1.lock();
        
        
        std::cout << "jake" << std::endl;
        mutex1.unlock();
        mutex2.unlock();
        
        };

    
    // Don't block separately
    std::thread t1(finn);
    std::thread t2(finn);

    std::thread t3(jake);
    std::thread t4(jake);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    // Finn and Jake should block each other
    //ASSERT_THROW(RunScheduler(adventure), DeadlockDetected);
}



int main() {

    //DeadlockOneThread();
    DeadlockTwoThread();


	return 0;
}