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

  void start() {
    gettimeofday(&start_, NULL);
  }

  uint64_t stop() {
    gettimeofday(&end_, NULL);
    return read();
  };

  uint64_t read() {
    return end_.tv_usec + end_.tv_sec * 1000000 -
      start_.tv_usec - start_.tv_sec * 1000000;
  };

  uint64_t restart() {
    uint64_t ret = stop();
    start();
    return ret;
  }

 private:
  timeval start_, end_;
};
#endif
