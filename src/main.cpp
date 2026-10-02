#include <iostream>
#include "tp.h"

int main(){
    ThreadPool pool(4);

    auto f1 = pool.Submit([]() {
        return 42;
    });

    auto f2 = pool.Submit([]() {
        return 3.14;
    });

    std::cout << f1.get() << "\n";
    std::cout << f2.get() << "\n";
    return 0;
}