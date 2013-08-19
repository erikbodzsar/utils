#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/mman.h>
#include <signal.h>

size_t mem;
void *p;

void terminate(int x) {
  printf("Terminating...\n");
  munlock(p, mem);
  free(p);
  exit(0);  
}

int main(int argc, char *argv[]) {
  int err;
  /* Read input */
  if (argc < 2) {
      fprintf(stderr,
              "Usage: %s size\n"
              "       to lock size MBs of memory", argv[0]);
      exit(1);
    }
  mem = atoi(argv[1]);
  mem <<= 20;

  /* Set locked memory limit */
  struct rlimit lim;
  lim.rlim_cur = lim.rlim_max = mem;
  err = setrlimit(RLIMIT_MEMLOCK, &lim);
  if (err) {
      fprintf(stderr,
              "Error setting locked memory limit\n"
              "Make sure you have root privileges!\n");
      exit(1);
    }

  /* Allocate and lock memory */
  p = malloc(mem);
  if (p == NULL) {
      fprintf(stderr, "Error allocating %zu MBs\n", mem>>20);
      exit(1);
    }
  err = mlock(p, mem);
  if (err) {
      fprintf(stderr, "Error locking pages\n");
      free(p);
      exit(1);
    }

  /* Set sigterm handler */
  if (signal(SIGINT, terminate) == SIG_ERR) {
      fprintf(stderr, "Error registering SIGINT handler\n");
      munlock(p, mem);
      free(p);
      exit(1);
    }

  printf("Memory allocated & locked successfully\n");
  printf("Terminate with Ctrl-C\n");

  /* Wait... */
  while (1)
    sleep(1000);
}
