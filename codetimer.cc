#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include "codetimer.h"

using namespace std::chrono;

struct RecordedValue {
public:
    double totalSecs {0};
    long occurrences {0};
    std::string name;
};

void CodeTimer::record(std::string name, std::chrono::high_resolution_clock::time_point start) {
    auto stop = high_resolution_clock::now();
    auto diff = duration_cast<duration<double>>(stop - start);
    auto updateFunction = [&diff](std::unique_ptr<RecordedValue>& existingValue) {
        existingValue->occurrences++;
        existingValue->totalSecs += diff.count();
    };
    // We try a regular update before calling upsert here. We assume that updates will be much
    // more common than insertions, and this avoids creating unnecessary RecordedValue instances
    // when performing updates. Unnecessary RecordedValue instances will only be created when
    // there are race conditions between threads, which is likely to be rare.
    if (!CodeTimer::timerMap.update_fn(name, updateFunction)) {
        RecordedValue *newValue = new RecordedValue();
        newValue->totalSecs = diff.count();
        newValue->occurrences = 1;
        newValue->name = name;
        CodeTimer::timerMap.upsert(name, updateFunction, std::unique_ptr<RecordedValue>(newValue));
    }
}

void CodeTimer::printStats() {
    std::vector<RecordedValue*> vals;
    vals.reserve(timerMap.size());
    for (const auto &item: CodeTimer::timerMap.lock_table()) {
        vals.push_back(item.second.get());
    }
    std::sort(begin(vals), end(vals), [](RecordedValue* t1, RecordedValue* t2) {
        return t1->totalSecs > t2->totalSecs;
    });
    for (auto val : vals) {
        std::cout << val->name << ": total=" << val->totalSecs << "; occurrences=" << val->occurrences;
        std::cout << std::endl;
    }
}

cuckoohash_map<std::string, std::unique_ptr<RecordedValue>> CodeTimer::timerMap = {};