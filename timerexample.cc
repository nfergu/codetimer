#include <chrono>
#include <iostream>
#include <thread>
#include "codetimer.h"

void sleepMillis(long milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

int main () {

    auto start = std::chrono::high_resolution_clock::now();
    sleepMillis(100);
    CodeTimer::record("key1", start);

    start = std::chrono::high_resolution_clock::now();
    sleepMillis(200);
    CodeTimer::record("key2", start);

    start = std::chrono::high_resolution_clock::now();
    sleepMillis(300);
    CodeTimer::record("key1", start);

    CodeTimer::printStats();

    return 0;
}