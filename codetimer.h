#ifndef TEST_TIMER_H
#define TEST_TIMER_H

#include <chrono>
#include "libcuckoo/cuckoohash_map.hh"

struct RecordedValue;

/**
 * Permits measuring of timings in C++ code. Call the \p record function to record a timing, and the
 * \p printStats function to print recorded timings.
 */
class CodeTimer {
private:
    static cuckoohash_map<std::string, std::unique_ptr<RecordedValue>> timerMap;
public:
    /**
     * Record a timing, with the specified name and start time. The end time is (approximately) the time at which
     * this method is called. Timings with the same name are aggregated.
     * @param name The name of the timing to be recorded.
     * @param start The time that we started recording.
     */
    static void record(std::string name, std::chrono::high_resolution_clock::time_point start);
    /**
     * Prints previously recorded timings to stdout. A line is printed for each unique name that was passed to the
     * \p record function, along with the total time and number of occurrences for that name. Timings are printed
     * in descending order, according to the total time.
     */
    static void printStats();
};

#endif //TEST_TIMER_H
