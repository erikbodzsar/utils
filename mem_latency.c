#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>

#define KB 1024LL
#define MB (KB*KB)

typedef uint64_t data_type;
static int progress_bar = 1;

static void random_permutation(data_type *array, int n) {
  int pb_width = 21;
  char progress[pb_width+1];
  if (progress_bar) {
    memset(progress, ' ', pb_width);
    progress[pb_width] = '\0';
    progress[0] = '[';
    progress[1] = '>';
    progress[pb_width-1] = ']';
    printf("%s", progress);
    fflush(stdout);
  }
  array[0] = 0;
  int i;
  for (i = 1; i <= n-1; i++) {
    int j = rand()%(i+1);
    if (j != i)
      array[i] = array[j];
    array[j] = i;

    if (progress_bar && i % (n/(pb_width-2)) == 0) {
      int p = i / (n/(pb_width-2));
      progress[p] = '-';
      progress[p+1] = '>';
      printf("\r%s", progress);
      fflush(stdout);
    }
  }

  if (progress_bar) {
    memset(progress, ' ', pb_width);
    printf("\r%s\r", progress);
    fflush(stdout);
  }
}

static struct timeval start_time;
static void start_timer() {
  gettimeofday(&start_time, NULL);
}

static double end_timer() {
  struct timeval end_time;
  gettimeofday(&end_time, NULL);
  return (end_time.tv_sec-start_time.tv_sec)*1000000 +
      end_time.tv_usec - start_time.tv_usec;
}

int main() {
  srand(time(NULL));
  data_type res = 0;

  double target_time = 0.5;  // seconds
  double latency = 1;  // nanoseconds

  printf("Array size | Latency\n"
         "---------------------\n");
  uint64_t num_elems;
  for (num_elems = KB; num_elems*sizeof(data_type) <(1<<21)*KB; num_elems<<=1) {
    data_type *array = (data_type*) malloc(num_elems * sizeof(array[0]));
    random_permutation(array, num_elems);

    start_timer();
    uint64_t reps = target_time*1e9 / latency;
    uint64_t t;
    for (t = 0; t < reps; t++)
        res = array[res];
    latency = end_timer()*1e3/reps;

    printf("%8lluKB | %6.2lfns\n", num_elems*sizeof(data_type)/KB, latency);
    free(array);
  }

  printf("%llu\n", (unsigned long long)res);

  return 0;
}
