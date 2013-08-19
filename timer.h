#ifndef _TIMER_H_
#define _TIMER_H_

#include <sys/time.h>
#include <inttypes.h>
#include <string.h>

class Timer {
 public:
  Timer() {
    start();
  }

  // Start the timer.
  inline void start() {
    gettimeofday(&start_time_, NULL);
  }

  // Returns useconds elapsed since last start (or creation).
  inline uint64_t read() const {
    timeval current;
    gettimeofday(&current, NULL);
    return difference(start_time_, current);
  };

  // Restart the timer, and return useconds elapsed since last start.
  inline uint64_t restart() {
    timeval old_start = start_time_;
    start();
    return difference(old_start, start_time_);
  }

  // Convert usec to sec.
  static inline double to_sec(uint64_t t) {
    return t/1e6;
  }

 private:
  timeval start_time_;
  inline uint64_t difference(const timeval &t1,
                             const timeval &t2) const {
    return (t2.tv_sec-t1.tv_sec)*1000000 +
        t2.tv_usec - t1.tv_usec;
  }
};

#endif
