#include <iostream>
#include "lock_free_stack.hpp"

struct A {
    int i = 5;
    A() {
        std::cout << "A()" << std::endl;
    }
    A(const A& a) {
        i = a.i;
        std::cout << "A(const A&)" << std::endl;
    }
    A(A&& a) {
        i = a.i;
        a.i = 0;
        std::cout << "A(A&&)" << std::endl;
    }
    ~A() {
        std::cout << "~A()" << std::endl;
    }
};

int main() {


    LockFreeStack<A> stack;
    A a;

    stack.Push(A());

    stack.Push(a);
    a.i++;
    stack.Push(a);
    
    auto x = stack.TryPop();

    std::cout << x->i << std::endl;

}