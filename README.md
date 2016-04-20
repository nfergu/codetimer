# Introduction

CodeTimer is a very simple utility for recording the performance of blocks of code. CodeTimer is designed to perform well when called
concurrently by multiple threads. Internally it uses a [libcuckoo](https://github.com/efficient/libcuckoo) hash table.

Here is a simple usage example:

```c++
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
```

When run this will produce something like the following:

```
key1: total=0.400211; occurrences=2
key2: total=0.200093; occurrences=1
```

For each key that we recorded a timing against, we can see the total time for that key, and the number of times we recorded a timing for the key.

In the above example we use `CodeTimer::record` to record a timing for a specific key, and ```CodeTimer::printStats()```. See the API docs in
[codetimer.h] for more details.

# Building

To build the usage example, first build and install [libcuckoo](https://github.com/efficient/libcuckoo). Then, compile as follows:

```
g++ -o timerexample -std=c++11 timerexample.cc codetimer.cc codetimer.h
```

You can then run the usage example as follows:

```
./timerexample
```
















